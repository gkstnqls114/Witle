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

	for (auto& effect : m_EffectList)
	{
		effect.pEffect = new HitEffect(m_GameObject, pd3dDevice, pd3dCommandList);
	} 
}

void EffectMgr::ReleaseUploadBuffers()
{
	for (auto& effect : m_EffectList)
	{
		effect.pEffect->ReleaseUploadBuffers();
		delete effect.pEffect;
		effect.pEffect = nullptr;
	} 
}

void EffectMgr::ReleaseObjects()
{ 
	for (auto& effect : m_EffectList)
	{
		effect.pEffect->ReleaseObjects();
		delete effect.pEffect;
		effect.pEffect = nullptr;
	}
}

void EffectMgr::Update(float fElapsedTime)
{ 
	for (auto& effect : m_EffectList)
	{
		effect.pEffect->Update(fElapsedTime);
	}
}

void EffectMgr::Render(ID3D12GraphicsCommandList * pd3dCommandList)
{ 
	for (const auto& effect : m_EffectList)
	{ 
		effect.pEffect->Render(pd3dCommandList, effect.pos);
	}
}

void EffectMgr::AddEffectPosition(ENUM_EFFECT type, const XMFLOAT3 pos)
{
	switch (type)
	{
	case EFFECT_NONE:

		break;
	case EFFECT_TEST:
		for (auto& effect : m_EffectList)
		{
			if (effect.pEffect->IsActive()) continue;

			effect.pos = pos;
			effect.pEffect->Active();
		}
		break;
	default:
		break;
	}
} 