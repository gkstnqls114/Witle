#pragma once
#include "Skill.h"

class CylinderMesh;
class GameObject;
class MyBSphere;

class WideareaMagic :
	public Skill
{
	const float rotateValue = 30.f; 
	MyBSphere* m_MyBSphere{ nullptr };
	CylinderMesh* m_CylinderMesh{ nullptr };

protected:
	virtual void ReleaseMembers() override;
	virtual void ReleaseMemberUploadBuffers() override;
	virtual void PrepareMember() override {};

public:
	WideareaMagic(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual ~WideareaMagic();

	void UpdateShaderVariable(ID3D12GraphicsCommandList *pd3dCommandList, XMFLOAT4X4 *pxmf4x4World)
	{
		XMFLOAT4X4 xmf4x4World;
		XMStoreFloat4x4(&xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(pxmf4x4World)));
		pd3dCommandList->SetGraphicsRoot32BitConstants(0, 16, &xmf4x4World, 0);
	}
	 

public:
	virtual void DoNotUse() override;
	virtual void DoUse() override;
	void SetPosition(XMFLOAT3 pos);

	virtual void Update(float fTimeElapsed) override;

	void Render(ID3D12GraphicsCommandList* commandList);
	BoundingSphere* GetBSphere();
};

