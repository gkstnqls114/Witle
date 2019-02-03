#include "stdafx.h"
#include "BasicCam.h"
#include "Transform.h"
#include "MyFrustum.h"
#include "CameraObject.h"

void CameraObject::ReleaseMembers()
{
	if (m_pCameraComponent)
	{
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

void CameraObject::CreateShaderVariables(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
{
}

void CameraObject::UpdateShaderVariables(ID3D12GraphicsCommandList * pd3dCommandList)
{
	// 쉐이더 변수 업데이트
	XMFLOAT4X4 xmf4x4View;
	XMStoreFloat4x4(&xmf4x4View, XMMatrixTranspose(XMLoadFloat4x4(&m_pCameraComponent->GetViewMatrix())));

	XMFLOAT4X4 xmf4x4Projection;
	XMStoreFloat4x4(&xmf4x4Projection, XMMatrixTranspose(XMLoadFloat4x4(&m_pCameraComponent->GetProjectionMatrix())));
	pd3dCommandList->SetGraphicsRoot32BitConstants(1, 16, &xmf4x4View, 0);
	pd3dCommandList->SetGraphicsRoot32BitConstants(1, 16, &xmf4x4Projection, 16);
}

void CameraObject::ReleaseShaderVariables()
{
}

void CameraObject::ChangeCamera(Camera * pNewCamera)
{
	if (m_pCameraComponent)
	{
		delete m_pCameraComponent;
	}

	m_pCameraComponent = pNewCamera;
}
