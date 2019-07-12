#include "stdafx.h"

// Skill Effect 관련 ////////////////////////////
#include "SkillEffect.h"
#include "FireBall.h"
#include "IceBall.h"
#include "ElectricBall.h"
// Skill Effect 관련 ////////////////////////////

#include "Player.h"
#include "PlayerSkillMgr.h"

PlayerSkillMgr::PlayerSkillMgr(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
{
	m_skill = new SKILL[m_count];

	for (int x = 0; x < m_count; ++x)
	{
		m_skill[x].skillEffect = new FireBall("FireBall", pd3dDevice, pd3dCommandList);
	}
}

PlayerSkillMgr::~PlayerSkillMgr()
{
}

void PlayerSkillMgr::Update(float fElapsedTime)
{
}

void PlayerSkillMgr::Render(ID3D12GraphicsCommandList * pd3dCommandList, bool isGBuffers)
{
	for (int x = 0; x < m_count; ++x)
	{
		if (!m_skill[x].isActive) continue;
		m_skill[x].skillEffect->Render(pd3dCommandList, isGBuffers);
	}
}

void PlayerSkillMgr::Activate(int index)
{
	m_skill[index].isActive = true;
}
