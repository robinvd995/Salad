#pragma once

#include "TextureAsset.hpp"
#include "ShaderAsset.hpp"

#include "Salad/Util/Archiver.hpp"

#include "stb_image/stb_image.h"

namespace Salad::Asset {

	/// <summary>
	/// Data Template:		size = 10 + TextureWidth * TextureHeight * Format.Channels
	///		ubyte		:	Format
	///		uint16_t	:	TextureWidth
	///		uint16_t	:	TextureHeight
	///		ubyte		:	MinFilter
	///		ubyte		:	MagFilter
	///		ubyte		:	WrapS
	///		ubyte		:	WrapT
	///		ubyte		:	WrapR
	///		float[]		:	TextureData		size = TextureWidth * TextureHeight
	/// </summary>
	/// <param name="asset">The TextureAsset object containing all the required information</param>
	inline void exportTextureAsset(Util::ArchiveIOBuffer& buffer, AssetBase* asset) {
		TextureAsset* texture = (TextureAsset*)asset;
		exportTextureAsset(buffer, texture);
	}

	inline void exportTextureAsset(Util::ArchiveIOBuffer& buffer, TextureAsset* texture) {
		//std::string& resourceId = transformAssetIdToResourceId(asset.getFilePath());
		uint64_t textureDataSize = ((uint64_t)texture->getTextureWidth() * (uint64_t)texture->getTextureHeight()) * (uint64_t)4; // TODO: replace (uint64_t)4 with the amount of channels for the given dataformat
		//Util::ArchiveIOBuffer buffer = Util::archiveWriteBuffer(archive, 10 + textureDataSize);
		buffer.writeUByte(0); // asset.getTextureFormat();
		buffer.writeUShort(texture->getTextureWidth());
		buffer.writeUShort(texture->getTextureHeight());
		buffer.writeUByte((unsigned char)texture->getTextureMinFilter());
		buffer.writeUByte((unsigned char)texture->getTextureMagFilter());
		buffer.writeUByte((unsigned char)texture->getTextureWrapS());
		buffer.writeUByte((unsigned char)texture->getTextureWrapT());
		buffer.writeUByte((unsigned char)texture->getTextureWrapR());

		int width, height, channels;

		// validate width, height and channels here

		stbi_uc* data = stbi_load(texture->getFilePath().c_str(), &width, &height, &channels, 0);
		buffer.writeArray<unsigned char>(&data[0], textureDataSize);
		stbi_image_free(data);
	}

	inline void exportShaderAsset(Util::ArchiveIOBuffer& buffer, AssetBase* asset) {
		ShaderAsset* shader = (ShaderAsset*)asset;
		exportShaderAsset(buffer, shader);
	}

	inline void exportShaderAsset(Util::ArchiveIOBuffer buffer, ShaderAsset* shader) {
	
	}
}
