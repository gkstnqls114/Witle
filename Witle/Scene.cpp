#include "stdafx.h" 
#include "Scene.h"

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
