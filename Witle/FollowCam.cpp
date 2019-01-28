#include "stdafx.h"
#include "Transform.h"
#include "GameObject.h"
#include "FollowCam.h"



FollowCam::FollowCam(GameObject * pOwner, GameObject * target)
	: Camera(pOwner)
{
	m_pTarget = target;
}

FollowCam::FollowCam(GameObject* pOwner, Camera * camera, GameObject * target)
	: Camera(pOwner)
{
	m_pTarget = target;
}

FollowCam::FollowCam(GameObject* pOwner, FollowCam * followcam)
	:Camera(pOwner)
{
	if (followcam)
	{
		*this = *followcam;
	}
}

void FollowCam::MoveSmoothly(float fTimeElapsed, const XMFLOAT3 & xmf3LookAt)
{ 
	XMFLOAT4X4 xmf4x4Rotate = Matrix4x4::Identity();

	XMFLOAT3 xmf3Right = m_pTarget->GetComponent<Transform>("")->GetRight();
	XMFLOAT3 xmf3Up = m_pTarget->GetComponent<Transform>("")->GetUp();
	XMFLOAT3 xmf3Look = m_pTarget->GetComponent<Transform>("")->GetLook();

	xmf4x4Rotate._11 = xmf3Right.x; xmf4x4Rotate._21 = xmf3Up.x; xmf4x4Rotate._31 = xmf3Look.x;
	xmf4x4Rotate._12 = xmf3Right.y; xmf4x4Rotate._22 = xmf3Up.y; xmf4x4Rotate._32 = xmf3Look.y;
	xmf4x4Rotate._13 = xmf3Right.z; xmf4x4Rotate._23 = xmf3Up.z; xmf4x4Rotate._33 = xmf3Look.z;

	XMFLOAT3 xmf3Offset = Vector3::TransformCoord(m_Offset, xmf4x4Rotate);
	XMFLOAT3 xmf3Position = Vector3::Add(xmf3LookAt, xmf3Offset);
	XMFLOAT3 xmf3Direction = Vector3::Subtract(xmf3Position, m_pOwner->GetTransform().GetPosition());
	float fLength = Vector3::Length(xmf3Direction);
	xmf3Direction = Vector3::Normalize(xmf3Direction);
	float fTimeLagScale = (m_fTimeLag) ? fTimeElapsed * (1.0f / m_fTimeLag) : 1.0f;
	float fDistance = fLength * fTimeLagScale;
	if (fDistance > fLength) fDistance = fLength;
	if (fLength < 0.01f) fDistance = fLength;
	if (fDistance > 0)
	{
		m_pOwner->GetTransform().SetPosition(Vector3::Add(m_pOwner->GetTransform().GetPosition(), xmf3Direction, fDistance));
		SetLookAt(xmf3LookAt);
	}
}

void FollowCam::SetLookAt(const XMFLOAT3 & xmf3LookAt)
{
	XMFLOAT4X4 mtxLookAt = Matrix4x4::LookAtLH(m_pOwner->GetTransform().GetPosition(), xmf3LookAt, m_pTarget->GetComponent<Transform>("")->GetUp());
	m_pOwner->GetTransform().SetRight(XMFLOAT3(mtxLookAt._11, mtxLookAt._21, mtxLookAt._31));
	m_pOwner->GetTransform().SetUp(XMFLOAT3(mtxLookAt._12, mtxLookAt._22, mtxLookAt._32));
	m_pOwner->GetTransform().SetLook(XMFLOAT3(mtxLookAt._13, mtxLookAt._23, mtxLookAt._33));
}

FollowCam::~FollowCam()
{
}

void FollowCam::Teleport(const XMFLOAT3 & at)
{
	m_At = at;
	m_pOwner->GetTransform().SetPosition(Vector3::Subtract(m_At, m_Offset));
	m_xmf4x4View = Matrix4x4::LookAtLH(m_pOwner->GetTransform().GetPosition(), m_At, m_pOwner->GetTransform().GetUp());
}

void FollowCam::Update(float fTimeElapsed, const XMFLOAT3 & xmf3LookAt)
{
	MoveSmoothly(fTimeElapsed, xmf3LookAt);
}

void FollowCam::LastUpdate(float fTimeElapsed)
{
	// ���� ���
	// ������ �浹 ��...
	
	//Move, Rotate�� At�� ī�޶� ��ǥ��(Right, Up, Look)�� �������� Position �缳��
	m_At = m_pTarget->GetTransform().GetPosition();
	m_pOwner->GetTransform().SetPosition(Vector3::Subtract(m_At, m_Offset));

	RegenerateViewMatrix();

	GenerateFrustum();
}

void FollowCam::Rotate(float x, float y, float z)
{ 
	XMFLOAT3 right = m_pTarget->GetTransform().GetRight();
	XMFLOAT3 up = m_pTarget->GetTransform().GetUp();
	XMFLOAT3 look = m_pTarget->GetTransform().GetLook();

	//XMFLOAT3 right{ 1.f, 0.f, 0.f };
	//XMFLOAT3 up{0.f, 1.f, 0.f};
	//XMFLOAT3 look{0.f, 0.f, 1.f};

	if (x != 0.0f)
	{
		XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&right), XMConvertToRadians(x));
		m_pOwner->GetTransform().SetLook(Vector3::TransformNormal(m_pTarget->GetTransform().GetLook(), xmmtxRotate));
		m_pOwner->GetTransform().SetUp(Vector3::TransformNormal(m_pTarget->GetTransform().GetUp(), xmmtxRotate));

		m_Offset = Vector3::TransformCoord(m_Offset, xmmtxRotate);
	}
	if (y != 0.0f)
	{
		XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&up), XMConvertToRadians(y));
		m_pOwner->GetTransform().SetLook(Vector3::TransformNormal(m_pTarget->GetTransform().GetLook(), xmmtxRotate));
		m_pOwner->GetTransform().SetRight(Vector3::TransformNormal(m_pTarget->GetTransform().GetRight(), xmmtxRotate));

		m_Offset = Vector3::TransformCoord(m_Offset, xmmtxRotate);
	}
	if (z != 0.0f)
	{
		XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&look), XMConvertToRadians(z));
		m_pOwner->GetTransform().SetUp(Vector3::TransformNormal(m_pTarget->GetTransform().GetUp(), xmmtxRotate));
		m_pOwner->GetTransform().SetRight(Vector3::TransformNormal(m_pTarget->GetTransform().GetRight(), xmmtxRotate));

		m_Offset = Vector3::TransformCoord(m_Offset, xmmtxRotate); 
	}

	/*ȸ������ ���� �÷��̾��� ���� x-��, y-��, z-���� ���� �������� ���� �� �����Ƿ� z-��(LookAt ����)�� ������
	�� �Ͽ� ���� �����ϰ� �������Ͱ� �ǵ��� �Ѵ�.*/
	m_pOwner->GetTransform().SetLook(Vector3::Normalize(m_pTarget->GetTransform().GetLook()));
	m_pOwner->GetTransform().SetRight(Vector3::CrossProduct(m_pTarget->GetTransform().GetUp(), m_pTarget->GetTransform().GetLook(), true));
	m_pOwner->GetTransform().SetUp(Vector3::CrossProduct(m_pTarget->GetTransform().GetLook(), m_pTarget->GetTransform().GetRight(), true));
}

void FollowCam::ZoomIn(float val)
{
	m_Offset = Vector3::ScalarProduct(m_Offset, Vector3::Length(m_Offset) * val);
}

void FollowCam::ZoomOut(float val)
{
	m_Offset = Vector3::ScalarProduct(m_Offset, Vector3::Length(m_Offset) * val);

}
