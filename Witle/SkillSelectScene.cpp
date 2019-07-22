#include "stdafx.h"
#include "GameInput.h"
#include "d3dUtil.h" 
#include "Button.h"
#include "GameScreen.H"
#include "ShaderManager.h"
#include "Texture.h"
#include "UI2DImage.h"
#include "Texture.h"
#include "GraphicsRootSignatureMgr.h"
#include "MyDescriptorHeap.h"
#include "SkillSelectScene.h"

ID3D12DescriptorHeap*		SkillSelectScene::m_pd3dCbvSrvDescriptorHeap;

D3D12_CPU_DESCRIPTOR_HANDLE	SkillSelectScene::m_d3dCbvCPUDescriptorStartHandle;
D3D12_GPU_DESCRIPTOR_HANDLE	SkillSelectScene::m_d3dCbvGPUDescriptorStartHandle;
D3D12_CPU_DESCRIPTOR_HANDLE	SkillSelectScene::m_d3dSrvCPUDescriptorStartHandle;
D3D12_GPU_DESCRIPTOR_HANDLE	SkillSelectScene::m_d3dSrvGPUDescriptorStartHandle;

D3D12_CPU_DESCRIPTOR_HANDLE	SkillSelectScene::m_d3dCbvCPUDescriptorNextHandle;
D3D12_GPU_DESCRIPTOR_HANDLE	SkillSelectScene::m_d3dCbvGPUDescriptorNextHandle;
D3D12_CPU_DESCRIPTOR_HANDLE	SkillSelectScene::m_d3dSrvCPUDescriptorNextHandle;
D3D12_GPU_DESCRIPTOR_HANDLE	SkillSelectScene::m_d3dSrvGPUDescriptorNextHandle;

SkillSelectScene::SkillSelectScene()
{

}

SkillSelectScene::~SkillSelectScene()
{

}
void SkillSelectScene::CreateCbvSrvDescriptorHeaps(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, int nConstantBufferViews, int nShaderResourceViews)
{
	D3D12_DESCRIPTOR_HEAP_DESC d3dDescriptorHeapDesc;
	d3dDescriptorHeapDesc.NumDescriptors = nConstantBufferViews + nShaderResourceViews; //CBVs + SRVs 
	d3dDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	d3dDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	d3dDescriptorHeapDesc.NodeMask = 0;
	pd3dDevice->CreateDescriptorHeap(&d3dDescriptorHeapDesc, __uuidof(ID3D12DescriptorHeap), (void **)&m_pd3dCbvSrvDescriptorHeap);

	m_d3dCbvCPUDescriptorNextHandle = m_d3dCbvCPUDescriptorStartHandle = m_pd3dCbvSrvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	m_d3dCbvGPUDescriptorNextHandle = m_d3dCbvGPUDescriptorStartHandle = m_pd3dCbvSrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart();
	m_d3dSrvCPUDescriptorNextHandle.ptr = m_d3dSrvCPUDescriptorStartHandle.ptr = m_d3dCbvCPUDescriptorStartHandle.ptr + (d3dUtil::gnCbvSrvDescriptorIncrementSize * nConstantBufferViews);
	m_d3dSrvGPUDescriptorNextHandle.ptr = m_d3dSrvGPUDescriptorStartHandle.ptr = m_d3dCbvGPUDescriptorStartHandle.ptr + (d3dUtil::gnCbvSrvDescriptorIncrementSize * nConstantBufferViews);
}

D3D12_GPU_DESCRIPTOR_HANDLE SkillSelectScene::CreateConstantBufferViews(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, int nConstantBufferViews, ID3D12Resource * pd3dConstantBuffers, UINT nStride)
{
	D3D12_GPU_DESCRIPTOR_HANDLE d3dCbvGPUDescriptorHandle = m_d3dCbvGPUDescriptorNextHandle;
	D3D12_GPU_VIRTUAL_ADDRESS d3dGpuVirtualAddress = pd3dConstantBuffers->GetGPUVirtualAddress();
	D3D12_CONSTANT_BUFFER_VIEW_DESC d3dCBVDesc;
	d3dCBVDesc.SizeInBytes = nStride;
	for (int j = 0; j < nConstantBufferViews; j++)
	{
		d3dCBVDesc.BufferLocation = d3dGpuVirtualAddress + (nStride * j);
		m_d3dCbvCPUDescriptorNextHandle.ptr = m_d3dCbvCPUDescriptorNextHandle.ptr + d3dUtil::gnCbvSrvDescriptorIncrementSize;
		pd3dDevice->CreateConstantBufferView(&d3dCBVDesc, m_d3dCbvCPUDescriptorNextHandle);
		m_d3dCbvGPUDescriptorNextHandle.ptr = m_d3dCbvGPUDescriptorNextHandle.ptr + d3dUtil::gnCbvSrvDescriptorIncrementSize;
	}
	return(d3dCbvGPUDescriptorHandle);
}

D3D12_GPU_DESCRIPTOR_HANDLE SkillSelectScene::CreateShaderResourceViews(ID3D12Device * pd3dDevice, Texture * pTexture, UINT nRootParameter, bool bAutoIncrement)
{
	assert(!(pTexture == nullptr));

	D3D12_GPU_DESCRIPTOR_HANDLE d3dSrvGPUDescriptorHandle = m_d3dSrvGPUDescriptorNextHandle;
	if (pTexture)
	{
		int nTextures = pTexture->GetTextures();
		int nTextureType = pTexture->GetTextureType();
		for (int i = 0; i < nTextures; i++)
		{
			ID3D12Resource *pShaderResource = pTexture->GetTexture(i);
			D3D12_RESOURCE_DESC d3dResourceDesc = pShaderResource->GetDesc();
			D3D12_SHADER_RESOURCE_VIEW_DESC d3dShaderResourceViewDesc = d3dUtil::GetShaderResourceViewDesc(d3dResourceDesc, nTextureType);
			pd3dDevice->CreateShaderResourceView(pShaderResource, &d3dShaderResourceViewDesc, m_d3dSrvCPUDescriptorNextHandle);
			m_d3dSrvCPUDescriptorNextHandle.ptr += d3dUtil::gnCbvSrvDescriptorIncrementSize;

			pTexture->SetRootArgument(i, (bAutoIncrement) ? (nRootParameter + i) : nRootParameter, m_d3dSrvGPUDescriptorNextHandle);
			m_d3dSrvGPUDescriptorNextHandle.ptr += d3dUtil::gnCbvSrvDescriptorIncrementSize;
		}
	}
	return(d3dSrvGPUDescriptorHandle);
}

bool SkillSelectScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_MOUSEWHEEL:
	case WM_MOUSEHWHEEL:

		break;
	case WM_LBUTTONDOWN:
		ClickSkillIcon(POINT{ LOWORD(lParam), HIWORD(lParam) });
		break;
	case WM_RBUTTONDOWN:
		break;
	case WM_LBUTTONUP:

		break;
	case WM_RBUTTONUP:
		break;
	case WM_MOUSEMOVE:

		break;

	default:
		break;
	}

	return true;
}

bool SkillSelectScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam, float ElapsedTime)
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

void SkillSelectScene::BuildObjects(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
	BuildLightsAndMaterials(pd3dDevice, pd3dCommandList);

	// 카메라 설정
	m_d3dViewport = D3D12_VIEWPORT{ 0.0f, 0.0f, static_cast<FLOAT>(GameScreen::GetWidth()) , static_cast<FLOAT>(GameScreen::GetHeight()), 0.0f, 1.0f };
	m_d3dScissorRect = D3D12_RECT{ 0, 0, static_cast<LONG>(GameScreen::GetWidth()) ,static_cast<LONG>(GameScreen::GetHeight()) };

	POINT tempselectedPoint[SKILL_SELECTED] = {
		 POINT{ int(GameScreen::GetWidth()) / 2 - 300, int(GameScreen::GetHeight()) / 2 + 270 },
		 POINT{ int(GameScreen::GetWidth()) / 2 - 100, int(GameScreen::GetHeight()) / 2 + 270 },
		 POINT{ int(GameScreen::GetWidth()) / 2 + 100, int(GameScreen::GetHeight()) / 2 + 270 },
		 POINT{ int(GameScreen::GetWidth()) / 2 + 300, int(GameScreen::GetHeight()) / 2 + 270 }
	};
	for (int x = 0; x < SKILL_SELECTED; ++x)
	{
		selectedPoint[x] = tempselectedPoint[x];
	}

	POINT tempchoosePoint[SKILL_TO_CHOOSE] = {
		 POINT{ int(GameScreen::GetWidth()) / 2 - 300, int(GameScreen::GetHeight()) / 2 - 250 },
		 POINT{ int(GameScreen::GetWidth()) / 2 - 100, int(GameScreen::GetHeight()) / 2 - 250 },
		 POINT{ int(GameScreen::GetWidth()) / 2 + 100, int(GameScreen::GetHeight()) / 2 - 250 },
		 POINT{ int(GameScreen::GetWidth()) / 2 + 300, int(GameScreen::GetHeight()) / 2 - 250 },

		 POINT{ int(GameScreen::GetWidth()) / 2 - 300, int(GameScreen::GetHeight()) / 2 - 50 },
		 POINT{ int(GameScreen::GetWidth()) / 2 - 100, int(GameScreen::GetHeight()) / 2 - 50 },
		 POINT{ int(GameScreen::GetWidth()) / 2 + 100, int(GameScreen::GetHeight()) / 2 - 50 },
		 POINT{ int(GameScreen::GetWidth()) / 2 + 300, int(GameScreen::GetHeight()) / 2 - 50 }
	};
	for (int x = 0; x < SKILL_TO_CHOOSE; ++x)
	{
		choosePoint[x] = tempchoosePoint[x];
	}

	m_TESTGameObject = new EmptyGameObject("ui");
	
	// 임시로 선택할 스킬 이미지 로드 ///////////////////////
	wchar_t* filepaths[SKILL_TO_CHOOSE + 1] = {
		L"Image/SkillIconTEST1.dds",
		L"Image/SkillIconTEST2.dds",
		L"Image/SkillIconTEST3.dds",
		L"Image/SkillIconTEST4.dds",
		L"Image/SkillIconTEST5.dds",
		L"Image/SkillIconTEST6.dds",
		L"Image/SkillIconTEST7.dds",
		L"Image/SkillIconTEST8.dds",
		L"Image/SkillIconTEST9.dds"
	};

	m_pHeap = new MyDescriptorHeap();
	m_pHeap->CreateCbvSrvUavDescriptorHeaps(pd3dDevice, pd3dCommandList, 0, SKILL_TO_CHOOSE + 1, 0);

	// 선택하지 않은 스킬도 설정하기위해 ...
	m_pTexture = new Texture(SKILL_TO_CHOOSE + 1, RESOURCE_TEXTURE2D);
	for (int x = 0; x < SKILL_TO_CHOOSE + 1; ++x)
	{
		m_pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, filepaths[x], x);
	}
	m_pHeap->CreateShaderResourceViews(pd3dDevice, pd3dCommandList, m_pTexture, ROOTPARAMETER_TEXTURE, false);
	 

	// 임시로 선택할 스킬 이미지 로드 ///////////////////////

	for (int x = 0; x < SKILL_TO_CHOOSE; ++x)
	{
		m_UISkillToChoose[x] = new UI2DImage(m_TESTGameObject, pd3dDevice, pd3dCommandList, choosePoint[x], 150, 150,
			nullptr);
	}
	// 임시로 선택할 스킬 이미지 로드 ///////////////////////


	// 임시로 선택된 스킬 이미지 로드 ///////////////////////
	for (int x = 0; x < SKILL_SELECTED; ++x)
	{
		m_UISkillSelected[x] = new UI2DImage(m_TESTGameObject, pd3dDevice, pd3dCommandList, selectedPoint[x], 100, 100,
			nullptr);
	} 
	// 임시로 선택된 스킬 이미지 로드 ///////////////////////

}

void SkillSelectScene::ReleaseObjects()
{

}

bool SkillSelectScene::ProcessInput(HWND hWnd, float ElapsedTime)
{

	return true;
}

void SkillSelectScene::UpdatePhysics(float ElapsedTime)
{
}

// ProcessInput에 의한 right, up, look, pos 를 월드변환 행렬에 갱신한다.
void SkillSelectScene::Update(float fElapsedTime)
{

}

void SkillSelectScene::LastUpdate(float fElapsedTime)
{

}

void SkillSelectScene::AnimateObjects(float fTimeElapsed)
{
}

void SkillSelectScene::Render(ID3D12GraphicsCommandList *pd3dCommandList, bool isGBuffers)
{
	pd3dCommandList->SetGraphicsRootSignature(GraphicsRootSignatureMgr::GetGraphicsRootSignature());

	// 클라 화면 설정
	pd3dCommandList->RSSetViewports(1, &m_d3dViewport);
	pd3dCommandList->RSSetScissorRects(1, &m_d3dScissorRect);

	ShaderManager::GetInstance()->SetPSO(pd3dCommandList, SHADER_UISCREEN, false);
	m_pHeap->UpdateShaderVariable(pd3dCommandList);

	for (int x = 0; x < SKILL_TO_CHOOSE; ++x)
	{
		m_pTexture->UpdateShaderVariable(pd3dCommandList, x);
		m_UISkillToChoose[x]->Render(pd3dCommandList);
	}

	for (int x = 0; x < SKILL_SELECTED; ++x)
	{
		m_pTexture->UpdateShaderVariable(pd3dCommandList, m_SelectedIndex[x]);
		m_UISkillSelected[x]->Render(pd3dCommandList);
	}
}

void SkillSelectScene::RenderForShadow(ID3D12GraphicsCommandList * pd3dCommandList)
{
}

void SkillSelectScene::ReleaseUploadBuffers()
{

}
void SkillSelectScene::BuildLightsAndMaterials(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{

}

void SkillSelectScene::RenderShadowMap(ID3D12GraphicsCommandList * pd3dCommandList)
{
}

void SkillSelectScene::ClickSkillIcon(POINT cursor)
{ 
	for (int x = 0; x < SKILL_TO_CHOOSE ; ++x)
	{
		if (((choosePoint[x].x - 75) < cursor.x && cursor.x < (choosePoint[x].x + 75)) &&
			((choosePoint[x].y - 75) < cursor.y && cursor.y < (choosePoint[x].y + 75)))
		{
			for (int y = 0; y < SKILL_SELECTED; ++y)
			{
				if (m_SelectedIndex[y] == 8)
				{
					m_SelectedIndex[y] = x;
					break;
				}
			}
		}
	}

	for (int x = 0; x < SKILL_SELECTED; ++x)
	{
		if ((selectedPoint[x].x - 50) < cursor.x && cursor.x < (selectedPoint[x].x + 50) &&
			(selectedPoint[x].y - 50) < cursor.y && cursor.y < (selectedPoint[x].y + 50))
		{
			m_SelectedIndex[x] = 8; // 8: 아무것도 선택하지 않음으로 변경
		}
	}
}

