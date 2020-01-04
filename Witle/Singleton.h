#pragma once
#include "stdafx.h"

class SingletonInitializer;

template<typename T>
class Singleton
{
	static T* instance  ;
	static bool bInitialize;

private:
	Singleton(Singleton const&) = delete;            // 복사 숨김
	Singleton & operator=(Singleton const&) = delete; // 할당 숨김


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
			instance = new T();
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

	// 초기화 시점을 관리하기 위해 const SingletonMgr* 매개 인자를 갖도록 설정
	// 실제로 내부에서 해당 포인터를 사용하지는 않음.
	virtual void Init(const SingletonInitializer* singletonMgr) = 0;
	 
};


template<typename T> T* Singleton<T>::instance = nullptr; 