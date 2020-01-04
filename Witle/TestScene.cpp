#include "stdafx.h"
#include "d3dUtil.h"  
#include "GameInput.h"
#include "GameScreen.h"
#include "UI2DImage.h"
#include "GraphicsRootSignatureMgr.h"
#include "ShaderManager.h" 
#include "GameObject.h"
#include "Texture.h" 
#include "SceneMgr.h"

#include "QuadtreeMgr.h"

#include "TestScene.h"
 
TestScene::TestScene()
{
	
}

TestScene::~TestScene()
{

} 

bool TestScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return false;
}

bool TestScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam, float ElapsedTime)
{
	switch (nMessageID)
	{
	case WM_KEYUP:
		switch (wParam)
		{
		case VK_F1:
		case VK_F2:
		case VK_F3:
			break;

		}
		break;
	case WM_KEYDOWN: 
		switch (wParam) {
		case 'A':
			break;
		case 'W':
			break;
		case 'w':
			break;
		case 'S':
		case 's':
			break;
		default:
			break;
		}
		break;
	}

	return false;
}

void TestScene::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
	QuadtreeMgr::GetInstance()->PrintInfo();
}

void TestScene::ReleaseObjects()
{

}

bool TestScene::ProcessInput(HWND hWnd, float ElapsedTime)
{
	return true;
}

void TestScene::UpdatePhysics(float ElapsedTime)
{
}

// ProcessInput에 의한 right, up, look, pos 를 월드변환 행렬에 갱신한다.
void TestScene::Update(float fElapsedTime)
{

}

void TestScene::LastUpdate(float fElapsedTime)
{

}

void TestScene::AnimateObjects(float fTimeElapsed)
{
}

void TestScene::Render(ID3D12GraphicsCommandList* pd3dCommandList, bool isGBuffers)
{
	pd3dCommandList->SetGraphicsRootSignature(GraphicsRootSignatureMgr::GetGraphicsRootSignature());

	// 클라 화면 설정
	pd3dCommandList->RSSetViewports(1, &m_d3dViewport);
	pd3dCommandList->RSSetScissorRects(1, &m_d3dScissorRect);

	ShaderManager::GetInstance()->SetPSO(pd3dCommandList, SHADER_UISCREEN, false);
	 
}

void TestScene::RenderForShadow(ID3D12GraphicsCommandList* pd3dCommandList)
{
}

void TestScene::RenderForPlayerShadow(ID3D12GraphicsCommandList* pd3dCommandList)
{
}

void TestScene::ReleaseUploadBuffers()
{

}
 