#pragma once

#include "Salad/Core/Core.h"

#include <string>

namespace Salad {

	class Texture {

	public:
		virtual ~Texture() = default;

		virtual uint32_t getWidth() const = 0;
		virtual uint32_t getHeight() const = 0;
		virtual uint32_t getRendererId() const = 0;

		virtual void setData(void* data, uint32_t size) = 0;

		virtual void bind(uint32_t slot = 0) const = 0;
	};

	class Texture2D : public Texture {

		friend class TextureManager;

	private:
		static Ref<Texture2D> create(const std::string& filepath);
		static Ref<Texture2D> create(uint32_t width, uint32_t height);
	};

	class TextureCubeMap : public Texture {

	public:
		static Ref<TextureCubeMap> create(const std::string& filepath, const std::string& extension);
	};
}
