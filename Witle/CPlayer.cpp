//-----------------------------------------------------------------------------
// File: CPlayer.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "LineCube.h"
#include "ShaderManager.h"
#include "Shader.h"
#include "HeightMapImage.h"
#include "LoadedModelInfo.h"
// #include "Shader.h"
#include "CPlayer.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CPlayer

CPlayer::CPlayer()
{
	m_xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_xmf3Right = XMFLOAT3(1.0f, 0.0f, 0.0f);
	m_xmf3Up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_xmf3Look = XMFLOAT3(0.0f, 0.0f, 1.0f);

	m_xmf3Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_xmf3Gravity = XMFLOAT3(0.0f, 0.0f, 0.0f); //�߷°��ӵ�
	m_fMaxVelocityXZ = 2000.0f;
	m_fMaxVelocityY = 400.0f;
	m_fFriction = 1700.f;

	m_fPitch = 0.0f;
	m_fRoll = 0.0f;
	m_fYaw = 0.0f;

	m_pPlayerUpdatedContext = NULL;
	m_pCameraUpdatedContext = NULL;
	 
}

CPlayer::~CPlayer()
{
	ReleaseShaderVariables();
}

XMFLOAT3 CPlayer::CalculateAlreadyVelocity(float fTimeElapsed)
{ 
	XMFLOAT3 AlreadyVelocity = Vector3::Add(m_xmf3Velocity, m_xmf3Gravity);
	float fLength = sqrtf(AlreadyVelocity.x * AlreadyVelocity.x + AlreadyVelocity.z * AlreadyVelocity.z);
	float fMaxVelocityXZ = m_fMaxVelocityXZ;
	if (fLength > m_fMaxVelocityXZ)
	{
		AlreadyVelocity.x *= (fMaxVelocityXZ / fLength);
		AlreadyVelocity.z *= (fMaxVelocityXZ / fLength);
	}
	float fMaxVelocityY = m_fMaxVelocityY;
	fLength = sqrtf(AlreadyVelocity.y * AlreadyVelocity.y);
	if (fLength > m_fMaxVelocityY) AlreadyVelocity.y *= (fMaxVelocityY / fLength);

	XMFLOAT3 xmf3Velocity = Vector3::ScalarProduct(AlreadyVelocity, fTimeElapsed, false); 

	return xmf3Velocity;
}

BoundingBox CPlayer::CalculateAlreadyBoundingBox(float fTimeElapsed)
{ 
	XMFLOAT3 AlreadyVelocity = CalculateAlreadyVelocity(fTimeElapsed);
	BoundingBox AlreadyBBox = m_BoundingBox;
	AlreadyBBox.Center = Vector3::Add(AlreadyBBox.Center, AlreadyVelocity);
	return AlreadyBBox;
}

XMFLOAT3 CPlayer::CalculateAlreadyPosition(float fTimeElapsed)
{
	XMFLOAT3 AlreadyVelocity = CalculateAlreadyVelocity(fTimeElapsed);
	XMFLOAT3 AlreadyPosition = m_xmf3Position;
	AlreadyPosition = Vector3::Add(AlreadyPosition, AlreadyVelocity);
	return AlreadyPosition;
}

void CPlayer::CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
{

}

void CPlayer::UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList)
{
}

void CPlayer::ReleaseShaderVariables()
{

}

void CPlayer::Move(DWORD dwDirection, float fDistance, bool bUpdateVelocity)
{
	if (dwDirection)
	{
		XMFLOAT3 xmf3Shift = XMFLOAT3(0, 0, 0);
		if (dwDirection & DIR_FORWARD) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Look, fDistance);
		if (dwDirection & DIR_BACKWARD) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Look, -fDistance);
		if (dwDirection & DIR_RIGHT) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Right, fDistance);
		if (dwDirection & DIR_LEFT) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Right, -fDistance);
		if (dwDirection & DIR_UP) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Up, fDistance);
		if (dwDirection & DIR_DOWN) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Up, -fDistance);

		Move(xmf3Shift, bUpdateVelocity);
	}
}

void CPlayer::Move(const XMFLOAT3& xmf3Shift, bool bUpdateVelocity)
{
	if (bUpdateVelocity)
	{ 
		m_xmf3Velocity = Vector3::Add(m_xmf3Velocity, xmf3Shift);
	}
	else
	{
		m_BoundingBox.Center = Vector3::Add(m_xmf3Position, xmf3Shift);
		m_xmf3Position = Vector3::Add(m_xmf3Position, xmf3Shift);
	}

}

void CPlayer::Rotate(float x, float y, float z)
{
	if (x != 0.0f)
	{
		XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Right), XMConvertToRadians(x));
		m_xmf3Look = Vector3::TransformNormal(m_xmf3Look, xmmtxRotate);
		m_xmf3Up = Vector3::TransformNormal(m_xmf3Up, xmmtxRotate);
	}
	if (y != 0.0f)
	{
		XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Up), XMConvertToRadians(y));
		m_xmf3Look = Vector3::TransformNormal(m_xmf3Look, xmmtxRotate);
		m_xmf3Right = Vector3::TransformNormal(m_xmf3Right, xmmtxRotate);
	}
	if (z != 0.0f)
	{
		XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Look), XMConvertToRadians(z));
		m_xmf3Up = Vector3::TransformNormal(m_xmf3Up, xmmtxRotate);
		m_xmf3Right = Vector3::TransformNormal(m_xmf3Right, xmmtxRotate);
	}

	/*ȸ������ ���� �÷��̾��� ���� x-��, y-��, z-���� ���� �������� ���� �� �����Ƿ� z-��(LookAt ����)�� ������
	�� �Ͽ� ���� �����ϰ� �������Ͱ� �ǵ��� �Ѵ�.*/
	m_xmf3Look = Vector3::Normalize(m_xmf3Look);
	m_xmf3Right = Vector3::CrossProduct(m_xmf3Up, m_xmf3Look, true);
	m_xmf3Up = Vector3::CrossProduct(m_xmf3Look, m_xmf3Right, true);

	UpdateTransform(NULL);
}

void CPlayer::Update(float fTimeElapsed)
{
	m_xmf3Velocity = Vector3::Add(m_xmf3Velocity, m_xmf3Gravity);
	float fLength = sqrtf(m_xmf3Velocity.x * m_xmf3Velocity.x + m_xmf3Velocity.z * m_xmf3Velocity.z);
	float fMaxVelocityXZ = m_fMaxVelocityXZ;
	if (fLength > m_fMaxVelocityXZ)
	{
		m_xmf3Velocity.x *= (fMaxVelocityXZ / fLength);
		m_xmf3Velocity.z *= (fMaxVelocityXZ / fLength);
	}
	float fMaxVelocityY = m_fMaxVelocityY;
	fLength = sqrtf(m_xmf3Velocity.y * m_xmf3Velocity.y);
	if (fLength > m_fMaxVelocityY) m_xmf3Velocity.y *= (fMaxVelocityY / fLength);

	XMFLOAT3 xmf3Velocity = Vector3::ScalarProduct(m_xmf3Velocity, fTimeElapsed, false);
	Move(xmf3Velocity, false);

	// TEST
	// if (m_pPlayerUpdatedContext) OnPlayerUpdateCallback(fTimeElapsed);

	//DWORD nCurrentCameraMode = m_pCamera->GetMode();
	//if (nCurrentCameraMode == THIRD_PERSON_CAMERA) m_pCamera->Update(m_xmf3Position, fTimeElapsed);
	//if (m_pCameraUpdatedContext) OnCameraUpdateCallback(fTimeElapsed);
	//if (nCurrentCameraMode == THIRD_PERSON_CAMERA) m_pCamera->SetLookAt(m_xmf3Position);
	//m_pCamera->RegenerateViewMatrix();

	// ���� ���� ����
	// fLength = Vector3::Length(m_xmf3Velocity);
	//float fDeceleration = (m_fFriction * fTimeElapsed);
	//if (fDeceleration > fLength) fDeceleration = fLength;
	//m_xmf3Velocity = Vector3::Add(m_xmf3Velocity, Vector3::ScalarProduct(m_xmf3Velocity, -fDeceleration, true));

	// SetTrackAnimationSet(0, ::IsZero(fLength) ? 0 : 1);

	UpdateTransform(NULL);
}

void CPlayer::OnPrepareRender()
{
	m_xmf4x4ToParent._11 = m_xmf3Right.x; m_xmf4x4ToParent._12 = m_xmf3Right.y; m_xmf4x4ToParent._13 = m_xmf3Right.z;
	m_xmf4x4ToParent._21 = m_xmf3Up.x; m_xmf4x4ToParent._22 = m_xmf3Up.y; m_xmf4x4ToParent._23 = m_xmf3Up.z;
	m_xmf4x4ToParent._31 = m_xmf3Look.x; m_xmf4x4ToParent._32 = m_xmf3Look.y; m_xmf4x4ToParent._33 = m_xmf3Look.z;
	m_xmf4x4ToParent._41 = m_xmf3Position.x; m_xmf4x4ToParent._42 = m_xmf3Position.y; m_xmf4x4ToParent._43 = m_xmf3Position.z;

	m_xmf4x4ToParent = Matrix4x4::Multiply(XMMatrixScaling(m_xmf3Scale.x, m_xmf3Scale.y, m_xmf3Scale.z), m_xmf4x4ToParent);
}

void CPlayer::Render(ID3D12GraphicsCommandList *pd3dCommandList)
{
	LoadObject::Render(pd3dCommandList);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 

// TEST
//
//void CSoundCallbackHandler::HandleCallback(void *pCallbackData)
//{
//   _TCHAR *pWavName = (_TCHAR *)pCallbackData; 
//#ifdef _WITH_DEBUG_CALLBACK_DATA
//	TCHAR pstrDebug[256] = { 0 };
//	_stprintf_s(pstrDebug, 256, _T("%s\n"), pWavName);
//	OutputDebugString(pstrDebug);
//#endif
//#ifdef _WITH_SOUND_RESOURCE
//   PlaySound(pWavName, ::ghAppInstance, SND_RESOURCE | SND_ASYNC);
//#else
//   PlaySound(pWavName, NULL, SND_FILENAME | SND_ASYNC);
//#endif
//}

CTerrainPlayer::CTerrainPlayer(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, void *pContext)
{
	// CLoadedModelInfo *pAngrybotModel = LoadObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/testbox.bin");
	CLoadedModelInfo *pAngrybotModel = LoadObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/AnimationTest.bin");
	SetChild(pAngrybotModel->m_pModelRootObject, true);

#ifdef _SHOW_BOUNDINGBOX
	XMFLOAT3 center{ 0.f, 75.f, 0.f };
	XMFLOAT3 extents{ 25.f, 75.f, 25.f };
	m_BoundingBox = BoundingBox(center, extents);
	m_pLineCube = new LineCube(pd3dDevice, pd3dCommandList, center, extents, true);
#endif // DEBUG 

	//m_pSkinnedAnimationController = new CAnimationController(pd3dDevice, pd3dCommandList, 1, pAngrybotModel);
	//m_pSkinnedAnimationController->SetTrackAnimationSet(0, 0);

	// TEST
//	m_pSkinnedAnimationController->SetCallbackKeys(0, 1, 3);
//#ifdef _WITH_SOUND_RESOURCE
//	m_pSkinnedAnimationController->SetCallbackKey(0, 0, 0.1f, _T("Footstep01"));
//	m_pSkinnedAnimationController->SetCallbackKey(0, 1, 0.5f, _T("Footstep02"));
//	m_pSkinnedAnimationController->SetCallbackKey(0, 2, 0.9f, _T("Footstep03"));
//#else
//	m_pSkinnedAnimationController->SetCallbackKey(0, 1, 0, 0.1f, _T("Sound/Footstep01.wav"));
//	m_pSkinnedAnimationController->SetCallbackKey(0, 1, 1, 0.5f, _T("Sound/Footstep02.wav"));
//	m_pSkinnedAnimationController->SetCallbackKey(0, 1, 2, 0.9f, _T("Sound/Footstep03.wav"));
//#endif
	// CAnimationCallbackHandler *pAnimationCallbackHandler = new CSoundCallbackHandler();
	// m_pSkinnedAnimationController->SetAnimationCallbackHandler(0, 1, pAnimationCallbackHandler);

	CreateShaderVariables(pd3dDevice, pd3dCommandList);

	SetPlayerUpdatedContext(pContext);
	SetCameraUpdatedContext(pContext);

	// TEST
	//HeightMapImage *pTerrain = (HeightMapImage *)pContext;
	//SetPosition(XMFLOAT3(370.0f, pTerrain->GetHeight(370.0f, 650.0f), 650.0f));

	if (pAngrybotModel) delete pAngrybotModel;
}

CTerrainPlayer::~CTerrainPlayer()
{
}

void CTerrainPlayer::OnPlayerUpdateCallback(float fTimeElapsed)
{
	HeightMapImage *pTerrain = (HeightMapImage *)m_pPlayerUpdatedContext;
	XMFLOAT3 xmf3Scale = pTerrain->GetScale();
	XMFLOAT3 xmf3PlayerPosition = GetPosition();
	int z = (int)(xmf3PlayerPosition.z / xmf3Scale.z);
	bool bReverseQuad = ((z % 2) != 0);
	float fHeight = pTerrain->GetHeight(xmf3PlayerPosition.x, xmf3PlayerPosition.z, bReverseQuad) + 0.0f;
	if (xmf3PlayerPosition.y < fHeight)
	{
		XMFLOAT3 xmf3PlayerVelocity = GetVelocity();
		xmf3PlayerVelocity.y = 0.0f;
		SetVelocity(xmf3PlayerVelocity);
		xmf3PlayerPosition.y = fHeight;
		SetPosition(xmf3PlayerPosition);
	}
}

void CTerrainPlayer::OnCameraUpdateCallback(float fTimeElapsed)
{
	// TEST
	//HeightMapImage *pTerrain = (HeightMapImage *)m_pCameraUpdatedContext;
	//XMFLOAT3 xmf3Scale = pTerrain->GetScale();
	//XMFLOAT3 xmf3CameraPosition = m_pCamera->GetPosition();
	//int z = (int)(xmf3CameraPosition.z / xmf3Scale.z);
	//bool bReverseQuad = ((z % 2) != 0);
	//float fHeight = pTerrain->GetHeight(xmf3CameraPosition.x, xmf3CameraPosition.z, bReverseQuad) + 5.0f;
	//if (xmf3CameraPosition.y <= fHeight)
	//{
	//	xmf3CameraPosition.y = fHeight;
	//	m_pCamera->SetPosition(xmf3CameraPosition);
	//	if (m_pCamera->GetMode() == THIRD_PERSON_CAMERA)
	//	{
	//		CThirdPersonCamera *p3rdPersonCamera = (CThirdPersonCamera *)m_pCamera;
	//		p3rdPersonCamera->SetLookAt(GetPosition());
	//	}
	//}
}

void CTerrainPlayer::Render(ID3D12GraphicsCommandList * pd3dCommandList)
{
	// Bounding Box Render
#ifdef _SHOW_BOUNDINGBOX 
	if (m_pLineCube)
	{
		pd3dCommandList->SetPipelineState(ShaderManager::GetInstance()->GetShader("Line")->GetPSO());
		m_pLineCube->Render(pd3dCommandList, m_xmf4x4World, true);
	}
#endif // _SHOW_BOUNDINGBOX
	//

	CPlayer::Render(pd3dCommandList);
}

ReflexTree::ReflexTree(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ID3D12RootSignature * pd3dGraphicsRootSignature, void * pContext)
{ 
	CLoadedModelInfo *pAngrybotModel = LoadObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/ReflexTree.bin");
	SetChild(pAngrybotModel->m_pModelRootObject, true);
	 
	ReflexTree::SetPosition(XMFLOAT3(100, 0, 100));
	
#ifdef _SHOW_BOUNDINGBOX
	XMFLOAT3 center{ -30.f, 100.f, 0.f };
	XMFLOAT3 extents{ 40.f, 100.f, 40.f };
	XMFLOAT3 pos{ m_xmf4x4ToParent._41,  m_xmf4x4ToParent._42,  m_xmf4x4ToParent._43 };
	center = Vector3::Add(center, pos);
	m_BoundingBox = BoundingBox(center, extents);
	
	m_pLineCube = new LineCube(pd3dDevice, pd3dCommandList, m_BoundingBox.Center, m_BoundingBox.Extents, false);
#endif // DEBUG 
}

ReflexTree::~ReflexTree()
{
}

void ReflexTree::SetPosition(XMFLOAT3 xmf3Position)
{
	m_xmf3Position = xmf3Position;
	LoadObject::SetPosition(xmf3Position.x, xmf3Position.y, xmf3Position.z);
}

void ReflexTree::Render(ID3D12GraphicsCommandList * pd3dCommandList)
{
	// Bounding Box Render
#ifdef _SHOW_BOUNDINGBOX 
	if (m_pLineCube)
	{
		pd3dCommandList->SetPipelineState(ShaderManager::GetInstance()->GetShader("Line")->GetPSO());
		m_pLineCube->Render(pd3dCommandList, m_xmf4x4World, false);
	}
#endif // _SHOW_BOUNDINGBOX
	//

	CPlayer::Render(pd3dCommandList);
}
