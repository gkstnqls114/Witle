#include "stdafx.h"
#include "QuadtreeMgr.h"
#include "SingletonInitializer.h"

void SingletonInitializer::Init()
{
	QuadtreeMgr::GetInstance()->Init(m_pSingletonInitializer);
}
