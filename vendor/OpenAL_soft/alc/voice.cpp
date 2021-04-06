/**
 * OpenAL cross platform audio library
 * Copyright (C) 1999-2007 by authors.
 * This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the
 *  Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 * Or go to http://www.gnu.org/copyleft/lgpl.html
 */

#include "config.h"

#include "voice.h"

#include <algorithm>
#include <array>
#include <atomic>
#include <cassert>
#include <climits>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <memory>
#include <new>
#include <utility>

#include "alcmain.h"
#include "albyte.h"
#include "alconfig.h"
#include "alcontext.h"
#include "alnumeric.h"
#include "aloptional.h"
#include "alspan.h"
#include "alstring.h"
#include "alu.h"
#include "async_event.h"
#include "buffer_storage.h"
#include "core/cpu_caps.h"
#include "core/devformat.h"
#include "core/filters/biquad.h"
#include "core/filters/nfc.h"
#include "core/filters/splitter.h"
#include "core/fmt_traits.h"
#include "core/logging.h"
#include "core/mixer/defs.h"
#include "core/mixer/hrtfdefs.h"
#include "hrtf.h"
#include "inprogext.h"
#include "opthelpers.h"
#include "ringbuffer.h"
#include "threads.h"
#include "vector.h"
#include "voice_change.h"

struct CTag;
#ifdef HAVE_SSE
struct SSETag;
#endif
#ifdef HAVE_NEON
struct NEONTag;
#endif
struct CopyTag;


static_assert(!(sizeof(Voice::BufferLine)&15), "Voice::BufferLine must be a multiple of 16 bytes");

Resampler ResamplerDefault{Resampler::Linear};

MixerFunc MixSamples{Mix_<CTag>};

namespace {

constexpr uint ResamplerPrePadding{MaxResamplerPadding / 2};

using HrtfMixerFunc = void(*)(const float *InSamples, float2 *AccumSamples, const uint IrSize,
    const MixHrtfFilter *hrtfparams, const size_t BufferSize);
using HrtfMixerBlendFunc = void(*)(const float *InSamples, float2 *AccumSamples,
    const uint IrSize, const HrtfFilter *oldparams, const MixHrtfFilter *newparams,
    const size_t BufferSize);

HrtfMixerFunc MixHrtfSamples{MixHrtf_<CTag>};
HrtfMixerBlendFunc MixHrtfBlendSamples{MixHrtfBlend_<CTag>};

inline MixerFunc SelectMixer()
{
#ifdef HAVE_NEON
    if((CPUCapFlags&CPU_CAP_NEON))
        return Mix_<NEONTag>;
#endif
#ifdef HAVE_SSE
    if((CPUCapFlags&CPU_CAP_SSE))
        return Mix_<SSETag>;
#endif
    return Mix_<CTag>;
}

inline HrtfMixerFunc SelectHrtfMixer()
{
#ifdef HAVE_NEON
    if((CPUCapFlags&CPU_CAP_NEON))
        return MixHrtf_<NEONTag>;
#endif
#ifdef HAVE_SSE
    if((CPUCapFlags&CPU_CAP_SSE))
        return MixHrtf_<SSETag>;
#endif
    return MixHrtf_<CTag>;
}

inline HrtfMixerBlendFunc SelectHrtfBlendMixer()
{
#ifdef HAVE_NEON
    if((CPUCapFlags&CPU_CAP_NEON))
        return MixHrtfBlend_<NEONTag>;
#endif
#ifdef HAVE_SSE
    if((CPUCapFlags&CPU_CAP_SSE))
        return MixHrtfBlend_<SSETag>;
#endif
    return MixHrtfBlend_<CTag>;
}

} // namespace


void aluInitMixer()
{
    if(auto resopt = ConfigValueStr(nullptr, nullptr, "resampler"))
    {
        struct ResamplerEntry {
            const char name[16];
            const Resampler resampler;
        };
        constexpr ResamplerEntry ResamplerList[]{
            { "none", Resampler::Point },
            { "point", Resampler::Point },
            { "linear", Resampler::Linear },
            { "cubic", Resampler::Cubic },
            { "bsinc12", Resampler::BSinc12 },
            { "fast_bsinc12", Resampler::FastBSinc12 },
            { "bsinc24", Resampler::BSinc24 },
            { "fast_bsinc24", Resampler::FastBSinc24 },
        };

        const char *str{resopt->c_str()};
        if(al::strcasecmp(str, "bsinc") == 0)
        {
            WARN("Resampler option \"%s\" is deprecated, using bsinc12\n", str);
            str = "bsinc12";
        }
        else if(al::strcasecmp(str, "sinc4") == 0 || al::strcasecmp(str, "sinc8") == 0)
        {
            WARN("Resampler option \"%s\" is deprecated, using cubic\n", str);
            str = "cubic";
        }

        auto iter = std::find_if(std::begin(ResamplerList), std::end(ResamplerList),
            [str](const ResamplerEntry &entry) -> bool
            { return al::strcasecmp(str, entry.name) == 0; });
        if(iter == std::end(ResamplerList))
            ERR("Invalid resampler: %s\n", str);
        else
            ResamplerDefault = iter->resampler;
    }

    MixSamples = SelectMixer();
    MixHrtfBlendSamples = SelectHrtfBlendMixer();
    MixHrtfSamples = SelectHrtfMixer();
}


namespace {

void SendSourceStoppedEvent(ALCcontext *context, uint id)
{
    RingBuffer *ring{context->mAsyncEvents.get()};
    auto evt_vec = ring->getWriteVector();
    if(evt_vec.first.len < 1) return;

    AsyncEvent *evt{::new(evt_vec.first.buf) AsyncEvent{EventType_SourceStateChange}};
    evt->u.srcstate.id = id;
    evt->u.srcstate.state = VChangeState::Stop;

    ring->writeAdvance(1);
}


const float *DoFilters(BiquadFilter &lpfilter, BiquadFilter &hpfilter, float *dst,
    const al::span<const float> src, int type)
{
    switch(type)
    {
    case AF_None:
        lpfilter.clear();
        hpfilter.clear();
        break;

    case AF_LowPass:
        lpfilter.process(src, dst);
        hpfilter.clear();
        return dst;
    case AF_HighPass:
        lpfilter.clear();
        hpfilter.process(src, dst);
        return dst;

    case AF_BandPass:
        DualBiquad{lpfilter, hpfilter}.process(src, dst);
        return dst;
    }
    return src.data();
}


void LoadSamples(const al::span<Voice::BufferLine> dstSamples, const size_t dstOffset,
    const al::byte *src, const size_t srcOffset, const size_t srcstep, FmtType srctype,
    const size_t samples) noexcept
{
#define HANDLE_FMT(T) case T:                                                 \
    {                                                                         \
        constexpr size_t sampleSize{sizeof(al::FmtTypeTraits<T>::Type)};      \
        src += srcOffset*srcstep*sampleSize;                                  \
        for(auto &dst : dstSamples)                                           \
        {                                                                     \
            al::LoadSampleArray<T>(dst.data() + dstOffset, src, srcstep, samples); \
            src += sampleSize;                                                \
        }                                                                     \
    }                                                                         \
    break

    switch(srctype)
    {
    HANDLE_FMT(FmtUByte);
    HANDLE_FMT(FmtShort);
    HANDLE_FMT(FmtFloat);
    HANDLE_FMT(FmtDouble);
    HANDLE_FMT(FmtMulaw);
    HANDLE_FMT(FmtAlaw);
    }
#undef HANDLE_FMT
}

void LoadBufferStatic(VoiceBufferItem *buffer, VoiceBufferItem *bufferLoopItem,
    const size_t dataPosInt, const FmtType sampleType, const size_t samplesToLoad,
    const al::span<Voice::BufferLine> voiceSamples)
{
    const size_t numChannels{voiceSamples.size()};
    const uint loopStart{buffer->mLoopStart};
    const uint loopEnd{buffer->mLoopEnd};
    ASSUME(loopEnd > loopStart);

    /* If current pos is beyond the loop range, do not loop */
    if(!bufferLoopItem || dataPosInt >= loopEnd)
    {
        /* Load what's left to play from the buffer */
        const size_t remaining{minz(samplesToLoad, buffer->mSampleLen-dataPosInt)};
        LoadSamples(voiceSamples, ResamplerPrePadding, buffer->mSamples, dataPosInt, numChannels,
            sampleType, remaining);

        if(const size_t toFill{samplesToLoad - remaining})
        {
            for(auto &chanbuffer : voiceSamples)
            {
                auto srcsamples = chanbuffer.data() + ResamplerPrePadding - 1 + remaining;
                std::fill_n(srcsamples + 1, toFill, *srcsamples);
            }
        }
    }
    else
    {
        /* Load what's left of this loop iteration */
        const size_t remaining{minz(samplesToLoad, loopEnd-dataPosInt)};
        LoadSamples(voiceSamples, ResamplerPrePadding, buffer->mSamples, dataPosInt, numChannels,
            sampleType, remaining);

        /* Load repeats of the loop to fill the buffer. */
        const auto loopSize = static_cast<size_t>(loopEnd - loopStart);
        size_t samplesLoaded{remaining};
        while(const size_t toFill{minz(samplesToLoad - samplesLoaded, loopSize)})
        {
            LoadSamples(voiceSamples, ResamplerPrePadding + samplesLoaded, buffer->mSamples,
                loopStart, numChannels, sampleType, toFill);
            samplesLoaded += toFill;
        }
    }
}

void LoadBufferCallback(VoiceBufferItem *buffer, const size_t numCallbackSamples,
    const FmtType sampleType, const size_t samplesToLoad,
    const al::span<Voice::BufferLine> voiceSamples)
{
    const size_t numChannels{voiceSamples.size()};
    /* Load what's left to play from the buffer */
    const size_t remaining{minz(samplesToLoad, numCallbackSamples)};
    LoadSamples(voiceSamples, ResamplerPrePadding, buffer->mSamples, 0, numChannels, sampleType,
        remaining);

    if(const size_t toFill{samplesToLoad - remaining})
    {
        for(auto &chanbuffer : voiceSamples)
        {
            auto srcsamples = chanbuffer.data() + ResamplerPrePadding - 1 + remaining;
            std::fill_n(srcsamples + 1, toFill, *srcsamples);
        }
    }
}

void LoadBufferQueue(VoiceBufferItem *buffer, VoiceBufferItem *bufferLoopItem,
    size_t dataPosInt, const FmtType sampleType, const size_t samplesToLoad,
    const al::span<Voice::BufferLine> voiceSamples)
{
    const size_t numChannels{voiceSamples.size()};
    /* Crawl the buffer queue to fill in the temp buffer */
    size_t samplesLoaded{0};
    while(buffer && samplesLoaded != samplesToLoad)
    {
        if(dataPosInt >= buffer->mSampleLen)
        {
            dataPosInt -= buffer->mSampleLen;
            buffer = buffer->mNext.load(std::memory_order_acquire);
            if(!buffer) buffer = bufferLoopItem;
            continue;
        }

        const size_t remaining{minz(samplesToLoad-samplesLoaded, buffer->mSampleLen-dataPosInt)};
        LoadSamples(voiceSamples, ResamplerPrePadding+samplesLoaded, buffer->mSamples, dataPosInt,
            numChannels, sampleType, remaining);

        samplesLoaded += remaining;
        if(samplesLoaded == samplesToLoad)
            break;

        dataPosInt = 0;
        buffer = buffer->mNext.load(std::memory_order_acquire);
        if(!buffer) buffer = bufferLoopItem;
    }
    if(const size_t toFill{samplesToLoad - samplesLoaded})
    {
        size_t chanidx{0};
        for(auto &chanbuffer : voiceSamples)
        {
            auto srcsamples = chanbuffer.data() + ResamplerPrePadding - 1 + samplesLoaded;
            std::fill_n(srcsamples + 1, toFill, *srcsamples);
            ++chanidx;
        }
    }
}


void DoHrtfMix(const float *samples, const uint DstBufferSize, DirectParams &parms,
    const float TargetGain, const uint Counter, uint OutPos, ALCdevice *Device)
{
    const uint IrSize{Device->mIrSize};
    auto &HrtfSamples = Device->HrtfSourceData;
    /* Source HRTF mixing needs to include the direct delay so it remains
     * aligned with the direct mix's HRTF filtering.
     */
    float2 *AccumSamples{Device->HrtfAccumData + HrtfDirectDelay};

    /* Copy the HRTF history and new input samples into a temp buffer. */
    auto src_iter = std::copy(parms.Hrtf.History.begin(), parms.Hrtf.History.end(),
        std::begin(HrtfSamples));
    std::copy_n(samples, DstBufferSize, src_iter);
    /* Copy the last used samples back into the history buffer for later. */
    std::copy_n(std::begin(HrtfSamples) + DstBufferSize, parms.Hrtf.History.size(),
        parms.Hrtf.History.begin());

    /* If fading and this is the first mixing pass, fade between the IRs. */
    uint fademix{0u};
    if(Counter && OutPos == 0)
    {
        fademix = minu(DstBufferSize, Counter);

        float gain{TargetGain};

        /* The new coefficients need to fade in completely since they're
         * replacing the old ones. To keep the gain fading consistent,
         * interpolate between the old and new target gains given how much of
         * the fade time this mix handles.
         */
        if(Counter > fademix)
        {
            const float a{static_cast<float>(fademix) / static_cast<float>(Counter)};
            gain = lerp(parms.Hrtf.Old.Gain, TargetGain, a);
        }

        MixHrtfFilter hrtfparams{
            parms.Hrtf.Target.Coeffs,
            parms.Hrtf.Target.Delay,
            0.0f, gain / static_cast<float>(fademix)};
        MixHrtfBlendSamples(HrtfSamples, AccumSamples+OutPos, IrSize, &parms.Hrtf.Old, &hrtfparams,
            fademix);

        /* Update the old parameters with the result. */
        parms.Hrtf.Old = parms.Hrtf.Target;
        parms.Hrtf.Old.Gain = gain;
        OutPos += fademix;
    }

    if(fademix < DstBufferSize)
    {
        const uint todo{DstBufferSize - fademix};
        float gain{TargetGain};

        /* Interpolate the target gain if the gain fading lasts longer than
         * this mix.
         */
        if(Counter > DstBufferSize)
        {
            const float a{static_cast<float>(todo) / static_cast<float>(Counter-fademix)};
            gain = lerp(parms.Hrtf.Old.Gain, TargetGain, a);
        }

        MixHrtfFilter hrtfparams{
            parms.Hrtf.Target.Coeffs,
            parms.Hrtf.Target.Delay,
            parms.Hrtf.Old.Gain,
            (gain - parms.Hrtf.Old.Gain) / static_cast<float>(todo)};
        MixHrtfSamples(HrtfSamples+fademix, AccumSamples+OutPos, IrSize, &hrtfparams, todo);

        /* Store the now-current gain for next time. */
        parms.Hrtf.Old.Gain = gain;
    }
}

void DoNfcMix(const al::span<const float> samples, FloatBufferLine *OutBuffer, DirectParams &parms,
    const float *TargetGains, const uint Counter, const uint OutPos, ALCdevice *Device)
{
    using FilterProc = void (NfcFilter::*)(const al::span<const float>, float*);
    static constexpr FilterProc NfcProcess[MaxAmbiOrder+1]{
        nullptr, &NfcFilter::process1, &NfcFilter::process2, &NfcFilter::process3};

    float *CurrentGains{parms.Gains.Current.data()};
    MixSamples(samples, {OutBuffer, 1u}, CurrentGains, TargetGains, Counter, OutPos);
    ++OutBuffer;
    ++CurrentGains;
    ++TargetGains;

    const al::span<float> nfcsamples{Device->NfcSampleData, samples.size()};
    size_t order{1};
    while(const size_t chancount{Device->NumChannelsPerOrder[order]})
    {
        (parms.NFCtrlFilter.*NfcProcess[order])(samples, nfcsamples.data());
        MixSamples(nfcsamples, {OutBuffer, chancount}, CurrentGains, TargetGains, Counter, OutPos);
        OutBuffer += chancount;
        CurrentGains += chancount;
        TargetGains += chancount;
        if(++order == MaxAmbiOrder+1)
            break;
    }
}

} // namespace

void Voice::mix(const State vstate, ALCcontext *Context, const uint SamplesToDo)
{
    static constexpr std::array<float,MAX_OUTPUT_CHANNELS> SilentTarget{};

    ASSUME(SamplesToDo > 0);

    /* Get voice info */
    uint DataPosInt{mPosition.load(std::memory_order_relaxed)};
    uint DataPosFrac{mPositionFrac.load(std::memory_order_relaxed)};
    VoiceBufferItem *BufferListItem{mCurrentBuffer.load(std::memory_order_relaxed)};
    VoiceBufferItem *BufferLoopItem{mLoopBuffer.load(std::memory_order_relaxed)};
    const uint increment{mStep};
    if UNLIKELY(increment < 1)
    {
        /* If the voice is supposed to be stopping but can't be mixed, just
         * stop it before bailing.
         */
        if(vstate == Stopping)
            mPlayState.store(Stopped, std::memory_order_release);
        return;
    }

    ALCdevice *Device{Context->mDevice.get()};
    const uint NumSends{Device->NumAuxSends};

    ResamplerFunc Resample{(increment == MixerFracOne && DataPosFrac == 0) ?
                           Resample_<CopyTag,CTag> : mResampler};

    uint Counter{(mFlags&VoiceIsFading) ? SamplesToDo : 0};
    if(!Counter)
    {
        /* No fading, just overwrite the old/current params. */
        for(auto &chandata : mChans)
        {
            {
                DirectParams &parms = chandata.mDryParams;
                if(!(mFlags&VoiceHasHrtf))
                    parms.Gains.Current = parms.Gains.Target;
                else
                    parms.Hrtf.Old = parms.Hrtf.Target;
            }
            for(uint send{0};send < NumSends;++send)
            {
                if(mSend[send].Buffer.empty())
                    continue;

                SendParams &parms = chandata.mWetParams[send];
                parms.Gains.Current = parms.Gains.Target;
            }
        }
    }
    else if UNLIKELY(!BufferListItem)
        Counter = std::min(Counter, 64u);

    uint buffers_done{0u};
    uint OutPos{0u};
    do {
        /* Figure out how many buffer samples will be needed */
        uint DstBufferSize{SamplesToDo - OutPos};
        uint SrcBufferSize;

        if(increment <= MixerFracOne)
        {
            /* Calculate the last written dst sample pos. */
            uint64_t DataSize64{DstBufferSize - 1};
            /* Calculate the last read src sample pos. */
            DataSize64 = (DataSize64*increment + DataPosFrac) >> MixerFracBits;
            /* +1 to get the src sample count, include padding. */
            DataSize64 += 1 + ResamplerPrePadding;

            /* Result is guaranteed to be <= BufferLineSize+ResamplerPrePadding
             * since we won't use more src samples than dst samples+padding.
             */
            SrcBufferSize = static_cast<uint>(DataSize64);
        }
        else
        {
            uint64_t DataSize64{DstBufferSize};
            /* Calculate the end src sample pos, include padding. */
            DataSize64 = (DataSize64*increment + DataPosFrac) >> MixerFracBits;
            DataSize64 += ResamplerPrePadding;

            if(DataSize64 <= BufferLineSize + ResamplerPrePadding)
                SrcBufferSize = static_cast<uint>(DataSize64);
            else
            {
                /* If the source size got saturated, we can't fill the desired
                 * dst size. Figure out how many samples we can actually mix.
                 */
                SrcBufferSize = BufferLineSize + ResamplerPrePadding;

                DataSize64 = SrcBufferSize - ResamplerPrePadding;
                DataSize64 = ((DataSize64<<MixerFracBits) - DataPosFrac) / increment;
                if(DataSize64 < DstBufferSize)
                {
                    /* Some mixers require being 16-byte aligned, so also limit
                     * to a multiple of 4 samples to maintain alignment.
                     */
                    DstBufferSize = static_cast<uint>(DataSize64) & ~3u;
                }
            }
        }

        if((mFlags&(VoiceIsCallback|VoiceCallbackStopped)) == VoiceIsCallback && BufferListItem)
        {
            if(SrcBufferSize > mNumCallbackSamples)
            {
                const size_t FrameSize{mChans.size() * mSampleSize};
                ASSUME(FrameSize > 0);

                const size_t byteOffset{mNumCallbackSamples*FrameSize};
                const size_t needBytes{SrcBufferSize*FrameSize - byteOffset};

                const int gotBytes{BufferListItem->mCallback(BufferListItem->mUserData,
                    &BufferListItem->mSamples[byteOffset], static_cast<int>(needBytes))};
                if(gotBytes < 0)
                    mFlags |= VoiceCallbackStopped;
                else if(static_cast<uint>(gotBytes) < needBytes)
                {
                    mFlags |= VoiceCallbackStopped;
                    mNumCallbackSamples += static_cast<uint>(static_cast<uint>(gotBytes) /
                        FrameSize);
                }
                else
                    mNumCallbackSamples = SrcBufferSize;
            }
        }

        if UNLIKELY(!BufferListItem)
        {
            for(auto &chanbuffer : mVoiceSamples)
            {
                auto srciter = chanbuffer.data() + ResamplerPrePadding;

                /* When loading from a voice that ended prematurely, only take
                 * the samples that get closest to 0 amplitude. This helps
                 * certain sounds fade out better.
                 */
                auto abs_lt = [](const float lhs, const float rhs) noexcept -> bool
                { return std::abs(lhs) < std::abs(rhs); };
                srciter = std::min_element(srciter, srciter+(MaxResamplerPadding>>1), abs_lt);

                std::fill(srciter+1, chanbuffer.data() + ResamplerPrePadding + SrcBufferSize,
                    *srciter);
            }
        }
        else if((mFlags&VoiceIsStatic))
            LoadBufferStatic(BufferListItem, BufferLoopItem, DataPosInt, mFmtType, SrcBufferSize,
                mVoiceSamples);
        else if((mFlags&VoiceIsCallback))
            LoadBufferCallback(BufferListItem, mNumCallbackSamples, mFmtType, SrcBufferSize,
                mVoiceSamples);
        else
            LoadBufferQueue(BufferListItem, BufferLoopItem, DataPosInt, mFmtType, SrcBufferSize,
                mVoiceSamples);

        ASSUME(DstBufferSize > 0);
        auto voiceSamples = mVoiceSamples.begin();
        for(auto &chandata : mChans)
        {
            /* Resample, then apply ambisonic upsampling as needed. */
            float *ResampledData{Resample(&mResampleState,
                voiceSamples->data() + ResamplerPrePadding, DataPosFrac, increment,
                {Device->ResampledData, DstBufferSize})};
            if((mFlags&VoiceIsAmbisonic))
                chandata.mAmbiSplitter.processHfScale({ResampledData, DstBufferSize},
                    chandata.mAmbiScale);

            /* Now filter and mix to the appropriate outputs. */
            const al::span<float,BufferLineSize> FilterBuf{Device->FilteredData};
            {
                DirectParams &parms = chandata.mDryParams;
                const float *samples{DoFilters(parms.LowPass, parms.HighPass, FilterBuf.data(),
                    {ResampledData, DstBufferSize}, mDirect.FilterType)};

                if((mFlags&VoiceHasHrtf))
                {
                    const float TargetGain{UNLIKELY(vstate == Stopping) ? 0.0f :
                        parms.Hrtf.Target.Gain};
                    DoHrtfMix(samples, DstBufferSize, parms, TargetGain, Counter, OutPos, Device);
                }
                else if((mFlags&VoiceHasNfc))
                {
                    const float *TargetGains{UNLIKELY(vstate == Stopping) ? SilentTarget.data()
                        : parms.Gains.Target.data()};
                    DoNfcMix({samples, DstBufferSize}, mDirect.Buffer.data(), parms, TargetGains,
                        Counter, OutPos, Device);
                }
                else
                {
                    const float *TargetGains{UNLIKELY(vstate == Stopping) ? SilentTarget.data()
                        : parms.Gains.Target.data()};
                    MixSamples({samples, DstBufferSize}, mDirect.Buffer,
                        parms.Gains.Current.data(), TargetGains, Counter, OutPos);
                }
            }

            for(uint send{0};send < NumSends;++send)
            {
                if(mSend[send].Buffer.empty())
                    continue;

                SendParams &parms = chandata.mWetParams[send];
                const float *samples{DoFilters(parms.LowPass, parms.HighPass, FilterBuf.data(),
                    {ResampledData, DstBufferSize}, mSend[send].FilterType)};

                const float *TargetGains{UNLIKELY(vstate == Stopping) ? SilentTarget.data()
                    : parms.Gains.Target.data()};
                MixSamples({samples, DstBufferSize}, mSend[send].Buffer,
                    parms.Gains.Current.data(), TargetGains, Counter, OutPos);
            }

            /* Store the last source samples used for next time. */
            const size_t srcOffset{(increment*DstBufferSize + DataPosFrac)>>MixerFracBits};
            std::copy_n(voiceSamples->data()+srcOffset, MaxResamplerPadding, voiceSamples->data());
            ++voiceSamples;
        }
        /* Update positions */
        DataPosFrac += increment*DstBufferSize;
        const uint SrcSamplesDone{DataPosFrac>>MixerFracBits};
        DataPosInt  += SrcSamplesDone;
        DataPosFrac &= MixerFracMask;

        OutPos += DstBufferSize;
        Counter = maxu(DstBufferSize, Counter) - DstBufferSize;

        if UNLIKELY(!BufferListItem)
        {
            /* Do nothing extra when there's no buffers. */
        }
        else if((mFlags&VoiceIsStatic))
        {
            if(BufferLoopItem)
            {
                /* Handle looping static source */
                const uint LoopStart{BufferListItem->mLoopStart};
                const uint LoopEnd{BufferListItem->mLoopEnd};
                if(DataPosInt >= LoopEnd)
                {
                    assert(LoopEnd > LoopStart);
                    DataPosInt = ((DataPosInt-LoopStart)%(LoopEnd-LoopStart)) + LoopStart;
                }
            }
            else
            {
                /* Handle non-looping static source */
                if(DataPosInt >= BufferListItem->mSampleLen)
                {
                    BufferListItem = nullptr;
                    break;
                }
            }
        }
        else if((mFlags&VoiceIsCallback))
        {
            if(SrcSamplesDone < mNumCallbackSamples)
            {
                const size_t FrameSize{mChans.size() * mSampleSize};
                ASSUME(FrameSize > 0);

                const size_t byteOffset{SrcSamplesDone*FrameSize};
                const size_t byteEnd{mNumCallbackSamples*FrameSize};
                al::byte *data{BufferListItem->mSamples};
                std::copy(data+byteOffset, data+byteEnd, data);
                mNumCallbackSamples -= SrcSamplesDone;
            }
            else
            {
                BufferListItem = nullptr;
                mNumCallbackSamples = 0;
            }
        }
        else
        {
            /* Handle streaming source */
            do {
                if(BufferListItem->mSampleLen > DataPosInt)
                    break;

                DataPosInt -= BufferListItem->mSampleLen;

                ++buffers_done;
                BufferListItem = BufferListItem->mNext.load(std::memory_order_relaxed);
                if(!BufferListItem) BufferListItem = BufferLoopItem;
            } while(BufferListItem);
        }
    } while(OutPos < SamplesToDo);

    mFlags |= VoiceIsFading;

    /* Don't update positions and buffers if we were stopping. */
    if UNLIKELY(vstate == Stopping)
    {
        mPlayState.store(Stopped, std::memory_order_release);
        return;
    }

    /* Capture the source ID in case it's reset for stopping. */
    const uint SourceID{mSourceID.load(std::memory_order_relaxed)};

    /* Update voice info */
    mPosition.store(DataPosInt, std::memory_order_relaxed);
    mPositionFrac.store(DataPosFrac, std::memory_order_relaxed);
    mCurrentBuffer.store(BufferListItem, std::memory_order_relaxed);
    if(!BufferListItem)
    {
        mLoopBuffer.store(nullptr, std::memory_order_relaxed);
        mSourceID.store(0u, std::memory_order_relaxed);
    }
    std::atomic_thread_fence(std::memory_order_release);

    /* Send any events now, after the position/buffer info was updated. */
    const uint enabledevt{Context->mEnabledEvts.load(std::memory_order_acquire)};
    if(buffers_done > 0 && (enabledevt&EventType_BufferCompleted))
    {
        RingBuffer *ring{Context->mAsyncEvents.get()};
        auto evt_vec = ring->getWriteVector();
        if(evt_vec.first.len > 0)
        {
            AsyncEvent *evt{::new(evt_vec.first.buf) AsyncEvent{EventType_BufferCompleted}};
            evt->u.bufcomp.id = SourceID;
            evt->u.bufcomp.count = buffers_done;
            ring->writeAdvance(1);
        }
    }

    if(!BufferListItem)
    {
        /* If the voice just ended, set it to Stopping so the next render
         * ensures any residual noise fades to 0 amplitude.
         */
        mPlayState.store(Stopping, std::memory_order_release);
        if((enabledevt&EventType_SourceStateChange))
            SendSourceStoppedEvent(Context, SourceID);
    }
}
