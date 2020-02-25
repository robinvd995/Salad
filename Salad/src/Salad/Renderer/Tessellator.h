#pragma once

namespace Salad {

	enum class TessellatorMode {
		NONE = 0,
		TRIANGLES = 1,
		QUADS = 2
	};

	class Tessellator {
	
	public:

		static Tessellator& get() {
			static Tessellator instance;
			return instance;
		}

		static void start(TessellatorMode mode) { get().startImpl(mode); }
		static void end() { get().endImpl(); }

		static void pushVertex(float x, float y, float z, float u, float v) { get().pushVertexImpl(x, y, z, u, v); }

		static void pushQuad(float x, float y, float z, float width, float height, float u, float v, float texWidth, float texHeight) { get().pushQuadImpl(x, y, z, width, height, u, v, texWidth, texHeight); }

		static void makeTriangle() { get().makeTriangleImpl(); }
		static void makeQuad() { get().makeQuadImpl(); }

		static void reserveVertexSize(uint32_t size) { return get().m_VertexBuffer.reserve(size); }
		static void reserveIndexSize(uint32_t size) { return get().m_IndexBuffer.reserve(size); }

		static std::vector<float>& getVertexBuffer() { return get().m_VertexBuffer; }
		static std::vector<uint32_t>& getIndexBuffer() { return get().m_IndexBuffer; }

		static void setTextureResolution(float resolution) { get().m_TextureResolution = resolution; }

	private:
		Tessellator() {}
		Tessellator(const Tessellator& other) = delete;
		~Tessellator() {}

		void startImpl(TessellatorMode mode);
		void endImpl();

		void pushVertexImpl(float x, float y, float z, float u, float v);

		void pushQuadImpl(float x, float y, float z, float width, float height, float u, float v, float texWidth, float texHeight);

		void makeTriangleImpl();
		void makeQuadImpl();

		Tessellator& operator= (const Tessellator&) = delete;

	private:
		std::vector<float> m_VertexBuffer;
		std::vector<uint32_t> m_IndexBuffer;
		uint32_t m_Index = 0;
		TessellatorMode m_Mode = TessellatorMode::NONE;
		float m_TextureResolution = 1.0f;
	};
}