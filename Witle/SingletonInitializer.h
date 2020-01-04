#pragma once
#include "Singleton.h"

// Singleton ���� ������ �����ϱ� ���� ����ϴ� Ŭ����
// static ���� ������ �����Ѵ�.
class SingletonInitializer
{ 
private:
	// �ν��Ͻ��� �ʿ������ʰ� �ش� Ŭ���� �����͸��� �ʿ���մϴ�.
	// ���� �����Ҵ��� �ʿ�� ���� �ʽ��ϴ�.
	static const SingletonInitializer* m_pSingletonInitializer;

public:
	static void Init();
	static void ReleaseInstance();
	
};
