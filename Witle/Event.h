#pragma once
#include "EventHandler.h"

// �̺�Ʈ �ڵ鷯�� �����Ѵ�.
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