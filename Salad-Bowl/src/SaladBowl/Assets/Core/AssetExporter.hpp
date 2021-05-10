#pragma once

#include "SaladBowl/Assets/TextureAsset.hpp"
#include "SaladBowl/Assets/ShaderAsset.hpp"

#include "Salad/Util/Archiver.hpp"

#include "stb_image/stb_image.h"

namespace Salad::Asset {

	////////////////////////////////////////////////////////////////////////////////////////////////
	// >>> REMEMBER!!! WRITING AN ARRAY ADDS ANOTHER 8 BYTES BECAUSE OF THE SIZE OF THE ARRAY <<< //
	////////////////////////////////////////////////////////////////////////////////////////////////

	/// <summary>
	/// 
	/// Data Template	:	size = 10 + TextureWidth * TextureHeight * Format.Channels + 8 (Array Overhead)
	///		ubyte		:	Format
	///		uint16_t	:	TextureWidth
	///		uint16_t	:	TextureHeight
	///		ubyte		:	MinFilter
	///		ubyte		:	MagFilter
	///		ubyte		:	WrapS
	///		ubyte		:	WrapT
	///		ubyte		:	WrapR
	///		float[]		:	TextureData		size = TextureWidth * TextureHeight
	/// 
	/// </summary>
	/// <param name="asset">The TextureAsset object containing all the required information</param>
	inline void exportTextureAsset(Util::ArchiveIOBuffer& buffer, TextureAsset* texture) {
		uint64_t textureDataSize = ((uint64_t)texture->getTextureWidth() * (uint64_t)texture->getTextureHeight()) * (uint64_t)4; // TODO: replace (uint64_t)4 with the amount of channels for the given dataformat
		buffer.writeUByte(0); // asset.getTextureFormat();
		buffer.writeUShort(texture->getTextureWidth());
		buffer.writeUShort(texture->getTextureHeight());
		buffer.writeUByte((unsigned char)texture->getTextureMinFilter());
		buffer.writeUByte((unsigned char)texture->getTextureMagFilter());
		buffer.writeUByte((unsigned char)texture->getTextureWrapS());
		buffer.writeUByte((unsigned char)texture->getTextureWrapT());
		buffer.writeUByte((unsigned char)texture->getTextureWrapR());

		int width, height, channels;

		// TODO validate width, height and channels here

		stbi_uc* data = stbi_load(texture->getFilePath().c_str(), &width, &height, &channels, 0);
		buffer.writeArray<unsigned char>(&data[0], textureDataSize);
		stbi_image_free(data);
	}

	/// <summary>
	/// 
	/// DataTemplate:	:	size = 8 + VertexShaderLength + FragmentShaderLength + 16 (Array Overhead)
	///		uint32_t	:	VertexShaderLength (0 = no vertex shader)
	///		char[]		:	VertexShaderSource
	///		uint32_t	:	FragmentShaderLength (0 = no fragment shader)
	///		char[]		:	FragmentShaderSource
	/// 
	/// </summary>
	/// <param name="buffer"></param>
	/// <param name="shader"></param>
	inline void exportShaderAsset(Util::ArchiveIOBuffer buffer, ShaderAsset* shader) {

		// write vertex stage
		if(shader->hasStage(ShaderStageType::Vertex)) {
			ShaderAssetStage& vertexStage = shader->getStage(ShaderStageType::Vertex);
			uint64_t stageSize = vertexStage.shaderSource.size();
			buffer.writeUInt(stageSize);
			buffer.writeArray<char>(&vertexStage.shaderSource[0], stageSize);
		}
		else {
			buffer.writeUInt(0);
		}

		// write fragment stage
		if(shader->hasStage(ShaderStageType::Fragment)) {
			ShaderAssetStage& fragmentStage = shader->getStage(ShaderStageType::Fragment);
			uint64_t stageSize = fragmentStage.shaderSource.size();
			buffer.writeUInt(stageSize);
			buffer.writeArray<char>(&fragmentStage.shaderSource[0], stageSize);
		}
		else {
			buffer.writeUInt(0);
		}
	}
}
