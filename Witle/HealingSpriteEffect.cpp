#include "stdafx.h"
#include "UI3DImage.h"
#include "HealingSpriteEffect.h"

HealingSpriteEffect::HealingSpriteEffect(GameObject* pOwner, ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
	: HitEffect(pOwner, 5, 5, 0.03f)
{
	m_HitEffect = new UI3DImage(pOwner, pd3dDevice, pd3dCommandList, POINT{ 0, 0 }, 120, 120, IMAGE_HEALING_SPRRITE);
}

HealingSpriteEffect::~HealingSpriteEffect()
{
}
