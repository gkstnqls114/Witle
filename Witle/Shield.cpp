#include "stdafx.h"
#include "PlayerManager.h"
#include "Player.h"
#include "ShieldEffect.h"
#include "Shield.h"


void Shield::Deactive()
{
	PlayerManager::GetMainPlayer()->DeactiveShield();
	m_UsingTime = 0.f;
}

void Shield::Active()
{ 
	PlayerManager::GetMainPlayer()->ActiveShield();
}

bool Shield::IsFinish()
{ 
	bool isfinish = (m_UsingTime >= m_MAXUsingTime);
	if (isfinish)
	{
		Deactive();
	}
	return isfinish;
}

void Shield::UpdateActive(float fElapsedTime)
{
	if (m_UsingTime >= m_MAXUsingTime)
	{
		m_UsingTime = m_MAXUsingTime;
	}
	else
	{
		m_UsingTime += fElapsedTime; 
	}
}

Shield::Shield(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
	:SelectableSkill("Shield", ENUM_SELECTABLESKILL::SELECTABLESKILL_SHIELD, COOLTIME_SHIELD, 5.f)
{
	m_skillEffect = new ShieldEffect("ShieldEffect", pd3dDevice, pd3dCommandList);
}


Shield::~Shield()
{
}
