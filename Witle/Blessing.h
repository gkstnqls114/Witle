#pragma once
#include "Skill.h"

class Blessing :
	public SelectableSkill
{
	float m_UsingTime{ 0 }; // ���� ��ų ���ð�
	const float m_MAXUsingTime{ 5.f }; // ��ų ���� �ð�

public:
	virtual void Deactive() override;
	virtual void Active() override;
	virtual bool IsFinish() override;
	virtual void UpdateActive(float fElapsedTime) override;

public:
	Blessing(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList);
	virtual ~Blessing();

};

