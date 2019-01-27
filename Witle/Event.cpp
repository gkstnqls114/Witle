#include "stdafx.h"
#include "Event.h"

void Event::NotifyHandlers()
{
	for (std::vector<std::unique_ptr<EventHandler>>::iterator func = m_Handlers.begin();
		func != m_Handlers.end();
		++func)
	{
		if (*func != nullptr)
		{
			(*(*func))();
		}
	}
}

Event::Event()
{
}

Event::~Event()
{
}

void Event::AddHandler(const EventHandler & handler)
{
	m_Handlers.push_back(std::unique_ptr<EventHandler>(new EventHandler{ handler }));
}

void Event::RemoveHandler(const EventHandler & handler)
{
	for (std::vector<std::unique_ptr<EventHandler>>::iterator to_remove = m_Handlers.begin();
		to_remove != m_Handlers.end();
		++to_remove)
	{
		if (*(*to_remove) == handler)
		{
			m_Handlers.erase(to_remove);
			break;
		}
	}
}

void Event::operator()()
{
	NotifyHandlers();
}

Event & Event::operator+=(const EventHandler & handler)
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	AddHandler(handler);

	return *this;
}

Event & Event::operator-=(const EventHandler & handler)
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	RemoveHandler(handler);

	return *this;
}
