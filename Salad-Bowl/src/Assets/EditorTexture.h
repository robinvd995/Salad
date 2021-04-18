#pragma once

#include "Salad/Renderer/Texture.h"
#include <iostream>

namespace Salad {

	class EditorTexture {
	
	public:
		EditorTexture() = delete;
		EditorTexture(std::string& path) : m_FilePath(path) {}
		EditorTexture(const EditorTexture& other) = default;
		~EditorTexture() = default;

		std::string& getFilePath() { return m_FilePath; }
		uint32_t getTextureRenderId() { return m_Texture->getRendererId(); }

		void loadTexture() {
			m_Texture = Texture2D::create(m_FilePath);
		}

	private:
		std::string m_FilePath;
		Ref<Texture2D> m_Texture;
	};

}