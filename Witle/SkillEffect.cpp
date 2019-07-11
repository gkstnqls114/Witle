#include "stdafx.h"

// Game Base /////////////////////
#include "GameInput.h"
#include "GameTimer.h"
#include "GameScreen.h"
// Game Base /////////////////////

// For SkillEffect ///////////////////// 
#include "RecognitionRange.h" 
#include "UI3DImage.h"
// GameBase /////////////////////

// DebugMesh /////////////////////
#include "MyBOBox.h" 
#include "LineSphere.h"
// DebugMesh /////////////////////

#include "CameraObject.h"
#include "MainCameraMgr.h"
#include "Texture.h"
#include "MyDescriptorHeap.h"
#include "Sniping.h"
#include "EffectRect.h"
#include "MyRectangle.h"
#include "Object.h"
#include "ShaderManager.h" 
#include "Transform.h"
#include "FollowCam.h"
#include "Terrain.h"

#include "SkillEffect.h"

void SkillEffect::Render(ID3D12GraphicsCommandList * pd3dCommandList, bool isGBuffers)
{
}

SkillEffect::SkillEffect(const std::string & entityID)
	:GameObject(entityID)
{
}

SkillEffect::~SkillEffect()
{
}

void SkillEffect::Update(float)
{
}

void SkillEffect::Move(ULONG nDirection, float fDistance, bool bVelocity)
{
}

void SkillEffect::Move(const XMFLOAT3 & xmf3Shift)
{
}

void SkillEffect::MoveVelocity(const XMFLOAT3 & xmf3Shift)
{
}

void SkillEffect::Move(float fxOffset, float fyOffset, float fzOffset)
{
}

void SkillEffect::Rotate(float x, float y, float z)
{
}
