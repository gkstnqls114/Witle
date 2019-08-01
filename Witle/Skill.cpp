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
	// ��Ÿ�� ����
	if (RemainCoolTime <= 0.f)
	{
		RemainCoolTime = 0.f;
	}
	else
	{
		RemainCoolTime -= fElapsedTime;
		RemainCoolTimePrecentage = RemainCoolTime / m_skillEffect->m_CoolTime;
	}
	 
	// ���� ������ ������ �ȴٸ� Active �� false�� ����
	bool isFinish = IsFinish();
	if (isActive && isFinish)
	{
		isActive = false;
	}
	 
	// ���� Ȱ��ȭ�� �Ǿ��ִٸ� ������Ʈ
	if (isActive)
	{
		// ��ų ����Ʈ �̵�
		m_skillEffect->Update(fElapsedTime);
		UpdateActive(fElapsedTime);
	} 
}

SelectableSkill::~SelectableSkill()
{
}
