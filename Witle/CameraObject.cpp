#include "stdafx.h"
#include "BasicCam.h"
#include "Transform.h"
#include "MyFrustum.h"
#include "CameraObject.h"

void CameraObject::ReleaseMembers()
{
	if (m_pCameraComponent)
	{
		m_pCameraComponent->ReleaseShaderVariables();
		delete m_pCameraComponent;
		m_pCameraComponent = nullptr;
	}
	if (m_pFrustum)
	{
		delete m_pFrustum;
		m_pFrustum = nullptr;
	}
}

CameraObject::CameraObject(const std::string & entityID)
	:GameObject(entityID)
{
	m_pCameraComponent = new BasicCam(this);
	m_pFrustum = new MyFrustum(this);
}

CameraObject::~CameraObject()
{
}

void CameraObject::LastUpdate(float fElapsedTime)
{
	m_pCameraComponent->LastUpdate(fElapsedTime);
	m_Transform.Update(fElapsedTime); // Transform 
	
	m_pFrustum->GenerateFrustum(m_pCameraComponent->GetProjectionMatrix(), m_pCameraComponent->GetViewMatrix());
}

void CameraObject::SetViewportsAndScissorRects(ID3D12GraphicsCommandList *pd3dCommandList)
{
	m_pCameraComponent->SetViewportsAndScissorRects(pd3dCommandList);
}

void CameraObject::ChangeCamera(Camera * pNewCamera)
{
	//if (m_pCameraComponent)
	//{
	//	delete m_pCameraComponent;
	//} 
	float deltaPitch = m_pCameraComponent->GetPitch() - pNewCamera->GetPitch();
	float deltaYaw = m_pCameraComponent->GetYaw() - pNewCamera->GetYaw();
	float deltaRoll = m_pCameraComponent->GetRoll() - pNewCamera->GetRoll();

	m_pCameraComponent = pNewCamera;
	m_pCameraComponent->Rotate(deltaPitch, deltaYaw, deltaRoll);
}
