#pragma once
#include "HitEffect.h"

class UI3DImage;

// Hit Effect
class FireBallHitEffect
	: public HitEffect
{
public:
	FireBallHitEffect(GameObject* pOwner, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual ~FireBallHitEffect();
};