#pragma once
#include "stdafx.h"

class SingletonInitializer;

template<typename T>
class Singleton
{
	static T* instance  ;
	static bool bInitialize;

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

	// �ʱ�ȭ ������ �����ϱ� ���� const SingletonMgr* �Ű� ���ڸ� ������ ����
	// ������ ���ο��� �ش� �����͸� ��������� ����.
	virtual void Init(const SingletonInitializer* singletonMgr) = 0;
	 
};


template<typename T> T* Singleton<T>::instance = nullptr; 