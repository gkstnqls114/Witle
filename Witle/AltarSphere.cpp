#include "stdafx.h"

// Game Base /////////////////////
#include "GameInput.h"
#include "GameTimer.h"
#include "GameScreen.h"
// Game Base /////////////////////

// For AltarSphere ///////////////////// 
#include "RecognitionRange.h" 
#include "UI3DImage.h"
// GameBase /////////////////////

// DebugMesh /////////////////////
#include "MyBOBox.h" 
#include "LineSphere.h"
// DebugMesh /////////////////////

#include "TextureStorage.h"
#include "PlayerStatus.h"
#include "CameraObject.h"
#include "MainCameraMgr.h"
#include "Texture.h"
#include "MyDescriptorHeap.h"
#include "Sniping.h"
#include "EffectRect.h"
#include "MyRectangle.h"
#include "Object.h"
#include "ShaderManager.h" 
#include "Transform.h"
#include "FollowCam.h"
#include "Terrain.h"

#include "AltarSphere.h"

static float AltarSphereOffestX = 0.f;
static float AltarSphereOffestY = 57.f;
static float AltarSphereOffestZ = 50.f;

bool AltarSphere::RENDER_DEBUG{ true };
  

AltarSphere::AltarSphere(const std::string & entityID, const XMFLOAT3& SpawnPoint, ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ID3D12RootSignature * pd3dGraphicsRootSignature)
	: GameObject(entityID)
{  
	m_pDeactiveTexture = new Texture(ENUM_SCENE::SCENE_GAME, ROOTPARAMETER_INDEX(ROOTPARAMETER_TEXTURE), false, 1, RESOURCE_TEXTURE2D);
	m_pDeactiveTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Model/Textures/Altar_Sphere_black.dds", 0);

	m_pActiveTexture = new Texture(ENUM_SCENE::SCENE_GAME, ROOTPARAMETER_INDEX(ROOTPARAMETER_TEXTURE), false, 1, RESOURCE_TEXTURE2D);
	m_pActiveTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Model/Textures/Altar_Sphere.dds", 0);
	 
	ANIMATION_INFO infos[ALTARSPHERE_ANIMATIONE];
	infos[0] = ALTARSPHERE_FLOW;
	infos[1] = ALTARSPHERE_IDLE;

	m_AltarSphereModel = LoadObject::LoadGeometryAndAnimationFromFile_forMonster(
		pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/Altar_Sphere.bin", NULL,
		ALTARSPHERE_ANIMATIONE, infos);

	m_pLoadObject = m_AltarSphereModel->m_pModelRootObject;
	m_pLoadObject->m_pSkinnedAnimationController = new CAnimationController(pd3dDevice, pd3dCommandList, 1, m_AltarSphereModel);
	m_pLoadObject->m_pSkinnedAnimationController->SetTrackAnimationSet(0, 0);
	 
	XMFLOAT3 extents{ 550.f, 500.f, 550.f };
	m_pMyBOBox = new MyBOBox(this, pd3dDevice, pd3dCommandList, XMFLOAT3{ 0.F, 0.F, 0.F }, extents);

	Move(SpawnPoint); 
	Animate(0.f); 

	m_ActiveGuageBar = new PlayerStatus(this, pd3dDevice, pd3dCommandList,
		POINT{ LONG(GameScreen::GetClientWidth() / 2), 100 },
		400, 30, "Red"
	);
	m_ActiveGuageBar->SetGuage(0.f);
}

AltarSphere::~AltarSphere()
{

}

void AltarSphere::Render(ID3D12GraphicsCommandList * pd3dCommandList, bool isGBuffers)
{
	if (RENDER_DEBUG)
	{
		m_pMyBOBox->Render(pd3dCommandList); 
	}
	 
	if (m_isActive)
	{
		m_pActiveTexture->UpdateShaderVariables(pd3dCommandList);
	}
	else
	{
		m_pDeactiveTexture->UpdateShaderVariables(pd3dCommandList);

	}
	m_pLoadObject->Render(pd3dCommandList, ShaderManager::GetInstance()->GetShader(SHADER_ALTARSPHERE)); 

	if (m_isEnguaged)
	{
		m_ActiveGuageBar->Render(pd3dCommandList);
	}
}

void AltarSphere::ReleaseMembers()
{ 
	if (m_pDeactiveTexture)
	{ 
		delete m_pDeactiveTexture;
		m_pDeactiveTexture = nullptr;
	}

	if (m_pActiveTexture)
	{ 
		delete m_pActiveTexture;
		m_pActiveTexture = nullptr;
	}

	if (m_RecognitionRange)
	{
		m_RecognitionRange->ReleaseObjects();
		delete m_RecognitionRange;
		m_RecognitionRange = nullptr;
	} 
	if (m_ActiveGuageBar)
	{
		m_ActiveGuageBar->ReleaseObjects();
		delete m_ActiveGuageBar;
		m_ActiveGuageBar = nullptr;
	}

	if (m_pLoadObject)
	{
		m_pLoadObject->ReleaseObjects();
		m_pLoadObject = nullptr;
	}

	if (m_AltarSphereModel)
	{
		m_AltarSphereModel->ReleaseObjects();
		delete m_AltarSphereModel;
		m_AltarSphereModel = nullptr;
	}
	if (m_pMyBOBox)
	{
		m_pMyBOBox->ReleaseObjects();
		delete m_pMyBOBox;
		m_pMyBOBox = nullptr;
	} 
}

void AltarSphere::ReleaseMemberUploadBuffers()
{ 
	if (m_ActiveGuageBar) m_ActiveGuageBar->ReleaseUploadBuffers();
	if (m_pDeactiveTexture) m_pDeactiveTexture->ReleaseUploadBuffers();
	if (m_pActiveTexture) m_pActiveTexture->ReleaseUploadBuffers();
	if (m_pLoadObject) m_pLoadObject->ReleaseUploadBuffers();
	if (m_AltarSphereModel)m_AltarSphereModel->ReleaseUploadBuffers();
	if (m_pMyBOBox)m_pMyBOBox->ReleaseUploadBuffers();
}
 
void AltarSphere::Animate(float fElapsedTime)
{ 
	// animate 이전에 현재 설정된 애니메이션 수행하도록 설정
	SetTrackAnimationSet();

	// 반드시 트랜스폼 업데이트..! 
	m_Transform.Update(fElapsedTime);

	m_pLoadObject->m_xmf4x4ToParent = m_Transform.GetWorldMatrix();

	m_pLoadObject->Animate(fElapsedTime);
}

void AltarSphere::Update(float fElapsedTime)
{
	if (m_isEnguaged)
	{
		AddGuage(fElapsedTime);
	}

	if (!m_isActive) return;
	if (m_isFinishFlow) return;

	// 만약 flow 가 끝났다면..
	if (m_pLoadObject->IsTrackAnimationSetFinish(0, ALTARSPHERE_FLOW.ID))
	{
		m_isFinishFlow = true;
		SetAnimationID(ALTARSPHERE_IDLE.ID);
	}
}
 
void AltarSphere::SetTrackAnimationSet()
{
	if (m_CurrAnimation != m_PrevAnimation)
	{
		m_pLoadObject->SetTrackAnimationSet(0, m_CurrAnimation);
		m_PrevAnimation = m_CurrAnimation;
	}
}

void AltarSphere::Move(const XMFLOAT3 & xmf3Shift)
{
	m_Transform.Move(xmf3Shift);
	m_pMyBOBox->Move(xmf3Shift.x, xmf3Shift.y, xmf3Shift.z);
}

void AltarSphere::Rotate(float x, float y, float z)
{
	m_Transform.Rotate(x, y, z);
	m_pMyBOBox->Rotate(x, y, z);
}

void AltarSphere::AddGuage(float time)
{
	if (m_guage >= 100)
	{
		m_guage = 100;
		SetisActive(true);
	}

	if (m_isActive) return;

	m_guage = m_guage + add_guage * time;
	m_ActiveGuageBar->SetGuage(m_guage);
}

void AltarSphere::SetAnimationID(int state)
{
	m_CurrAnimation = state;
	m_pLoadObject->SetTrackAnimationSet(0, m_CurrAnimation);
}
