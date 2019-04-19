#include "stdafx.h"
#include "GameScreen.h"
#include "MyRectangle.h"
#include "PlayerStatus.h"

void PlayerStatus::ReleaseObjects()
{

}

void PlayerStatus::ReleaseUploadBuffers()
{
}

PlayerStatus::PlayerStatus(GameObject * pOwner, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
	: ComponentBase(pOwner)
{

}

PlayerStatus::~PlayerStatus()
{
}

void PlayerStatus::Update(float fTimeElapsed)
{

}

void PlayerStatus::Render(ID3D12GraphicsCommandList * pd3dCommandList)
{

}
