#pragma once

#include "Salad/Core/Core.h"

namespace Salad {

	class Timestep {

	public:

		Timestep(float time = 0.0f) :
			m_Time(time)
		{}

		operator float() const { return m_Time; }

		float getSeconds() const { return m_Time; }
		float getMilis() const { return m_Time * 1000.0f; }

	private:
		float m_Time;
	};

}