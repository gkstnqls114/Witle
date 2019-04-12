#pragma once

class PlayerStatus;

class Skill
{ 
protected:
	bool  m_isUsing        { false }; // 스킬을 현재 사용주인
	float m_CooldownTime   { 0.f }; // 재사용 대기시간
	float m_SkillTime      { 0.f }; // 스킬 사용 중인 시간

	virtual void DoNotUse() = 0;
public:
	Skill() {};
	virtual ~Skill();

	void Update(float fTimeElapsed);
	virtual void Use() = 0;
};