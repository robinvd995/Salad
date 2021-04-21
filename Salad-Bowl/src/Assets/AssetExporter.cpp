#include "AssetExporter.h"

#include <iostream>

namespace Salad::Asset::IO {

	void Salad::Asset::IO::exportTextureAsset(TextureAsset& asset) {
		std::cout << "export asset" << asset.getFilePath() << std::endl;
	}
	/*void Salad::Asset::IO::ziplibExportTest() {
		std::string archivestr = "test.resources";
		std::string file = "message";
		const char* data = "Hello World! This is your captain speaking!";

		{
			int error = 0;
			zip* archive = zip_open(archivestr.c_str(), ZIP_CREATE, &error);
			if (archive == NULL) {
				std::cout << "Error creating archive! code: '" << error << "'" << std::endl;
				return;
			}

			zip_source* source = zip_source_buffer(archive, data, 44, 0);
			if (source == nullptr) {
				std::cout << "Failed to create source buffer. " << zip_strerror(archive) << std::endl;
				return;
			}

			int index = (int)zip_file_add(archive, file.c_str(), source, ZIP_FL_OVERWRITE);
			if (index < 0) {
				std::cout << "failed to add file to archive. " << zip_strerror(archive) << std::endl;
				return;
			}

			zip_close(archive);
		}

		{
			int error = 0;
			zip* archive = zip_open(archivestr.c_str(), 0, &error);
			zip_file* zipfile = zip_fopen(archive, file.c_str(), 0);

			const char buffer[128] = {};
			int readBytes = zip_fread(zipfile, (void*)&buffer[0], 128);

			std::cout << "read file: " << buffer << std::endl;
			zip_close(archive);
		}
	}*/
}