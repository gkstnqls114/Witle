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
	bool  m_isUsing        { false }; // 스킬을 현재 사용주인
	float m_CooldownTime   { 0.f }; // 재사용 대기시간
	float m_SkillTime      { 0.f }; // 스킬 사용 중인 시간

public:
	Skill() {};
	virtual ~Skill();

	bool GetisUsing() const { return m_isUsing; }
	virtual void Update(float fTimeElapsed);
	virtual void DoNotUse() override = 0;
	virtual void DoUse() override = 0;
};