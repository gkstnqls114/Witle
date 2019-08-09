#pragma once
#include "HitEffect.h"

class UI3DImage;

// Hit Effect
class NormalHitEffect
	: public HitEffect
{   
public:
	NormalHitEffect(GameObject* pOwner, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual ~NormalHitEffect();
};