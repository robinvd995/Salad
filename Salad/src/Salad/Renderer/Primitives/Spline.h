#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace Salad {

	struct SplinePoint2f{
		float x;
		float y;
		float length;
	};

	class Spline2f {

	public:
		Spline2f() = delete;
		Spline2f(bool looped) : m_Looped(looped) {}

		SplinePoint2f getSplinePoint(float t);
		SplinePoint2f getSplineGradient(float t);
		void calculateSegmentLengths();

	private:
		bool m_Looped;

		std::vector<SplinePoint2f> m_Points;
		float calculateSegmentLength(int node);
	};

}