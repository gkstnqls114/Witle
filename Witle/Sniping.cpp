#include "stdafx.h"
#include "FollowCam.h" 
#include "Player.h"
#include "GameScreen.h"
#include "CameraObject.h"
#include "MyBOBox.h"
#include "GameInput.h"
#include "Sniping.h"
 
void Sniping::ReleaseMembers()
{ 
	if (m_pBaseCameraComponent)
	{
		m_pBaseCameraComponent->ReleaseObjects(); 
		delete m_pBaseCameraComponent;
		m_pBaseCameraComponent = nullptr;
	}

	if (m_pSnipingCameraComponent)
	{
		m_pSnipingCameraComponent->ReleaseObjects(); 
		delete m_pSnipingCameraComponent;
		m_pSnipingCameraComponent = nullptr;
	}
}

void Sniping::ReleaseMemberUploadBuffers()
{
	if (m_pBaseCameraComponent) m_pBaseCameraComponent->ReleaseUploadBuffers();
	if (m_pSnipingCameraComponent) m_pSnipingCameraComponent->ReleaseUploadBuffers();
}

Sniping::Sniping(CameraObject* pCamera, Player* pPlayer, ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
	:FixedSkill("Sniping")
{  
	m_pCameraObject = pCamera;
	m_pPlayer = pPlayer;

	m_pBaseCameraComponent = new FollowCam(pCamera, pPlayer);
	m_pSnipingCameraComponent = new FollowCam(pCamera, pPlayer);

	// 기본 카메라 컴포넌트 
	m_pBaseCameraComponent->CreateShaderVariables(pd3dDevice, pd3dCommandList);
	
	// 스나이핑 카메라 컴포넌트
	m_pSnipingCameraComponent->CreateShaderVariables(pd3dDevice, pd3dCommandList);
	
	Init();
}

Sniping::~Sniping()
{
}

void Sniping::Init()
{ 
	m_pBaseCameraComponent->Init();
	m_pBaseCameraComponent->SetAt(XMFLOAT3(15000.f, 0.f, 1500.f));
	m_pBaseCameraComponent->SetAtOffset(XMFLOAT3( 0.f, 200.f, 0.f ));
	m_pBaseCameraComponent->SetOffset(XMFLOAT3(0.f, -100.f, 500.f));
	m_pBaseCameraComponent->SetViewport(0, 0, GameScreen::GetWidth(), GameScreen::GetHeight(), 0.0f, 1.0f);
	m_pBaseCameraComponent->SetScissorRect(0, 0, GameScreen::GetWidth(), GameScreen::GetHeight());
	m_pBaseCameraComponent->GenerateProjectionMatrix(0.01f, CAMERA_FAR, float(GameScreen::GetWidth()) / float(GameScreen::GetHeight()), 60.0f);
	//m_pBaseCameraComponent->GenerateViewMatrix(); 
	m_pBaseCameraComponent->LastUpdate(0.f);

	m_pSnipingCameraComponent->Init();
	m_pSnipingCameraComponent->SetAt(XMFLOAT3(15000.f, 0.f, 1500.f));
	m_pSnipingCameraComponent->SetOffset(XMFLOAT3(0, 0, 50.f));
	m_pSnipingCameraComponent->SetAtOffset(XMFLOAT3(0.f, 200.f, 0.f));
	m_pSnipingCameraComponent->SetViewport(0, 0, GameScreen::GetWidth(), GameScreen::GetHeight(), 0.0f, 1.0f);
	m_pSnipingCameraComponent->SetScissorRect(0, 0, GameScreen::GetWidth(), GameScreen::GetHeight());
	m_pSnipingCameraComponent->GenerateProjectionMatrix(0.01f, CAMERA_FAR * 1.5f, float(GameScreen::GetWidth()) / float(GameScreen::GetHeight()), 30.0f);
	m_pSnipingCameraComponent->GenerateViewMatrix(); 
	m_pBaseCameraComponent->LastUpdate(0.f);
}

void Sniping::Rotate(float x, float y, float z)
{ 
}
 
void Sniping::LastUpdate(float , const XMFLOAT2& ScreenPos)
{  
}
 
void Sniping::DoNotUse()
{
	m_isUsing = false;
	m_pPlayer->DoRendering();
	m_pCameraObject->ChangeCamera(m_pBaseCameraComponent);
}

void Sniping::DoUse()
{
	m_isUsing = true;
	m_pPlayer->DoNotRendering();
	m_pCameraObject->ChangeCamera(m_pSnipingCameraComponent);
}
