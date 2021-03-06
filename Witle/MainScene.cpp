#include "stdafx.h"
#include "GameInput.h"
#include "d3dUtil.h" 
#include "Button.h"
#include "GameObject.h"
#include "GameScreen.H"
#include "ShaderManager.h"
#include "Texture.h"
#include "UI2DImage.h"
#include "MyRectangle.h"
#include "Shader.h"
#include "GraphicsRootSignatureMgr.h"
#include "GameScene.h"
 
#include "MainScene.h"
 
MainScene::MainScene()
{

}

MainScene::~MainScene()
{

}
 
bool MainScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return false;
}

bool MainScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
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

void MainScene::BuildObjects(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
	BuildLightsAndMaterials(pd3dDevice, pd3dCommandList);

	// 카메라 설정
	m_d3dViewport = D3D12_VIEWPORT{ 0.0f, 0.0f, static_cast<FLOAT>(GameScreen::GetWidth()) , static_cast<FLOAT>(GameScreen::GetHeight()), 0.0f, 1.0f };
	m_d3dScissorRect = D3D12_RECT{ 0, 0, static_cast<LONG>(GameScreen::GetWidth()) ,static_cast<LONG>(GameScreen::GetHeight()) };
	 
	m_gameobject = new EmptyGameObject("back");
	m_Background = new UI2DImage(m_gameobject, ENUM_SCENE::SCENE_MAIN, pd3dDevice, pd3dCommandList, RECT{
		0, 0,
		static_cast<LONG>(GameScreen::GetWidth()), static_cast<LONG>(GameScreen::GetHeight()) },
		"Wittle_1280x720"
		);
	 

	m_pSceneShader = ShaderManager::GetInstance()->GetShader(SHADER_UISCREEN);
	
}

void MainScene::ReleaseObjects()
{
	if (m_gameobject)
	{
		m_gameobject->ReleaseObjects();
		delete m_gameobject;
		m_gameobject = nullptr;
	}
	if (m_Background)
	{ 
		m_Background->ReleaseObjects();
		delete m_Background;
		m_Background = nullptr;
	}
}

bool MainScene::ProcessInput(HWND hWnd)
{

	return true;
}

void MainScene::UpdatePhysics()
{
}

// ProcessInput에 의한 right, up, look, pos 를 월드변환 행렬에 갱신한다.
void MainScene::Update()
{

}

void MainScene::LastUpdate()
{

}
 
void MainScene::AnimateObjects()
{ 
}

void MainScene::Render(ID3D12GraphicsCommandList *pd3dCommandList, bool isGBuffers)
{ 
	pd3dCommandList->SetGraphicsRootSignature(GraphicsRootSignatureMgr::GetGraphicsRootSignature());

	// 클라 화면 설정
	pd3dCommandList->RSSetViewports(1, &m_d3dViewport);
	pd3dCommandList->RSSetScissorRects(1, &m_d3dScissorRect);

	pd3dCommandList->SetPipelineState(m_pSceneShader->GetPSO());
	
	GameScene::SetDescriptorHeap(pd3dCommandList);
	for (int i = 0; i < 100; ++i)
	{
		m_Background->Render(pd3dCommandList);
	}
}

void MainScene::RenderForShadow(ID3D12GraphicsCommandList * pd3dCommandList)
{
}

void MainScene::ReleaseUploadBuffers()
{ 
	if (m_gameobject)
	{
		m_gameobject->ReleaseUploadBuffers(); 
	} 
	if (m_Background)
	{
		m_Background->ReleaseUploadBuffers();
	}
} 
void MainScene::BuildLightsAndMaterials(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{

}

void MainScene::RenderShadowMap(ID3D12GraphicsCommandList * pd3dCommandList)
{
}

