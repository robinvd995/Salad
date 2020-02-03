#include <fstream>
#include <iostream>

#include <vector>
#include <string>

int readInt(unsigned char* data) {
	int i = (*(int*)data);
	return i;
}

float readFloat(unsigned char* data) {
	float f = (*(float*)data);
	f = roundf(f * 1000) / 1000;
	return f;
}

int readInt(std::ifstream& inFile) {
	unsigned char buffer[4];
	inFile.read((char*)buffer, sizeof(unsigned int));
	int i = readInt(&buffer[0]);
	return i;
}

float readFloat(std::ifstream& inFile) {
	unsigned char buffer[4];
	inFile.read((char*)buffer, sizeof(float));
	float f = readFloat(&buffer[0]);
	return f;
}

std::string readString(std::ifstream& inFile) {
	unsigned char buffer[4];
	inFile.read((char*)buffer, sizeof(unsigned int));
	int size = readInt(&buffer[0]);
	std::string s = "";
	for (int i = 0; i < size; i++) {
		char c[1];
		inFile.read(&c[0], sizeof(char));
		s += c[0];
	}
	return s;
}

void generateSpriteFile(std::string filename, const char* spriteId, int spriteIdSize, const char* textureId, int textureIdSize,
	int texCoordX, int texCoordY, int spriteWidth, int spriteHeight, int frameCount, float frameTime, int spritesPerRow) {

	std::string filepath = "output/sprites/" + filename + ".sprite";
	std::fstream file(filepath, std::ios::out | std::ios::binary);

	file.write((char*)&spriteIdSize, sizeof(unsigned int));
	file.write(spriteId, spriteIdSize);
	file.write((char*)&textureIdSize, sizeof(unsigned int));
	file.write(textureId, textureIdSize);
	file.write((char*)&texCoordX, sizeof(unsigned int));
	file.write((char*)&texCoordY, sizeof(unsigned int));
	file.write((char*)&spriteWidth, sizeof(unsigned int));
	file.write((char*)&spriteHeight, sizeof(unsigned int));
	file.write((char*)&frameCount, sizeof(unsigned int));
	file.write((char*)&frameTime, sizeof(float));
	file.write((char*)&spritesPerRow, sizeof(unsigned int));

	file.flush();
	file.close();

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
				std::ifstream inFile(filepath, std::ios::in | std::ios::binary);

				unsigned char buffer[4];

				std::string spriteId = readString(inFile);
				std::string textureId = readString(inFile);

				int texCoordX = readInt(inFile);
				int texCoordY = readInt(inFile);
				int spriteWidth = readInt(inFile);
				int spriteHeight = readInt(inFile);
				int frameCount = readInt(inFile);
				float frameTime = readFloat(inFile);
				int spritesPerRow = readInt(inFile);

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

