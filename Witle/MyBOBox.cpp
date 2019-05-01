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
	m_BoBoxPlane[3] = Plane::Plane(Vector3::Add(center, XMFLOAT3(0.f, 0.f, -extents.z)), XMFLOAT3(0.f, 0.f, -1.f)); // -z면 normal (0, 0, -1)
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
void MyBOBox::ReleaseObjects()
{
	m_pLineCube->ReleaseObjects();
}
void MyBOBox::ReleaseUploadBuffers()
{
	m_pLineCube->ReleaseUploadBuffers();
}

void MyBOBox::Render(ID3D12GraphicsCommandList * pd3dCommandList, const XMFLOAT4X4& xmf4x4World)
{
	if (m_pLineCube)
	{
		pd3dCommandList->SetPipelineState(ShaderManager::GetInstance()->GetShader("Line")->GetPSO());
		m_pLineCube->Render(pd3dCommandList, xmf4x4World, true);
	}
}
void MyBOBox::RenderInstancing(ID3D12GraphicsCommandList * pd3dCommandList, int InstancingCount)
{
	if (m_pLineCube) m_pLineCube->RenderInstancing(pd3dCommandList, InstancingCount);
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

void MyBOBox::SetPosition(const XMFLOAT3 & pos)
{
	m_BOBox.Center = pos;
}

bool MyBOBox::IsIn(int index, const XMFLOAT3 & intersectionPoint)
{ 
	assert(!(index < 0 && index >= 4));
	XMFLOAT3 cmp_pos{ intersectionPoint.x, 0.0f, intersectionPoint.z }; // 비교할위치
	switch (index)
	{
	case 0: // +X 
	case 1: // -X
		if (m_BOBox.Center.z - m_BOBox.Extents.z <= cmp_pos.z && cmp_pos.z <= m_BOBox.Center.z + m_BOBox.Extents.z)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;

	case 2: // +Z 
	case 3: // -Z
		if (m_BOBox.Center.x - m_BOBox.Extents.x <= cmp_pos.x && cmp_pos.x <= m_BOBox.Center.x + m_BOBox.Extents.x)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	} 
}

void MyBOBox::Move(float x, float y, float z)
{
	Move(XMFLOAT3(x, y, z));
}
