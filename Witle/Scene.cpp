#include "stdafx.h" 
#include "Scene.h"

#define _WITH_PLAYER_TOP

Scene::Scene()
{

}

Scene::~Scene()
{
	if (m_pd3dGraphicsRootSignature)
	{
		m_pd3dGraphicsRootSignature->Release();
		m_pd3dGraphicsRootSignature = nullptr;
	}
}
