#pragma once

#include "sldpch.h"

#include "Salad/Core/Core.h"

namespace Salad {

	enum class EventType {
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory {
		None = 0,
		EventCategoryApplication =    BIT(0),
		EventCategoryInput =          BIT(1),
		EventCategoryKeyboard =       BIT(2),
		EventCategoryMouse =          BIT(3),
		EventCategoryMouseButton =    BIT(4)
	};

	class Event {

		friend class EventDispatcher;

	public:
		bool handled = false;

		virtual EventType getEventType() const = 0;
		virtual const char* getName() const = 0;
		virtual int getCategoryFlags() const = 0;
		virtual std::string toString() const { return getName(); }

	public:
		inline bool isInCategory(EventCategory category) {
			return getCategoryFlags() & category;
		}

	};

	class EventDispatcher {

		template<typename T>
		using EventFn = std::function<bool(T&)>;

	public:
		EventDispatcher(Event& event) : 
			m_Event(event)
		{}

		template<typename T>
		bool dispatch(EventFn<T> func) {
			if (m_Event.getEventType() == T::getStaticType()) {
				m_Event.handled = func(*(T*)&m_Event);
				return true;
			}
			return false;
		}

	private:
		Event& m_Event;
	};

	inline std::ostream& operator <<(std::ostream& os, const Event& e) {
		return os << e.toString();
	}

#define EVENT_CLASS_TYPE(type) static EventType getStaticType() { return EventType::##type; }\
                               virtual EventType getEventType() const override { return getStaticType(); }\
                               virtual const char* getName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int getCategoryFlags() const override { return category; }
}