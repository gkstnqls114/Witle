#include "stdafx.h"
#include "Shader.h"
#include "ShaderManager.h"
#include "LineCube.h"
#include "MyBOBox.h"
bool MyBOBox::RENDER_BBOX = true;

MyBOBox::MyBOBox(GameObject* pOwner, ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, XMFLOAT3 center, XMFLOAT3 extents, XMFLOAT4 quaternion)
	: MyCollider(pOwner, COLLIDER_TYPE::BOUNDING_BOX)
{
	m_BOBox = BoundingOrientedBox(XMFLOAT3(0.F, 0.F, 0.F), extents, quaternion);

	m_world = Matrix4x4::Identity();
	m_Pivot = center;
	m_world._41 = m_Pivot.x;
	m_world._42 = m_Pivot.y;
	m_world._43 = m_Pivot.z;
	m_pLineCube = new LineCube(pOwner, pd3dDevice, pd3dCommandList, m_BOBox.Center, m_BOBox.Extents);
	 
	m_BoBoxPlane[0] = Plane::Plane(Vector3::Add(center, XMFLOAT3(extents.x, 0.f, 0.f)), XMFLOAT3(1.f, 0.f, 0.f)); // +x�� normal (1, 0, 0) 
	m_BoBoxPlane[1] = Plane::Plane(Vector3::Add(center, XMFLOAT3(-extents.x, 0.f, 0.f)), XMFLOAT3(-1.f, 0.f, 0.f)); // -x�� normal (-1, 0, 0)
	m_BoBoxPlane[2] = Plane::Plane(Vector3::Add(center, XMFLOAT3(0.f, 0.f, extents.z)), XMFLOAT3(0.f, 0.f, 1.f)); // +z�� normal (0, 0, 1)
	m_BoBoxPlane[3] = Plane::Plane(Vector3::Add(center, XMFLOAT3(0.f, 0.f, -extents.z)), XMFLOAT3(0.f, 0.f, -1.f)); // -z�� normal (0, 0, -1)
}

MyBOBox::MyBOBox(const MyBOBox & other) 
	:MyCollider(m_pOwner, COLLIDER_TYPE::BOUNDING_BOX)
{
	m_pLineCube = nullptr;
	m_BOBox = other.m_BOBox;
	for (int i = 0; i < 4; ++i)
	{
		m_BoBoxPlane[i] = other.m_BoBoxPlane[i]; 
	}
}

MyBOBox::MyBOBox(XMFLOAT3 center, XMFLOAT3 extents)
	:MyCollider(m_pOwner, COLLIDER_TYPE::BOUNDING_BOX)
{
	m_BOBox = BoundingOrientedBox(center, extents, XMFLOAT4(0, 0, 0, 1));

	m_BoBoxPlane[0] = Plane::Plane(Vector3::Add(center, XMFLOAT3(extents.x, 0.f, 0.f)), XMFLOAT3(1.f, 0.f, 0.f)); // +x�� normal (1, 0, 0) 
	m_BoBoxPlane[1] = Plane::Plane(Vector3::Add(center, XMFLOAT3(-extents.x, 0.f, 0.f)), XMFLOAT3(-1.f, 0.f, 0.f)); // -x�� normal (-1, 0, 0)
	m_BoBoxPlane[2] = Plane::Plane(Vector3::Add(center, XMFLOAT3(0.f, 0.f, extents.z)), XMFLOAT3(0.f, 0.f, 1.f)); // +z�� normal (0, 0, 1)
	m_BoBoxPlane[3] = Plane::Plane(Vector3::Add(center, XMFLOAT3(0.f, 0.f, -extents.z)), XMFLOAT3(0.f, 0.f, -1.f)); // -z�� normal (0, 0, -1)
}

MyBOBox::~MyBOBox()
{  
	if (m_pLineCube)
	{
		delete m_pLineCube;
	} 
}
  
void MyBOBox::ReleaseObjects()
{
	if(m_pLineCube) m_pLineCube->ReleaseObjects();
}
void MyBOBox::ReleaseUploadBuffers()
{
	if(m_pLineCube) m_pLineCube->ReleaseUploadBuffers();
}

XMFLOAT4X4 MyBOBox::SetRotate(float x, float y, float z)
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

	/*ȸ������ ���� �÷��̾��� ���� x-��, y-��, z-���� ���� �������� ���� �� �����Ƿ� z-��(LookAt ����)�� ������
	�� �Ͽ� ���� �����ϰ� �������Ͱ� �ǵ��� �Ѵ�.*/
	XMFLOAT3 new_look = Vector3::Normalize(look);
	XMFLOAT3 new_right = Vector3::CrossProduct(up, look, true);
	XMFLOAT3 new_up = Vector3::CrossProduct(look, right, true);

	return Matrix4x4::WorldMatrix(m_BOBox.Center, new_right, new_up, new_look);
}

void MyBOBox::Render(ID3D12GraphicsCommandList * pd3dCommandList)
{ 
	// if (!RENDER_BBOX) return;
	if (m_pLineCube)
	{
		pd3dCommandList->SetPipelineState(ShaderManager::GetInstance()->GetShader("Line")->GetPSO());
		m_pLineCube->Render(pd3dCommandList, m_world, true);
	}
}
void MyBOBox::RenderInstancing(ID3D12GraphicsCommandList * pd3dCommandList, int InstancingCount)
{
	if (!RENDER_BBOX) return;
	if (m_pLineCube) m_pLineCube->RenderInstancing(pd3dCommandList, InstancingCount);
} 

// ���� Rotate����, ������ roll, yaw, pitch�� ���缭 ���� �ٽ� �����ϰ� �ִ�.
// ������ �Ǵ� position �� ���� ���� ����
void MyBOBox::Rotate(float roll, float pitch, float yaw)
{
	m_BOBox.Orientation = Quaternion::ToQuaternion(roll, pitch, yaw);

	m_world = SetRotate(roll, pitch, yaw);
}

void MyBOBox::Move(const XMFLOAT3 & xmf3Shift)
{
	m_BOBox.Center = Vector3::Add(m_BOBox.Center, xmf3Shift);
	 
	m_world._41 = m_BOBox.Center.x;
	m_world._42 = m_BOBox.Center.y;
	m_world._43 = m_BOBox.Center.z; 
}

void MyBOBox::SetPosition(const XMFLOAT3 & pos)
{
	m_BOBox.Center = pos;
	 
	m_world._41 = m_BOBox.Center.x;
	m_world._42 = m_BOBox.Center.y;
	m_world._43 = m_BOBox.Center.z; 
}

bool MyBOBox::IsIn(int index, const XMFLOAT3 & intersectionPoint)
{ 
	assert(!(index < 0 && index >= 4));
	XMFLOAT3 cmp_pos{ intersectionPoint.x, 0.0f, intersectionPoint.z }; // ������ġ
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
