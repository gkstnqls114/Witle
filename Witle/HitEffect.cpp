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

HitEffect::HitEffect(GameObject* pOwner, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
	: ComponentBase(pOwner)
{
	m_ResolX = 5;
	m_ResolY = 1;
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
		if (m_CurrIndex > m_ResolX * m_ResolY - 1)
		{
			isActive = false;
			m_CurrIndex = 0;
		}
	}
}

void HitEffect::Render(ID3D12GraphicsCommandList * pd3dCommandList, const XMFLOAT3& pos)
{
	ShaderManager::GetInstance()->SetPSO(pd3dCommandList, SHADER_HITEFFECT, false);

	// set look at.... ºôº¸µå Ã³¸®...  
	XMFLOAT4X4 mtxLookAt = Matrix4x4::LookAtLH(pos, MainCameraMgr::GetMainCamera()->GetTransform().GetPosition(), XMFLOAT3(0, 1.f, 0));
	mtxLookAt._41 = pos.x;
	mtxLookAt._42 = pos.y;
	mtxLookAt._43 = pos.z;
	// set look at....  

	XMFLOAT2 Resol{ m_ResolX, m_ResolY };

	pd3dCommandList->SetGraphicsRoot32BitConstants(ROOTPARAMETER_PICKINGPOINT, 2, &Resol, 0);
	pd3dCommandList->SetGraphicsRoot32BitConstants(ROOTPARAMETER_HPPERCENTAGE, 1, &m_CurrIndex, 0);

	m_HitEffect->Render(pd3dCommandList, mtxLookAt);
}
 