#pragma once
#include "EventHandler.h"

// 이벤트 핸들러를 관리한다.
class Event
{
	std::vector<std::unique_ptr<EventHandler>> m_Handlers;
	
	void NotifyHandlers();

public:
	Event();
	~Event();

	void AddHandler(const EventHandler& handler);
	void RemoveHandler(const EventHandler& handler);
	void operator()();
	Event & operator+=(const EventHandler& handler);
	Event & operator-=(const EventHandler& handler);
};