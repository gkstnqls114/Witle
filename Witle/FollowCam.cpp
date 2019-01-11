#include "stdafx.h"
#include "Transform.h"
#include "GameObject.h"
#include "FollowCam.h"


void FollowCam::RotateOnLookingobject(float x, float y, float z)
{
	// DWORD nCameraMode = m_pCamera->GetMode();
	//1��Ī ī�޶� �Ǵ� 3��Ī ī�޶��� ��� �÷��̾��� ȸ���� �ణ�� ������ ������.
	//if ((nCameraMode == FIRST_PERSON_CAMERA) || (nCameraMode == THIRD_PERSON_CAMERA))
	//{
	//	/*���� x-���� �߽����� ȸ���ϴ� ���� ���� �յڷ� ���̴� ���ۿ� �ش��Ѵ�. �׷��Ƿ� x-���� �߽����� ȸ���ϴ�
	//	������ -89.0~+89.0�� ���̷� �����Ѵ�. x�� ������ m_fPitch���� ���� ȸ���ϴ� �����̹Ƿ� x��ŭ ȸ���� ����
	//	Pitch�� +89�� ���� ũ�ų� -89�� ���� ������ m_fPitch�� +89�� �Ǵ� -89���� �ǵ��� ȸ������(x)�� �����Ѵ�.*/
	//	if (x != 0.0f)
	//	{
	//		m_fPitch += x;
	//		if (m_fPitch > +89.0f) { x -= (m_fPitch - 89.0f); m_fPitch = +89.0f; }
	//		if (m_fPitch < -89.0f) { x -= (m_fPitch + 89.0f); m_fPitch = -89.0f; }
	//	}
	//	if (y != 0.0f)
	//	{
	//		//���� y-���� �߽����� ȸ���ϴ� ���� ������ ������ ���̹Ƿ� ȸ�� ������ ������ ����.
	//		m_fYaw += y;
	//		if (m_fYaw > 360.0f) m_fYaw -= 360.0f;
	//		if (m_fYaw < 0.0f) m_fYaw += 360.0f;
	//	}
	//	if (z != 0.0f)
	//	{
	//		/*���� z-���� �߽����� ȸ���ϴ� ���� ������ �¿�� ����̴� ���̹Ƿ� ȸ�� ������ -20.0~+20.0�� ���̷� ���ѵ�
	//		��. z�� ������ m_fRoll���� ���� ȸ���ϴ� �����̹Ƿ� z��ŭ ȸ���� ���� m_fRoll�� +20�� ���� ũ�ų� -20������
	//		������ m_fRoll�� +20�� �Ǵ� -20���� �ǵ��� ȸ������(z)�� �����Ѵ�.*/
	//		m_fRoll += z;
	//		if (m_fRoll > +20.0f) { z -= (m_fRoll - 20.0f); m_fRoll = +20.0f; }
	//		if (m_fRoll < -20.0f) { z -= (m_fRoll + 20.0f); m_fRoll = -20.0f; }
	//	}
	//	//ī�޶� x, y, z ��ŭ ȸ���Ѵ�. �÷��̾ ȸ���ϸ� ī�޶� ȸ���ϰ� �ȴ�.
	//	m_pCamera->Rotate(x, y, z);

	//	/*�÷��̾ ȸ���Ѵ�. 1��Ī ī�޶� �Ǵ� 3��Ī ī�޶󿡼� �÷��̾��� ȸ���� ���� y-�࿡���� �Ͼ��. �÷��̾�
	//	�� ���� y-��(Up ����)�� �������� ���� z-��(Look ����)�� ���� x-��(Right ����)�� ȸ����Ų��. �⺻������ Up ��
	//	�͸� �������� ȸ���ϴ� ���� �÷��̾ �ȹٷ� ���ִ� ���� �����Ѵٴ� �ǹ��̴�.*/
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
		//�÷��̾��� ���� x-�࿡ ���� x ������ ȸ�� ����� ����Ѵ�.

		XMFLOAT3 xmf3Right = tr->GetRight();
		XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&xmf3Right), XMConvertToRadians(x));
		//ī�޶��� ���� x-��, y-��, z-���� ȸ���Ѵ�.
		m_Right = Vector3::TransformNormal(m_Right, xmmtxRotate);
		m_Up = Vector3::TransformNormal(m_Up, xmmtxRotate);
		m_Look = Vector3::TransformNormal(m_Look, xmmtxRotate);

		// At Pos�� ���������� ȸ��
		XMFLOAT3 LookObject_To_At = Vector3::Subtract(m_At, tr->GetPosition());
		LookObject_To_At = Vector3::TransformCoord(LookObject_To_At, xmmtxRotate);
		m_At = Vector3::Add(tr->GetPosition(), LookObject_To_At);

		// Offset �� ���������� ȸ��
		m_Offset = Vector3::TransformCoord(m_Offset, xmmtxRotate);
	}
	if ((y != 0.0f))
	{
		XMFLOAT3 xmf3Up = tr->GetUp();
		XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&xmf3Up), XMConvertToRadians(y));
		m_Right = Vector3::TransformNormal(m_Right, xmmtxRotate);
		m_Up = Vector3::TransformNormal(m_Up, xmmtxRotate);
		m_Look = Vector3::TransformNormal(m_Look, xmmtxRotate);

		// At Pos�� ���������� ȸ��
		XMFLOAT3 LookObject_To_At = Vector3::Subtract(m_At, tr->GetPosition());
		LookObject_To_At = Vector3::TransformCoord(LookObject_To_At, xmmtxRotate);
		m_At = Vector3::Add(tr->GetPosition(), LookObject_To_At);

		// Offset �� ���������� ȸ��
		m_Offset = Vector3::TransformCoord(m_Offset, xmmtxRotate);
	}
	if (z != 0.0f)
	{
		XMFLOAT3 xmf3Look = tr->GetLook();
		XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&xmf3Look), XMConvertToRadians(z));
		m_Right = Vector3::TransformNormal(m_Right, xmmtxRotate);
		m_Up = Vector3::TransformNormal(m_Up, xmmtxRotate);
		m_Look = Vector3::TransformNormal(m_Look, xmmtxRotate);

		// At Pos�� ���������� ȸ��
		XMFLOAT3 LookObject_To_At = Vector3::Subtract(m_At, tr->GetPosition());
		LookObject_To_At = Vector3::TransformCoord(LookObject_To_At, xmmtxRotate);
		m_At = Vector3::Add(tr->GetPosition(), LookObject_To_At);

		// Offset �� ���������� ȸ��
		m_Offset = Vector3::TransformCoord(m_Offset, xmmtxRotate);
	}

	/*ȸ������ ���� �÷��̾��� ���� x-��, y-��, z-���� ���� �������� ���� �� �����Ƿ� z-��(LookAt ����)�� ������
	�� �Ͽ� ���� �����ϰ� �������Ͱ� �ǵ��� �Ѵ�.*/
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
