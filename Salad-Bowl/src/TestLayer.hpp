#pragma once
#include <Salad.h>

#include "Util/Archiver.hpp"

#include <iostream>

#define SLD_ARCHIVE_TEST_FILE "test.zip"

namespace Salad {

	class TestLayer : public Layer {

	public:

		TestLayer() {}

		virtual void onAttach() override {
			int error = 0;

			using namespace Util;

			Archive archive = archiveOpen(SLD_ARCHIVE_TEST_FILE, ArchiverOpenFlags_Create, &error);

			// String writing
			ArchiveFileWriteBuffer mWriteBuffer = archiveWriteBuffer(archive, "string.message", 70);
			const char* messageData = "This a example of writing/reading a simple string using the archiver!";
			for(int i = 0; i < 70; i++) { mWriteBuffer.writeByte(messageData[i]); }
			archiveAddFile(archive, mWriteBuffer, 0);

			// Float writing
			ArchiveFileWriteBuffer fWriteBuffer = archiveWriteBuffer(archive, "data.float", 16);
			fWriteBuffer.writeFloat(5.216f);
			fWriteBuffer.writeFloat(0.0625f);
			fWriteBuffer.writeFloat(22.5f);
			fWriteBuffer.writeFloat(9875.021587f);
			archiveAddFile(archive, fWriteBuffer, 0);
			
			// Int writing
			ArchiveFileWriteBuffer iWriteBuffer = archiveWriteBuffer(archive, "src.data.int", 12);
			iWriteBuffer.writeInt(6885);
			iWriteBuffer.writeInt(7675676);
			iWriteBuffer.writeInt(2345);
			archiveAddFile(archive, iWriteBuffer, 0);

			archiveClose(archive);
			mWriteBuffer.freeBuffer();
			fWriteBuffer.freeBuffer();
			iWriteBuffer.freeBuffer();

			Archive archiveRead = archiveOpen(SLD_ARCHIVE_TEST_FILE, ArchiverOpenFlags_Create, &error);

			// String reading start
			ArchiveFileReadBuffer mBuffer = archiveReadFile(archiveRead, mWriteBuffer.getFileName());
			std::string s = "";
			for(int i = 0; i < mBuffer.getBufferSize(); i++) { char c = mBuffer.readByte(); s += c; }
			mBuffer.freeBuffer();
			std::cout << s << std::endl;

			// Float reading
			ArchiveFileReadBuffer fBuffer = archiveReadFile(archiveRead, fWriteBuffer.getFileName());
			while (fBuffer.canRead(4)) {
				std::cout << fBuffer.readFloat() << std::endl;
			}
			fBuffer.freeBuffer();

			// Int reading
			ArchiveFileReadBuffer iBuffer = archiveReadFile(archiveRead, iWriteBuffer.getFileName());
			while(iBuffer.canRead(4)) {
				std::cout << iBuffer.readInt() << std::endl;
			}
			iBuffer.freeBuffer();

			archiveClose(archiveRead);
		}

		virtual void onDetach() override {}

		virtual void onUpdate(Timestep ts) override {
			Application::get().close();
		}
		virtual void onEvent(Event& e) override {}

		virtual void onImGuiRender() override {}

	};

}