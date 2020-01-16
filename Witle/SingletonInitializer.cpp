#include "stdafx.h"
#include "QuadtreeMgr.h"
#include "MapInfoMgr.h"
#include "SingletonInitializer.h"


const SingletonInitializer* SingletonInitializer::m_pSingletonInitializer{ nullptr };

void SingletonInitializer::Init()
{ 
	QtTerrainCalculator::GetInstance()->Init(m_pSingletonInitializer);
	QtTerrainCalculator::GetInstance()->Init(MapInfoMgr::GetMapCenter(), XMFLOAT3{ MapInfoMgr::GetMapExtentsX(), 10000.f, MapInfoMgr::GetMapExtentsZ() }, MapInfoMgr::GetMapSizeX() / 4.f);
}

void SingletonInitializer::ReleaseInstance()
{
	QtTerrainCalculator::ReleaseInstance();
}