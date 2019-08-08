#include "stdafx.h"

#include "SkillEffect.h"
// Selectable Skill 관련 //////////////////////////// 
#include "Skill.h"
#include "FireBallSkill.h"
#include "IceBallSkill.h"
#include "LightningBallSkill.h"
#include "Shield.h"
#include "Blessing.h"
#include "Healing.h"
// Selectable Skill 관련 ////////////////////////////

#include "EffectMgr.h"

EffectMgr* EffectMgr::m_Instance{ nullptr };

EffectMgr::EffectMgr()
{
}

EffectMgr::~EffectMgr()
{
}

void EffectMgr::BuildObjects(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
{


}

void EffectMgr::ReleaseUploadBuffers()
{

}

void EffectMgr::ReleaseObjects()
{

}

void EffectMgr::Update(float fElapsedTime)
{
	
}

void EffectMgr::Render(ID3D12GraphicsCommandList * pd3dCommandList)
{
	XMFLOAT4X4 world;

	for (const auto& pos : m_effectPositionList)
	{

	}
}

void EffectMgr::AddEffectPosition(ENUM_EFFECT type, const XMFLOAT3 pos)
{
	switch (type)
	{
	case EFFECT_NONE:

		break;
	case EFFECT_TEST:
		m_effectPositionList.push_back(pos);
		break;
	default:
		break;
	}
} 