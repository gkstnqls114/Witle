#include "stdafx.h"
#include "BasicCam.h"
#include "Transform.h"
#include "MyFrustum.h"
#include "CameraObject.h"

void CameraObject::ReleaseMembers()
{
	//if (m_pCameraComponent)
	//{
	//	m_pCameraComponent->ReleaseShaderVariables();
	//	delete m_pCameraComponent;
	//	m_pCameraComponent = nullptr;
	//}
	if (m_pFrustum)
	{
		delete m_pFrustum;
		m_pFrustum = nullptr;
	}
}

CameraObject::CameraObject(const std::string & entityID)
	:GameObject(entityID)
{
	// m_pCameraComponent = new BasicCam(this);
	m_pFrustum = new MyFrustum(this);
}

CameraObject::~CameraObject()
{
}

void CameraObject::LastUpdate(float fElapsedTime)
{
	if (!m_pCameraComponent) return;

	m_pCameraComponent->LastUpdate(fElapsedTime);
	m_Transform.Update(fElapsedTime); // Transform 
	
	m_pFrustum->GenerateFrustum(m_pCameraComponent->GetProjectionMatrix(), m_pCameraComponent->GetViewMatrix());
}

void CameraObject::SetViewportsAndScissorRects(ID3D12GraphicsCommandList *pd3dCommandList)
{
	if (!m_pCameraComponent) return;
	m_pCameraComponent->SetViewportsAndScissorRects(pd3dCommandList);
}

void CameraObject::ChangeCamera(Camera * pNewCamera)
{ 
	float deltaPitch, deltaYaw, deltaRoll = 0.f;

	if (m_pCameraComponent)
	{ 
		deltaPitch = m_pCameraComponent->GetPitch() - pNewCamera->GetPitch();
		deltaYaw = m_pCameraComponent->GetYaw() - pNewCamera->GetYaw();
		deltaRoll = m_pCameraComponent->GetRoll() - pNewCamera->GetRoll();
	}
	else
	{ 
		deltaPitch = pNewCamera->GetPitch();
		deltaYaw = pNewCamera->GetYaw();
		deltaRoll = pNewCamera->GetRoll();
	}

	m_pCameraComponent = pNewCamera;
	m_pCameraComponent->Rotate(deltaPitch, deltaYaw, deltaRoll);
}
