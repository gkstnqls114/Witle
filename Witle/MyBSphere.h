#pragma once
#include "MyCollider.h"

class LineCube;

class MyBSphere
	: public MyCollider
{
protected:
#ifdef _SHOW_BOUNDINGBOX
	LineCube* m_pLineCube{ nullptr };
#endif 
	BoundingSphere m_BSphere; 

public:
	MyBSphere(GameObject* pOwner, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, XMFLOAT3 center, float radius);
	virtual ~MyBSphere();

	void ReleaseObjects();
	void ReleaseUploadBuffers();
	 
#ifdef _SHOW_BOUNDINGBOX 
	void Render(ID3D12GraphicsCommandList * pd3dCommandList, const XMFLOAT4X4& xmf4x4World);
#endif // _SHOW_BOUNDINGBOX

	// 공전이 아닌 자전을 수행한다. 
	void Move(const XMFLOAT3& xmf3Shift);
	const BoundingSphere& GetBOBox() const { return m_BSphere; } 
};