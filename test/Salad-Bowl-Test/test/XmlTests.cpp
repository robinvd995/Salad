#include "gtest/gtest.h"

#include <Salad.h>

#include "Salad/Util/Archiver.hpp"
#include "SaladBowl/Assets/Core/AssetManager.h"
#include "SaladBowl/Assets/Core/AssetManagerSerializer.hpp"

#include "Salad/Core/ResourceManager.h"
#include "Salad/Util/DataBuffers.hpp"

#include "SaladBowl/Assets/Io/ColladaLoader.h"
#include "SaladBowl/Assets/Io/ShaderCompiler.h"

#include <vector>
#include <iostream>

namespace Salad {

	TEST(Xml_Tests, Generic) {
		using namespace Salad::Xml;

		XmlDocument writeXmlDoc = createDocument("root");

		XmlNode nodeInt = writeXmlDoc.allocateNode("int");
		nodeInt.appendAttribute("string", "hello world!");
		nodeInt.appendAttribute("int", 256);
		nodeInt.appendAttribute("float", 1.0625f);
		nodeInt.appendAttribute("bool", true);
		writeXmlDoc.appendNodeToRoot(nodeInt);
		/*XmlNode nodeString = writeXmlDoc.allocateNode("string");
		//nodeString.appendAttribute("key", "value");
		nodeString.setValue("string");
		writeXmlDoc.appendNodeToRoot(nodeString);

		XmlNode nodeInt = writeXmlDoc.allocateNode("int");
		std::string val = std::to_string(128);
		//nodeInt.appendAttribute("key", val.c_str());
		nodeInt.setValue("64");
		writeXmlDoc.appendNodeToRoot(nodeInt);

		XmlNode nodeFloat = writeXmlDoc.allocateNode("float");
		//nodeFloat.appendAttribute("key", "value");
		nodeFloat.setValue("0.625");
		writeXmlDoc.appendNodeToRoot(nodeFloat);

		XmlNode nodeBool = writeXmlDoc.allocateNode("bool");
		//nodeBool.appendAttribute("key", "value");
		nodeBool.setValue("1");
		nodeFloat.appendNode(nodeBool);

		XmlNode nodeBool2 = writeXmlDoc.allocateNode("bool");
		//nodeBool2.appendAttribute("key", "value");
		nodeBool2.setValue("0");
		nodeFloat.appendNode(nodeBool2);*/

		//printToStream(writeXmlDoc, std::cout);
		std::cout << writeXmlDoc;
		std::ofstream file("writetest.xml");
		file << writeXmlDoc;
		file.close();

		writeXmlDoc.close();
		/*XmlDocument readXmlDoc = parseXmlFile("test.xml");
		printToStream(readXmlDoc, std::cout);
		std::cout << "\Strings:\n";
		readXmlDoc.getRootNode().iterate("resource", [](const Salad::Xml::XmlNode& node) -> bool {
			std::cout << node.getValue() << std::endl;
			return false;
		});

		std::cout << "\nIntegers:\n";
		readXmlDoc.getRootNode().iterate("integer", [](const Salad::Xml::XmlNode& node) -> bool {
			std::cout << node.getValueAsInt() << std::endl;
			return false;
		});

		std::cout << "\nFloats:\n";
		readXmlDoc.getRootNode().iterate("float", [](const Salad::Xml::XmlNode& node) -> bool {
			std::cout << node.getValueAsFloat() << std::endl;
			return false;
		});*/
	}

}