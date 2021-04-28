#pragma once

#include "Texture.h"
#include "Salad/Util/Archiver.hpp"

namespace Salad {

	Ref<Texture2D> loadTexture2D(const char* filepath) {
		using namespace Salad::Util;

		int error = 0;
		Archive archive = archiveOpen("resources.zip", ArchiverOpenFlags_None, &error);

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
		Ref<Texture2D> texture = Texture2D::create(spec, textureData);

		buffer.freeBuffer();
		archiveClose(archive);

		return texture;
	}

}
