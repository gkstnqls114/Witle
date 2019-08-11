#pragma once
#include "SkillEffect.h"

class Texture;
class CLoadedModelInfo;
class LoadObject;
class SphereMesh;

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
	CLoadedModelInfo*  m_ShieldEffectModelInfo{ nullptr };
	LoadObject*		   m_ShieldEffectObject{ nullptr };
	SphereMesh * m_ShieldEffectMesh{ nullptr};
	Texture* m_ShiedlTexture{ nullptr };

public:
	ShieldEffect(const std::string& entityID, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual ~ShieldEffect();

	virtual void Update(float) override;
};