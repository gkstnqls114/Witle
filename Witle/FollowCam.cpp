#include "stdafx.h"
#include "Transform.h"
#include "GameObject.h"
#include "FollowCam.h"


void FollowCam::RotateOnLookingobject(float x, float y, float z)
{
	// DWORD nCameraMode = m_pCamera->GetMode();
	//1인칭 카메라 또는 3인칭 카메라의 경우 플레이어의 회전은 약간의 제약이 따른다.
	//if ((nCameraMode == FIRST_PERSON_CAMERA) || (nCameraMode == THIRD_PERSON_CAMERA))
	//{
	//	/*로컬 x-축을 중심으로 회전하는 것은 고개를 앞뒤로 숙이는 동작에 해당한다. 그러므로 x-축을 중심으로 회전하는
	//	각도는 -89.0~+89.0도 사이로 제한한다. x는 현재의 m_fPitch에서 실제 회전하는 각도이므로 x만큼 회전한 다음
	//	Pitch가 +89도 보다 크거나 -89도 보다 작으면 m_fPitch가 +89도 또는 -89도가 되도록 회전각도(x)를 수정한다.*/
	//	if (x != 0.0f)
	//	{
	//		m_fPitch += x;
	//		if (m_fPitch > +89.0f) { x -= (m_fPitch - 89.0f); m_fPitch = +89.0f; }
	//		if (m_fPitch < -89.0f) { x -= (m_fPitch + 89.0f); m_fPitch = -89.0f; }
	//	}
	//	if (y != 0.0f)
	//	{
	//		//로컬 y-축을 중심으로 회전하는 것은 몸통을 돌리는 것이므로 회전 각도의 제한이 없다.
	//		m_fYaw += y;
	//		if (m_fYaw > 360.0f) m_fYaw -= 360.0f;
	//		if (m_fYaw < 0.0f) m_fYaw += 360.0f;
	//	}
	//	if (z != 0.0f)
	//	{
	//		/*로컬 z-축을 중심으로 회전하는 것은 몸통을 좌우로 기울이는 것이므로 회전 각도는 -20.0~+20.0도 사이로 제한된
	//		다. z는 현재의 m_fRoll에서 실제 회전하는 각도이므로 z만큼 회전한 다음 m_fRoll이 +20도 보다 크거나 -20도보다
	//		작으면 m_fRoll이 +20도 또는 -20도가 되도록 회전각도(z)를 수정한다.*/
	//		m_fRoll += z;
	//		if (m_fRoll > +20.0f) { z -= (m_fRoll - 20.0f); m_fRoll = +20.0f; }
	//		if (m_fRoll < -20.0f) { z -= (m_fRoll + 20.0f); m_fRoll = -20.0f; }
	//	}
	//	//카메라를 x, y, z 만큼 회전한다. 플레이어를 회전하면 카메라가 회전하게 된다.
	//	m_pCamera->Rotate(x, y, z);

	//	/*플레이어를 회전한다. 1인칭 카메라 또는 3인칭 카메라에서 플레이어의 회전은 로컬 y-축에서만 일어난다. 플레이어
	//	의 로컬 y-축(Up 벡터)을 기준으로 로컬 z-축(Look 벡터)와 로컬 x-축(Right 벡터)을 회전시킨다. 기본적으로 Up 벡
	//	터를 기준으로 회전하는 것은 플레이어가 똑바로 서있는 것을 가정한다는 의미이다.*/
	//	if (y != 0.0f)
	//	{
	//		XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_Up),
	//			XMConvertToRadians(y));
	//		m_Look = Vector3::TransformNormal(m_Look, xmmtxRotate);
	//		m_Right = Vector3::TransformNormal(m_Right, xmmtxRotate);
	//	}
	//}
	//	}
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

		// At Pos도 마찬가지로 회전
		XMFLOAT3 LookObject_To_At = Vector3::Subtract(m_At, tr->GetPosition());
		LookObject_To_At = Vector3::TransformCoord(LookObject_To_At, xmmtxRotate);
		m_At = Vector3::Add(tr->GetPosition(), LookObject_To_At);

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

		// At Pos도 마찬가지로 회전
		XMFLOAT3 LookObject_To_At = Vector3::Subtract(m_At, tr->GetPosition());
		LookObject_To_At = Vector3::TransformCoord(LookObject_To_At, xmmtxRotate);
		m_At = Vector3::Add(tr->GetPosition(), LookObject_To_At);

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

		// At Pos도 마찬가지로 회전
		XMFLOAT3 LookObject_To_At = Vector3::Subtract(m_At, tr->GetPosition());
		LookObject_To_At = Vector3::TransformCoord(LookObject_To_At, xmmtxRotate);
		m_At = Vector3::Add(tr->GetPosition(), LookObject_To_At);

		// Offset 도 마찬가지로 회전
		m_Offset = Vector3::TransformCoord(m_Offset, xmmtxRotate);
	}

	/*회전으로 인해 플레이어의 로컬 x-축, y-축, z-축이 서로 직교하지 않을 수 있으므로 z-축(LookAt 벡터)을 기준으
	로 하여 서로 직교하고 단위벡터가 되도록 한다.*/
	m_Look = Vector3::Normalize(m_Look);
	m_Right = Vector3::CrossProduct(m_Up, m_Look, true);
	m_Up = Vector3::CrossProduct(m_Look, m_Right, true);
}

void FollowCam::MoveAroundLookingobject(const XMFLOAT3 & Shift)
{
	m_At.x += Shift.x;
	m_At.y += Shift.y;
	m_At.z += Shift.z;
}

FollowCam::FollowCam(Camera * camera, GameObject * target)
	: m_pTarget(target)
{
}

FollowCam::FollowCam(FollowCam * followcam)
{
}

FollowCam::FollowCam(GameObject * target)
	: m_pTarget(target)
{
}

FollowCam::~FollowCam()
{
}

void FollowCam::Move(const XMFLOAT3 & Shift)
{
	if (m_pTarget) {
		MoveAroundLookingobject(Shift);
	} 
}

void FollowCam::Rotate(float x, float y, float z)
{
	if (m_pTarget) {
		RotateOnLookingobject(x, y, z);
	}
}

void FollowCam::ZoomIn(float val)
{
	m_Offset = Vector3::ScalarProduct(m_Offset, Vector3::Length(m_Offset) * val);
}

void FollowCam::ZoomOut(float val)
{
	m_Offset = Vector3::ScalarProduct(m_Offset, Vector3::Length(m_Offset) * val);

}
