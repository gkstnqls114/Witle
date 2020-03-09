#include "stdafx.h"
#include "Shader.h"
#include "ShaderManager.h"
#include "LineCube.h"
#include "DebugOption.h"
#include "MyBBox.h"

MyBBox::MyBBox(GameObject* pOwner, ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, XMFLOAT3 center, XMFLOAT3 extents, XMFLOAT4 quaternion)
	: MyCollider(pOwner, COLLIDER_TYPE::BOUNDING_BOX)
{
	m_BOBox = BoundingBox(center, extents);

	m_world = Matrix4x4::Identity();
	m_Pivot = center;
	m_world._41 = center.x;
	m_world._42 = center.y;
	m_world._43 = center.z;

#ifdef SHOW_DEBUGMESH
	m_pLineCube = new LineCube(pOwner, pd3dDevice, pd3dCommandList, m_BOBox.Center, m_BOBox.Extents);
#endif // SHOW_DEBUGMESH

	m_BoBoxPlane[0] = Plane::Plane(Vector3::Add(center, XMFLOAT3(extents.x, 0.f, 0.f)), XMFLOAT3(1.f, 0.f, 0.f)); // +x면 normal (1, 0, 0) 
	m_BoBoxPlane[1] = Plane::Plane(Vector3::Add(center, XMFLOAT3(-extents.x, 0.f, 0.f)), XMFLOAT3(-1.f, 0.f, 0.f)); // -x면 normal (-1, 0, 0)
	m_BoBoxPlane[2] = Plane::Plane(Vector3::Add(center, XMFLOAT3(0.f, 0.f, extents.z)), XMFLOAT3(0.f, 0.f, 1.f)); // +z면 normal (0, 0, 1)
	m_BoBoxPlane[3] = Plane::Plane(Vector3::Add(center, XMFLOAT3(0.f, 0.f, -extents.z)), XMFLOAT3(0.f, 0.f, -1.f)); // -z면 normal (0, 0, -1)
}

MyBBox::MyBBox(const MyBBox& other)
	:MyCollider(m_pOwner, COLLIDER_TYPE::BOUNDING_BOX)
{
#ifdef SHOW_DEBUGMESH 
	m_pLineCube = nullptr;
#endif // SHOW_DEBUGMESH

	m_BOBox = other.m_BOBox;
	for (int i = 0; i < 4; ++i)
	{
		m_BoBoxPlane[i] = other.m_BoBoxPlane[i];
	}
}

MyBBox::MyBBox(XMFLOAT3 center, XMFLOAT3 extents)
	:MyCollider(m_pOwner, COLLIDER_TYPE::BOUNDING_BOX)
{
	m_BOBox = BoundingBox(center, extents);

	m_BoBoxPlane[0] = Plane::Plane(Vector3::Add(center, XMFLOAT3(extents.x, 0.f, 0.f)), XMFLOAT3(1.f, 0.f, 0.f)); // +x면 normal (1, 0, 0) 
	m_BoBoxPlane[1] = Plane::Plane(Vector3::Add(center, XMFLOAT3(-extents.x, 0.f, 0.f)), XMFLOAT3(-1.f, 0.f, 0.f)); // -x면 normal (-1, 0, 0)
	m_BoBoxPlane[2] = Plane::Plane(Vector3::Add(center, XMFLOAT3(0.f, 0.f, extents.z)), XMFLOAT3(0.f, 0.f, 1.f)); // +z면 normal (0, 0, 1)
	m_BoBoxPlane[3] = Plane::Plane(Vector3::Add(center, XMFLOAT3(0.f, 0.f, -extents.z)), XMFLOAT3(0.f, 0.f, -1.f)); // -z면 normal (0, 0, -1)
}

MyBBox::~MyBBox()
{
#ifdef SHOW_DEBUGMESH  
	if (m_pLineCube)
	{
		delete m_pLineCube;
		m_pLineCube = nullptr;
	} 
#endif // SHOW_DEBUGMESH
}

void MyBBox::ReleaseObjects()
{
#ifdef SHOW_DEBUGMESH  
	if (m_pLineCube) m_pLineCube->ReleaseObjects(); 
#endif // SHOW_DEBUGMESH
}
void MyBBox::ReleaseUploadBuffers()
{
#ifdef SHOW_DEBUGMESH  
	if (m_pLineCube) m_pLineCube->ReleaseUploadBuffers(); 
#endif // SHOW_DEBUGMESH
}

XMFLOAT4X4 MyBBox::SetRotate(float x, float y, float z)
{
	XMFLOAT3 right{ 1, 0, 0 };
	XMFLOAT3 up{ 0, 1, 0 };
	XMFLOAT3 look{ 0, 0, 1 };

	if (x != 0.0f)
	{
		XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&right), XMConvertToRadians(x));
		look = Vector3::TransformNormal(look, xmmtxRotate);
		up = Vector3::TransformNormal(up, xmmtxRotate);
	}
	if (y != 0.0f)
	{
		XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&up), XMConvertToRadians(y));
		look = Vector3::TransformNormal(look, xmmtxRotate);
		right = Vector3::TransformNormal(right, xmmtxRotate);
	}
	if (z != 0.0f)
	{
		XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&look), XMConvertToRadians(z));
		up = Vector3::TransformNormal(up, xmmtxRotate);
		right = Vector3::TransformNormal(right, xmmtxRotate);
	}

	/*회전으로 인해 플레이어의 로컬 x-축, y-축, z-축이 서로 직교하지 않을 수 있으므로 z-축(LookAt 벡터)을 기준으
	로 하여 서로 직교하고 단위벡터가 되도록 한다.*/
	XMFLOAT3 new_look = Vector3::Normalize(look);
	XMFLOAT3 new_right = Vector3::CrossProduct(up, look, true);
	XMFLOAT3 new_up = Vector3::CrossProduct(look, right, true);

	return Matrix4x4::WorldMatrix(m_BOBox.Center, new_right, new_up, new_look);
}

void MyBBox::Render(ID3D12GraphicsCommandList* pd3dCommandList)
{
	if (!DebugOption::GetisShowBoundingBox()) return;

#ifdef SHOW_DEBUGMESH  
	if (m_pLineCube)
	{
		pd3dCommandList->SetPipelineState(ShaderManager::GetInstance()->GetShader("Line")->GetPSO());
		m_pLineCube->Render(pd3dCommandList, m_world, true);
	} 
#endif // SHOW_DEBUGMESH
}
void MyBBox::RenderInstancing(ID3D12GraphicsCommandList* pd3dCommandList, int InstancingCount)
{
	if (!DebugOption::GetisShowBoundingBox()) return;

#ifdef SHOW_DEBUGMESH  
	if (m_pLineCube) m_pLineCube->RenderInstancing(pd3dCommandList, InstancingCount); 
#endif // SHOW_DEBUGMESH
}
 
void MyBBox::Move(const XMFLOAT3& xmf3Shift)
{
	m_BOBox.Center = Vector3::Add(m_BOBox.Center, xmf3Shift);

	m_world._41 = m_BOBox.Center.x;
	m_world._42 = m_BOBox.Center.y;
	m_world._43 = m_BOBox.Center.z;
}

void MyBBox::SetPosition(const XMFLOAT3& pos)
{
	m_BOBox.Center = pos;

	m_world._41 = m_BOBox.Center.x + m_Pivot.x;
	m_world._42 = m_BOBox.Center.y + m_Pivot.y;
	m_world._43 = m_BOBox.Center.z + m_Pivot.z;
}

bool MyBBox::IsIn(int index, const XMFLOAT3& intersectionPoint)
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

void MyBBox::Move(float x, float y, float z)
{
	Move(XMFLOAT3(x, y, z));
}
