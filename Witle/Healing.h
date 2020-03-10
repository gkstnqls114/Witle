#pragma once
#include "Skill.h"

class HealingSpriteEffect;

class Healing :
	public SelectableSkill
{
private:
	HealingSpriteEffect* m_HealingEffect{ nullptr }; //sprite effect
	XMFLOAT3 m_EffectPos{ 0, 0, 0 };

protected:
	virtual void ReleaseMembers() override;
	virtual void ReleaseMemberUploadBuffers() override;

	virtual void Update(float) override;

	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, bool isGBuffers) override;

public:
	virtual void Deactive() override;
	virtual void Active() override;
	virtual bool IsFinish() override; // active���ڸ��� ȸ���ǹǷ� ������ true ��ȯ
	virtual void UpdateActive(float fElapsedTime) override;

public:
	Healing(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList);
	virtual ~Healing();
};
