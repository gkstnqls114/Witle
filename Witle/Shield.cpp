#include "stdafx.h"
#include "ShieldEffect.h"
#include "Shield.h"


void Shield::Active()
{
}

bool Shield::IsFinish()
{ 
	bool isfinish = (m_UsingTime >= m_MAXUsingTime);
	if (isfinish) m_UsingTime = 0.f;
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
	:SelectableSkill("Shield", ENUM_SELECTABLESKILL::SELECTABLESKILL_SHIELD, COOLTIME_SHIELD)
{
	m_skillEffect = new ShieldEffect("ShieldEffect", pd3dDevice, pd3dCommandList);
}


Shield::~Shield()
{
}
