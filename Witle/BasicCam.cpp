#include "stdafx.h"
#include "GameObject.h"
#include "BasicCam.h"

BasicCam::BasicCam(GameObject * pOwner)
	:Camera(pOwner)
{

}

BasicCam::~BasicCam()
{
}

void BasicCam::LastUpdate(float fTimeElapsed)
{
	//Move, Rotate된 At과 카메라 좌표축(Right, Up, Look)을 기준으로 Position 재설정
	m_At = m_At;
	m_pOwner->GetTransform().SetPosition(Vector3::Subtract(m_At, m_Offset));

	RegenerateViewMatrix(); 
	GenerateFrustum();
}

void BasicCam::Rotate(float x, float y, float z)
{
	XMFLOAT3 right = m_pOwner->GetTransform().GetRight();
	XMFLOAT3 up = m_pOwner->GetTransform().GetUp();
	XMFLOAT3 look = m_pOwner->GetTransform().GetLook();

	//XMFLOAT3 right{ 1.f, 0.f, 0.f };
	//XMFLOAT3 up{0.f, 1.f, 0.f};
	//XMFLOAT3 look{0.f, 0.f, 1.f};

	if (x != 0.0f)
	{
		XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&right), XMConvertToRadians(x));
		m_pOwner->GetTransform().SetLook(Vector3::TransformNormal(m_pOwner->GetTransform().GetLook(), xmmtxRotate));
		m_pOwner->GetTransform().SetUp(Vector3::TransformNormal(m_pOwner->GetTransform().GetUp(), xmmtxRotate));

		m_Offset = Vector3::TransformCoord(m_Offset, xmmtxRotate);
	}
	if (y != 0.0f)
	{
		XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&up), XMConvertToRadians(y));
		m_pOwner->GetTransform().SetLook(Vector3::TransformNormal(m_pOwner->GetTransform().GetLook(), xmmtxRotate));
		m_pOwner->GetTransform().SetRight(Vector3::TransformNormal(m_pOwner->GetTransform().GetRight(), xmmtxRotate));

		m_Offset = Vector3::TransformCoord(m_Offset, xmmtxRotate);
	}
	if (z != 0.0f)
	{
		XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&look), XMConvertToRadians(z));
		m_pOwner->GetTransform().SetUp(Vector3::TransformNormal(m_pOwner->GetTransform().GetUp(), xmmtxRotate));
		m_pOwner->GetTransform().SetRight(Vector3::TransformNormal(m_pOwner->GetTransform().GetRight(), xmmtxRotate));

		m_Offset = Vector3::TransformCoord(m_Offset, xmmtxRotate);
	}

	/*회전으로 인해 플레이어의 로컬 x-축, y-축, z-축이 서로 직교하지 않을 수 있으므로 z-축(LookAt 벡터)을 기준으
	로 하여 서로 직교하고 단위벡터가 되도록 한다.*/
	m_pOwner->GetTransform().SetLook(Vector3::Normalize(m_pOwner->GetTransform().GetLook()));
	m_pOwner->GetTransform().SetRight(Vector3::CrossProduct(m_pOwner->GetTransform().GetUp(), m_pOwner->GetTransform().GetLook(), true));
	m_pOwner->GetTransform().SetUp(Vector3::CrossProduct(m_pOwner->GetTransform().GetLook(), m_pOwner->GetTransform().GetRight(), true));
}
