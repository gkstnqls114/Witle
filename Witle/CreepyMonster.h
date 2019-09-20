#pragma once
#include "Monster.h"

class CreepyMonster
	: public Monster
{
	virtual int GetAnimationHitID() { return CREEPYMONSTER_HIT.ID; };

protected:
	virtual void ReleaseMembers() override;
	virtual void ReleaseMemberUploadBuffers() override;

public:
	CreepyMonster(const std::string& entityID, const XMFLOAT3& SpawnPoint, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature);
	virtual ~CreepyMonster();

	virtual void Init() override;

	virtual void Update(float) override;
	 
	virtual void Animate(float fElapsedTime) override;

};