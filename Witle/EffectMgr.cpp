#include "stdafx.h"

#include "HitEffect.h"
#include "GameObject.h"

#include "EffectMgr.h"

EffectMgr* EffectMgr::m_Instance{ nullptr };
 
EffectMgr::~EffectMgr()
{
}

void EffectMgr::BuildObjects(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
{
	m_GameObject = new EmptyGameObject("test");

	for (int i = 0 ; i < m_MaxEffect ; ++i)
	{
		m_EffectList[i].pEffect = new HitEffect(m_GameObject, pd3dDevice, pd3dCommandList);
	} 
}

void EffectMgr::ReleaseUploadBuffers()
{
	for (int i = 0; i < m_MaxEffect; ++i)
	{
		m_EffectList[i].pEffect->ReleaseUploadBuffers(); 
	} 
}

void EffectMgr::ReleaseObjects()
{
	for (int i = 0; i < m_MaxEffect; ++i)
	{
		m_EffectList[i].pEffect->ReleaseObjects();
		delete m_EffectList[i].pEffect;
		m_EffectList[i].pEffect = nullptr;
	}
}

void EffectMgr::Update(float fElapsedTime)
{
	for (int i = 0; i < m_MaxEffect; ++i)
	{
		m_EffectList[i].pEffect->Update(fElapsedTime);
	}
}

void EffectMgr::Render(ID3D12GraphicsCommandList * pd3dCommandList)
{
	for (int i = 0; i < m_MaxEffect; ++i)
	{
		m_EffectList[i].pEffect->Render(pd3dCommandList, m_EffectList[i].pos);
	}
}

void EffectMgr::AddEffectPosition(ENUM_EFFECT type, const XMFLOAT3 pos)
{
	switch (type)
	{
	case EFFECT_NONE:

		break;
	case EFFECT_TEST:
		for (int i = 0; i < m_MaxEffect; ++i)
		{
			if (m_EffectList[i].pEffect->IsActive()) continue;

			m_EffectList[i].pos = pos;
			m_EffectList[i].pEffect->Active();
		}
		break;
	default:
		break;
	}
} 