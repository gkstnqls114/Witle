#include "stdafx.h"

// Skill Effect 관련 ////////////////////////////
#include "SkillEffect.h"
#include "FireBall.h"
#include "IceBall.h"
#include "LightningBall.h" 
#include "ShieldEffect.h"
#include "HealingEffect.h"
#include "BlessingEffect.h"
// Skill Effect 관련 ////////////////////////////

#include "SkillStg.h"

SkillStg* SkillStg::m_Instance{ nullptr };

SkillStg::SkillStg()
{
}

SkillStg::~SkillStg()
{
}

void SkillStg::BuildObjects(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
{
	skillEffect[ENUM_SKILL::SKILL_FIREBALL] = new FireBall("FireBall", pd3dDevice, pd3dCommandList);
	skillEffect[ENUM_SKILL::SKILL_ICEBALL] = new IceBall("IceBall", pd3dDevice, pd3dCommandList);
	skillEffect[ENUM_SKILL::SKILL_LIGHTNINGBALL] = new LightningBall("LightingBall", pd3dDevice, pd3dCommandList);

	skillEffect[ENUM_SKILL::SKILL_SHIELD] = new ShieldEffect("Shield", pd3dDevice, pd3dCommandList);
	skillEffect[ENUM_SKILL::SKILL_BLESSING] = new BlessingEffect("Blessing", pd3dDevice, pd3dCommandList);
	skillEffect[ENUM_SKILL::SKILL_HEALING] = new HealingEffect("Healing", pd3dDevice, pd3dCommandList);

	skillEffect[ENUM_SKILL::SKILL_METEOR] = new FireBall("FireBall", pd3dDevice, pd3dCommandList);
	skillEffect[ENUM_SKILL::SKILL_ICESPEAR] = new IceBall("FireBall", pd3dDevice, pd3dCommandList);
}

void SkillStg::ReleaseUploadBuffers()
{
	for (auto& se : skillEffect)
	{
		se->ReleaseUploadBuffers();
	}
}

void SkillStg::ReleaseObjects()
{
	for (auto& se : skillEffect)
	{
		se->ReleaseObjects();
		delete se;
		se = nullptr;
	}
}
 