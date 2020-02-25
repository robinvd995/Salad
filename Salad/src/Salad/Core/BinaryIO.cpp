#include "sldpch.h"
#include "BinaryIO.h"

namespace Salad {

	BinaryReader::BinaryReader(std::string& filepath) :
		m_FileIn(std::ifstream(filepath, std::ios::in | std::ios::binary)) {
		SLD_CORE_ASSERT(m_FileIn.is_open(), "Could not open file '" + filepath + "'!")
	}

	BinaryReader::~BinaryReader() {
		m_FileIn.close();
	}

	int BinaryReader::readInt() {
		m_FileIn.read((char*)buffer_4, sizeof(unsigned int));
		int i = (*(int*)buffer_4);
		return i;
	}

	float BinaryReader::readFloat() {
		m_FileIn.read((char*)buffer_4, sizeof(float));
		float f = (*(float*)buffer_4);
		return f;
	}

	std::string BinaryReader::readString() {
		int size = readInt();
		std::string s = "";
		for (int i = 0; i < size; i++) {
			char c[1];
			m_FileIn.read(&c[0], sizeof(char));
			s += c[0];
		}
		return s;
	}

	BinaryWriter::BinaryWriter(std::string& filepath) :
		m_FileOut(std::ofstream(filepath, std::ios::out | std::ios::binary)) {
	}

	BinaryWriter::~BinaryWriter() {
		m_FileOut.flush();
		m_FileOut.close();
	}

	void BinaryWriter::writeInt(int i) {
		m_FileOut.write((char*)&i, sizeof(int));
	}

	void BinaryWriter::writeFloat(float f) {
		m_FileOut.write((char*)&f, sizeof(float));
	}

	void BinaryWriter::writeString(const char* s, unsigned int size) {
		m_FileOut.write((char*)&size, sizeof(unsigned int));
		m_FileOut.write(s, size);
	}

}