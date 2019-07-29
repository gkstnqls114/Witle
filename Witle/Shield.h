#pragma once
#include "Skill.h"

class Shield :
	public SelectableSkill
{
public:
	virtual void Active() override {};

public:
	Shield(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList);
	virtual ~Shield();
};

