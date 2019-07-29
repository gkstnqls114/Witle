#pragma once
#include "Skill.h"

class Blessing :
	public SelectableSkill
{
public:
	virtual void Active() override;

public:
	Blessing(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList);
	virtual ~Blessing();

};

