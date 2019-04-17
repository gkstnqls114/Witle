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
	XMFLOAT4 m_BoBoxPlane[4]; // ���

private:
	MyBOBox() {};

public:
	MyBOBox(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, XMFLOAT3 center, XMFLOAT3 extents, XMFLOAT4 quaternion = XMFLOAT4(0.f, 0.f, 0.f, 1.f));
	MyBOBox(const MyBOBox& other);
	virtual ~MyBOBox();

#ifdef _SHOW_BOUNDINGBOX 
	void Render(ID3D12GraphicsCommandList * pd3dCommandList, const XMFLOAT4X4& xmf4x4World);
#endif // _SHOW_BOUNDINGBOX

	// ������ �ƴ� ������ �����Ѵ�.
	void Rotate(float roll, float yaw, float pitch); 
	void Move(const XMFLOAT3& xmf3Shift);
	
	const BoundingOrientedBox& GetBOBox() const { return m_BOBox; }
	// �ش� �������� ���� �̵��� ���ο� bobox�� ����´�.
	MyBOBox GetBOBox(const XMFLOAT3& position); 
	const XMFLOAT4 GetPlane(int index) { return m_BoBoxPlane[index]; }

	// LineCube�� �׸��� �ʴ´�.
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