#include "stdafx.h"
#include "SkillEffect.h"
#include "Skill.h"

FixedSkill::~FixedSkill()
{
}

void FixedSkill::Init()
{ 
	 m_isPrepare = false ;   // ��ų�� ���� �������
	 m_isUsing = false ; // ��ų�� ���� �������
	 // m_UsingTime = 0.f ; // ���� ���ð�
	 m_SkillTime = 0.f ; // ��ų ��� ���� �ð�
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
		RemainCoolTimePrecentage = RemainCoolTime / m_CoolTime;
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

void SelectableSkill::Init()
{ 
	 isActive = false  ; // Ȱ��ȭ ����
	 RemainCoolTime = 0.F; // ���� ��Ÿ��
	 RemainCoolTimePrecentage = 0.F; // ���� ��Ÿ�� .. 0�̸� ��ų ��밡�� 1�̸� �� ��ų ����� 
}
