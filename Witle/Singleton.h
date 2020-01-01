#pragma once
#include "stdafx.h"

template<typename T>
class Singleton
{
	static T* instance  ;

private:
	Singleton(Singleton const&) = delete;            // ���� ����
	Singleton & operator=(Singleton const&) = delete; // �Ҵ� ����

protected:
	Singleton()
	{ 
	}

	~Singleton()
	{

	}
	 
public:
	static T* GetInstance()
	{ 
		if (instance == nullptr)
		{
			instance = new T;
		}
		return instance;
	}

	static void ReleaseInstance()
	{
		if (instance)
		{
			delete instance;
			instance = nullptr;
		}
	}

};


template<typename T> T* Singleton<T>::instance = nullptr;