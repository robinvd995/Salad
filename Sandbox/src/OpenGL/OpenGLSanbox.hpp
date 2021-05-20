#pragma once

#include "OpenGlSandboxConstants.h"
#include "Noise.h"

#include "Salad/Core/Layer.h"
#include "Salad/Util/DataBuffers.hpp"

#include "Salad/Renderer/Renderer.h"
#include "Salad/Renderer/Texture.h"

#include <glm/glm.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

#include "SaladBowl/Assets/Io/ShaderCompiler.h"

#include <imgui/imgui.h>

#include <fstream>
#include <filesystem>
#include <chrono>

namespace Salad {

	class Timer {
	
	public:

		void start() {
			m_StartTime = std::chrono::high_resolution_clock::now();
		}

		void end() {
			m_EndTime = std::chrono::high_resolution_clock::now();
		}

		float getTimeMillis() {
			std::chrono::duration<float, std::milli> ms_float = m_EndTime - m_StartTime;
			return ms_float.count();
		}

	private:
		std::chrono::steady_clock::time_point m_StartTime;
		std::chrono::steady_clock::time_point m_EndTime;
	};

	class RenderDirective {

		virtual void execute() = 0;

	};

	class RenderQueue {

	};

	struct MaterialTexture {
		Ref<Texture2D> m_Texture;
		uint32_t m_TextureBinding;

		MaterialTexture() = default;
		MaterialTexture(const MaterialTexture&) = default;
		MaterialTexture(Ref<Texture2D> texture, uint32_t binding) :
			m_Texture(texture), m_TextureBinding(binding) 
		{}
		~MaterialTexture() = default;
	};

	class Material {

	public:
		Material() = delete;
		~Material() = default;

		Material(uint32_t size, char* data) :
			m_BufferSize(size)
		{
			m_MaterialBuffer.allocate<char>(m_BufferSize);
			m_MaterialBuffer.writeArray<char>(data, m_BufferSize);
		}

		void bind() {
			UniformBuffer* materialBuffer = Renderer::getUniformBuffer(1); // TODO get material binding by some constant
			materialBuffer->updateBuffer(0, m_BufferSize, m_MaterialBuffer.get<char>());

			for(MaterialTexture* texture : m_TextureBuffer) texture->m_Texture->bind(texture->m_TextureBinding);
		}

		void attachTexture(Ref<Texture2D> texture, uint32_t binding) {
			MaterialTexture* materialTexture = new MaterialTexture(texture, binding);
			m_TextureBuffer.push_back(materialTexture);
		}

		void update(char* data) {
			m_MaterialBuffer.moveIterator(0);
			m_MaterialBuffer.writeArray<char>(data, m_BufferSize);
		}

		void free() {
			m_MaterialBuffer.freeBuffer();
		}

	private:
		uint32_t m_BufferSize;

		Util::ByteBuffer m_MaterialBuffer;
		std::vector<MaterialTexture*> m_TextureBuffer;
	};

	class OpenGLSandbox : public Layer {

	public:
		OpenGLSandbox() : Layer("OpenGLSandbox") {}
		~OpenGLSandbox() = default;

		void addVertex(std::vector<float>& data, float x, float y, float z, float u, float v, float normalX, float normalY, float normalZ) {
			data.push_back(x);
			data.push_back(y);
			data.push_back(z);

			data.push_back(u);
			data.push_back(v);

			data.push_back(normalX);
			data.push_back(normalY);
			data.push_back(normalZ);
		}

		void loadTerrain() {

			const static uint32_t terrainVertexSize = 8;
			const static uint32_t waterVertexSize = 7;

			Timer timer;
			timer.start();
			float amplitude = 8.0f;

			float terrainWidth = 512;
			float terrainHeight = 512;
			float terrainResolution = 4.0f;
			float terrainPow = 4.0f;

			float seaLevel = 2.0f;

			Timer noiseTimer;
			noiseTimer.start();
			NoiseData noise = generateNoisePerlin2D(512, 512, 1.2f, 6, 0.5f, 2.0f, false);
			noiseTimer.end();
			std::cout << "Noise gen time: " << noiseTimer.getTimeMillis() << "ms" << std::endl;

			std::vector<float> vertexData;
			std::vector<uint32_t> indices;

			std::vector<float> waterVertexData;
			std::vector<uint32_t> waterIndices;

			vertexData.resize(noise.m_Data.size() * terrainVertexSize);
			uint32_t indicesCount = ((noise.c_Width * noise.c_Height) - (noise.c_Width + 1)) * 6;
			indices.resize(indicesCount);

			waterVertexData.resize(noise.m_Data.size() * waterVertexSize);
			waterIndices.resize(indicesCount);

			for(uint32_t i = 0; i < noise.m_Data.size(); i++) {
				int x = i % noise.c_Width;
				int y = i / noise.c_Width;

				float height = pow(noise.m_Data[i], terrainPow) * amplitude;

				int ileft = i - 1;
				int iright = i + 1;
				int itop = i - noise.c_Width;
				int ibot = i + noise.c_Width;

				if (ileft < 0) ileft = i;
				if (iright >= noise.c_Width) iright = i;
				if (itop < 0) itop = i;
				if (ibot >= noise.c_Height) ibot = i;

				float left = pow(noise.m_Data[ileft], 4) * amplitude;;
				float right = pow(noise.m_Data[iright], 4) * amplitude;
				float top = pow(noise.m_Data[itop], 4) * amplitude;
				float bot = pow(noise.m_Data[ibot], 4) * amplitude;

				glm::vec3 normal = glm::normalize(glm::vec3(left - right, 2.0f, bot - top));

				vertexData[(size_t)i * terrainVertexSize + 0] = (float)x / terrainResolution;
				vertexData[(size_t)i * terrainVertexSize + 1] = height;
				vertexData[(size_t)i * terrainVertexSize + 2] = (float)y / terrainResolution;

				vertexData[(size_t)i * terrainVertexSize + 3] = (float)x / (noise.c_Width - 1);
				vertexData[(size_t)i * terrainVertexSize + 4] = (float)y / (noise.c_Height - 1);

				vertexData[(size_t)i * terrainVertexSize + 5] = normal.x;
				vertexData[(size_t)i * terrainVertexSize + 6] = normal.y;
				vertexData[(size_t)i * terrainVertexSize + 7] = normal.z;

				waterVertexData[(size_t)i * waterVertexSize + 0] = (float)x / terrainResolution;
				waterVertexData[(size_t)i * waterVertexSize + 1] = seaLevel;
				waterVertexData[(size_t)i * waterVertexSize + 2] = (float)y / terrainResolution;

				waterVertexData[(size_t)i * waterVertexSize + 3] = 0.0f;
				waterVertexData[(size_t)i * waterVertexSize + 4] = 1.0f;
				waterVertexData[(size_t)i * waterVertexSize + 5] = 0.0f;

				waterVertexData[(size_t)i * waterVertexSize + 6] = seaLevel - height;

				if (x == noise.c_Width - 1) continue;
				if (y == noise.c_Height - 1) continue;

				int a = i;
				int b = a + 1;
				int c = a + noise.c_Width;
				int d = c + 1;

				indices[(size_t)i * 6 + 0] = a;
				indices[(size_t)i * 6 + 1] = c;
				indices[(size_t)i * 6 + 2] = b;

				indices[(size_t)i * 6 + 3] = c;
				indices[(size_t)i * 6 + 4] = d;
				indices[(size_t)i * 6 + 5] = b;

				waterIndices[(size_t)i * 6 + 0] = a;
				waterIndices[(size_t)i * 6 + 1] = c;
				waterIndices[(size_t)i * 6 + 2] = b;

				waterIndices[(size_t)i * 6 + 3] = c;
				waterIndices[(size_t)i * 6 + 4] = d;
				waterIndices[(size_t)i * 6 + 5] = b;
			}

			m_Terrain = VertexArray::create();

			Ref<VertexBuffer> vertexBuffer = VertexBuffer::create(vertexData.data(), vertexData.size() * sizeof(float), SLD_STATIC_DRAW);
			vertexBuffer->setLayout({
				{ "a_Position", Salad::ShaderDataType::Float3 },
				{ "a_TexCoord", Salad::ShaderDataType::Float2 },
				{ "a_Normal", Salad::ShaderDataType::Float3 }
			});

			m_Terrain->addVertexBuffer(vertexBuffer);
			Ref<IndexBuffer> indexBuffer = IndexBuffer::create(indices.data(), indices.size());
			m_Terrain->setIndexBuffer(indexBuffer);
			m_Terrain->unbind();

			m_TerrainTransform.setPosition(0.0f, -5.0f, 0.0f);
			m_TerrainTransform.calculateTransformationMatrix();

			m_Water = VertexArray::create();

			Ref<VertexBuffer> waterVertexBuffer = VertexBuffer::create(waterVertexData.data(), waterVertexData.size() * sizeof(float), SLD_STATIC_DRAW);
			waterVertexBuffer->setLayout({
				{ "a_Position", Salad::ShaderDataType::Float3 },
				{ "a_Normal", Salad::ShaderDataType::Float3 },
				{ "a_SurfaceDistance", Salad::ShaderDataType::Float }
			});

			m_Water->addVertexBuffer(waterVertexBuffer);
			Ref<IndexBuffer> waterIndexBuffer = IndexBuffer::create(waterIndices.data(), waterIndices.size());
			m_Water->setIndexBuffer(waterIndexBuffer);
			m_Water->unbind();

			timer.end();
			std::cout << "Terrain total load time: " << timer.getTimeMillis() << "ms" << std::endl;
		}

		void exportShader(const std::string shaderfile, bool forceBuild = false) {

			if (std::filesystem::exists(shaderfile + ".shader") && !forceBuild) return;

			Asset::ShaderCompilerFlags flags = Asset::ShaderCompilerFlags_CompileOpenGL;
			Asset::ShaderCompiler compiler(shaderfile + ".glsl", flags);

			if(compiler.hasCompilerErrors(Asset::ShaderCompilerFlags_CompileOpenGL)) {
				std::cout << compiler.getCompilerErrorMessages(Asset::ShaderCompilerFlags_CompileOpenGL) << std::endl;
			}

			Asset::ShaderConverter converter(&compiler);
			Asset::ShaderExporter exporter(&converter);

			Util::ByteBuffer* buffer = exporter.buffer();

			std::ofstream out(shaderfile + ".shader");
			out << *buffer;
			out.close();

			exporter.cleanup();
		}

		virtual void onAttach() override {

			Salad::RenderCommand::setClearColor(glm::vec4(0.05f, 0.05f, 0.05f, 1.0f));
			m_Camera.setPerspectiveProjection(PerspectiveCameraProperties());

			m_Cube = VertexArray::create();

			Ref<Salad::VertexBuffer> vertexBuffer = VertexBuffer::create(Constants::vertexData, sizeof(Constants::vertexData), SLD_STATIC_DRAW);
			vertexBuffer->setLayout({
				{ "a_Position", Salad::ShaderDataType::Float3 },
				{ "a_TexCoord", Salad::ShaderDataType::Float2 },
				{ "a_Normal",   Salad::ShaderDataType::Float3 }
			});

			m_Cube->addVertexBuffer(vertexBuffer);
			Ref<IndexBuffer> indexBuffer = IndexBuffer::create(Constants::indexData, sizeof(Constants::indexData));
			m_Cube->setIndexBuffer(indexBuffer);
			m_Cube->unbind();

			// Export shaders
			
			exportShader("assets/shaders/FlatColor", false);
			exportShader("assets/shaders/Terrain", false);
			exportShader("assets/shaders/Water", true);

			// Load shaders

			std::ifstream fileShader("assets/shaders/FlatColor.shader");
			std::string contents((std::istreambuf_iterator<char>(fileShader)), std::istreambuf_iterator<char>());
			Util::ByteBuffer byteBuffer(contents.data(), contents.size());

			m_RedShader = Shader::create("FlatColorRed", &byteBuffer);
			m_BlueShader = Shader::create("FlatColorBlue", &byteBuffer);

			std::ifstream terrainShaderFile("assets/shaders/Terrain.shader");
			std::string terrainShaderContents((std::istreambuf_iterator<char>(terrainShaderFile)), std::istreambuf_iterator<char>());
			Util::ByteBuffer terrainShaderBuffer(terrainShaderContents.data(), terrainShaderContents.size());

			m_TerrainShader = Shader::create("Terrain", &terrainShaderBuffer);

			std::ifstream waterShaderFile("assets/shaders/Water.shader");
			std::string waterShaderContents((std::istreambuf_iterator<char>(waterShaderFile)), std::istreambuf_iterator<char>());
			Util::ByteBuffer waterShaderBuffer(waterShaderContents.data(), waterShaderContents.size());

			m_WaterShader = Shader::create("Water", &waterShaderBuffer);

			// Load noise texture
			NoiseData noise = generateNoisePerlin2D(256, 256, m_NoiseTextureScale, m_NoiseTextureOctaves,
				m_NoiseTexturePersistance, m_NoiseTextureLacunarity, m_NoiseTextureAbsolute);
			std::vector<unsigned char> noiseTextureData;
			noiseTextureData.resize((size_t)noise.c_Width * noise.c_Height * 4);
			for(int i = 0; i < noise.m_Data.size(); i++) {
				unsigned char value = noise.m_Data[i] * 255;
				noiseTextureData[(size_t)i * 4 + 0] = value;
				noiseTextureData[(size_t)i * 4 + 1] = value;
				noiseTextureData[(size_t)i * 4 + 2] = value;
				noiseTextureData[(size_t)i * 4 + 3] = 255;
			}

			TextureSpecification textureSpecs;
			textureSpecs.filterWrapSpec.minFilter = TextureMinFilterSpecification::Nearest;
			textureSpecs.filterWrapSpec.magFilter = TextureMagFilterSpecification::Nearest;
			textureSpecs.width = noise.c_Width;
			textureSpecs.height = noise.c_Height;
			textureSpecs.format = TextureFormat::RGBA;

			m_NoiseTexture = Texture2D::create(textureSpecs, &noiseTextureData[0]);

			loadTerrain();

			// Create uniform buffers

			Renderer::registerUniformBuffer(0, 256);
			Renderer::registerUniformBuffer(1, 1024);
			Renderer::registerUniformBuffer(2, 256);
			Renderer::registerUniformBuffer(3, 256);
			Renderer::registerUniformBuffer(4, 64);

			// Create materials
			Util::ByteBuffer materialBuffer;
			materialBuffer.allocate<char>(sizeof(glm::vec4));

			materialBuffer.write<float>(1.0f);
			materialBuffer.write<float>(0.0f);
			materialBuffer.write<float>(0.0f);
			materialBuffer.write<float>(1.0f);

			m_RedMaterial = new Material(materialBuffer.getBufferSize(), materialBuffer.get<char>());
			Ref<Texture2D> redTexture = Texture2D::create("assets/textures/crate_diffuse.png", TextureFilterWrapSpecification());
			m_RedMaterial->attachTexture(redTexture, 0);

			materialBuffer.moveIterator(0);
			materialBuffer.write<float>(0.0f);
			materialBuffer.write<float>(0.0f);
			materialBuffer.write<float>(1.0f);
			materialBuffer.write<float>(1.0f);
			
			m_BlueMaterial = new Material(materialBuffer.getBufferSize(), materialBuffer.get<char>());
			//Ref<Texture2D> blueTexture = Texture2D::create("assets/textures/checkerboard.png", TextureFilterWrapSpecification());
			m_BlueMaterial->attachTexture(m_NoiseTexture, 0);

			materialBuffer.moveIterator(0);
			materialBuffer.write<float>(0.0f);
			materialBuffer.write<float>(0.8f);
			materialBuffer.write<float>(0.0f);
			materialBuffer.write<float>(1.0f);

			m_TerrainMaterial = new Material(materialBuffer.getBufferSize(), materialBuffer.get<char>());

			Util::ByteBuffer waterMaterialBuffer;
			waterMaterialBuffer.allocate<char>(sizeof(glm::vec4) * 2);
			waterMaterialBuffer.moveIterator(0);

			// Bump0 properties
			waterMaterialBuffer.write<float>(32.0f);
			waterMaterialBuffer.write<float>(0.05f);
			waterMaterialBuffer.write<float>(0.5f);
			waterMaterialBuffer.write<float>(0.25f);

			// Bump1 properties
			waterMaterialBuffer.write<float>(64.0f);
			waterMaterialBuffer.write<float>(0.01f);
			waterMaterialBuffer.write<float>(0.25f);
			waterMaterialBuffer.write<float>(0.125f);

			m_WaterMaterial = new Material(waterMaterialBuffer.getBufferSize(), waterMaterialBuffer.get<char>());
			Ref<Texture2D> waterBump0 = Texture2D::create("assets/textures/water_bump_0.png", TextureFilterWrapSpecification());
			Ref<Texture2D> waterBump1 = Texture2D::create("assets/textures/water_bump_1.png", TextureFilterWrapSpecification());
			m_WaterMaterial->attachTexture(waterBump0, 0);
			m_WaterMaterial->attachTexture(waterBump1, 1);

			// Cube transforms

			m_Red0Transform.setPosition(-2.0f, -2.0f, 0.0f);
			m_Red0Transform.calculateTransformationMatrix();

			m_Blue0Transform.setPosition(2.0f, -2.0f, 0.0f);
			m_Blue0Transform.calculateTransformationMatrix();

			m_Red1Transform.setPosition(2.0f, 2.0f, 0.0f);
			m_Red1Transform.calculateTransformationMatrix();

			m_Blue1Transform.setPosition(0.0f, 10.0f, 0.0f);
			m_Blue1Transform.setScale(8.0f, 8.0f, 0.1f);
			m_Blue1Transform.calculateTransformationMatrix();

			// Camera Transforms
			m_CameraTransform.setPosition(0.0f, 10.0f, 10.0f);
			m_CameraTransform.calculateTransformationMatrix();

			//m_DirectionalLightTransform.calculateTransformationMatrix();
		}

		virtual void onDetach() override {}

		void moveCamera(Timestep ts) {

			if (!Input::isMouseButtonPressed(1)) {
				Input::lockMouseToCenter(false);
				return;
			}

			const float sensitivity = 0.0005f;

			float mdx = Input::getMouseDeltaX() * sensitivity;
			float mdy = Input::getMouseDeltaY() * sensitivity;

			glm::quat orientation = glm::normalize(m_CameraTransform.getOrientation());
			glm::quat pitch = glm::quat(glm::vec3(-mdy, 0.0f, 0.0f));
			glm::quat yaw   = glm::quat(glm::vec3(0.0f, -mdx, 0.0f));

			m_CameraTransform.setOrientation(yaw * orientation * pitch);

			glm::vec3 translation{ 0.0f, 0.0f, 0.0f };

			float speed = 10.0f;
			if (Input::isKeyPressed(SLD_KEY_LEFT_SHIFT)) speed = speed * 10.0f;

			if (Input::isKeyPressed(SLD_KEY_W)) translation.z -= speed * ts;
			if (Input::isKeyPressed(SLD_KEY_S)) translation.z += speed * ts;
			if (Input::isKeyPressed(SLD_KEY_A)) translation.x -= speed * ts;
			if (Input::isKeyPressed(SLD_KEY_D)) translation.x += speed * ts;

			m_CameraTransform.translate(translation.z, glm::vec3(0.0f, 0.0f, 1.0f));
			m_CameraTransform.translate(translation.x, glm::vec3(1.0f, 0.0f, 0.0f));

			m_CameraTransform.calculateTransformationMatrix();
			Input::lockMouseToCenter(true);
		}

		virtual void onUpdate(Timestep ts) override {

			moveCamera(ts);

			timer = timer + ts;

			glm::mat4 camTransform = glm::mat4(1.0f);
			camTransform[3][2] = 10.0f;
			m_Camera.recalculateViewMatrix();
			glm::mat4 view = glm::inverse(camTransform);

			glm::vec3 angleX = { 1.0f, 0.0f, 0.0f };
			glm::vec3 angleY = { 0.0f, 1.0f, 0.0f };
			glm::vec3 angleZ = { 0.0f, 0.0f, 1.0f };

			m_Red0Transform.rotate(0.01f, angleY);
			m_Red0Transform.translateForward(-0.03f);
			m_Red0Transform.calculateTransformationMatrix();

			m_Red1Transform.rotate(-0.01f, angleY);
			m_Red1Transform.translateLeft(-0.01f);
			m_Red1Transform.calculateTransformationMatrix();

			m_Blue0Transform.rotate(0.01f, angleX);
			m_Blue0Transform.translateUp(0.02f);
			m_Blue0Transform.calculateTransformationMatrix();

			//m_Blue1Transform.rotate(-0.01f, angleX);
			//m_Blue1Transform.calculateTransformationMatrix();

			Salad::RenderCommand::clear();

			// Enable blend
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			// Enable back face culling
			glEnable(GL_CULL_FACE);
			glCullFace(GL_FRONT);

			// Bind the cube vao to draw
			m_Cube->bind();

			// Get the uniform buffers
			UniformBuffer* sceneBuffer = Renderer::getUniformBuffer(0);
			UniformBuffer* materialBuffer = Renderer::getUniformBuffer(1);
			UniformBuffer* transformBuffer = Renderer::getUniformBuffer(2);
			UniformBuffer* lightBuffer = Renderer::getUniformBuffer(3);
			UniformBuffer* timeBuffer = Renderer::getUniformBuffer(4);

			// Update scene buffer with projection and view matrix
			glm::mat4 camMatrix = m_CameraTransform.getMatrix();
			glm::mat4 viewmatrix = glm::inverse(m_CameraTransform.getMatrixCpy());
			sceneBuffer->updateBuffer(0, sizeof(glm::mat4), glm::value_ptr(m_Camera.getProjection()));
			sceneBuffer->updateBuffer(sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(viewmatrix));

			m_LightTransform.setOrientation(glm::quat(glm::vec3((-m_LightDirX) * 3.141519f, 0.0f, (-m_LightDirZ) * 3.141519f)));
			glm::vec3 lightDir = m_LightTransform.getOrientation() * glm::vec3(0.0f, 0.0f, 1.0f);
			lightBuffer->updateBuffer(0, sizeof(glm::vec3), glm::value_ptr(lightDir));

			// Update time and buffer
			float sinTimer = (sin(timer) + 1.0f) * 0.5f;
			float normalizedTime = timer - (int)timer;
			timeBuffer->updateBuffer(0, sizeof(float), &timer);
			timeBuffer->updateBuffer(sizeof(float), sizeof(float), &normalizedTime);

			// Bind the shader
			m_BlueShader->bind();

			// Update material buffer with blue color data
			m_BlueMaterial->bind();

			// Update transform buffer with the blue transform
			transformBuffer->updateBuffer(glm::value_ptr(m_Blue0Transform.getMatrix()));

			// Render cube
			Salad::RenderCommand::drawIndexed(m_Cube);

			m_RedMaterial->bind();
			transformBuffer->updateBuffer(glm::value_ptr(m_Red1Transform.getMatrix()));
			Salad::RenderCommand::drawIndexed(m_Cube);

			// Update material buffer with red color data
			m_RedMaterial->bind();

			// Update transform buffer with the red transform
			transformBuffer->updateBuffer(glm::value_ptr(m_Red0Transform.getMatrix()));

			// Render cube
			m_RedShader->bind();
			Salad::RenderCommand::drawIndexed(m_Cube);

			m_BlueMaterial->bind();
			transformBuffer->updateBuffer(glm::value_ptr(m_Blue1Transform.getMatrix()));
			Salad::RenderCommand::drawIndexed(m_Cube);

			glCullFace(GL_BACK);

			// Render terrain
			m_TerrainShader->bind();
			m_TerrainMaterial->bind();
			m_Terrain->bind();
			transformBuffer->updateBuffer(glm::value_ptr(m_TerrainTransform.getMatrix()));
			Salad::RenderCommand::drawIndexed(m_Terrain);

			// Render water
			m_WaterShader->bind();
			m_WaterMaterial->bind();
			m_Water->bind();
			Salad::RenderCommand::drawIndexed(m_Water);
		}

		virtual void onImGuiRender() override {
			ImGui::Begin("Controls");

			if(ImGui::TreeNodeEx("Light")) {
				ImGui::SliderFloat("LightDirX", &m_LightDirX, .0f, 1.0f, "%.3f");
				ImGui::SliderFloat("LightDirZ", &m_LightDirZ, .0f, 1.0f, "%.3f");
				ImGui::TreePop();
			}

			if(ImGui::TreeNodeEx("Noise")) {

				ImGui::SliderFloat("Scale", &m_NoiseTextureScale, 0.001f, 1.0f);
				ImGui::SliderInt("Octaves", &m_NoiseTextureOctaves, 1, 8);
				ImGui::SliderFloat("Persistance", &m_NoiseTexturePersistance, 0.0f, 1.0f);
				ImGui::SliderFloat("Lucanarity", &m_NoiseTextureLacunarity, 1.0f, 4.0f);
				ImGui::Checkbox("Absolute", &m_NoiseTextureAbsolute);
				ImGui::SliderFloat("Power", &m_NoiseTexturePow, 0.001f, 4.0f);
				ImGui::SliderFloat("Mask", &m_NoiseTextureMask, 0.0f, 1.0f);

				if (ImGui::Button("Generate")) {
					NoiseData noise = generateNoisePerlin2D(256, 256, m_NoiseTextureScale, m_NoiseTextureOctaves, 
						m_NoiseTexturePersistance, m_NoiseTextureLacunarity, m_NoiseTextureAbsolute);

					std::vector<unsigned char> noiseTextureData;
					noiseTextureData.resize((size_t)noise.c_Width * noise.c_Height * 4);
					for (int i = 0; i < noise.m_Data.size(); i++) {

						float noiseValue = pow(noise.m_Data[i], m_NoiseTexturePow);
						if (noiseValue < m_NoiseTextureMask) noiseValue = 0.0f;

						unsigned char value = noiseValue * 255;
						noiseTextureData[(size_t)i * 4 + 0] = value;
						noiseTextureData[(size_t)i * 4 + 1] = value;
						noiseTextureData[(size_t)i * 4 + 2] = value;
						noiseTextureData[(size_t)i * 4 + 3] = 255;
					}

					m_NoiseTexture->setData(noiseTextureData.data(), noiseTextureData.size() * 4);
				}
				ImGui::TreePop();
			}

			ImGui::End();
		}

		virtual void onEvent(Event& event) override {}

	private:
		Camera m_Camera;
		Transform m_CameraTransform;
		Ref<VertexArray> m_Cube;

		Ref<Shader> m_RedShader;
		Ref<Shader> m_BlueShader;

		Ref<Shader> m_TerrainShader;
		Ref<Shader> m_WaterShader;

		Material* m_BlueMaterial;
		Material* m_RedMaterial;

		Transform m_Red0Transform;
		Transform m_Blue0Transform;
		Transform m_Red1Transform;
		Transform m_Blue1Transform;

		float timer = 0.0f;

		Ref<VertexArray> m_Terrain;
		Ref<VertexArray> m_Water;
		Transform m_TerrainTransform;
		Material* m_TerrainMaterial;
		Material* m_WaterMaterial;
		Transform m_LightTransform;
		float m_LightDirX = 0.5f;
		float m_LightDirZ = 0.25f;

		Ref<Texture2D> m_NoiseTexture;
		float m_NoiseTextureScale = 0.25f;
		int m_NoiseTextureOctaves = 1;
		float m_NoiseTexturePersistance = 0.5f;
		float m_NoiseTextureLacunarity = 2.0f;
		bool m_NoiseTextureAbsolute = false;
		float m_NoiseTexturePow = 1;
		float m_NoiseTextureMask = 0.0f;
	};
}
