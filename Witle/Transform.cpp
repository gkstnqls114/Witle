#include "stdafx.h" 
#include "Transform.h"


void Transform::SetTransform(const XMFLOAT3 & right, const XMFLOAT3 & up, const XMFLOAT3 & look, const XMFLOAT3 & pos)
{
	m_transformWorld._11 = right.x;  m_transformWorld._12 = right.y;  m_transformWorld._13 = right.z; m_transformWorld._14 = 0;
	m_transformWorld._21 = up.x;     m_transformWorld._22 = up.y;     m_transformWorld._23 = up.z;    m_transformWorld._24 = 0;
	m_transformWorld._31 = look.x;   m_transformWorld._32 = look.y;   m_transformWorld._33 = look.z;  m_transformWorld._34 = 0;
	m_transformWorld._41 = pos.x;    m_transformWorld._42 = pos.y;    m_transformWorld._43 = pos.z;   m_transformWorld._44 = 1;
}

Transform::Transform(GameObject * pOwner)
	:ComponentBase(pOwner)
{
	m_FamilyID.InitTransform();
}

Transform::~Transform()
{
}

void Transform::Update(float fTimeElapsed)
{
	SetTransform(m_Right, m_Up, m_Look, m_Position);
}

void Transform::Rotate(float x, float y, float z)
{
	if (x != 0.0f)
	{
		XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_Right), XMConvertToRadians(x));
		m_Look = Vector3::TransformNormal(m_Look, xmmtxRotate);
		m_Up = Vector3::TransformNormal(m_Up, xmmtxRotate);
	}
	if (y != 0.0f)
	{
		XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_Up), XMConvertToRadians(y));
		m_Look = Vector3::TransformNormal(m_Look, xmmtxRotate);
		m_Right = Vector3::TransformNormal(m_Right, xmmtxRotate);
	}
	if (z != 0.0f)
	{
		XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_Look), XMConvertToRadians(z));
		m_Up = Vector3::TransformNormal(m_Up, xmmtxRotate);
		m_Right = Vector3::TransformNormal(m_Right, xmmtxRotate);
	}

	/*회전으로 인해 플레이어의 로컬 x-축, y-축, z-축이 서로 직교하지 않을 수 있으므로 z-축(LookAt 벡터)을 기준으
	로 하여 서로 직교하고 단위벡터가 되도록 한다.*/
	m_Look = Vector3::Normalize(m_Look);
	m_Right = Vector3::CrossProduct(m_Up, m_Look, true);
	m_Up = Vector3::CrossProduct(m_Look, m_Right, true);

	SetTransform(m_Right, m_Up, m_Look, m_Position);
}

void Transform::SetRotate(float x, float y, float z)
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
	m_Look = Vector3::Normalize(look);
	m_Right = Vector3::CrossProduct(up, look, true);
	m_Up = Vector3::CrossProduct(look, right, true);

	SetTransform(m_Right, m_Up, m_Look, m_Position);
}

void Transform::Move(const XMFLOAT3 & vMove)
{
	m_Position = Vector3::Add(m_Position, vMove); 
}
