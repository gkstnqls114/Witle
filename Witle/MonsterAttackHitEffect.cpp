#include "stdafx.h"

#include "GameScreen.h"
#include "UI3DImage.h" 
#include "Monster.h"
#include "MainCameraMgr.h" 
#include "ShaderManager.h"
#include "Monster.h" 
#include "CameraObject.h"
#include "GameObject.h"
#include "MonsterAttackHitEffect.h"

MonsterAttackHitEffect::MonsterAttackHitEffect(GameObject* pOwner, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
	: HitEffect(pOwner, 5, 1)
{
	m_HitEffect = new UI3DImage(pOwner, pd3dDevice, pd3dCommandList, POINT{ 0, 0 }, 100, 123, IMAGE_MONSTERATTACKHIT_SPRRITE);
}

MonsterAttackHitEffect::~MonsterAttackHitEffect()
{

}
