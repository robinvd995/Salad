#include "Noise.h"

#include "FastNoise.h"

#include <iostream>

namespace Salad {

	typedef struct {
		float x, y;
	} vector2;

	NoiseData::NoiseData(uint32_t width, uint32_t height) :
		c_Width(width), c_Height(height)
	{}

	float interpolate(float f0, float f1, float w) {
		return (f1 - f0) * w + f0;
	}

	float interpolateSmooth(float f0, float f1, float w) {
		return (f1 - f0) * (3.0 - w * 2.0) * w * w + f0;
	}

	float interpolateSmoother(float f0, float f1, float w) {
		return (f1 - f0)* ((w * (w * 6.0 - 15.0) + 10.0) * w * w * w) + f0;
	}

	vector2 randomGradient(int ix, int iy) {
		float random = 2920.f * sin(ix * 21942.f + iy * 171324.f + 8912.f) * cos(ix * 23157.f * iy * 217832.f + 9758.f);
		vector2 vec{};
		vec.x = (float)cos(random);
		vec.y = (float)sin(random);
		return vec;
	}

	float dotGridGradient(int ix, int iy, float x, float y) {
		vector2 gradient = randomGradient(ix, iy);

		float dx = x - (float)ix;
		float dy = y - (float)iy;

		float value = (dx * gradient.x + dy * gradient.y);
		return value;
	}

	float perlin(float x, float y) {
		int x0 = (int)x;
		int x1 = x0 + 1;
		int y0 = (int)y;
		int y1 = y0 + 1;
		
		//if (x0 == 0) x0 = 256;
		//if (y0 == 0) y0 = 256;

		float sx = x - (float)x0;
		float sy = y - (float)y0;

		float n0 = dotGridGradient(x0, y0, x, y);
		float n1 = dotGridGradient(x1, y0, x, y);
		float n2 = dotGridGradient(x0, y1, x, y);
		float n3 = dotGridGradient(x1, y1, x, y);

		float ix0 = interpolate(n0, n1, sx);
		float ix1 = interpolate(n2, n3, sx);

		float value = interpolate(ix0, ix1, sy);
		return value;
	}

	/*NoiseData generateNoisePerlin2D(uint32_t width, uint32_t height) {
		NoiseData data(width, height);

		float scalarx = (float)width / (16384);
		float scalary = (float)height / (16384);

		float frequency = 1.0f;

		float maxValue = 0.0f;
		float minValue = 0.0f;

		for(int i = 0; i < width; i++) {
			for(int j = 0; j < height; j++) {
				int index = j * height + i;

				float perlinValue = perlin(((float)i) * frequency * scalarx, ((float)j) * frequency * scalary);
				if (perlinValue > maxValue) maxValue = perlinValue;
				if (perlinValue < minValue) minValue = perlinValue;

				data.m_Data.push_back(perlinValue);
			}
		}

		for(int i = 0; i < data.m_Data.size(); i++) {
			float f = (data.m_Data[i] + minValue) / (maxValue + minValue);
			data.m_Data[i] = f;
		}

		return data;
	}*/

	NoiseData generateNoisePerlin2D(uint32_t width, uint32_t height) {
		NoiseData data(width, height);

		data.m_Data.resize((size_t)width * height);

		FastNoiseLite noise;
		noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);

		float scale = 0.25f;

		int index = 0;
		for(int i = 0; i < width; i++) {
			for(int j = 0; j < height; j++) {

				float sampleX = i / scale;
				float sampleY = j / scale;

				float value = (noise.GetNoise((float)sampleX, (float)sampleY) + 1.0f) / 2.0f;
				data.m_Data[index++] = value;
			}
		}

		return data;
	}

	NoiseData generateNoisePerlin2D(uint32_t width, uint32_t height, float scale, int octaves, float persistance, float lacunarity, bool absolute) {
		NoiseData data(width, height);

		data.m_Data.resize((size_t)width * height);

		FastNoiseLite noise;
		noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);

		float max = -100.0f;
		float min = 100.0f;

		int index = 0;
		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {

				float amplitude = 1.0f;
				float frequency = 1.0f;
				float noiseHeight = 0.0f;

				for(int k = 0; k < octaves; k++) {

					float sampleX = i / scale * frequency;
					float sampleY = j / scale * frequency;

					float value = noise.GetNoise((float)sampleX, (float)sampleY);
					if (absolute) value = abs(value);
					noiseHeight += value * amplitude;

					amplitude *= persistance;
					frequency *= lacunarity;
				}

				if (noiseHeight > max) max = noiseHeight;
				else if (noiseHeight < min) min = noiseHeight;

				data.m_Data[index++] = noiseHeight;
			}
		}

		for(uint32_t i = 0; i < data.m_Data.size(); i++) {
			data.m_Data[i] = (data.m_Data[i] - min) / (max - min);
		}

		return data;
	}

}