#include <fstream>
#include <iostream>
#include <vector>
#include <string>

#include "BinaryIO.h"

void generateSpriteFile(std::string filename, const char* spriteId, int spriteIdSize, const char* textureId, int textureIdSize,
	int texCoordX, int texCoordY, int spriteWidth, int spriteHeight, int frameCount, float frameTime, int spritesPerRow) {

	std::string filepath = "output/sprites/" + filename + ".sprite";
	{
		BinaryWriter writer(filepath);
		writer.writeString(spriteId, spriteIdSize);
		writer.writeString(textureId, textureIdSize);
		writer.writeInt(texCoordX);
		writer.writeInt(texCoordY);
		writer.writeInt(spriteWidth);
		writer.writeInt(spriteHeight);
		writer.writeInt(frameCount);
		writer.writeFloat(frameTime);
		writer.writeInt(spritesPerRow);
	}
	std::cout << "Generated sprite file '" << filepath << "'!" << std::endl;
}

int main() {

	while (true) {
		char line[200];
		std::cin.getline(line, sizeof(line));

		std::string command = "";
		int i;
		for (i = 0; i < 200; i++) {
			if (line[i] == ' ' || line[i] == 0)
				break;

			command += line[i];
		}

		if (command == "stop") {
			break;
		}

		if (command == "sprite") {
			std::vector<std::string> argumentVector;
			std::string curArg;
			while (i < 200) {
				char c = line[i];
				i++;

				if (c == 0) break;
				if (c == ' ') {
					std::string cpy = curArg;
					if (cpy.size() > 0)
						argumentVector.push_back(cpy);
					curArg = "";
					continue;
				}
				if (c != ' ')
					curArg += c;
			}

			if (curArg.size() > 0)
				argumentVector.push_back(curArg);

			if (argumentVector.size() == 1 && argumentVector[0] == "help") {
				std::cout << "filepath(s), texCoordX(i) texCoordY(i) spriteWidth(i) spriteHeight(i) frameCount(i) frameTime(f) spritesPerRow(i)" << std::endl;
				continue;
			}

			if(argumentVector.size() == 2 && argumentVector[0] == "read") {
				std::string filepath = "output/sprites/" + argumentVector[1] + ".sprite";

				BinaryReader reader(filepath);

				std::string spriteId = reader.readString();
				std::string textureId = reader.readString();

				int texCoordX = reader.readInt();
				int texCoordY = reader.readInt();
				int spriteWidth = reader.readInt();
				int spriteHeight = reader.readInt();
				int frameCount = reader.readInt();
				float frameTime = reader.readFloat();
				int spritesPerRow = reader.readInt();

				std::cout << "spriteId: '" << spriteId << "'" << std::endl;
				std::cout << "textureId: '" << textureId << "'" << std::endl;

				std::cout << "texCoordX: '" << texCoordX << "'" << std::endl;
				std::cout << "texCoordY: '" << texCoordY << "'" << std::endl;

				std::cout << "spriteWidth: '" << spriteWidth << "'" << std::endl;
				std::cout << "spriteHeight: '" << spriteHeight << "'" << std::endl;

				std::cout << "frameCount: '" << frameCount << "'" << std::endl;
				std::cout << "frameTime: '" << frameTime << "'" << std::endl;
				std::cout << "spritesPerRow: '" << spritesPerRow << "'" << std::endl;
				continue;
			}

			if (argumentVector.size() != 10) {
				std::cout << "Invalid amount of arguments!" << std::endl;
				continue;
			}

			std::string arg0 = argumentVector[0];
			std::string arg1 = argumentVector[1];
			std::string arg2 = argumentVector[2];
			int arg3 = std::stoi(argumentVector[3]);
			int arg4 = std::stoi(argumentVector[4]);
			int arg5 = std::stoi(argumentVector[5]);
			int arg6 = std::stoi(argumentVector[6]);
			int arg7 = std::stoi(argumentVector[7]);
			float arg8 = std::stof(argumentVector[8]);
			int arg9 = std::stoi(argumentVector[9]);

			generateSpriteFile(arg0, arg1.c_str(), arg1.size(), arg2.c_str(), arg2.size(), arg3, arg4, arg5, arg6, arg7, arg8, arg9);
		}
	}
}
