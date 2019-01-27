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

FollowCam::FollowCam(FollowCam * followcam)
	:Camera(followcam->GetOwner())
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
	XMFLOAT3 xmf3Direction = Vector3::Subtract(xmf3Position, GetOwner()->GetTransform()->GetPosition());
	float fLength = Vector3::Length(xmf3Direction);
	xmf3Direction = Vector3::Normalize(xmf3Direction);
	float fTimeLagScale = (m_fTimeLag) ? fTimeElapsed * (1.0f / m_fTimeLag) : 1.0f;
	float fDistance = fLength * fTimeLagScale;
	if (fDistance > fLength) fDistance = fLength;
	if (fLength < 0.01f) fDistance = fLength;
	if (fDistance > 0)
	{
		GetOwner()->GetTransform()->SetPosition(Vector3::Add(GetOwner()->GetTransform()->GetPosition(), xmf3Direction, fDistance));
		SetLookAt(xmf3LookAt);
	}
}

void FollowCam::SetLookAt(const XMFLOAT3 & xmf3LookAt)
{
	XMFLOAT4X4 mtxLookAt = Matrix4x4::LookAtLH(GetOwner()->GetTransform()->GetPosition(), xmf3LookAt, m_pTarget->GetComponent<Transform>("")->GetUp());
	GetOwner()->GetTransform()->SetRight(XMFLOAT3(mtxLookAt._11, mtxLookAt._21, mtxLookAt._31));
	GetOwner()->GetTransform()->SetUp(XMFLOAT3(mtxLookAt._12, mtxLookAt._22, mtxLookAt._32));
	GetOwner()->GetTransform()->SetLook(XMFLOAT3(mtxLookAt._13, mtxLookAt._23, mtxLookAt._33));
}

FollowCam::~FollowCam()
{
}

void FollowCam::Teleport(const XMFLOAT3 & at)
{
	m_At = at;
	GetOwner()->GetTransform()->SetPosition(Vector3::Subtract(m_At, m_Offset));
	m_xmf4x4View = Matrix4x4::LookAtLH(GetOwner()->GetTransform()->GetPosition(), m_At, GetOwner()->GetTransform()->GetUp());
}

void FollowCam::Update(float fTimeElapsed, const XMFLOAT3 & xmf3LookAt)
{
	MoveSmoothly(fTimeElapsed, xmf3LookAt);
}

void FollowCam::LastUpdate(float fTimeElapsed)
{
	// 물리 계산
	// 벽과의 충돌 등...
	
	//Move, Rotate된 At과 카메라 좌표축(Right, Up, Look)을 기준으로 Position 재설정
	m_At = m_pTarget->GetComponent<Transform>("")->GetPosition();
	GetOwner()->GetTransform()->SetPosition(Vector3::Subtract(m_At, m_Offset));

	RegenerateViewMatrix();

	GenerateFrustum();

	std::cout << "At: ";
	Vector3::Show(m_At);
	std::cout << "Pos: ";
	Vector3::Show(GetOwner()->GetTransform()->GetPosition());
	std::cout << "Offset: ";
	Vector3::Show(m_Offset);
	std::cout << std::endl;
	 
	std::cout << "View: ";
	Matrix4x4::Show(m_xmf4x4View);
	std::cout << std::endl;

	std::cout << "Projection: ";
	Matrix4x4::Show(m_xmf4x4Projection);
	std::cout << std::endl;
}

void FollowCam::Rotate(float x, float y, float z)
{ 
	if (x != 0.0f)
	{
		XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&GetOwner()->GetTransform()->GetRight()), XMConvertToRadians(x));
		GetOwner()->GetTransform()->SetLook(Vector3::TransformNormal(GetOwner()->GetTransform()->GetLook(), xmmtxRotate));
		GetOwner()->GetTransform()->SetUp(Vector3::TransformNormal(GetOwner()->GetTransform()->GetUp(), xmmtxRotate));

		m_Offset = Vector3::TransformCoord(m_Offset, xmmtxRotate);
	}
	if (y != 0.0f)
	{
		XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&GetOwner()->GetTransform()->GetUp()), XMConvertToRadians(y));
		GetOwner()->GetTransform()->SetLook(Vector3::TransformNormal(GetOwner()->GetTransform()->GetLook(), xmmtxRotate));
		GetOwner()->GetTransform()->SetRight(Vector3::TransformNormal(GetOwner()->GetTransform()->GetRight(), xmmtxRotate));

		m_Offset = Vector3::TransformCoord(m_Offset, xmmtxRotate);
	}
	if (z != 0.0f)
	{
		XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&GetOwner()->GetTransform()->GetLook()), XMConvertToRadians(z));
		GetOwner()->GetTransform()->SetUp(Vector3::TransformNormal(GetOwner()->GetTransform()->GetUp(), xmmtxRotate));
		GetOwner()->GetTransform()->SetRight(Vector3::TransformNormal(GetOwner()->GetTransform()->GetRight(), xmmtxRotate));

		m_Offset = Vector3::TransformCoord(m_Offset, xmmtxRotate); 
	}

	/*회전으로 인해 플레이어의 로컬 x-축, y-축, z-축이 서로 직교하지 않을 수 있으므로 z-축(LookAt 벡터)을 기준으
	로 하여 서로 직교하고 단위벡터가 되도록 한다.*/
	GetOwner()->GetTransform()->SetLook(Vector3::Normalize(GetOwner()->GetTransform()->GetLook()));
	GetOwner()->GetTransform()->SetRight(Vector3::CrossProduct(GetOwner()->GetTransform()->GetUp(), GetOwner()->GetTransform()->GetLook(), true));
	GetOwner()->GetTransform()->SetUp(Vector3::CrossProduct(GetOwner()->GetTransform()->GetLook(), GetOwner()->GetTransform()->GetRight(), true)); 
}

void FollowCam::ZoomIn(float val)
{
	m_Offset = Vector3::ScalarProduct(m_Offset, Vector3::Length(m_Offset) * val);
}

void FollowCam::ZoomOut(float val)
{
	m_Offset = Vector3::ScalarProduct(m_Offset, Vector3::Length(m_Offset) * val);

}
