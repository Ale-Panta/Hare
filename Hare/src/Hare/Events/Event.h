#pragma once

#include "Hare/Core.h"

namespace Hare
{
	/* 
	Events in Hare are currently blocking, meaning when an event occurs it
	immediatly gets dispatched and must be dealt with right then an there.
	For the future, a better strategy might be to buffer events in an event
	bus and process them during the "event" part of the update stage.
	*/

	enum class EventType {
		None = 0,
		WindowClose, WindowResize, WindowFocus, WIndowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory
	{
		None = 0,
		EventCategoryApplication	= BIT(0),
		EventCategoryInput			= BIT(1),
		EventCategoryKeyboard		= BIT(2),
		EventCategoryMouse			= BIT(3),
		EventCategoryMouseButton	= BIT(4)
	};

	/*
	Those macros are used to override that functions in a cleaner way.
	We want to be able to at runtime check to see which type this
	key pressed event is, so obviously we need a function that return 
	which event type it is. This does not have to be a member function.
	It can just be a static function because if we say: "key pressed 
	event type or whatever we don't need to have an instance of the key
	pressed event class to actually see what type it is.
	*/
#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	class HARE_API Event 
	{
		friend class EventDispatcher;

	public:

		bool Handled = false;

		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		inline bool IsInCategory(EventCategory category)
		{
			return GetCategoryFlags() & category;
		}
	};

	class EventDispatcher 
	{
		template<typename T>
		using EventFn = std::function<bool(T&)>;

	public:
		EventDispatcher(Event& event)
			: m_Event(event) { }

		template<typename T>
		bool Dispatch(EventFn<T> func)
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.Handled = func(*(T*)&m_Event);
				return true;
			}
			return false;
		}

	private:
		Event& m_Event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}
}