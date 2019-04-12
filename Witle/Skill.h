#pragma once

class PlayerStatus;

class Skill
{ 
protected:
	bool  m_isUsing        { false }; // ��ų�� ���� �������
	float m_CooldownTime   { 0.f }; // ���� ���ð�
	float m_SkillTime      { 0.f }; // ��ų ��� ���� �ð�

	virtual void DoNotUse() = 0;
public:
	Skill() {};
	virtual ~Skill();

	void Update(float fTimeElapsed);
	virtual void Use() = 0;
};