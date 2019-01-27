#pragma once

// function 객체를 담아놓기 위해 래핑한 클래스
class EventHandler
{
public:
	using Func = std::function<void()>;

private:
	Func _func;

public:
	int m_id{ 0 };
	static int m_Counter;

	EventHandler() : m_id{ 0 } {};
	EventHandler(const Func& func);
	void operator()();
	void operator=(const EventHandler& func);
	bool operator==(const EventHandler& del);
	bool operator!=(nullptr_t);
};