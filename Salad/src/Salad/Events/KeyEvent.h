#pragma once

#include "Salad/Events/Event.h"

#include <sstream>

namespace Salad {

	class KeyEvent : public Event {

	public:
		inline int getKeyCode() const { return m_KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	protected:
		KeyEvent(int keycode) 
			: m_KeyCode(keycode) 
		{}

		int m_KeyCode;
	};

	class KeyPressedEvent : public KeyEvent {

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

	class KeyReleasedEvent : public KeyEvent {

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

	class KeyTypedEvent : public KeyEvent {

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
