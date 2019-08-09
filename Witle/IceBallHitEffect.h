#pragma once
#include "HitEffect.h"

class UI3DImage;

// Hit Effect
class IceBallHitEffect
	: public HitEffect
{
public:
	IceBallHitEffect(GameObject* pOwner, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual ~IceBallHitEffect();
};