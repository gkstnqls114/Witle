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

void Scene::CreateRootSignature(ID3D12Device * pd3dDevice)
{
	m_pd3dGraphicsRootSignature = CreateGraphicsRootSignature(pd3dDevice);
}
