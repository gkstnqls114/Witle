#pragma once
#include "MyCollider.h"

class LineCube;

class MyBSphere
	: public MyCollider
{
protected:

	LineCube* m_pLineCube{ nullptr };

	BoundingSphere m_BSphere; 

public:
	MyBSphere(GameObject* pOwner, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, float radius, XMFLOAT3 center = XMFLOAT3(0, 0, 0));
	virtual ~MyBSphere();

	void ReleaseObjects();
	void ReleaseUploadBuffers();
	
	// 디버그 메쉬를 렌더링합니다.
	void Render(ID3D12GraphicsCommandList * pd3dCommandList, const XMFLOAT4X4& xmf4x4World);

	// center를 shift만큼 이동시킵니다.
	void Move(const XMFLOAT3& xmf3Shift);

	BoundingSphere* GetBSphere() { return &m_BSphere; }
};