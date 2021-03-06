#pragma once

#include <map>
#include <string>
#include <stack>
#include <vector>

#include <ostream>

#include "Assets/EditorShader.h"

// ----- Defines Start -----

#define WORD_BUILDER_BUFFER_SIZE 256
#define SECTION_BUILDER_BUFFER_SIZE 8192
#define SECTION_ILLEGAL_CHARACTER_SIZE 3

// ----- Defines End -----

// ----- Helper Functions Start -----

namespace Util {
	static uint32_t getShaderDataTypeSize(Salad::EditorShaderDataType type) {
		switch (type) {
			default: return 0;

			case Salad::EditorShaderDataType::Mat2: return 4 * 2 * 2;
			case Salad::EditorShaderDataType::Mat3: return 4 * 3 * 3;
			case Salad::EditorShaderDataType::Mat4: return 4 * 4 * 4;

			case Salad::EditorShaderDataType::Vec2: return 4 * 2;
			case Salad::EditorShaderDataType::Vec3: return 4 * 3;
			case Salad::EditorShaderDataType::Vec4: return 4 * 4;

			case Salad::EditorShaderDataType::Int: return 4;
			case Salad::EditorShaderDataType::Float: return 4;

			case Salad::EditorShaderDataType::Sampler2D: return 4;
		}
	}
}

// ----- Helper Functions End -----

namespace Salad {

	// ----- Enums Start -----

	enum class BuilderState {
		Normal = 0,
		Parameter = 1
	};

	enum class ShaderSection {
		None,
		Glsl_Common,
		Vertex, Fragment
	};

	// ----- Enums End -----

	// ----- Builders Start -----

	class WordBuilder {

	public:

		WordBuilder() = default;
		~WordBuilder() = default;

		void append(char c) { m_Buffer[m_Iterator++] = c; }
		int size() { return m_Iterator; }

		std::string build();

	private:
		char m_Buffer[WORD_BUILDER_BUFFER_SIZE]{}; // TODO: Maybe allocate on heap?
		int m_Iterator = 0;
	};

	class SectionBuilder {

	public:

		SectionBuilder() = default;
		~SectionBuilder() = default;

		uint32_t size() { return m_Iterator; }

		void append(char c);
		void back(uint32_t amount = 1);
		std::string build();

	private:
		char m_Buffer[SECTION_BUILDER_BUFFER_SIZE]{}; // TODO: Allocate buffer on heap
		uint32_t m_Iterator = 0;

		static const char s_IllegalCharacters[SECTION_ILLEGAL_CHARACTER_SIZE];
	};

	// ----- Builders End -----

	// ----- Data Structures Start -----

	class BuilderDeclaration {

	public:
		BuilderDeclaration() = default;
		~BuilderDeclaration() = default;

		bool hasQualifier(ShaderDeclarationQualifier qualifier);
		std::string getQualifierParameter(ShaderDeclarationQualifier qualifier, const std::string& parameter);

	private:
		std::string m_Identifier;
		EditorShaderDataType m_DataType = EditorShaderDataType::None;
		std::vector<ShaderDeclarationQualifier> m_Qualifiers;
		std::map<ShaderDeclarationQualifier, std::map<std::string, std::string>> m_Parameters;

		friend class ShaderBuilder;

	};

	struct BuilderLineKeyword {
		std::string identifier;
		std::map<std::string, std::string> parameterMap;

		BuilderLineKeyword(std::string& word) : identifier(word) {}
	};

	struct BuilderLine {
		std::vector<BuilderLineKeyword> keywords;

		BuilderLine() = default;
		BuilderLine(const BuilderLine&) = default;
	};

	// ----- Data Structures End -----

	class ShaderBuilder {

		typedef std::map<std::string, EditorShaderDataType> ShaderDataMap;

	public:

		EditorShader build(const std::string& filepath);

	private:

		void parseFile(const std::string& filepath);
		void parseStages(EditorShader& shader, uint32_t* error);
		void setVertexBufferLayout(EditorShader& shader, uint32_t* error);

		void pushWord();

		void pushScope();
		void popScope();
		void startParameters();
		void stopParameters();

		void finishLine();
		void pushSeperator();

		void pushOperatorAssign();

		void pushState(BuilderState state) { m_BuilderState.push_back(state); }
		void popState() { m_BuilderState.pop_back(); }
		BuilderState getState() { return m_BuilderState[m_BuilderState.size() - 1]; }

	private:

		static std::map<std::string, ShaderDeclarationQualifier> s_DeclarationQualifiers;
		static std::map<std::string, EditorShaderDataType> s_DeclarationTypes;
		static std::map<std::string, ShaderSection> s_ShaderSections;

	private:
		WordBuilder m_WordBuilder;
		std::vector<BuilderState> m_BuilderState;

		// Builder function variables
		bool m_IsComment = false;
		char m_PrevChar;

		BuilderLine m_CurrentLine;

		std::string m_ParameterId;
		std::string m_ParameterValue;
		int m_ParameterState = 0;

		int m_ScopeDepth = 0;
		int m_SectionStartScopeDepth = -1;
		ShaderSection m_CurrentSection = ShaderSection::None;

		std::map<ShaderSection, std::vector<BuilderDeclaration>> m_SectionDeclarations;
		std::map<ShaderSection, std::string> m_SectionSource;

		SectionBuilder m_SectionBuilder;
	};
}
