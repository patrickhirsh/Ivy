#include "Core/IvyPCH.h"
#include "Event/Event.h"

namespace _Ivy
{
	EventDispatcher* EventDispatcher::_instance = nullptr;

	void EventDispatcher::InternalRegister(Ivy::EventCategory category, Ivy::EventCallback callback)
	{
		_handlers.push_back(EventHandler(category, callback));
	}

	void EventDispatcher::InternalUnregister(Ivy::EventCategory category, Ivy::EventCallback callback)
	{
		for (auto it = _handlers.begin(); it != _handlers.end(); )
		{
			if (it->Category								== category &&
				it->Callback.target_type()					== callback.target_type() && 
				it->Callback.target<void(Ivy::Event&)>()	== callback.target<void(Ivy::Event&)>())
			{
				_handlers.erase(it);
			}
			else { ++it; }
		}
	}

	void EventDispatcher::InternalFire(Ivy::Event& event)
	{
		for (EventHandler handler : _handlers)
		{
			if (event.IsInCategory(handler.Category))
			{
				handler.Callback(event);
				if (event.IsHandled()) { return; }
			}
		}
	}
}