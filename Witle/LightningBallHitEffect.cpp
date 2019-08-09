#include "stdafx.h"

#include "GameScreen.h"
#include "UI3DImage.h" 
#include "Monster.h"
#include "MainCameraMgr.h" 
#include "ShaderManager.h"
#include "Monster.h" 
#include "CameraObject.h"
#include "GameObject.h"
#include "LightningBallHitEffect.h"
    
 
void LightningBallHitEffect::ReleaseObjects()
{
	if (m_LightningBallHitEffect)
	{
		m_LightningBallHitEffect->ReleaseObjects();
		delete m_LightningBallHitEffect;
		m_LightningBallHitEffect = nullptr;
	}
}

void LightningBallHitEffect::ReleaseUploadBuffers()
{
	if (m_LightningBallHitEffect) m_LightningBallHitEffect->ReleaseUploadBuffers();
}

LightningBallHitEffect::LightningBallHitEffect(GameObject* pOwner, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
	: ComponentBase(pOwner)
{
	m_ResolX = 5;
	m_ResolY = 1;
	m_LightningBallHitEffect = new UI3DImage(pOwner, pd3dDevice, pd3dCommandList, POINT{ 0, 0 }, 100, 123,
	L"Image/sprite test2.dds");
}

LightningBallHitEffect::~LightningBallHitEffect()
{

}

void LightningBallHitEffect::Update(float fTimeElapsed)
{
	if (!isActive) return;

	m_TotalTime += fTimeElapsed;

	if (m_TotalTime > m_ChangeIndexTime)
	{
		m_TotalTime -= m_ChangeIndexTime;
		m_CurrIndex += 1;
		if (m_CurrIndex > m_ResolX * m_ResolY - 1)
		{
			isActive = false;
			m_CurrIndex = 0;
		}
	}
}

void LightningBallHitEffect::Render(ID3D12GraphicsCommandList * pd3dCommandList, const XMFLOAT3& pos)
{
	if (!isActive) return;

	ShaderManager::GetInstance()->SetPSO(pd3dCommandList, SHADER_HITEFFECT, false);

	// set look at.... ������ ó��...  
	XMFLOAT4X4 mtxLookAt = Matrix4x4::LookAtLH(pos, MainCameraMgr::GetMainCamera()->GetTransform().GetPosition(), XMFLOAT3(0, 1.f, 0));
	XMFLOAT4X4 uiWorld = Matrix4x4::Identity();

	uiWorld._11 = mtxLookAt._11; uiWorld._12 = mtxLookAt._21; uiWorld._13 = mtxLookAt._31;
	uiWorld._21 = mtxLookAt._12; uiWorld._22 = mtxLookAt._22; uiWorld._23 = mtxLookAt._32;
	uiWorld._31 = mtxLookAt._13; uiWorld._32 = mtxLookAt._23; uiWorld._33 = mtxLookAt._33; 
	uiWorld._41 = pos.x;
	uiWorld._42 = pos.y;
	uiWorld._43 = pos.z;
	// set look at....  

	XMFLOAT2 Resol{ m_ResolX, m_ResolY };

	pd3dCommandList->SetGraphicsRoot32BitConstants(ROOTPARAMETER_PICKINGPOINT, 2, &Resol, 0);
	pd3dCommandList->SetGraphicsRoot32BitConstants(ROOTPARAMETER_HPPERCENTAGE, 1, &m_CurrIndex, 0);

	m_LightningBallHitEffect->Render(pd3dCommandList, uiWorld);
}
 