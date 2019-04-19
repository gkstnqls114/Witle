#pragma once
#include "GameObject.h"

class PlayerStatus;

class ISkill
	: public GameObject
{  
protected:
	ISkill(const std::string& entityID) : GameObject(entityID) {};
	~ISkill() {};
	 
protected:
	virtual void ReleaseMembers() = 0;
	virtual void ReleaseMemberUploadBuffers() = 0;
	 
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

protected:
	virtual void ReleaseMembers() = 0;
	virtual void ReleaseMemberUploadBuffers() = 0;

public:
	Skill() : ISkill("Skill") {};
	virtual ~Skill();

	bool GetisUsing() const { return m_isUsing; }
	virtual void Update(float fTimeElapsed);
	virtual void DoNotUse() override = 0;
	virtual void DoUse() override = 0;
};