#include "stdafx.h" 
#include "Transform.h"


void Transform::SetTransform(const XMFLOAT3 & right, const XMFLOAT3 & up, const XMFLOAT3 & look, const XMFLOAT3 & pos)
{
	m_transformWorld._11 = right.x;  m_transformWorld._12 = up.x;  m_transformWorld._13 = look.x; m_transformWorld._14 = 0;
	m_transformWorld._21 = right.y;  m_transformWorld._22 = up.y;  m_transformWorld._23 = look.y; m_transformWorld._24 = 0;
	m_transformWorld._31 = right.z;  m_transformWorld._32 = up.z;  m_transformWorld._33 = look.z; m_transformWorld._34 = 0;
	m_transformWorld._41 = pos.x;    m_transformWorld._42 = pos.y; m_transformWorld._43 = pos.z;  m_transformWorld._44 = 1;
}

void Transform::Move(const XMFLOAT3 & xmf3Shift, bool bUpdateVelocity)
{
	//bUpdateVelocity가 참이면 플레이어를 이동하지 않고 속도 벡터를 변경한다.
	if (bUpdateVelocity)
	{
		//플레이어의 속도 벡터를 xmf3Shift 벡터만큼 변경한다.
		// m_xmf3Velocity = Vector3::Add(m_xmf3Velocity, xmf3Shift);
	}
	else
	{
		//플레이어를 현재 위치 벡터에서 xmf3Shift 벡터만큼 이동한다.
		m_Position = Vector3::Add(m_Position, xmf3Shift); 
	}

}

Transform::Transform()
{
	m_FamillyID = "Transform";
}

Transform::~Transform()
{
}

void Transform::Update()
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

void Transform::Move(const XMFLOAT3 & vMove)
{
	m_Position = Vector3::Add(m_Position, vMove);
	SetTransform(m_Right, m_Up, m_Look, m_Position);
}
