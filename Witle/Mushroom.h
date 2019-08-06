#pragma once
#include "Monster.h"

class Mushroom
	: public Monster
{
	virtual int GetAnimationHitID() { return ANI_MUSHROOM_HIT.ID; };

protected:
	virtual void ReleaseMembers() override;
	virtual void ReleaseMemberUploadBuffers() override;

public:
	Mushroom(const std::string& entityID, const XMFLOAT3& SpawnPoint, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature);
	virtual ~Mushroom();

	virtual void Update(float) override;

	// ���� ������ �׼�(���ӵ������ϴ�Ŭ����)�� �����մϴ�.
	virtual void UpdateState(float fElapsedTime) override;

	virtual void Animate(float fElapsedTime) override;

};