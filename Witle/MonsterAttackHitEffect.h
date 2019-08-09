#pragma once
#include "HitEffect.h"

class UI3DImage;

// Hit Effect
class MonsterAttackHitEffect
	: public HitEffect
{
public:
	MonsterAttackHitEffect(GameObject* pOwner, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual ~MonsterAttackHitEffect();
};