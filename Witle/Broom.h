#pragma once
#include "Skill.h" 

class Broom
	: public Skill
{
	PlayerMovement* m_pPlayerMovement{ nullptr };

private:
	virtual void DoNotUse() override;

public:
	Broom(PlayerMovement* playerStatus);
	virtual ~Broom();

	virtual void DoUse() override;
};