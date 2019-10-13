#pragma once
#include "Skill.h"

class BallSkill :
	public SelectableSkill
{
	// ��ų�� ���ư��� �ִ� �Ÿ�
	const float m_distance{ 2000.f };

public:
	virtual void Deactive() override {};
	virtual void Active() override {};
	virtual bool IsFinish() override;
	virtual void UpdateActive(float fElapsedTime) override {};

public:
	BallSkill(const std::string& entityID, ENUM_SELECTABLESKILL skilltype, float cooltime);
	virtual ~BallSkill();

};

