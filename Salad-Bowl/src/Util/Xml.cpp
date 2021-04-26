#include "Xml.h"

#include "Salad/Core/Log.h"

#include "rapidxml_print.hpp"

#include <string>

namespace Salad::Xml {

	// ----- XmlDocument -----

	XmlDocument::XmlDocument(XmlDocumentData* data) : 
		m_Xml(data) 
	{}

	/*XmlDocumentData* XmlDocument::getDocData() {
		return m_Xml;
	}*/

	XmlNode XmlDocument::allocateNode(const char* name) {
		return XmlNode(this, m_Xml->xml_doc.allocate_node(rapidxml::node_type::node_element, name)); 
	}

	void  XmlDocument::appendNodeToRoot(XmlNode& node) {
		m_Xml->xml_root_node->append_node(node.m_Node);
	}

	template<typename T>
	void XmlDocument::print(std::basic_ostream<T>& stream) const {
		stream << m_Xml->xml_doc;
	}

	std::ostream& operator<<(std::ostream& os, const XmlDocument& doc) {
		doc.print(os);
		return os;
	}

	std::fstream& operator<<(std::fstream& fs, const XmlDocument& doc) {
		fs << doc.m_Xml->xml_doc;
		return fs;
	}

	void XmlDocument::close() {
		m_Xml->xml_root_node = nullptr;
		m_Xml->xml_doc.clear();
		m_Xml->source_buffer.clear();
		delete m_Xml;
	}

	// ----- XmlDocument -----

	// ----- XmlNode -----

	XmlNode::XmlNode(XmlDocument* doc, rapidxml::xml_node<>* node) : 
		m_Document(doc), m_Node(node) 
	{}

	void XmlNode::iterate(const char* nodeId, bool (*lambda_func)(const XmlNode&)) {
		for (rapidxml::xml_node<>* node = m_Node->first_node(nodeId); node; node = node->next_sibling()) {
			if (std::strcmp(nodeId, node->name())) continue;
			if (lambda_func(XmlNode(m_Document, node))) break;
		}
	}

	std::string XmlNode::getValue() const {
		return m_Node->value(); 
	}

	int XmlNode::getValueAsInt() const { 
		return atoi(m_Node->value()); 
	}

	float XmlNode::getValueAsFloat() const { 
		return atof(m_Node->value());
	}

	bool XmlNode::getValueAsBool() const { 
		return atoi(m_Node->value()) != 0; 
	}

	std::string XmlNode::getAttribute(const char* key) const { 
		return m_Node->first_attribute(key)->value(); 
	}

	int XmlNode::getAttributeAsInt(const char* key) const { 
		return atoi(m_Node->first_attribute(key)->value());
	}

	float XmlNode::getAttributeAsFloat(const char* key) const { 
		return atof(m_Node->first_attribute(key)->value());
	}

	bool XmlNode::getAttributeAsBool(const char* key) const { 
		return atoi(m_Node->value()) != 0;
	}

	void XmlNode::appendNode(XmlNode& node) {
		m_Node->append_node(node.m_Node);
	}

	void XmlNode::appendAttribute(const char* key, const char* value) {
		m_Node->append_attribute(m_Document->m_Xml->xml_doc.allocate_attribute(key, value));
	};

	void XmlNode::appendAttribute(const char* key, int value) {
		m_Node->append_attribute(m_Document->m_Xml->xml_doc.allocate_attribute(key, addSource<int>(value)));
	}

	void XmlNode::appendAttribute(const char* key, float value) {
		m_Node->append_attribute(m_Document->m_Xml->xml_doc.allocate_attribute(key, addSource<float>(value)));
	}

	void XmlNode::appendAttribute(const char* key, bool value) {
		m_Node->append_attribute(m_Document->m_Xml->xml_doc.allocate_attribute(key, addSource<int>(value ? 1 : 0)));
	}

	void XmlNode::setValue(const char* value) {
		m_Node->value(value); 
	}

	template<typename T>
	const char* XmlNode::addSource(T value) {
		int curSize = m_Document->m_Xml->source_buffer.size();
		std::string strVal = std::to_string(value);
		int newSize = curSize + strVal.size();
		if (newSize > XML_SOURCE_BUFFER_SIZE) { SLD_CORE_ERROR("Xml source buffer exceeded the predetermined max size!"); }
		m_Document->m_Xml->source_buffer.resize(newSize);
		memcpy(&m_Document->m_Xml->source_buffer.data()[curSize], strVal.data(), strVal.length());
		m_Document->m_Xml->source_buffer.push_back('\0');
		char* thing = &m_Document->m_Xml->source_buffer.data()[curSize];
		return thing;
	}

	// ----- XmlNode -----
}
