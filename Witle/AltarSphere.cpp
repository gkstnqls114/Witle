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
	m_pHaep = new MyDescriptorHeap();
	m_pHaep->CreateCbvSrvUavDescriptorHeaps(pd3dDevice, pd3dCommandList, 0, 1, 0);
	m_pTexture = new Texture(1, RESOURCE_TEXTURE2D);
	m_pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Model/Textures/Altar_Sphere.dds", 0);

	m_pHaep->CreateShaderResourceViews(pd3dDevice, pd3dCommandList, m_pTexture, ROOTPARAMETER_TEXTURE, false, 0);

	ANIMATION_INFO infos[ALTARSPHERE_ANIMATIONE];
	infos[0] = ALTARSPHERE_FLOW;
	infos[1] = ALTARSPHERE_IDLE;

	m_AltarSphereModel = LoadObject::LoadGeometryAndAnimationFromFile_forMonster(
		pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/Altar_Sphere.bin", NULL,
		ALTARSPHERE_ANIMATIONE, infos);

	m_pLoadObject = m_AltarSphereModel->m_pModelRootObject;
	m_pLoadObject->m_pSkinnedAnimationController = new CAnimationController(pd3dDevice, pd3dCommandList, 1, m_AltarSphereModel);
	m_pLoadObject->m_pSkinnedAnimationController->SetTrackAnimationSet(0, 0);

	m_Transform.SetPosition(SpawnPoint);

	XMFLOAT3 extents{ 50.f, 50.f, 50.f };
	m_pMyBOBox = new MyBOBox(this, pd3dDevice, pd3dCommandList, XMFLOAT3{ 0.F, 75.F, 0.F }, extents);
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

	m_pHaep->UpdateShaderVariable(pd3dCommandList);
	m_pTexture->UpdateShaderVariable(pd3dCommandList, 0);
	m_pLoadObject->Render(pd3dCommandList, isGBuffers); 
}

void AltarSphere::ReleaseMembers()
{ 
	if (m_RecognitionRange)
	{
		m_RecognitionRange->ReleaseObjects();
		delete m_RecognitionRange;
		m_RecognitionRange = nullptr;
	} 
	if (m_pTexture)
	{
		m_pTexture->ReleaseObjects();
		delete m_pTexture;
		m_pTexture = nullptr;
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
	if (m_pTexture) m_pTexture->ReleaseUploadBuffers();
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
}

void AltarSphere::Rotate(float x, float y, float z)
{
	m_Transform.Rotate(x, y, z);
	m_pMyBOBox->Rotate(x, y, z);
}

void AltarSphere::SetAnimationState(int state)
{
	m_CurrAnimation = state;
	m_pLoadObject->SetTrackAnimationSet(0, m_CurrAnimation);
}
