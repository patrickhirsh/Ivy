#pragma once


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
		NONE			= 0,
		APPLICATION		= BIT(0),
		INPUT			= BIT(1),
		KEYBOARD		= BIT(2),
		MOUSE_MOVEMENT	= BIT(3),
		MOUSE_BUTTON	= BIT(4)
	};

	/* Event base class */
	class IVY_API Event
	{
		//friend class _Ivy::EventDispatcher;
	public:
		virtual EventType GetEventType() const = 0;
		virtual int GetEventCategory() const = 0;
		inline bool IsInCategory(EventCategory category) { return GetEventCategory() & category; }
	protected:
		bool _handled = false;
	};
}

namespace _Ivy
{
	/* Event Dispatcher */
	class EventDispatcher
	{

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
		inline int GetKeyCode() const { return _keyCode; }
		IMPL_EVENT_CLASS_CATEGORY(EventCategory::INPUT | EventCategory::KEYBOARD)
	protected:
		EventKey(int keycode)
			: _keyCode(keycode) {}
		int _keyCode;
	};

	class IVY_API EventKeyPressed : public EventKey
	{
	public:
		EventKeyPressed(int keycode)
			: EventKey(keycode) {}
		IMPL_EVENT_CLASS_TYPE(EventType::KEY_PRESSED)
	};

	class IVY_API EventKeyReleased : public EventKey
	{
	public:
		EventKeyReleased(int keycode)
			: EventKey(keycode) {}
		IMPL_EVENT_CLASS_TYPE(EventType::KEY_RELEASED)
	};


	/* MOUSE EVENTS */

	class IVY_API EventMouseButton : public Event
	{
	public:
		inline int GetMouseButton() const { return _button; }
		IMPL_EVENT_CLASS_CATEGORY(EventCategory::INPUT | EventCategory::MOUSE_BUTTON)
	protected:
		EventMouseButton(int button)
			: _button(button) {}
		int _button;
	};

	class IVY_API EventMouseButtonPressed : public EventMouseButton
	{
	public:
		EventMouseButtonPressed(int button)
			: EventMouseButton(button) {}
		IMPL_EVENT_CLASS_TYPE(EventType::MOUSEBUTTON_PRESSED)
	};

	class IVY_API EventMouseButtonReleased : public EventMouseButton
	{
	public:
		EventMouseButtonReleased(int button)
			: EventMouseButton(button) {}
		IMPL_EVENT_CLASS_TYPE(EventType::MOUSEBUTTON_RELEASED)
	};

	class IVY_API EventMouseMoved : public Event
	{
	public:
		EventMouseMoved(float x, float y)
			: _xPos(x), _yPos(y) {}
		inline float GetPosX() const { return _xPos; }
		inline float GetPosY() const { return _yPos; }
		IMPL_EVENT_CLASS_TYPE(EventType::MOUSE_MOVED)
		IMPL_EVENT_CLASS_CATEGORY(EventCategory::INPUT | EventCategory::MOUSE_MOVEMENT)
	private:
		float _xPos, _yPos;
	};

	class IVY_API EventMouseScrolled : public Event
	{
	public:
		EventMouseScrolled(float xOffset, float yOffset)
			: _xOffset(xOffset), _yOffset(yOffset) {}
		inline float GetOffsetX() const { return _xOffset; }
		inline float GetOffsetY() const { return _yOffset; }
		IMPL_EVENT_CLASS_TYPE(EventType::MOUSE_SCROLLED)
		IMPL_EVENT_CLASS_CATEGORY(EventCategory::INPUT | EventCategory::MOUSE_BUTTON)
	private:
		float _xOffset, _yOffset;
	};


	/* APPLICATION EVENTS */

	class IVY_API EventWindowClosed : public Event
	{
	public:
		EventWindowClosed() {}
		IMPL_EVENT_CLASS_TYPE(EventType::WINDOW_CLOSED)
		IMPL_EVENT_CLASS_CATEGORY(EventCategory::APPLICATION)
	};

	class IVY_API EventWindowFocused : public Event
	{
	public:
		EventWindowFocused() {}
		IMPL_EVENT_CLASS_TYPE(EventType::WINDOW_FOCUSED)
		IMPL_EVENT_CLASS_CATEGORY(EventCategory::APPLICATION)
	};

	class IVY_API EventWindowLostFocus : public Event
	{
	public:
		EventWindowLostFocus() {}
		IMPL_EVENT_CLASS_TYPE(EventType::WINDOW_LOST_FOCUS)
		IMPL_EVENT_CLASS_CATEGORY(EventCategory::APPLICATION)
	};

	class IVY_API EventWindowResized : public Event
	{
	public:
		EventWindowResized(int width, int height)
			: _width(width), _height(height) {}
		inline int GetWidth() const { return _width; }
		inline int GetHeight() const { return _height; }
		IMPL_EVENT_CLASS_TYPE(EventType::WINDOW_RESIZED)
		IMPL_EVENT_CLASS_CATEGORY(EventCategory::APPLICATION)
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
		IMPL_EVENT_CLASS_TYPE(EventType::WINDOW_RESIZED)
		IMPL_EVENT_CLASS_CATEGORY(EventCategory::APPLICATION)
	private:
		float _xPos, _yPos;
	};
}