#pragma once

#include <cstdint>

namespace Salad::Constants {

	inline float minPos = -0.5f, maxPos = 0.5f;
	inline float minUV = 0.0f, maxUV = 1.0f;

	inline float vertexData[] = {
		minPos, minPos, minPos, minUV, minUV, 0.0f, 0.0f, -1.0f, // Front facing p0
		maxPos, minPos, minPos, maxUV, minUV, 0.0f, 0.0f, -1.0f, // Front facing p1
		maxPos, maxPos, minPos, maxUV, maxUV, 0.0f, 0.0f, -1.0f, // Front facing p2
		minPos, maxPos, minPos, minUV, maxUV, 0.0f, 0.0f, -1.0f, // Front facing p3

		minPos, minPos, maxPos, maxUV, minUV, 0.0f, 0.0f, 1.0f, // Back facing p4
		maxPos, minPos, maxPos, minUV, minUV, 0.0f, 0.0f, 1.0f, // Back facing p5
		maxPos, maxPos, maxPos, minUV, maxUV, 0.0f, 0.0f, 1.0f, // Back facing p6
		minPos, maxPos, maxPos, maxUV, maxUV, 0.0f, 0.0f, 1.0f, // Back facing p7

		maxPos, minPos, minPos, minUV, minUV, 1.0f, 0.0f, 0.0f, // Right facing p1
		maxPos, minPos, maxPos, maxUV, minUV, 1.0f, 0.0f, 0.0f, // Right facing p5
		maxPos, maxPos, maxPos, maxUV, maxUV, 1.0f, 0.0f, 0.0f, // Right facing p6
		maxPos, maxPos, minPos, minUV, maxUV, 1.0f, 0.0f, 0.0f, // Right facing p2

		minPos, minPos, minPos, maxUV, minUV, -1.0f, 0.0f, 0.0f, // Left facing p0
		minPos, maxPos, minPos, maxUV, maxUV, -1.0f, 0.0f, 0.0f, // Left facing p3
		minPos, maxPos, maxPos, minUV, maxUV, -1.0f, 0.0f, 0.0f, // Left facing p7
		minPos, minPos, maxPos, minUV, minUV, -1.0f, 0.0f, 0.0f, // Left facing p4

		minPos, minPos, minPos, minUV, minUV, 0.0f, -1.0f, 0.0f, // Bottom facing p0
		minPos, minPos, maxPos, maxUV, minUV, 0.0f, -1.0f, 0.0f, // Bottom facing p4
		maxPos, minPos, maxPos, maxUV, maxUV, 0.0f, -1.0f, 0.0f, // Bottom facing p5
		maxPos, minPos, minPos, minUV, maxUV, 0.0f, -1.0f, 0.0f, // Bottom facing p1

		minPos, maxPos, minPos, maxUV, minUV, 0.0f, 1.0f, 0.0f, // Top facing p3
		maxPos, maxPos, minPos, maxUV, maxUV, 0.0f, 1.0f, 0.0f, // Top facing p2
		maxPos, maxPos, maxPos, minUV, maxUV, 0.0f, 1.0f, 0.0f, // Top facing p6
		minPos, maxPos, maxPos, minUV, minUV, 0.0f, 1.0f, 0.0f, // Top facing p7
	};

	inline uint32_t indexData[] = {
		0, 1, 2, 0, 2, 3, // Front Face
		7, 6, 5, 5, 4, 7, // Back face
		8, 9, 10, 8, 10, 11, // Right face
		12, 13, 14, 12, 14, 15, // Left face
		16, 17, 18, 16, 18, 19, // Bottom face
		20, 21, 22, 20, 22, 23  // Top face
	};

}
