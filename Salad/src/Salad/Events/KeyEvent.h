#pragma once

#include "Salad/Events/Event.h"

#include <sstream>

namespace Salad {

	class SALAD_API KeyEvent : public Event {

	public:
		inline int getKeyCode() const { return m_KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	protected:
		KeyEvent(int keycode) 
			: m_KeyCode(keycode) 
		{}

		int m_KeyCode;
	};

	class SALAD_API KeyPressedEvent : public KeyEvent {

	public:
		KeyPressedEvent(int keycode, int repeatCount) : 
			KeyEvent(keycode),
			m_RepeatCount(repeatCount)
		{}

		inline int getRepeatCount() { return m_RepeatCount; }

		std::string toString() const override {
			std::stringstream ss;
			ss << "KeyPressedEvent(keycode=" << m_KeyCode << ", repeatcount=" << m_RepeatCount << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)
	private:
		int m_RepeatCount = 0;

	};

	class SALAD_API KeyReleasedEvent : public KeyEvent {

	public:
		KeyReleasedEvent(int keycode) :
			KeyEvent(keycode)
		{}

		std::string toString() const override {
			std::stringstream ss;
			ss << "KeyReleasedEvent(keycode=" << m_KeyCode << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};

	class SALAD_API KeyTypedEvent : public KeyEvent {

	public:
		KeyTypedEvent(int keycode) :
			KeyEvent(keycode)
		{}

		std::string toString() const override {
			std::stringstream ss;
			ss << "KeyTypedEvent(keycode=" << m_KeyCode << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)
	};
}
