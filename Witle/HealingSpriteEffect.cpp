#include "stdafx.h"
#include "UI3DImage.h"
#include "HealingSpriteEffect.h"

HealingSpriteEffect::HealingSpriteEffect(GameObject* pOwner, ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
	: HitEffect(pOwner, 6, 2, 0.01f)
{
	m_HitEffect = new UI3DImage(pOwner, pd3dDevice, pd3dCommandList, POINT{ 0, 0 }, 200, 250, IMAGE_NORMAL_SPRRITE);
}

HealingSpriteEffect::~HealingSpriteEffect()
{
}
