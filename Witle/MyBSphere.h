#pragma once
#include "ComponentBase.h"

class LineCube;

class MyBSphere
{
protected:
#ifdef _SHOW_BOUNDINGBOX
	LineCube* m_pLineCube{ nullptr };
#endif 
	BoundingSphere m_BSphere; 

public:
	MyBSphere(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, XMFLOAT3 center, float radius);
	virtual ~MyBSphere();

#ifdef _SHOW_BOUNDINGBOX 
	void Render(ID3D12GraphicsCommandList * pd3dCommandList, const XMFLOAT4X4& xmf4x4World);
#endif // _SHOW_BOUNDINGBOX

	// ������ �ƴ� ������ �����Ѵ�. 
	void Move(const XMFLOAT3& xmf3Shift);
	const BoundingSphere& GetBOBox() const { return m_BSphere; } 
};