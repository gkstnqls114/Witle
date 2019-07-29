#pragma once
#include "Skill.h"

class BallSkill :
	public SelectableSkill
{
public:
	virtual void Active() override {};

public:
	BallSkill();
	virtual ~BallSkill();

};

