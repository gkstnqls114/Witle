#pragma once
#include "SkillEffect.h"

class SphereMesh;
class Movement;
class MyBSphere;

class ShieldEffect :
	public SkillEffect
{
public:
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, bool isGBuffers) override;
	virtual void UpdateCollider() override;

protected:
	virtual void ReleaseMembers() override;
	virtual void ReleaseMemberUploadBuffers() override;

private:
	SphereMesh* m_ShieldEffectMesh{ nullptr };
	

public:
	ShieldEffect(const std::string& entityID, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual ~ShieldEffect();

	virtual void Update(float) override;
};