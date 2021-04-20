#include "ShaderAssetBuilder.h"

#include <fstream>
#include <iostream>

#include <algorithm>

namespace Salad::Asset {

	std::string WordBuilder::build() {
		std::string s = std::string(m_Buffer, m_Iterator);
		m_Iterator = 0;
		return s;
	}

	void SectionBuilder::append(char c) {
		bool illegal = false;
		for (int i = 0; i < SECTION_ILLEGAL_CHARACTER_SIZE; i++) {
			if (c == s_IllegalCharacters[i]) illegal = true;
		}

		if (!illegal) m_Buffer[m_Iterator++] = c;
	}

	void SectionBuilder::back(uint32_t amount) {
		if (m_Iterator < amount) m_Iterator = 0;
		else m_Iterator -= amount;
	}

	std::string SectionBuilder::build() {
		std::string s = std::string(m_Buffer, m_Iterator);
		m_Iterator = 0;
		return s;
	}

	bool BuilderDeclaration::hasQualifier(Asset::ShaderDeclarationQualifier qualifier) {
		for (Asset::ShaderDeclarationQualifier q : m_Qualifiers)
			if (q == qualifier) return true;
		return false;
	}

	std::string BuilderDeclaration::getQualifierParameter(Asset::ShaderDeclarationQualifier qualifier, const std::string& parameter) {
		auto pmap = m_Parameters.find(qualifier);
		if (pmap == m_Parameters.end()) return "";

		auto it = pmap->second.find(parameter);
		if (it == pmap->second.end()) return "";

		return it->second;
	}

	const char SectionBuilder::s_IllegalCharacters[SECTION_ILLEGAL_CHARACTER_SIZE] = { '\r', '\n', '\t' };

	std::map<std::string, Asset::ShaderDeclarationQualifier> ShaderAssetBuilder::s_DeclarationQualifiers = {
		{"in", Asset::ShaderDeclarationQualifier::In}, {"out", Asset::ShaderDeclarationQualifier::Out}, {"uniform", Asset::ShaderDeclarationQualifier::Uniform},
		{"layout", Asset::ShaderDeclarationQualifier::Layout}, {"flat", Asset::ShaderDeclarationQualifier::Flat}
	};

	std::map<std::string, Asset::ShaderAssetDataType> ShaderAssetBuilder::s_DeclarationTypes = {
		{"mat2", Asset::ShaderAssetDataType::Mat2}, {"mat3", Asset::ShaderAssetDataType::Mat3}, {"mat4", Asset::ShaderAssetDataType::Mat4},
		{"vec2", Asset::ShaderAssetDataType::Vec2}, {"vec3", Asset::ShaderAssetDataType::Vec3}, {"vec4", Asset::ShaderAssetDataType::Vec4},
		{"int", Asset::ShaderAssetDataType::Int}, {"float", Asset::ShaderAssetDataType::Float},
		{"sampler2D", Asset::ShaderAssetDataType::Sampler2D}
	};

	std::map<std::string, ShaderSection> ShaderAssetBuilder::s_ShaderSections = {
		{"glsl_common", ShaderSection::Glsl_Common},
		{"vertex", ShaderSection::Vertex}, {"fragment", ShaderSection::Fragment}
	};

	void ShaderAssetBuilder::pushWord() {
		if (m_WordBuilder.size() > 0) {
			std::string word = m_WordBuilder.build();

			switch (getState()) {
				case BuilderState::Normal:
					m_CurrentLine.keywords.push_back(word);
					break;

				case BuilderState::Parameter:
					switch (m_ParameterState) {
						case 0: m_ParameterId = word; break;
						case 1: m_ParameterValue = word; break;
					}

					break;
			}
		}
	}

	void ShaderAssetBuilder::pushScope() {

		if (m_CurrentLine.keywords.empty()) return;

		auto it = s_ShaderSections.find(m_CurrentLine.keywords.back().identifier);
		if (it != s_ShaderSections.end()) {
			m_CurrentSection = it->second;
			m_SectionStartScopeDepth = m_ScopeDepth;
			m_SectionDeclarations.insert({ it->second, std::vector<BuilderDeclaration>() });
		}

		finishLine();

		m_ScopeDepth++;
	}

	void ShaderAssetBuilder::popScope() {
		m_ScopeDepth--;

		pushWord();
		finishLine();

		if (m_ScopeDepth == m_SectionStartScopeDepth) {

			m_SectionBuilder.back();
			std::string sectionSource = m_SectionBuilder.build();
			m_SectionSource.insert({ m_CurrentSection, sectionSource });
			m_CurrentSection = ShaderSection::None;
		}
	}

	void ShaderAssetBuilder::startParameters() {
		m_ParameterState = 0;
		m_ParameterId = "";
		m_ParameterValue = "";
		pushState(BuilderState::Parameter);
	}

	void ShaderAssetBuilder::stopParameters() {
		m_ParameterState = 0;
		popState();
	}

	void ShaderAssetBuilder::finishLine() {
		if (m_CurrentLine.keywords.size() == 0) return;

		if (m_CurrentSection != ShaderSection::None && m_ScopeDepth == 1) {

			BuilderDeclaration declaration{};
			bool isDeclaration = false;

			for (auto itt = m_CurrentLine.keywords.begin(); itt != m_CurrentLine.keywords.end(); itt++) {
				//for (int i = 0; i < m_CurrentLine.keywords.size(); i++) {
					//BuilderLineKeyword& keyword = m_CurrentLine.keywords[i];
				auto it = s_DeclarationTypes.find(itt->identifier);
				if (it != s_DeclarationTypes.end()) {
					isDeclaration = true;
					declaration.m_DataType = it->second;
					m_CurrentLine.keywords.erase(itt);
					break;
				}
			}

			if (isDeclaration) {
				for (int i = 0; i < m_CurrentLine.keywords.size(); i++) {
					BuilderLineKeyword& keyword = m_CurrentLine.keywords[i];
					auto it = s_DeclarationQualifiers.find(keyword.identifier);

					if (it == s_DeclarationQualifiers.end()) {
						if (declaration.m_Identifier.empty()) {
							declaration.m_Identifier = keyword.identifier;

							// TODO: if identifier is parameterized 'has a ( after the identifier' it is a function declaration
						}
						else
							std::cout << "Found duplicate declaration identifier!" << std::endl;
					}
					else {
						declaration.m_Qualifiers.push_back(it->second);

						if (!keyword.parameterMap.empty()) {
							declaration.m_Parameters.insert({ it->second, keyword.parameterMap });
						}
					}
				}

			}

			if (!declaration.m_Qualifiers.empty()) {
				m_SectionDeclarations[m_CurrentSection].push_back(declaration);
			}

		}

		m_CurrentLine = BuilderLine{};
	}

	void ShaderAssetBuilder::pushOperatorAssign() {
		switch (getState()) {
			case BuilderState::Normal:
			{
				std::string s = "=";
				m_CurrentLine.keywords.push_back(s);
			}break;
			case BuilderState::Parameter:
				m_ParameterState = 1;
				break;
		}
	}

	void ShaderAssetBuilder::pushSeperator() {
		switch (getState()) {
			case BuilderState::Normal: break;
			case BuilderState::Parameter:
				auto& pmap = m_CurrentLine.keywords[m_CurrentLine.keywords.size() - 1].parameterMap;
				pmap.insert({ m_ParameterId, m_ParameterValue });
				m_ParameterState = 0;
				break;
		}
	}

	void ShaderAssetBuilder::parseFile(const std::string& filepath) {

		m_BuilderState.push_back(BuilderState::Normal);

		std::ifstream input(filepath);

		for (std::string line; getline(input, line); ) {

			m_PrevChar = ' ';
			m_IsComment = false;

			for (int i = 0; i < line.size(); i++) {
				char c = line[i];

				if (m_CurrentSection != ShaderSection::None) m_SectionBuilder.append(c);

				if (m_IsComment) continue;

				switch (c) {
					case '{': pushScope(); break;
					case '}': finishLine(); popScope(); break;

					case '(': pushWord(); startParameters(); break;
					case ')': pushWord(); pushSeperator(); stopParameters(); break;

					case ',': pushSeperator(); break;

					case ';': pushWord(); finishLine(); break;

					case '=': pushOperatorAssign(); break;

					case '/': if (line[i + 1] == '/') { m_IsComment = true; } break;

					case ' ': case '\t': case '\n': case '\r':
						pushWord();
						break;

					default: m_WordBuilder.append(c); break;
				}

				m_PrevChar = line[i];
			}
		}

	}

	void ShaderAssetBuilder::parseStages(Asset::ShaderAsset& shader, uint32_t* error) {

		static const size_t shaderStageSize = 2;
		static const Asset::ShaderStageType shaderStages[] = { Asset::ShaderStageType::Vertex, Asset::ShaderStageType::Fragment };
		static const ShaderSection shaderSections[] = { ShaderSection::Vertex, ShaderSection::Fragment };

		for (int i = 0; i < shaderStageSize; i++) {

			Asset::ShaderStageType stagetype = shaderStages[i];
			ShaderSection sectiontype = shaderSections[i];

			auto it_source = m_SectionSource.find(sectiontype);

			if (it_source != m_SectionSource.end()) {

				Asset::ShaderStage stage;
				stage.shaderSource = it_source->second;

				auto it_declarations = m_SectionDeclarations.find(sectiontype);
				if (it_declarations != m_SectionDeclarations.end()) {

					for (BuilderDeclaration declaration : it_declarations->second) {

						Asset::ShaderVariable variable;
						variable.identifier = declaration.m_Identifier;
						variable.type = declaration.m_DataType;
						variable.flat = declaration.hasQualifier(Asset::ShaderDeclarationQualifier::Flat);

						std::string locationStr = declaration.getQualifierParameter(Asset::ShaderDeclarationQualifier::Layout, "location");
						if (!locationStr.empty()) { variable.layoutLocation = std::atoi(locationStr.c_str()); }

						if (declaration.hasQualifier(Asset::ShaderDeclarationQualifier::In))              stage.inputs.push_back(variable);
						else if (declaration.hasQualifier(Asset::ShaderDeclarationQualifier::Out))        stage.outputs.push_back(variable);
						else if (declaration.hasQualifier(Asset::ShaderDeclarationQualifier::Uniform))    stage.uniforms.push_back(variable);
					}
				}

				stage.shaderType = stagetype;
				shader.m_ShaderStages.insert({ stagetype, stage });

			}
		}

		// Shader stage validation
		if (!shader.hasStage(Asset::ShaderStageType::Vertex)) { *error = 1; return; }
		if (shader.getStage(Asset::ShaderStageType::Vertex).inputs.empty()) { *error = 2; return; }

		if (!shader.hasStage(Asset::ShaderStageType::Fragment)) { *error = 3; return; }
		if (shader.getStage(Asset::ShaderStageType::Fragment).outputs.empty()) { *error = 4; return; }

		// No errors
		*error = 0;
	}

	void ShaderAssetBuilder::setVertexBufferLayout(Asset::ShaderAsset& shader, uint32_t* error) {

		// TODO: Every input needs to set its layout location, if not an error

		Asset::ShaderStage& stage = shader.getStage(Asset::ShaderStageType::Vertex);

		std::sort(stage.inputs.begin(), stage.inputs.end(), [](const Asset::ShaderVariable& var0, const Asset::ShaderVariable& var1) -> bool {
			if (var0.layoutLocation == -1) return 0;
			if (var1.layoutLocation == -1) return 1;
			return var0.layoutLocation < var1.layoutLocation;
		});

		shader.m_VertexBufferLayout.m_BufferElements.resize(stage.inputs.size());

		uint32_t offset = 0;

		int i = 0;
		for (auto& input : stage.inputs) {
			Asset::ShaderVertexBufferElement element;
			element.name = input.identifier;
			element.offset = offset;
			element.size = Util::getShaderDataTypeSize(input.type);
			element.type = input.type;
			shader.m_VertexBufferLayout.m_BufferElements[i] = element;

			offset += element.size;
			i++;
		}

		shader.m_VertexBufferLayout.m_Stride = offset;
		*error = 0;
	}

	Asset::ShaderAsset ShaderAssetBuilder::build(const std::string& filepath) {

		// Create an new instance of a shader
		Asset::ShaderAsset shader(filepath);
		uint32_t errorCode = 0;

		// Parse file to get the variables and the sources
		parseFile(filepath);

		// Parse all the shader stages into the shader
		parseStages(shader, &errorCode);
		if (errorCode > 0) {
			std::cout << "Error with code '" << errorCode << "' occured while parsing stages!" << std::endl;
			return Asset::ShaderAsset();
		}

		// Determine the vertex buffer layout
		setVertexBufferLayout(shader, &errorCode);
		if (errorCode > 0) {
			std::cout << "Error with code '" << errorCode << "' occured while setting the vertex buffer layout!" << std::endl;
			return Asset::ShaderAsset();
		}

		return shader;
	}
}
