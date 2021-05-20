#pragma once

#include <vector>

namespace Salad {

	struct NoiseData {
		const uint32_t c_Width, c_Height;
		std::vector<float> m_Data;

		NoiseData() = delete;
		NoiseData(uint32_t width, uint32_t height);
	};

	NoiseData generateNoisePerlin2D(uint32_t width, uint32_t height);
	NoiseData generateNoisePerlin2D(uint32_t width, uint32_t height, float scale, int octaves, float persistance, float lacunarity, bool absolute = false);

}