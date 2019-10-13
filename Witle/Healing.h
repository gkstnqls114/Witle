#pragma once
#include "Skill.h"

class Healing :
	public SelectableSkill
{
public:
	virtual void Deactive() override;
	virtual void Active() override;
	virtual bool IsFinish() override; // active���ڸ��� ȸ���ǹǷ� ������ true ��ȯ
	virtual void UpdateActive(float fElapsedTime) override;

public:
	Healing(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList);
	virtual ~Healing();
};
