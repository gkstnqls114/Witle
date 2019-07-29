#include "stdafx.h"
#include "SkillEffect.h"
#include "Skill.h"

FixedSkill::~FixedSkill()
{
}

void FixedSkill::Update(float fTimeElapsed)
{
	if (!m_isUsing) return;
	if (m_UsingTime < m_SkillTime)
	{
		DoNotUse(); 
	}

	m_SkillTime += fTimeElapsed; 
}

void SelectableSkill::ReleaseMembers()
{
	if (m_skillEffect)
	{
		m_skillEffect->ReleaseObjects();
		delete m_skillEffect;
		m_skillEffect = nullptr;
	}
}

void SelectableSkill::ReleaseMemberUploadBuffers()
{
	if (m_skillEffect) m_skillEffect->ReleaseUploadBuffers(); 
}

void SelectableSkill::Update(float fElapsedTime)
{
	// 쿨타임 조절
	if (RemainCoolTime <= 0.f)
	{
		RemainCoolTime = 0.f;
	}
	else
	{
		RemainCoolTime -= fElapsedTime;
		RemainCoolTimePrecentage = RemainCoolTime / m_skillEffect->m_CoolTime;
	}
	 
	// 만약 끝나는 조건이 된다면 Active 를 false로 설정
	bool isFinish = IsFinish();
	if (isActive && isFinish)
	{
		isActive = false;
	}
	 
	// 만약 활성화가 되어있다면 업데이트
	if (isActive)
	{
		// 스킬 이펙트 이동
		m_skillEffect->Update(fElapsedTime);
		UpdateActive(fElapsedTime);
	} 
}

SelectableSkill::~SelectableSkill()
{
}
