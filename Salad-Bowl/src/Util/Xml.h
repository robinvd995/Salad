// Wrapper for the RapidXML library

#pragma once

#include <vector>
#include <fstream>
#include <iterator>

#include "rapidxml.hpp"

#define XML_SOURCE_BUFFER_SIZE 65536

namespace Salad::Xml {

	struct XmlDocumentData {
		rapidxml::xml_document<> xml_doc;
		rapidxml::xml_node<>* xml_root_node = NULL;
		std::vector<char> source_buffer;
	};

	class XmlNode;

	class XmlDocument {

	public:
		XmlDocument() = delete;
		XmlDocument(const XmlDocument&) = delete;
		XmlDocument(XmlDocumentData* data);
		~XmlDocument() = default;

		XmlDocumentData* getDocData(); // TODO: change this so only the document itself and the namespace functions can use it
		XmlNode allocateNode(const char* name);
		void appendNodeToRoot(XmlNode& node);

		template<typename T>
		void print(std::basic_ostream<T>& stream) const;

		friend std::ostream& operator<<(std::ostream& os, const XmlDocument& doc);
		friend std::fstream& operator<<(std::fstream& fs, const XmlDocument& doc);

		void close();

	private:
		XmlDocumentData* m_Xml;

		friend class XmlNode;
	};

	class XmlNode {

	public:
		XmlNode() = delete;
		XmlNode(XmlDocument* doc, rapidxml::xml_node<>* node);
		~XmlNode() = default;

		void iterate(const char* nodeId, bool (*lambda_func)(const XmlNode&));

		std::string getValue() const;
		int getValueAsInt() const;
		float getValueAsFloat() const;
		bool getValueAsBool() const;

		std::string getAttribute(const char* key) const;
		int getAttributeAsInt(const char* key) const;
		float getAttributeAsFloat(const char* key) const;
		bool getAttributeAsBool(const char* key) const;

		void appendNode(XmlNode& node);
		void appendAttribute(const char* key, const char* value);
		void appendAttribute(const char* key, int value);
		void appendAttribute(const char* key, float value);
		void appendAttribute(const char* key, bool value);
		void setValue(const char* value);

	private:
		template<typename T>
		const char* addSource(T value);

	private:
		XmlDocument* m_Document = nullptr;
		rapidxml::xml_node<>* m_Node = NULL;

		friend class XmlDocument;
	};

	// ----- Namespace Functions -----

	inline XmlDocument parseXmlFile(const std::string& filepath) {
		std::ifstream file(filepath);

		XmlDocumentData* docdata = new XmlDocumentData();
		docdata->source_buffer = std::vector<char>((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		docdata->source_buffer.push_back('\0');

		docdata->xml_doc.parse<0>(&docdata->source_buffer[0]);
		docdata->xml_root_node = docdata->xml_doc.first_node();

		return XmlDocument(docdata);
	}

	inline XmlDocument createDocument() {
		XmlDocumentData* data = new XmlDocumentData();
		data->xml_root_node = data->xml_doc.allocate_node(rapidxml::node_type::node_element, "root");
		data->xml_doc.append_node(data->xml_root_node);
		data->source_buffer.reserve(XML_SOURCE_BUFFER_SIZE);
		return XmlDocument(data);
	}

	// ----- Namespace Functions -----
}
