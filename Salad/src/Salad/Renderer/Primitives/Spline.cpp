#include "sldpch.h"

#include "Spline.h"

namespace Salad {

	SplinePoint2f Spline2f::getSplinePoint(float t) {

		int p0, p1, p2, p3;
		if (!m_Looped) {
			p1 = (int)t + 1;
			p2 = p1 + 1;
			p3 = p2 + 1;
			p0 = p1 - 1;
		}
		else {
			p1 = (int)t + 1;
			p2 = (p1 + 1) % m_Points.size();
			p3 = (p2 + 1) % m_Points.size();
			p0 = p1 >= 1 ? p1 - 1 : m_Points.size() - 1;
		}

		float r = t - (int)t;

		float rr = r * r;
		float rrr = rr * r;

		float q0 = -rrr + 2.0f * rr - r;
		float q1 = 3.0f * rrr - 5.0f * rr + 2.0f;
		float q2 = -3.0f * rrr + 4.0f * rr + r;
		float q3 = rrr - rr;

		float tx = m_Points[p0].x * q0 + m_Points[p1].x * q1 + m_Points[p2].x * q2 + m_Points[p3].x * q3;
		float ty = m_Points[p0].y * q0 + m_Points[p1].y * q1 + m_Points[p2].y * q2 + m_Points[p3].y * q3;

		return { tx, ty, 0.0f };
	}

	SplinePoint2f Spline2f::getSplineGradient(float t) {

		int p0, p1, p2, p3;
		if (!m_Looped) {
			p1 = (int)t + 1;
			p2 = p1 + 1;
			p3 = p2 + 1;
			p0 = p1 - 1;
		}
		else {
			p1 = (int)t + 1;
			p2 = (p1 + 1) % m_Points.size();
			p3 = (p2 + 1) % m_Points.size();
			p0 = p1 >= 1 ? p1 - 1 : m_Points.size() - 1;
		}

		float r = t - (int)t;

		float rr = r * r;
		float rrr = rr * r;

		float q0 = -3.0f * rr + 4.0f * r - 1.0f;
		float q1 = 9.0f * rr - 10.0f * r;
		float q2 = -9.0f * rr + 8.0f * r + 1.0f;
		float q3 = 3.0f * rr - 2.0f * t;

		float tx = m_Points[p0].x * q0 + m_Points[p1].x * q1 + m_Points[p2].x * q2 + m_Points[p3].x * q3;
		float ty = m_Points[p0].y * q0 + m_Points[p1].y * q1 + m_Points[p2].y * q2 + m_Points[p3].y * q3;

		return { tx, ty, 0.0f };
	}
}