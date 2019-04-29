#pragma once
#include "ComponentBase.h"

class LineCube;

class MyBOBox
{ 
protected:
#ifdef _SHOW_BOUNDINGBOX
	LineCube* m_pLineCube{ nullptr };
#endif
	BoundingOrientedBox m_BOBox;
	XMFLOAT4 m_BoBoxPlane[4]; // 평면
	 
public:
	MyBOBox(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, XMFLOAT3 center, XMFLOAT3 extents, XMFLOAT4 quaternion = XMFLOAT4(0.f, 0.f, 0.f, 1.f));
	MyBOBox(const MyBOBox& other);
	virtual ~MyBOBox();

#ifdef _SHOW_BOUNDINGBOX 
	void ReleaseObjects();
	void ReleaseUploadBuffers();
	void Render(ID3D12GraphicsCommandList * pd3dCommandList, const XMFLOAT4X4& xmf4x4World);
	void RenderInstancing(ID3D12GraphicsCommandList * pd3dCommandList, int InstancingCount);
#endif // _SHOW_BOUNDINGBOX

	// 공전이 아닌 자전을 수행한다.
	void Rotate(float roll, float yaw, float pitch); 
	void Move(const XMFLOAT3& xmf3Shift);
	void Move(float x, float y, float z);
	void SetPosition(const XMFLOAT3& pos);
	
	BoundingOrientedBox GetBOBox() const { return m_BOBox; } 
	const XMFLOAT4 GetPlane(int index) const { 
		assert(!(index < 0 && index >= 4));
		return m_BoBoxPlane[index]; 
	}

	// LineCube는 그리지 않는다.
	MyBOBox operator=(const MyBOBox& other)
	{  
		m_pLineCube = nullptr;
		m_BOBox = other.m_BOBox;
		for (int i = 0; i < 4; ++i)
		{
			m_BoBoxPlane[i] = other.m_BoBoxPlane[i];
		}
		return *this;
	}
};