#include "stdafx.h"
#include "QuadtreeMgr.h"
#include "SingletonInitializer.h"


const SingletonInitializer* SingletonInitializer::m_pSingletonInitializer{ nullptr };

void SingletonInitializer::Init()
{ 
	QuadtreeMgr::GetInstance()->Init(m_pSingletonInitializer);
}
