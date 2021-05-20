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

	TEST(Buffer_Tests, Generic) {
		Util::ByteBuffer buffer;
		buffer.allocate<float>(7);
		std::cout << "buffer element count(float): " << buffer.getBufferElementCount<float>() << std::endl;
		std::cout << "buffer element count(int): " << buffer.getBufferElementCount<int>() << std::endl;
		std::cout << "buffer element count(char): " << buffer.getBufferElementCount<char>() << std::endl;

		buffer.write<float>(1.0f);
		buffer.write<float>(2.0f);
		buffer.write<float>(3.0f);
		buffer.write<float>(4.0f);

		buffer.moveIterator(0);
		std::cout << "buffer read float: " << buffer.read<float>() << std::endl;
		std::cout << "buffer read float: " << buffer.read<float>() << std::endl;
		std::cout << "buffer read float: " << buffer.read<float>() << std::endl;
		std::cout << "buffer read float: " << buffer.read<float>() << std::endl;

		buffer.mark();
		buffer.write<int>(23);
		buffer.write<int>(58);
		buffer.write<int>(85);

		buffer.moveToMark();
		std::cout << "buffer read int: " << buffer.read<int>() << std::endl;
		std::cout << "buffer read int: " << buffer.read<int>() << std::endl;
		std::cout << "buffer read int: " << buffer.read<int>() << std::endl;

		buffer.moveToMark();
		float floatArray[] = { 0.0625f, 0.125f, 0.1875f };
		buffer.writeArray(floatArray, 3);

		buffer.moveToMark();
		std::cout << "buffer read floatArray: " << buffer.read<float>() << std::endl;
		std::cout << "buffer read floatArray: " << buffer.read<float>() << std::endl;
		std::cout << "buffer read floatArray: " << buffer.read<float>() << std::endl;

		buffer.moveToMark();
		float* floatData = buffer.readArray<float>(3);
		for (int i = 0; i < 3; i++) {
			std::cout << "buffer read_array float: " << floatData[i] << std::endl;
		}

		buffer.freeBuffer();
		buffer.moveIterator(0);
		std::cout << "buffer read after free: " << buffer.read<float>() << std::endl;
	}

}

