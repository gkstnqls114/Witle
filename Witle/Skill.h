#pragma once

class PlayerStatus;

class ISkill
{  
public:
	virtual void DoNotUse() = 0;
	virtual void DoUse() = 0; 
};

class Skill
	: public ISkill
{ 
protected:
	bool  m_isUsing        { false }; // ��ų�� ���� �������
	float m_CooldownTime   { 0.f }; // ���� ���ð�
	float m_SkillTime      { 0.f }; // ��ų ��� ���� �ð�

public:
	Skill() {};
	virtual ~Skill();

	bool GetisUsing() const { return m_isUsing; }
	virtual void Update(float fTimeElapsed);
	virtual void DoNotUse() override = 0;
	virtual void DoUse() override = 0;
};