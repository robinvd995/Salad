#pragma once

#include "Texture.h"
#include "Salad/Util/Archiver.hpp"

#include "Salad/Core/ResourceManager.h"

namespace Salad {

	namespace Defaults {
		inline Texture2D* loadDefaultTexture2D() {
			unsigned char defaultTextureData[16] = {
				255, 000, 255, 255,
				000, 000, 000, 255,
				255, 000, 255, 255,
				000, 000, 000, 255
			};
			TextureSpecification spec;
			spec.format = TextureFormat::RGBA;
			spec.width = 2;
			spec.height = 2;
			spec.filterWrapSpec.minFilter = TextureMinFilterSpecification::Nearest;
			spec.filterWrapSpec.magFilter = TextureMagFilterSpecification::Nearest;
			spec.filterWrapSpec.wrapS = TextureWrapSpecification::Repeat;
			spec.filterWrapSpec.wrapT = TextureWrapSpecification::Repeat;
			spec.filterWrapSpec.wrapR = TextureWrapSpecification::Repeat;

			Texture2D* defaultTexture = Texture2D::createPtr(spec, &defaultTextureData[0]);
			return defaultTexture;
		}
	}

	inline Texture2D* loadTexture2D(const char* filepath) {
		using namespace Salad::Util;

		int error = 0;
		Archive archive = archiveOpen(ResourceManager::getResourceArchiveLocation().c_str(), ArchiverOpenFlags_None, &error);

		// error handling here

		ArchiveIOBuffer buffer = archiveReadFile(archive, filepath);

		TextureSpecification spec;

		spec.format = static_cast<TextureFormat>((int)buffer.readUByte());
		spec.width = buffer.readUShort();
		spec.height = buffer.readUShort();
		spec.filterWrapSpec.minFilter = static_cast<TextureMinFilterSpecification>((int)buffer.readByte());
		spec.filterWrapSpec.magFilter = static_cast<TextureMagFilterSpecification>((int)buffer.readByte());
		spec.filterWrapSpec.wrapS = static_cast<TextureWrapSpecification>((int)buffer.readByte());
		spec.filterWrapSpec.wrapT = static_cast<TextureWrapSpecification>((int)buffer.readByte());
		spec.filterWrapSpec.wrapR = static_cast<TextureWrapSpecification>((int)buffer.readByte());

		uint64_t count = 0;
		unsigned char* textureData = buffer.readArray<unsigned char>(&count);
		Texture2D* texture = Texture2D::createPtr(spec, textureData);

		buffer.freeBuffer();
		archiveClose(archive);

		return texture;
	}

}
