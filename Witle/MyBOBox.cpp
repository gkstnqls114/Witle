#include "stdafx.h"
#include "Shader.h"
#include "ShaderManager.h"
#include "LineCube.h"
#include "MyBOBox.h"
 
MyBOBox::MyBOBox(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, XMFLOAT3 center, XMFLOAT3 extents, XMFLOAT4 quaternion)
{
	m_BOBox = BoundingOrientedBox(center, extents, quaternion);
#ifdef _SHOW_BOUNDINGBOX
	m_pLineCube = new LineCube(pd3dDevice, pd3dCommandList, m_BOBox.Center, m_BOBox.Extents);
#endif // DEBUG 

	m_BoBoxPlane[0] = Plane::Plane(Vector3::Add(center, XMFLOAT3(extents.x, 0.f, 0.f)), XMFLOAT3(1.f, 0.f, 0.f)); // +x면 normal (1, 0, 0)
	m_BoBoxPlane[1] = Plane::Plane(Vector3::Add(center, XMFLOAT3(-extents.x, 0.f, 0.f)), XMFLOAT3(-1.f, 0.f, 0.f)); // -x면 normal (-1, 0, 0)
	m_BoBoxPlane[2] = Plane::Plane(Vector3::Add(center, XMFLOAT3(0.f, 0.f, extents.z)), XMFLOAT3(0.f, 0.f, 1.f)); // +z면 normal (0, 0, 1)
	m_BoBoxPlane[3] = Plane::Plane(Vector3::Add(center, XMFLOAT3(0.f, 0.f, -extents.z)), XMFLOAT3(0.f, 0.f, -1.f)); // +z면 normal (0, 0, -1)
}

MyBOBox::MyBOBox(const MyBOBox & other)
{
	m_pLineCube = nullptr;
	m_BOBox = other.m_BOBox;
	for (int i = 0; i < 4; ++i)
	{
		m_BoBoxPlane[i] = other.m_BoBoxPlane[i];
	}
}

MyBOBox::~MyBOBox()
{ 
#ifdef _SHOW_BOUNDINGBOX
	if (m_pLineCube)
	{
		delete m_pLineCube;
	}
#endif
}


#ifdef _SHOW_BOUNDINGBOX
void MyBOBox::Render(ID3D12GraphicsCommandList * pd3dCommandList, const XMFLOAT4X4& xmf4x4World)
{
	if (m_pLineCube)
	{
		pd3dCommandList->SetPipelineState(ShaderManager::GetInstance()->GetShader("Line")->GetPSO());
		m_pLineCube->Render(pd3dCommandList, xmf4x4World, true);
	}
}
#endif

void MyBOBox::Rotate(float roll, float yaw, float pitch)
{
	m_BOBox.Orientation = Quaternion::ToQuaternion(roll, yaw, pitch);
}

void MyBOBox::Move(const XMFLOAT3 & xmf3Shift)
{
	m_BOBox.Center = Vector3::Add(m_BOBox.Center, xmf3Shift);
}

MyBOBox MyBOBox::GetBOBox(const XMFLOAT3& position)
{
	MyBOBox newBoBox;
	newBoBox.m_BOBox.Center = Vector3::Add(m_BOBox.Center, position);
	newBoBox.m_BOBox.Orientation = m_BOBox.Orientation;
	return newBoBox;
}
