#pragma once
#include "IvyPCH.h"


#define BIND_EVENT_FUNCTION(x) std::bind(&x, std::placeholders::_1)
#define BIND_EVENT_MEMBER_FUNCTION(x) std::bind(&x, this, std::placeholders::_1)

namespace Ivy
{
	enum class EventType
	{
		NONE = 0,
		WINDOW_CLOSED,
		WINDOW_FOCUSED,
		WINDOW_LOST_FOCUS,
		WINDOW_RESIZED,
		WINDOW_MOVED,
		KEY_PRESSED,
		KEY_RELEASED,
		MOUSEBUTTON_PRESSED,
		MOUSEBUTTON_RELEASED,
		MOUSE_MOVED,
		MOUSE_SCROLLED
	};

	enum EventCategory
	{
		NONE = 0,
		APPLICATION = BIT(0),
		INPUT = BIT(1),
		KEYBOARD = BIT(2),
		MOUSE_MOVEMENT = BIT(3),
		MOUSE_BUTTON = BIT(4)
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
		virtual EventType GetEventType	() const { return EventType::NONE; }
		virtual int GetEventCategory	() const { return EventCategory::NONE; }
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
	/* Event Dispatcher */
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
		EventDispatcher		() {};
		void Register		(Ivy::EventCategory category, Ivy::EventCallback callback);
		void Unregister		(Ivy::EventCategory category, Ivy::EventCallback callback);
		void Fire			(Ivy::Event& event);
	private:
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
		IMPL_EVENT_CLASS_CATEGORY(EventCategory::INPUT | EventCategory::KEYBOARD)
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
		IMPL_EVENT_CLASS_TYPE(EventType::KEY_PRESSED)
	protected:
		bool _repeat;
	};

	class IVY_API EventKeyReleased : public EventKey
	{
	public:
		EventKeyReleased(int keycode, int mods)
			: EventKey(keycode, mods) {}
		IMPL_EVENT_CLASS_TYPE(EventType::KEY_RELEASED)
	};


	/* MOUSE EVENTS */

	class IVY_API EventMouseButton : public Event
	{
	public:
		inline int GetMouseButton() const { return _button; }
		inline int GetMods() const { return _mods; }
		IMPL_EVENT_CLASS_CATEGORY(EventCategory::INPUT | EventCategory::MOUSE_BUTTON)
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
		IMPL_EVENT_CLASS_TYPE(EventType::MOUSEBUTTON_PRESSED)
	};

	class IVY_API EventMouseButtonReleased : public EventMouseButton
	{
	public:
		EventMouseButtonReleased(int button, int mods)
			: EventMouseButton(button, mods) {}
		IMPL_EVENT_CLASS_TYPE(EventType::MOUSEBUTTON_RELEASED)
	};

	class IVY_API EventMouseMoved : public Event
	{
	public:
		EventMouseMoved(double x, double y)
			: _xPos(x), _yPos(y) {}
		inline double GetPosX() const { return _xPos; }
		inline double GetPosY() const { return _yPos; }
		IMPL_EVENT_CLASS_TYPE(EventType::MOUSE_MOVED)
		IMPL_EVENT_CLASS_CATEGORY(EventCategory::INPUT | EventCategory::MOUSE_MOVEMENT)
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
		IMPL_EVENT_CLASS_TYPE(EventType::MOUSE_SCROLLED)
		IMPL_EVENT_CLASS_CATEGORY(EventCategory::INPUT | EventCategory::MOUSE_BUTTON)
	private:
		double _xOffset, _yOffset;
	};


	/* APPLICATION EVENTS */

	class Window;

	class IVY_API EventWindowClosed : public Event
	{
	public:
		EventWindowClosed(Window* window)
			: _window(window) {}
		inline Window* GetWindow() const { return _window; }
		IMPL_EVENT_CLASS_TYPE(EventType::WINDOW_CLOSED)
		IMPL_EVENT_CLASS_CATEGORY(EventCategory::APPLICATION)
	private:
		Window* _window;
	};

	class IVY_API EventWindowFocused : public Event
	{
	public:
		EventWindowFocused(Window* window)
			: _window(window) {}
		inline Window* GetWindow() const { return _window; }
		IMPL_EVENT_CLASS_TYPE(EventType::WINDOW_FOCUSED)
		IMPL_EVENT_CLASS_CATEGORY(EventCategory::APPLICATION)
	private:
		Window* _window;
	};

	class IVY_API EventWindowLostFocus : public Event
	{
	public:
		EventWindowLostFocus(Window* window)
			: _window(window) {}
		inline Window* GetWindow() const { return _window; }
		IMPL_EVENT_CLASS_TYPE(EventType::WINDOW_LOST_FOCUS)
		IMPL_EVENT_CLASS_CATEGORY(EventCategory::APPLICATION)
	private:
		Window* _window;
	};

	class IVY_API EventWindowResized : public Event
	{
	public:
		EventWindowResized(Window* window, int width, int height)
			: _window(window), _width(width), _height(height) {}
		inline Window* GetWindow() const { return _window; }
		inline int GetWidth() const { return _width; }
		inline int GetHeight() const { return _height; }
		IMPL_EVENT_CLASS_TYPE(EventType::WINDOW_RESIZED)
		IMPL_EVENT_CLASS_CATEGORY(EventCategory::APPLICATION)
	private:
		Window* _window;
		int _width, _height;
	};

	class IVY_API EventWindowMoved : public Event
	{
	public:
		EventWindowMoved(Window* window, float x, float y)
			: _window(window), _xPos(x), _yPos(y) {}
		inline Window* GetWindow() const { return _window; }
		inline float GetPosX() const { return _xPos; }
		inline float GetPosY() const { return _yPos; }
		IMPL_EVENT_CLASS_TYPE(EventType::WINDOW_RESIZED)
		IMPL_EVENT_CLASS_CATEGORY(EventCategory::APPLICATION)
	private:
		Window* _window;
		float _xPos, _yPos;
	};
}