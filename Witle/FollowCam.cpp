#include "stdafx.h"
#include "Transform.h"
#include "GameObject.h"
#include "FollowCam.h"



FollowCam::FollowCam(Camera * camera, GameObject * target)
	: m_pTarget(target)
{
}

FollowCam::FollowCam(FollowCam * followcam)
{
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
	XMFLOAT3 xmf3Direction = Vector3::Subtract(xmf3Position, m_Position);
	float fLength = Vector3::Length(xmf3Direction);
	xmf3Direction = Vector3::Normalize(xmf3Direction);
	float fTimeLagScale = (m_fTimeLag) ? fTimeElapsed * (1.0f / m_fTimeLag) : 1.0f;
	float fDistance = fLength * fTimeLagScale;
	if (fDistance > fLength) fDistance = fLength;
	if (fLength < 0.01f) fDistance = fLength;
	if (fDistance > 0)
	{
		m_Position = Vector3::Add(m_Position, xmf3Direction, fDistance);
		SetLookAt(xmf3LookAt);
	}
}

void FollowCam::SetLookAt(const XMFLOAT3 & xmf3LookAt)
{
	XMFLOAT4X4 mtxLookAt = Matrix4x4::LookAtLH(m_Position, xmf3LookAt, m_pTarget->GetComponent<Transform>("")->GetUp());
	m_Right = XMFLOAT3(mtxLookAt._11, mtxLookAt._21, mtxLookAt._31);
	m_Up = XMFLOAT3(mtxLookAt._12, mtxLookAt._22, mtxLookAt._32);
	m_Look = XMFLOAT3(mtxLookAt._13, mtxLookAt._23, mtxLookAt._33);
}

FollowCam::FollowCam(GameObject * target)
	: m_pTarget(target)
{
}

FollowCam::~FollowCam()
{
}

void FollowCam::Teleport(const XMFLOAT3 & at)
{
	m_At = at;
	m_Position = Vector3::Subtract(m_At, m_Offset);
	m_xmf4x4View = Matrix4x4::LookAtLH(m_Position, m_At, m_Up);
}

void FollowCam::Update(float fTimeElapsed, const XMFLOAT3 & xmf3LookAt)
{
	MoveSmoothly(fTimeElapsed, xmf3LookAt);
}

void FollowCam::LastUpdate()
{
	// 물리 계산
	// 벽과의 충돌 등...
	
	//Move, Rotate된 At과 카메라 좌표축(Right, Up, Look)을 기준으로 Position 재설정
	m_At = m_pTarget->GetComponent<Transform>("")->GetPosition();
	m_Position = Vector3::Subtract(m_At, m_Offset);

	// 카메라 변환 행렬을 재설정한다.
	RegenerateViewMatrix();

	// 카메라 변환 행렬이 바뀔 때마다 카메라 절두체를 다시 생성한다(절두체는 월드 좌표계로 생성한다).
	GenerateFrustum();
}

void FollowCam::Rotate(float x, float y, float z)
{
	Transform* tr = m_pTarget->GetComponent<Transform>("");

	if ((x != 0.0f))
	{
		//플레이어의 로컬 x-축에 대한 x 각도의 회전 행렬을 계산한다. 
		XMFLOAT3 xmf3Right = tr->GetRight();
		XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&xmf3Right), XMConvertToRadians(x));

		//카메라의 로컬 x-축, y-축, z-축을 회전한다.
		m_Right = Vector3::TransformNormal(m_Right, xmmtxRotate);
		m_Up = Vector3::TransformNormal(m_Up, xmmtxRotate);
		m_Look = Vector3::TransformNormal(m_Look, xmmtxRotate); 

		// Offset 도 마찬가지로 회전
		m_Offset = Vector3::TransformCoord(m_Offset, xmmtxRotate);
	}
	if ((y != 0.0f))
	{
		XMFLOAT3 xmf3Up = tr->GetUp();
		XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&xmf3Up), XMConvertToRadians(y));
		m_Right = Vector3::TransformNormal(m_Right, xmmtxRotate);
		m_Up = Vector3::TransformNormal(m_Up, xmmtxRotate);
		m_Look = Vector3::TransformNormal(m_Look, xmmtxRotate);
		 
		// Offset 도 마찬가지로 회전
		m_Offset = Vector3::TransformCoord(m_Offset, xmmtxRotate);
	}
	if (z != 0.0f)
	{
		XMFLOAT3 xmf3Look = tr->GetLook();
		XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&xmf3Look), XMConvertToRadians(z));
		m_Right = Vector3::TransformNormal(m_Right, xmmtxRotate);
		m_Up = Vector3::TransformNormal(m_Up, xmmtxRotate);
		m_Look = Vector3::TransformNormal(m_Look, xmmtxRotate);
		 
		// Offset 도 마찬가지로 회전
		m_Offset = Vector3::TransformCoord(m_Offset, xmmtxRotate);
	}

	/*회전으로 인해 플레이어의 로컬 x-축, y-축, z-축이 서로 직교하지 않을 수 있으므로 z-축(LookAt 벡터)을 기준으
	로 하여 서로 직교하고 단위벡터가 되도록 한다.*/
	m_Look = Vector3::Normalize(m_Look);
	m_Right = Vector3::CrossProduct(m_Up, m_Look, true);
	m_Up = Vector3::CrossProduct(m_Look, m_Right, true);
}

void FollowCam::ZoomIn(float val)
{
	m_Offset = Vector3::ScalarProduct(m_Offset, Vector3::Length(m_Offset) * val);
}

void FollowCam::ZoomOut(float val)
{
	m_Offset = Vector3::ScalarProduct(m_Offset, Vector3::Length(m_Offset) * val);

}
