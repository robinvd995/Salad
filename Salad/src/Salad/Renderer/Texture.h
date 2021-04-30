#pragma once

#include "Salad/Core/Core.h"

#include <string>

namespace Salad {

	enum class TextureMinFilterSpecification {
		Linear = 0,
		Nearest = 1,
		NearestMipmapNearest = 2,
		LinearMipmapNearest = 3,
		NearestMipmapLinear = 4,
		LinearMipmapLinear = 5
	};

	enum class TextureMagFilterSpecification {
		Linear = 0,
		Nearest = 1
	};

	enum class TextureWrapSpecification {
		Repeat = 0,
		ClampToEdge = 1,
		ClampToBorder = 2,
		MirroredRepeat = 3,
		MirroredClampToEdge = 4
	};

	enum class TextureFormat {
		RGBA = 0,
		RGB = 1
	};

	struct TextureFilterWrapSpecification {
		TextureMinFilterSpecification minFilter = TextureMinFilterSpecification::Linear;
		TextureMagFilterSpecification magFilter = TextureMagFilterSpecification::Linear;

		TextureWrapSpecification wrapS = TextureWrapSpecification::Repeat;
		TextureWrapSpecification wrapT = TextureWrapSpecification::Repeat;
		TextureWrapSpecification wrapR = TextureWrapSpecification::Repeat;
	};

	struct TextureSpecification {
		TextureFormat format;
		uint32_t width, height;

		TextureFilterWrapSpecification filterWrapSpec;
	};

	class Texture {

	public:
		virtual ~Texture() = default;

		virtual uint32_t getWidth() const = 0;
		virtual uint32_t getHeight() const = 0;
		virtual uint32_t getRendererId() const = 0;

		virtual void setData(void* data, uint32_t size) = 0;

		virtual void updateFilterWrapSpecification(TextureFilterWrapSpecification& spec) = 0;

		virtual void bind(uint32_t slot = 0) const = 0;
	};

	class Texture2D : public Texture {

		friend class TextureManager;

	public:
		virtual ~Texture2D() = default;

		static Ref<Texture2D> create(const std::string& filepath, TextureFilterWrapSpecification filterWrapSpec);
		static Ref<Texture2D> create(uint32_t width, uint32_t height);
		static Ref<Texture2D> create(uint32_t width, uint32_t height, unsigned char* data);
		static Ref<Texture2D> create(TextureSpecification& specs, unsigned char* data);
		static Texture2D* createPtr(TextureSpecification& specs, unsigned char* data);
	};

	class TextureCubeMap : public Texture {

	public:
		static Ref<TextureCubeMap> create(const std::string& filepath, const std::string& extension);
	};
}
