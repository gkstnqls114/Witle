#include "stdafx.h" 
#include "Transform.h"


void Transform::SetTransform(const XMFLOAT3 & right, const XMFLOAT3 & up, const XMFLOAT3 & look, const XMFLOAT3 & pos)
{
	m_transformWorld._11 = right.x;  m_transformWorld._21 = up.x;  m_transformWorld._31 = look.x; m_transformWorld._41 = 0;
	m_transformWorld._12 = right.y;  m_transformWorld._22 = up.y;  m_transformWorld._32 = look.y; m_transformWorld._42 = 0;
	m_transformWorld._13 = right.z;  m_transformWorld._23 = up.z;  m_transformWorld._33 = look.z; m_transformWorld._43 = 0;
	m_transformWorld._14 = pos.x;    m_transformWorld._24 = pos.x; m_transformWorld._34 = pos.x;  m_transformWorld._44 = 1;
}

void Transform::Move(const XMFLOAT3 & xmf3Shift, bool bUpdateVelocity)
{
	//bUpdateVelocity�� ���̸� �÷��̾ �̵����� �ʰ� �ӵ� ���͸� �����Ѵ�.
	if (bUpdateVelocity)
	{
		//�÷��̾��� �ӵ� ���͸� xmf3Shift ���͸�ŭ �����Ѵ�.
		// m_xmf3Velocity = Vector3::Add(m_xmf3Velocity, xmf3Shift);
	}
	else
	{
		//�÷��̾ ���� ��ġ ���Ϳ��� xmf3Shift ���͸�ŭ �̵��Ѵ�.
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

	/*ȸ������ ���� �÷��̾��� ���� x-��, y-��, z-���� ���� �������� ���� �� �����Ƿ� z-��(LookAt ����)�� ������
	�� �Ͽ� ���� �����ϰ� �������Ͱ� �ǵ��� �Ѵ�.*/
	m_Look = Vector3::Normalize(m_Look);
	m_Right = Vector3::CrossProduct(m_Up, m_Look, true);
	m_Up = Vector3::CrossProduct(m_Look, m_Right, true);

	SetTransform(m_Right, m_Up, m_Look, m_Position);
}

void Transform::Move(DWORD dwDirection, float fDistance, bool bUpdateVelocity)
{
	if (dwDirection)
	{
		XMFLOAT3 xmf3Shift = XMFLOAT3(0, 0, 0);
		//ȭ��ǥ Ű ���衯�� ������ ���� z-�� �������� �̵�(����)�Ѵ�. ���顯�� ������ �ݴ� �������� �̵��Ѵ�.
		if (dwDirection & DIR_FORWARD) xmf3Shift = Vector3::Add(xmf3Shift, m_Look,
			fDistance);
		if (dwDirection & DIR_BACKWARD) xmf3Shift = Vector3::Add(xmf3Shift, m_Look,
			-fDistance);
		//ȭ��ǥ Ű ���桯�� ������ ���� x-�� �������� �̵��Ѵ�. ���硯�� ������ �ݴ� �������� �̵��Ѵ�.
		if (dwDirection & DIR_RIGHT) xmf3Shift = Vector3::Add(xmf3Shift, m_Right,
			fDistance);
		if (dwDirection & DIR_LEFT) xmf3Shift = Vector3::Add(xmf3Shift, m_Right,
			-fDistance);
		//��Page Up���� ������ ���� y-�� �������� �̵��Ѵ�. ��Page Down���� ������ �ݴ� �������� �̵��Ѵ�.
		//if (dwDirection & DIR_UP) xmf3Shift = Vector3::Add(xmf3Shift, m_Up, fDistance);
		//if (dwDirection & DIR_DOWN) xmf3Shift = Vector3::Add(xmf3Shift, m_Up,
		//	-fDistance);

		//�÷��̾ ���� ��ġ ���Ϳ��� xmf3Shift ���͸�ŭ �̵��Ѵ�.
		Move(xmf3Shift, bUpdateVelocity);
	}
}

void Transform::Move(const XMFLOAT3 & vMove)
{
	m_Position = Vector3::Add(m_Position, vMove);
}
