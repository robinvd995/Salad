#pragma once
#include <fstream>

class BinaryReader {

public:
	BinaryReader() = delete;
	BinaryReader(std::string& filepath);
	~BinaryReader();

	int readInt();
	float readFloat();
	std::string readString();

private:
	std::ifstream m_FileIn;

	unsigned char buffer_1[1];
	unsigned char buffer_2[2];
	unsigned char buffer_4[4];
	unsigned char buffer_8[8];

};

class BinaryWriter {

public:
	BinaryWriter() = delete;
	BinaryWriter(std::string& filepath);
	~BinaryWriter();

	void writeInt(int i);
	void writeFloat(float f);
	void writeString(const char* s, unsigned int size);

private:
	std::ofstream m_FileOut;

};