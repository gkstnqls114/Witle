#include "stdafx.h"

#include "GameScreen.h"
#include "UI3DImage.h" 
#include "Monster.h"
#include "MainCameraMgr.h" 
#include "ShaderManager.h"
#include "Monster.h" 
#include "CameraObject.h"
#include "GameObject.h"
#include "NormalHitEffect.h"
     
NormalHitEffect::NormalHitEffect(GameObject* pOwner, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
	: HitEffect(pOwner, 6, 2, 0.01f)
{ 
	m_HitEffect = new UI3DImage(pOwner, pd3dDevice, pd3dCommandList, POINT{ 0, 0 }, 200, 250, IMAGE_NORMAL_SPRRITE);
}

NormalHitEffect::~NormalHitEffect()
{

}
 