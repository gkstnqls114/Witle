#pragma once
#include "Singleton.h"

// Singleton 생성 시점을 관리하기 위해 사용하는 클래스
// static 전역 변수로 관리한다.
class SingletonInitializer
{ 
private:
	// 인스턴스가 필요하지않고 해당 클래스 포인터만을 필요로합니다.
	// 따라서 동적할당은 필요로 하지 않습니다.
	static const SingletonInitializer* m_pSingletonInitializer;

public:
	static void Init();
	static void ReleaseInstance();
	
};
