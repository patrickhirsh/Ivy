#pragma once
#include "Core/IvyPCH.h"


#define BIND_EVENT_FUNCTION(x) std::bind(&x, std::placeholders::_1)
#define BIND_EVENT_MEMBER_FUNCTION(x) std::bind(&x, this, std::placeholders::_1)

namespace Ivy
{
	enum class EventType
	{
		E_NONE = 0,
		E_WINDOW_CLOSED,
		E_WINDOW_FOCUSED,
		E_WINDOW_LOST_FOCUS,
		E_WINDOW_RESIZED,
		E_WINDOW_MOVED,
		E_KEY_PRESSED,
		E_KEY_RELEASED,
		E_MOUSEBUTTON_PRESSED,
		E_MOUSEBUTTON_RELEASED,
		E_MOUSE_MOVED,
		E_MOUSE_SCROLLED
	};

	enum EventCategory
	{
		C_NONE = 0,
		C_APPLICATION = BIT(0),
		C_INPUT = BIT(1),
		C_KEYBOARD = BIT(2),
		C_MOUSE_MOVEMENT = BIT(3),
		C_MOUSE_BUTTON = BIT(4)
	};
}

namespace _Ivy
{
	class EventDispatcher;
}

namespace Ivy
{
	/* Event base class */
	class IVY_API Event
	{
		friend class _Ivy::EventDispatcher;
	public:
		virtual EventType GetEventType	() const { return EventType::E_NONE; }
		virtual int GetEventCategory	() const { return EventCategory::C_NONE; }
		inline bool IsInCategory		(EventCategory category) { return GetEventCategory() & category; }
		inline bool IsHandled			() const { return _handled; }
		inline void SetHandled			(bool handled) { _handled = handled; }

		/* Allows down-casting with (some level of) type security. If this Assert fails, You've 
		failed to ensure the Event you tried to down-cast is ACTUALLY of that type (use GetEventType()!) */
		template<typename T>
		inline T As						() { ASSERT(this->GetEventType() == T::GetStaticType()); return *(T*)this; }

	protected:
		bool							_handled = false;
	};

	/* Event callback template */
	using EventCallback = std::function<void(Event&)>;

	class Window;
}

namespace _Ivy
{
	class EventDispatcher
	{
		struct EventHandler
		{
		public:
			EventHandler(Ivy::EventCategory category, Ivy::EventCallback callback)
				: Category(category), Callback(callback) {}
			Ivy::EventCategory Category;
			Ivy::EventCallback Callback;
		};
	public:
		static void Register	(Ivy::EventCategory category, Ivy::EventCallback callback)	{ Get()->InternalRegister(category, callback); }
		static void Unregister	(Ivy::EventCategory category, Ivy::EventCallback callback)	{ Get()->InternalUnregister(category, callback); }
		static void Fire		(Ivy::Event& event)											{ Get()->InternalFire(event); }
	private:
		static EventDispatcher* Get() { return _instance ? _instance : _instance = new EventDispatcher(); }
		static EventDispatcher* _instance;
		EventDispatcher			() {};
		void InternalRegister	(Ivy::EventCategory category, Ivy::EventCallback callback);
		void InternalUnregister	(Ivy::EventCategory category, Ivy::EventCallback callback);
		void InternalFire		(Ivy::Event& event);
		std::vector<EventHandler> _handlers;
	};
}

/* Macro implementations for getting EventType enum val from derived Event */
#define IMPL_EVENT_CLASS_TYPE(type)\
	static Ivy::EventType GetStaticType() { return Ivy::##type; }\
	virtual Ivy::EventType GetEventType() const override { return GetStaticType(); }

/* Macro implementations for getting EventCategory flags from derived Event */
#define IMPL_EVENT_CLASS_CATEGORY(category)\
	virtual int GetEventCategory() const override { return category; }


/* IVY EVENT TYPE IMPLEMENTATIONS */

namespace Ivy
{

	/* KEY EVENTS */

	class IVY_API EventKey : public Event
	{
	public:
		inline int GetKey() const { return _keyCode; }
		inline int GetMods() const { return _mods; }
		IMPL_EVENT_CLASS_CATEGORY(EventCategory::C_INPUT | EventCategory::C_KEYBOARD)
	protected:
		EventKey(int keycode, int mods)
			: _keyCode(keycode), _mods(mods) {}
		int _keyCode;
		int _mods;
	};

	class IVY_API EventKeyPressed : public EventKey
	{
	public:
		EventKeyPressed(int keycode, int mods, bool repeat)
			: EventKey(keycode, mods), _repeat(repeat) {}
		IMPL_EVENT_CLASS_TYPE(EventType::E_KEY_PRESSED)
	protected:
		bool _repeat;
	};

	class IVY_API EventKeyReleased : public EventKey
	{
	public:
		EventKeyReleased(int keycode, int mods)
			: EventKey(keycode, mods) {}
		IMPL_EVENT_CLASS_TYPE(EventType::E_KEY_RELEASED)
	};


	/* MOUSE EVENTS */

	class IVY_API EventMouseButton : public Event
	{
	public:
		inline int GetMouseButton() const { return _button; }
		inline int GetMods() const { return _mods; }
		IMPL_EVENT_CLASS_CATEGORY(EventCategory::C_INPUT | EventCategory::C_MOUSE_BUTTON)
	protected:
		EventMouseButton(int button, int mods)
			: _button(button), _mods(mods) {}
		int _button;
		int _mods;
	};

	class IVY_API EventMouseButtonPressed : public EventMouseButton
	{
	public:
		EventMouseButtonPressed(int button, int mods)
			: EventMouseButton(button, mods) {}
		IMPL_EVENT_CLASS_TYPE(EventType::E_MOUSEBUTTON_PRESSED)
	};

	class IVY_API EventMouseButtonReleased : public EventMouseButton
	{
	public:
		EventMouseButtonReleased(int button, int mods)
			: EventMouseButton(button, mods) {}
		IMPL_EVENT_CLASS_TYPE(EventType::E_MOUSEBUTTON_RELEASED)
	};

	class IVY_API EventMouseMoved : public Event
	{
	public:
		EventMouseMoved(double x, double y)
			: _xPos(x), _yPos(y) {}
		inline double GetPosX() const { return _xPos; }
		inline double GetPosY() const { return _yPos; }
		IMPL_EVENT_CLASS_TYPE(EventType::E_MOUSE_MOVED)
		IMPL_EVENT_CLASS_CATEGORY(EventCategory::C_INPUT | EventCategory::C_MOUSE_MOVEMENT)
	private:
		double _xPos, _yPos;
	};

	class IVY_API EventMouseScrolled : public Event
	{
	public:
		EventMouseScrolled(double xOffset, double yOffset)
			: _xOffset(xOffset), _yOffset(yOffset) {}
		inline double GetOffsetX() const { return _xOffset; }
		inline double GetOffsetY() const { return _yOffset; }
		IMPL_EVENT_CLASS_TYPE(EventType::E_MOUSE_SCROLLED)
		IMPL_EVENT_CLASS_CATEGORY(EventCategory::C_INPUT | EventCategory::C_MOUSE_BUTTON)
	private:
		double _xOffset, _yOffset;
	};


	/* APPLICATION EVENTS */

	class Window;

	class IVY_API EventWindowClosed : public Event
	{
	public:
		EventWindowClosed() {};
		IMPL_EVENT_CLASS_TYPE(EventType::E_WINDOW_CLOSED)
		IMPL_EVENT_CLASS_CATEGORY(EventCategory::C_APPLICATION)
	};

	class IVY_API EventWindowFocused : public Event
	{
	public:
		EventWindowFocused() {};
		IMPL_EVENT_CLASS_TYPE(EventType::E_WINDOW_FOCUSED)
		IMPL_EVENT_CLASS_CATEGORY(EventCategory::C_APPLICATION)
	};

	class IVY_API EventWindowLostFocus : public Event
	{
	public:
		EventWindowLostFocus() {};
		IMPL_EVENT_CLASS_TYPE(EventType::E_WINDOW_LOST_FOCUS)
		IMPL_EVENT_CLASS_CATEGORY(EventCategory::C_APPLICATION)
	};

	class IVY_API EventWindowResized : public Event
	{
	public:
		EventWindowResized(int width, int height)
			: _width(width), _height(height) {}
		inline int GetWidth() const { return _width; }
		inline int GetHeight() const { return _height; }
		IMPL_EVENT_CLASS_TYPE(EventType::E_WINDOW_RESIZED)
		IMPL_EVENT_CLASS_CATEGORY(EventCategory::C_APPLICATION)
	private:
		int _width, _height;
	};

	class IVY_API EventWindowMoved : public Event
	{
	public:
		EventWindowMoved(float x, float y)
			: _xPos(x), _yPos(y) {}
		inline float GetPosX() const { return _xPos; }
		inline float GetPosY() const { return _yPos; }
		IMPL_EVENT_CLASS_TYPE(EventType::E_WINDOW_RESIZED)
		IMPL_EVENT_CLASS_CATEGORY(EventCategory::C_APPLICATION)
	private:
		float _xPos, _yPos;
	};
}