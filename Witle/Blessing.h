#pragma once
#include "Skill.h"

class UI3DImage;

class Blessing :
	public SelectableSkill
{
	float m_UsingTime{ 0 }; // 현재 스킬 사용시간
	const float m_MAXUsingTime{ 5.f }; // 스킬 유지 시간
	UI3DImage* m_BlessingEffect{ nullptr }; // sprite 이펙트

public:
	virtual void Deactive() override;
	virtual void Active() override;
	virtual bool IsFinish() override;
	virtual void UpdateActive(float fElapsedTime) override;

public:
	Blessing(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList);
	virtual ~Blessing();

};

