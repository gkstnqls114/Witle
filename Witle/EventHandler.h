#pragma once

// function ��ü�� ��Ƴ��� ���� ������ Ŭ����
class EventHandler
{
public:
	using Func = std::function<void()>;

private:
	Func _func;

public:
	int m_id{ 0 };
	static int m_Counter;

	EventHandler(const Func& func);
	void operator()();
	void operator=(const EventHandler& func);
	bool operator==(const EventHandler& del);
	bool operator!=(nullptr_t);
};