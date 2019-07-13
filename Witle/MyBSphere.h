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
	
	// ����� �޽��� �������մϴ�.
	void Render(ID3D12GraphicsCommandList * pd3dCommandList, const XMFLOAT4X4& xmf4x4World);

	// center�� shift��ŭ �̵���ŵ�ϴ�.
	void Move(const XMFLOAT3& xmf3Shift);

	BoundingSphere* GetBSphere() { return &m_BSphere; }
};