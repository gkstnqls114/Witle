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

HitEffect::HitEffect(GameObject* pOwner, float resolX, float resolY, float changeIndexTime)
	: ComponentBase(pOwner), m_ResolX(resolX), m_ResolY(resolY), m_ChangeIndexTime(changeIndexTime)
{  
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
	if (!isActive) return;

	ShaderManager::GetInstance()->SetPSO(pd3dCommandList, SHADER_HITEFFECT, false);

	XMFLOAT4X4 uiWorld = Matrix4x4::Identity();
	if (isBillBoard)
	{
		// set look at.... ������ ó��...  
		XMFLOAT4X4 mtxLookAt = Matrix4x4::LookAtLH(pos, MainCameraMgr::GetMainCamera()->GetTransform().GetPosition(), XMFLOAT3(0, 1.f, 0));
	
		uiWorld._11 = mtxLookAt._11; uiWorld._12 = mtxLookAt._21; uiWorld._13 = mtxLookAt._31;
		uiWorld._21 = mtxLookAt._12; uiWorld._22 = mtxLookAt._22; uiWorld._23 = mtxLookAt._32;
		uiWorld._31 = mtxLookAt._13; uiWorld._32 = mtxLookAt._23; uiWorld._33 = mtxLookAt._33;
		uiWorld._41 = pos.x;
		uiWorld._42 = pos.y;
		uiWorld._43 = pos.z;
		// set look at....   
	}
	else
	{ 
		// ���� �ٶ󺸵��� ����
		uiWorld._11 = 1.f; uiWorld._12 = 0.f; uiWorld._13 = 0.f;
		uiWorld._21 = 0.f; uiWorld._22 = 0.f; uiWorld._23 = -1.f;
		uiWorld._31 = 0.f; uiWorld._32 = 1.f; uiWorld._33 = 0.f;
		uiWorld._41 = pos.x;
		uiWorld._42 = pos.y;
		uiWorld._43 = pos.z;
	}

	XMFLOAT2 Resol{ m_ResolX, m_ResolY };

	pd3dCommandList->SetGraphicsRoot32BitConstants(ROOTPARAMETER_PICKINGPOINT, 2, &Resol, 0);
	pd3dCommandList->SetGraphicsRoot32BitConstants(ROOTPARAMETER_HPPERCENTAGE, 1, &m_CurrIndex, 0);

	m_HitEffect->Render(pd3dCommandList, uiWorld);
}
 