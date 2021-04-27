#pragma once
#include <Salad.h>

#include "Salad/Util/Archiver.hpp"
#include "Util/Xml.h"
#include "Assets/AssetManager.h"
#include "Assets/AssetManagerSerializer.hpp"

#include <iostream>

#define SLD_ARCHIVE_TEST_FILE "test.zip"

namespace Salad {

	class TestLayer : public Layer {

	public:

		TestLayer() {}

		void testArchive() {
			int error = 0;

			using namespace Util;

			Archive archive = archiveOpen(SLD_ARCHIVE_TEST_FILE, ArchiverOpenFlags_Create, &error);

			// Byte writing
			ArchiveIOBuffer mWriteBuffer = archiveWriteBuffer(archive, 70);
			const char* messageData = "This a example of writing/reading a simple string using the archiver!";
			for (int i = 0; i < 70; i++) { mWriteBuffer.writeByte(messageData[i]); }
			archiveAddFile(archive, "data.byte", mWriteBuffer, 0);

			// Float writing
			ArchiveIOBuffer fWriteBuffer = archiveWriteBuffer(archive, 16);
			fWriteBuffer.writeFloat(5.216f);
			fWriteBuffer.writeFloat(0.0625f);
			fWriteBuffer.writeFloat(22.5f);
			fWriteBuffer.writeFloat(9875.021587f);
			archiveAddFile(archive, "data.float", fWriteBuffer, 0);

			// Int writing
			ArchiveIOBuffer iWriteBuffer = archiveWriteBuffer(archive, 12);
			iWriteBuffer.writeInt(6885);
			iWriteBuffer.writeInt(7675676);
			iWriteBuffer.writeInt(2345);
			archiveAddFile(archive, "data.int", iWriteBuffer, 0);

			// Mixed primitive writing
			ArchiveIOBuffer pWriteBuffer = archiveWriteBuffer(archive, 58);
			pWriteBuffer.writeByte('b');
			pWriteBuffer.writeUByte('h');
			pWriteBuffer.writeShort(-25565);
			pWriteBuffer.writeUShort(52050);
			pWriteBuffer.writeInt(-777777);
			pWriteBuffer.writeUInt(89898989);
			pWriteBuffer.writeLong(-101010101010101010);
			pWriteBuffer.writeULong(101010101010101010);
			pWriteBuffer.writeFloat(64.0125f);
			pWriteBuffer.writeDouble(64.000125);
			pWriteBuffer.writeBool(true);
			archiveAddFile(archive, "primitives.data", pWriteBuffer, 0);

			// Array writing
			char cArray[] = { 'a','b','c','d','e','f','g' };
			uint32_t iArray[] = { 0,1,2,3,4,5,6,7,8,9 };
			float fArray[] = { 0.0f, 0.1f, 0.2f, 0.3f, 0.4f, 0.5f };
			ArchiveIOBuffer aWriteBuffer = archiveWriteBuffer(archive, 15 + 48 + 32);
			aWriteBuffer.writeArray<char>(cArray, 7);
			aWriteBuffer.writeArray<uint32_t>(iArray, 10);
			aWriteBuffer.writeArray<float>(fArray, 6);

			archiveAddFile(archive, "arrays.dat", aWriteBuffer, 0);

			archiveClose(archive);
			mWriteBuffer.freeBuffer();
			fWriteBuffer.freeBuffer();
			iWriteBuffer.freeBuffer();
			pWriteBuffer.freeBuffer();
			aWriteBuffer.freeBuffer();

			Archive archiveRead = archiveOpen(SLD_ARCHIVE_TEST_FILE, ArchiverOpenFlags_Create, &error);

			// Byte reading
			ArchiveIOBuffer mBuffer = archiveReadFile(archiveRead, "data.byte");
			std::string s = "";
			for (int i = 0; i < mBuffer.getBufferSize(); i++) { char c = mBuffer.readByte(); s += c; }
			mBuffer.freeBuffer();
			std::cout << s << std::endl;

			// Float reading
			ArchiveIOBuffer fBuffer = archiveReadFile(archiveRead, "data.float");
			while (fBuffer.hasNext(4)) {
				std::cout << fBuffer.readFloat() << std::endl;
			}
			fBuffer.freeBuffer();

			// Int reading
			ArchiveIOBuffer iBuffer = archiveReadFile(archiveRead, "data.int");
			while (iBuffer.hasNext(4)) {
				std::cout << iBuffer.readInt() << std::endl;
			}
			iBuffer.freeBuffer();

			ArchiveIOBuffer pBuffer = archiveReadFile(archiveRead, "primitives.data");
			std::cout << pBuffer.readByte() << std::endl;
			std::cout << pBuffer.readUByte() << std::endl;
			std::cout << pBuffer.readShort() << std::endl;
			std::cout << pBuffer.readUShort() << std::endl;
			std::cout << pBuffer.readInt() << std::endl;
			std::cout << pBuffer.readUInt() << std::endl;
			std::cout << pBuffer.readLong() << std::endl;
			std::cout << pBuffer.readULong() << std::endl;
			std::cout << pBuffer.readFloat() << std::endl;
			std::cout << pBuffer.readDouble() << std::endl;
			std::cout << pBuffer.readBool() << std::endl;
			pBuffer.freeBuffer();

			ArchiveIOBuffer aBuffer = archiveReadFile(archiveRead, "arrays.dat");
			uint64_t count = 0;
			char* cReadArray = aBuffer.readArray<char>(&count);
			SLD_CORE_ASSERT((count == 8), "Expected count 7!");
			std::cout << count << std::endl;
			for (int i = 0; i < count; i++) {
				std::cout << cReadArray[i];
			}
			std::cout << std::endl;

			uint32_t* iReadArray = aBuffer.readArray<uint32_t>(&count);
			std::cout << count << std::endl;
			for (int i = 0; i < count; i++) {
				std::cout << iReadArray[i] << " ";
			}
			std::cout << std::endl;

			float* fReadArray = aBuffer.readArray<float>(&count);
			for (int i = 0; i < count; i++) {
				std::cout << fReadArray[i] << " ";
			}
			std::cout << std::endl;

			aBuffer.freeBuffer();

			archiveClose(archiveRead);
		}

		void testXml() {
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

		void testAssetManager() {
			using namespace Salad::Asset;

			const char* filepath = "assetmanagertest.xml";
			{ // Write
				AssetManager manager;

				manager.includeAsset("assets/textures/texture.png");
				manager.includeAsset("assets/shaders/shader.glsl");
				manager.includeAsset("assets/models/model.png");
				manager.includeAsset("assets/material/material.png");

				AssetManagerSerializer serializer;
				serializer.serialize(manager, filepath);
			} // Write

			{ // Read
				AssetManager manager;
				AssetManagerSerializer serializer;
				serializer.deserialize(manager, filepath);
			} // Read
		}

		void testInheritance() {
			Asset::ShaderAsset shader("somepath");
			Asset::AssetBase* assets = &shader;
			std::string s0 = "pth";
			std::string s1 = "nm";
			Asset::TextureAsset texture(s0, s1, nullptr, {});
			Asset::AssetBase* assett = &texture;

			//std::cout << shader.getNumber() << std::endl;
			//std::cout << assets->getNumber() << std::endl;

			//std::cout << texture.getNumber() << std::endl;
			//std::cout << assett->getNumber() << std::endl;

			std::cout << static_cast<int>(shader.getAssetType()) << std::endl;
			std::cout << static_cast<int>(assets->getAssetType()) << std::endl;

			std::cout << static_cast<int>(texture.getAssetType()) << std::endl;
			std::cout << static_cast<int>(assett->getAssetType()) << std::endl;
		}

		virtual void onAttach() override {
			//testArchive();
			//testXml();
			//testAssetManager();
			testInheritance();
		}

		virtual void onDetach() override {}

		virtual void onUpdate(Timestep ts) override {
			Application::get().close();
		}
		virtual void onEvent(Event& e) override {}

		virtual void onImGuiRender() override {}

	};
}
