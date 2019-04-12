#pragma once

class Skill
{
	bool  m_isUsing        { false }; // 스킬을 현재 사용주인
	float m_CooldownTime  { 0.f }; // 재사용 대기시간
	float m_SkillTime     { 0.f }; // 스킬 사용시칸

public:
	Skill();
	virtual ~Skill();

};