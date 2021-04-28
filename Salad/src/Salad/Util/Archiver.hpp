// A simple wrapper around the libzip(zlib) library

// TODO: use stack allocater for buffers that are to be released after use, and pool allocaters for buffers that persist

#pragma once

#include "Salad/Core/Log.h"
#include "zip.h"

#define ARCHIVE_ASSERT_READ SLD_CORE_ASSERT((m_BufferMode == ArchiveIOBufferMode::Read), "Trying to call a read operation when not in read mode!")
#define ARCHIVE_ASSERT_WRITE SLD_CORE_ASSERT((m_BufferMode == ArchiveIOBufferMode::Write), "Trying to call a write operation when not in write mode!")

namespace Salad::Util {

	typedef zip* Archive;
	typedef zip_source* ArchiveSource;

	enum ArchiverOpenFlags_ {
		ArchiverOpenFlags_None                          = 0,						// No flag
		ArchiverOpenFlags_Create                        = ZIP_CREATE,				// Create the archive if it does not exist
		ArchiverOpenFlags_Excl                          = ZIP_EXCL,					// Error if the archive already exists
		ArchiverOpenFlags_Truncate                      = ZIP_TRUNCATE,				// If archive exists, ignore its current contents. In other words, handle it the same way as an empty archive.
		ArchiverOpenFlags_CheckCons                     = ZIP_CHECKCONS,			// Perform additional stricter consistency checks on the archive, and error if they fail.
	};

	enum ArchiverFlags_ {
		ArchiverFlags_IgnoreCase 						= ZIP_FL_NOCASE,			// Ignore case on name lookup
		ArchiverFlags_IgnoreDirectory					= ZIP_FL_NODIR,				// Ignore directory component
		ArchiverFlags_Compressed						= ZIP_FL_COMPRESSED,		// Read compressed data
		ArchiverFlags_Unchanged							= ZIP_FL_UNCHANGED,			// Use original data
		ArchiverFlags_Recompress						= ZIP_FL_RECOMPRESS,		// Force recompression of data
		ArchiverFlags_Encrypted							= ZIP_FL_ENCRYPTED,			// Read encrypted data (implies ArchiverFlags_Compressed)
		ArchiverFlags_EncodingGuess						= ZIP_FL_ENC_GUESS,			// Guess string encoding (default)
		ArchiverFlags_EncodingRaw						= ZIP_FL_ENC_RAW,			// Get unmodified string
		ArchiverFlags_EncodingStrict					= ZIP_FL_ENC_STRICT,		// Follow specification strictly
		ArchiverFlags_Local								= ZIP_FL_LOCAL,				// In local header
		ArchiverFlags_Central							= ZIP_FL_CENTRAL,			// In central directory
		ArchiverFlags_EncodingUTF8						= ZIP_FL_ENC_UTF_8,			// String is UTF8 encoded
		ArchiverFlags_EncodingCP437						= ZIP_FL_ENC_CP437,			// String is CP437 encoded
		ArchiverFlags_Overwrite							= ZIP_FL_OVERWRITE,			// zip_file_add: if file with name exists, overwrite (replace) it
	};

	enum ArchiverErrors_ {
		ArchiverErrors_None                             = ZIP_ER_OK,				// No errors
		ArchiverErrors_Multidisk                        = ZIP_ER_MULTIDISK,			// Multi-disk archives are not supported
		ArchiverErrors_Rename                           = ZIP_ER_RENAME,			// Renaming temporary file failed
		ArchiverErrors_Close                            = ZIP_ER_CLOSE,				// Closing zip archive failed
		ArchiverErrors_Seek                             = ZIP_ER_SEEK,				// Seek error
		ArchiverErrors_Read                             = ZIP_ER_READ,				// Read error
		ArchiverErrors_Write                            = ZIP_ER_WRITE,				// Write error
		ArchiverErrors_CRC								= ZIP_ER_CRC,				// CRC error
		ArchiverErrors_ZipClosed						= ZIP_ER_ZIPCLOSED,			// Containing archive was closed
		ArchiverErrors_NoFile							= ZIP_ER_NOENT,				// No such file
		ArchiverErrors_FileExists						= ZIP_ER_EXISTS,			// File already exists
		ArchiverErrors_Open								= ZIP_ER_OPEN,				// Can't open file
		ArchiverErrors_TempOpen							= ZIP_ER_TMPOPEN,			// Failed to create temporary file
		ArchiverErrors_Zlib								= ZIP_ER_ZLIB,				// Zlib error
		ArchiverErrors_Memory							= ZIP_ER_MEMORY,			// Malloc failure
		ArchiverErrors_EntryChanged						= ZIP_ER_CHANGED,			// Entry has been changed
		ArchiverErrors_CompressionNotSupported			= ZIP_ER_COMPNOTSUPP,		// Compression method not supported
		ArchiverErrors_EndOfFile						= ZIP_ER_EOF,				// Premature EOF
		ArchiverErrors_InvalidArgument                  = ZIP_ER_INVAL,				// Invalid argument
		ArchiverErrors_NoArchive						= ZIP_ER_NOZIP,				// Not a zip archive
		ArchiverErrors_Internal                         = ZIP_ER_INTERNAL,			// Internal error
		ArchiverErrors_Inconsistent					    = ZIP_ER_INCONS,			// Zip archive is inconsistent
		ArchiverErrors_Remove							= ZIP_ER_REMOVE,			// Can't remove file
		ArchiverErrors_Deleted							= ZIP_ER_DELETED,			// Entry has been deleted
		ArchiverErrors_EncryptionNotSupported			= ZIP_ER_ENCRNOTSUPP,		// Encryption method not supported
		ArchiverErrors_ReadOnly							= ZIP_ER_RDONLY,			// Read-only archive
		ArchiverErrors_NoPassword						= ZIP_ER_NOPASSWD,			// No password provided
		ArchiverErrors_WrongPassword					= ZIP_ER_WRONGPASSWD		// Wrong password provided
	};

	enum class ArchiveIOBufferMode {
		None = 0, Read = 1, Write = 2
	};

	class ArchiveIOBuffer {

	public:
		ArchiveIOBuffer() = delete;
		ArchiveIOBuffer(char* data, uint64_t size) : m_Data(data), m_Size(size) { m_BufferMode = ArchiveIOBufferMode::Read; }
		ArchiveIOBuffer(uint64_t size) : m_Size(size) { m_Data = (char*)malloc(m_Size); m_BufferMode = ArchiveIOBufferMode::Write; }
		//ArchiveIOBuffer(char* data, uint64_t size) : m_Size(size) { m_Data.resize(size); memcpy(&m_Data[0], data, size); m_BufferMode = ArchiveIOBufferMode::Read; }
		//ArchiveIOBuffer(uint64_t size) : m_Size(size) { m_Data.resize(size); m_BufferMode = ArchiveIOBufferMode::Write; }

		uint64_t getBufferSize() { return m_Size; }
		char* getBuffer() { return m_Data; }
		void freeBuffer() { free(m_Data); /*m_Data.clear();*/ }

		bool hasNext(uint64_t amount = 1) { return m_Iterator + amount <= m_Size; }

		// Primitive data read

		inline char readByte()							{ ARCHIVE_ASSERT_READ; return m_Data[iit(sizeof(char))]; }
		inline unsigned char readUByte()				{ ARCHIVE_ASSERT_READ; return (unsigned char)m_Data[iit(sizeof(unsigned char))]; }
		inline int16_t readShort()						{ ARCHIVE_ASSERT_READ; return *((int16_t*)&m_Data[iit(sizeof(int16_t))]); }
		inline uint16_t readUShort()					{ ARCHIVE_ASSERT_READ; return *((uint16_t*)&m_Data[iit(sizeof(uint16_t))]); }
		inline int32_t readInt()						{ ARCHIVE_ASSERT_READ; return *((int32_t*)&m_Data[iit(sizeof(int32_t))]); }
		inline uint32_t readUInt()						{ ARCHIVE_ASSERT_READ; return *((uint32_t*)&m_Data[iit(sizeof(uint32_t))]); }
		inline int64_t readLong()						{ ARCHIVE_ASSERT_READ; return *((int64_t*)&m_Data[iit(sizeof(int64_t))]); }
		inline uint64_t readULong()						{ ARCHIVE_ASSERT_READ; return *((uint64_t*)&m_Data[iit(sizeof(uint64_t))]); }
		inline float readFloat()					  	{ ARCHIVE_ASSERT_READ; return *((float*)&m_Data[iit(sizeof(float))]); }
		inline double readDouble()						{ ARCHIVE_ASSERT_READ; return *((double*)&m_Data[iit(sizeof(double))]); }
		inline bool readBool()							{ ARCHIVE_ASSERT_READ; return *((bool*)&m_Data[iit(sizeof(bool))]); }

		// Primitive data write

		inline void writeByte(char value)				{ ARCHIVE_ASSERT_WRITE; m_Data[iit(sizeof(char))] = value; }
		inline void writeUByte(unsigned char value)		{ ARCHIVE_ASSERT_WRITE; m_Data[iit(sizeof(unsigned char))] = value; }
		inline void writeShort(int16_t value)			{ ARCHIVE_ASSERT_WRITE; memcpy(&m_Data[iit(sizeof(int16_t))], &value, sizeof(int16_t)); }
		inline void writeUShort(uint16_t value)			{ ARCHIVE_ASSERT_WRITE; memcpy(&m_Data[iit(sizeof(uint16_t))], &value, sizeof(uint16_t)); };
		inline void writeInt(int32_t value)				{ ARCHIVE_ASSERT_WRITE; memcpy(&m_Data[iit(sizeof(int32_t))], &value, sizeof(int32_t)); }
		inline void writeUInt(uint32_t value)			{ ARCHIVE_ASSERT_WRITE; memcpy(&m_Data[iit(sizeof(uint32_t))], &value, sizeof(uint32_t)); }
		inline void writeLong(int64_t value)			{ ARCHIVE_ASSERT_WRITE; memcpy(&m_Data[iit(sizeof(int64_t))], &value, sizeof(int64_t)); }
		inline void writeULong(uint64_t value)			{ ARCHIVE_ASSERT_WRITE; memcpy(&m_Data[iit(sizeof(uint64_t))], &value, sizeof(uint64_t)); }
		inline void writeFloat(float value)				{ ARCHIVE_ASSERT_WRITE; memcpy(&m_Data[iit(sizeof(float))], &value, sizeof(float)); }
		inline void writeDouble(double value)			{ ARCHIVE_ASSERT_WRITE; memcpy(&m_Data[iit(sizeof(double))], &value, sizeof(double)); }
		inline void writeBool(bool value)				{ ARCHIVE_ASSERT_WRITE; memcpy(&m_Data[iit(sizeof(bool))], &value, sizeof(bool)); }
		
		
		template<typename T>
		void writeArray(T* start, uint64_t count) {
			ARCHIVE_ASSERT_WRITE;

			static const int segmentSize = 65536 * 16;
			writeULong(count);
			uint64_t size = count * sizeof(T);

			if(size > segmentSize) {
				// Size to big, start fragmenting
				uint64_t it = 0;
				uint64_t remainingSize = size;
				while(remainingSize > 0) {
					if (remainingSize > segmentSize) remainingSize = segmentSize;

					memcpy(&m_Data[iit(remainingSize)], &start[it], remainingSize);
					it += remainingSize;

					remainingSize -= segmentSize;
				}

			}
			else {
				memcpy(&m_Data[iit(size)], start, size);
			}
		}

		template<typename T>
		T* readArray(uint64_t* count) {
			ARCHIVE_ASSERT_READ;
			*count = readULong();
			uint64_t size = (*count) * sizeof(T);
			T* arr = (T*) &m_Data[iit(size)];
			return arr;
		}

	private:
		uint64_t iit(uint64_t amount) { uint64_t pos = m_Iterator; m_Iterator += amount; return pos; }

	private:
		ArchiveIOBufferMode m_BufferMode = ArchiveIOBufferMode::None;

		uint64_t m_Size;
		char* m_Data = nullptr;
		//std::vector<char> m_Data;
		uint64_t m_Iterator = 0;
	};

	/// <summary>
	/// 
	/// </summary>
	inline Archive archiveOpen(const char* archivePath, int openFlags, int* errorCode) {
		Archive archive = zip_open(archivePath, openFlags, errorCode);
		if(archive == nullptr) {
			SLD_CORE_ERROR("Error whilst trying to open archive! Error code: {0}", *errorCode);
			return nullptr;
		}
		return archive;
	}

	inline void archiveClose(Archive archive) {
		zip_close(archive);
	}

	/// <summary>
	/// 
	/// </summary>
	inline ArchiveSource archiveSourceBuffer(Archive archive, const void* data, uint64_t size, int free) {
		ArchiveSource source = zip_source_buffer(archive, data, size, free);
		if (source == nullptr) {
			SLD_CORE_ERROR("Failed to create archive source buffer.'{0}'", zip_strerror(archive));
			return nullptr;
		}
		return source;
	}

	/// <summary>
	/// 
	/// </summary>
	inline uint64_t archiveAddFile(Archive archive, const char* filename, ArchiveIOBuffer buffer, int free) {
		ArchiveSource source = archiveSourceBuffer(archive, buffer.getBuffer(), buffer.getBufferSize(), free);
		if (source == nullptr) return -1;

		uint64_t index = zip_file_add(archive, filename, source, ZIP_FL_OVERWRITE);
		if(index < 0) {
			SLD_CORE_ERROR("failed to add file to archive.'{0}'", zip_strerror(archive));
		}
		return index;
	}

	/// <summary>
	/// 
	/// </summary>
	inline ArchiveIOBuffer archiveReadFile(Archive archive, const char* file) {
		zip_file* zipfile = zip_fopen(archive, file, 0);
		struct zip_stat st;
		zip_stat(archive, file, ZIP_STAT_SIZE, &st);
		char* buffer = (char*) malloc((size_t)st.size);

		uint64_t readBytes = zip_fread(zipfile, buffer, st.size);
		return ArchiveIOBuffer(buffer, readBytes);
	}

	/// <summary>
	/// 
	/// </summary>
	inline ArchiveIOBuffer archiveWriteBuffer(Archive archive, uint64_t size) {
		return ArchiveIOBuffer(size);
	}

	/// <summary>
	/// 
	/// </summary>
	inline std::string archiveErrorToString(ArchiverErrors_ error) {
		std::string s = "Archiver error: ";
		return s.append(std::to_string(error));
	}
}
