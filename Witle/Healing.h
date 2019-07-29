#pragma once
#include "Skill.h"

class Healing :
	public SelectableSkill
{
public:
	virtual void Active() override;
	virtual bool IsFinish() override; // active하자마자 회복되므로 언제나 true 반환
	virtual void UpdateActive(float fElapsedTime) override;

public:
	Healing(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList);
	virtual ~Healing();
};
