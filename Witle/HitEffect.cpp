#include "stdafx.h"

#include "GameScreen.h"
#include "UI3DImage.h" 
#include "Monster.h"
#include "MainCameraMgr.h" 
#include "ShaderManager.h"
#include "Monster.h" 
#include "CameraObject.h"
#include "GameObject.h"
#include "HitEffect.h"
 
void HitEffect::ReleaseObjects()
{
	if (m_HitEffect)
	{
		m_HitEffect->ReleaseObjects();
		delete m_HitEffect;
		m_HitEffect = nullptr;
	}
}

void HitEffect::ReleaseUploadBuffers()
{
	if (m_HitEffect) m_HitEffect->ReleaseUploadBuffers();
}

HitEffect::HitEffect(GameObject * pOwner, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
	: ComponentBase(pOwner)
{
	m_ResolX = 4;
	m_ResolX = 1;
	m_HitEffect = new UI3DImage(pOwner, pd3dDevice, pd3dCommandList, POINT{ 0, 0 }, 100, 123, 
	L"Image/sprite test2.dds");
}

HitEffect::~HitEffect()
{

}

void HitEffect::Update(float fTimeElapsed)
{
	if (!isActive) return;

	m_TotalTime += fTimeElapsed;

	if (m_TotalTime > m_ChangeIndexTime)
	{
		m_TotalTime -= m_ChangeIndexTime;
		m_CurrIndex += 1;
		m_CurrIndex = min(m_CurrIndex, m_ResolX * m_ResolY - 1);
	}
	
}

void HitEffect::Render(ID3D12GraphicsCommandList * pd3dCommandList, const XMFLOAT4X4 world)
{
	ShaderManager::GetInstance()->SetPSO(pd3dCommandList, SHADER_HITEFFECT, false);

	// set look at.... 빌보드 처리...
	//XMFLOAT4X4 uiWorld = world;
	//uiWorld._42 += 150; // 임시로 위로 올림

	//XMFLOAT3 xmf3Position(uiWorld._41, uiWorld._42, uiWorld._43);

	//XMFLOAT4X4 mtxLookAt = Matrix4x4::LookAtLH(xmf3Position, MainCameraMgr::GetMainCamera()->GetTransform().GetPosition(), XMFLOAT3(0, 1.f, 0));
	//uiWorld._11 = mtxLookAt._11; uiWorld._12 = mtxLookAt._21; uiWorld._13 = mtxLookAt._31;
	//uiWorld._21 = mtxLookAt._12; uiWorld._22 = mtxLookAt._22; uiWorld._23 = mtxLookAt._32;
	//uiWorld._31 = mtxLookAt._13; uiWorld._32 = mtxLookAt._23; uiWorld._33 = mtxLookAt._33;
	// set look at....


	XMFLOAT4X4 uiWorld = Matrix4x4::Identity();
	uiWorld._41 = 15000; // 임시로 위로 올림
	uiWorld._42 = 150; // 임시로 위로 올림
	uiWorld._43 = 15000; // 임시로 위로 올림

	XMFLOAT2 Resol{ m_ResolX, m_ResolY };
	pd3dCommandList->SetGraphicsRoot32BitConstants(ROOTPARAMETER_PICKINGPOINT, 2, &Resol, 0);
	 
	pd3dCommandList->SetGraphicsRoot32BitConstants(ROOTPARAMETER_HPPERCENTAGE, 1, &m_CurrIndex, 0);

	m_HitEffect->Render(pd3dCommandList, uiWorld);
}
 