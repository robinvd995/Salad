#pragma once

#include "Salad/Events/Event.h"

#include <sstream>

namespace Salad {

	class SALAD_API MouseMoveEvent : public Event {

	public:
		MouseMoveEvent(float mx, float my) :
			m_MouseX(mx),
			m_MouseY(my)
		{}

		inline float getX() { return m_MouseX; }
		inline float getY() { return m_MouseY; }

		std::string toString() const override {
			std::stringstream ss;
			ss << "MouseMoveEvent(mouseX=" << m_MouseX << ", mouseY=" << m_MouseY << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	private:
		float m_MouseX, m_MouseY;
	};

	class SALAD_API MouseScrolledEvent : public Event {

	public:
		MouseScrolledEvent(float xoffset, float yoffset) :
			m_XOffset(xoffset),
			m_YOffset(yoffset)
		{}

		inline float getXOffset() { return m_XOffset; }
		inline float getYOffset() { return m_YOffset; }

		std::string toString() const override {
			std::stringstream ss;
			ss << "MouseScrollEvent(XOffset=" << m_XOffset << ", YOffset" << m_YOffset << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

	private:
		float m_XOffset, m_YOffset;
	};

	class SALAD_API MouseButtonEvent : public Event {
	public:
		inline int getMouseButton() { return m_Button; }

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	protected:
		MouseButtonEvent(int button) :
			m_Button(button)
		{}

		int m_Button;
	};

	class SALAD_API MouseButtonPressedEvent : public MouseButtonEvent {
	public:
		MouseButtonPressedEvent(int button) :
			MouseButtonEvent(button)
		{}

		inline int getMouseButton() { return m_Button; }

		std::string toString() const override {
			std::stringstream ss;
			ss << "MouseButtonPressedEvent(Button=" << m_Button << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class SALAD_API MouseButtonReleasedEvent : public MouseButtonEvent {
	
	public:
		MouseButtonReleasedEvent(int button) : 
			MouseButtonEvent(button)
		{}

		inline int getMouseButton() { return m_Button; }

		std::string toString() const override {
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent(Button=" << m_Button << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};

}