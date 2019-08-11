#pragma once
#include "Monster.h"
class Dragon
	: public Monster
{
public:
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, bool isGBuffers) override;
	bool m_isStone{ true };

private:
	LoadObject* m_BOBoxFrame{ nullptr };
	LoadObject* m_HeadFrame{ nullptr };
	
	MyBOBox* m_BOBoxForTailAttack{ nullptr };
	Texture* m_pStoneTexture{ nullptr };

protected:
	virtual void ReleaseMembers() override;
	virtual void ReleaseMemberUploadBuffers() override;
	virtual int GetAnimationHitID() override { return 0; }; // 현재 임시로 0 반환합니다.

	float GetFinishAttackUnlockTime();

public:
	Dragon(const std::string& entityID, const XMFLOAT3& SpawnPoint, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature);
	virtual ~Dragon();

	virtual void SubstractHP(int sub) override;
	virtual void Rotate(float x, float y, float z) override;
	virtual void Update(float) override;

	// 현재 수행할 액션(가속도관리하는클래스)를 관리합니다.
	virtual void UpdateState(float fElapsedTime) override;

	virtual void Animate(float fElapsedTime) override;
	virtual MyBOBox* GetBOBox() const override;

	bool GetisStone() const { return m_isStone; }
	void IsStone();
	void IsNotStone();

	const LoadObject* GetHeadFrame() const { return m_HeadFrame; }
};