#pragma once
#include "Skill.h"

class Healing :
	public SelectableSkill
{
public:
	Healing(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList);
	virtual ~Healing();
};

