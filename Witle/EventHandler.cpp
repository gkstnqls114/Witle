#include "stdafx.h"
#include "EventHandler.h"

int EventHandler::m_Counter{ 0 };

EventHandler::EventHandler(const Func & func)
{
	m_id = ++EventHandler::m_Counter;
}

void EventHandler::operator()()
{
	_func();
}

void EventHandler::operator=(const EventHandler & func)
{
	if (_func == nullptr)
	{
		_func = func;
		m_id = ++EventHandler::m_Counter;
	}
	else
	{
		std::cerr << "¾ÈµÊ" << std::endl;
	}
}

bool EventHandler::operator==(const EventHandler & del)
{
	return (m_id == del.m_id);
}

bool EventHandler::operator!=(nullptr_t)
{
	return (_func != nullptr);
}
