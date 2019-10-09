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
#include "PlayerSkillMgr.h"
#include "SkillStg.h"
#include "SkillSelectScene.h"

MyDescriptorHeap* SkillSelectScene::m_pHeap{ nullptr };
Texture* SkillSelectScene::m_pTexture{ nullptr };
int SkillSelectScene::m_SelectedIndex[SKILL_SELECTED]
{ ENUM_SELECTABLESKILL::SELECTABLESKILL_SHIELD, ENUM_SELECTABLESKILL::SELECTABLESKILL_BLESSING, ENUM_SELECTABLESKILL::SELECTABLESKILL_HEALING, ENUM_SELECTABLESKILL::SELECTABLESKILL_FIREBALL }; // 선택된 네 개의 인덱스들. 기본으로 0 1 2 3으로 설정

SkillSelectScene::SkillSelectScene()
{

}

SkillSelectScene::~SkillSelectScene()
{

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
		 POINT{ int(GameScreen::GetWidth()) / 2 - 200, int(GameScreen::GetHeight()) / 2 - 250 },
		 POINT{ int(GameScreen::GetWidth()) / 2 -  0, int(GameScreen::GetHeight()) / 2 - 250 },
		 POINT{ int(GameScreen::GetWidth()) / 2 + 200, int(GameScreen::GetHeight()) / 2 - 250 },
		 
		 POINT{ int(GameScreen::GetWidth()) / 2 - 200, int(GameScreen::GetHeight()) / 2 - 50 },
		 POINT{ int(GameScreen::GetWidth()) / 2 -  0, int(GameScreen::GetHeight()) / 2 - 50 },
		 POINT{ int(GameScreen::GetWidth()) / 2 + 200, int(GameScreen::GetHeight()) / 2 - 50 },
	};
	for (int x = 0; x < SKILL_TO_CHOOSE; ++x)
	{
		choosePoint[x] = tempchoosePoint[x];
	}

	m_TESTGameObject = new EmptyGameObject("ui");
	
	// 임시로 선택할 스킬 이미지 로드 ///////////////////////
	wchar_t* filepaths[SKILL_TO_CHOOSE + 1] = {
		L"Image/FireBall_icon.dds",
		L"Image/IceBall_icon.dds",
		L"Image/Lightning_icon.dds",
		L"Image/Shield_icon.dds", 
		L"Image/Blessing_icon.dds",
		L"Image/Heal_icon.dds",
		L"Image/Black.dds"
	};

	m_pHeap = new MyDescriptorHeap();
	m_pHeap->CreateCbvSrvUavDescriptorHeaps(pd3dDevice, pd3dCommandList, 0, SKILL_TO_CHOOSE + 1, 0, ENUM_SCENE::SCENE_SKILLSELECT);

	// 선택하지 않은 스킬도 설정하기위해 ...
	m_pTexture = new Texture(ENUM_SCENE::SCENE_SKILLSELECT, ROOTPARAMETER_INDEX(ROOTPARAMETER_TEXTURE), false, SKILL_TO_CHOOSE + 1, RESOURCE_TEXTURE2D);
	for (int x = 0; x < SKILL_TO_CHOOSE + 1; ++x)
	{
		m_pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, filepaths[x], x);
	}
	m_pHeap->CreateShaderResourceViews(pd3dDevice,  m_pTexture, ROOTPARAMETER_TEXTURE, false);
	 

	// 임시로 선택할 스킬 이미지 로드 ///////////////////////

	for (int x = 0; x < SKILL_TO_CHOOSE; ++x)
	{
		m_UISkillToChoose[x] = new UI2DImage(m_TESTGameObject, ENUM_SCENE::SCENE_SKILLSELECT, pd3dDevice, pd3dCommandList, choosePoint[x], 150, 150,
			"");
	}
	// 임시로 선택할 스킬 이미지 로드 ///////////////////////


	// 임시로 선택된 스킬 이미지 로드 ///////////////////////
	for (int x = 0; x < SKILL_SELECTED; ++x)
	{
		m_UISkillSelected[x] = new UI2DImage(m_TESTGameObject, ENUM_SCENE::SCENE_SKILLSELECT, pd3dDevice, pd3dCommandList, selectedPoint[x], 100, 100,
			"");
	} 
	// 임시로 선택된 스킬 이미지 로드 ///////////////////////

	m_GameStartButton = new UI2DImage(m_TESTGameObject, ENUM_SCENE::SCENE_SKILLSELECT, pd3dDevice, pd3dCommandList, POINT{ int(GameScreen::GetWidth()) - 125, int(GameScreen::GetHeight()) - 75 }, 150, 70, "");
}

void SkillSelectScene::ReleaseObjects()
{
	if (m_pHeap)
	{
		m_pHeap->ReleaseObjects();
		delete m_pHeap;
		m_pHeap = nullptr; 
	}
	if (m_pTexture)
	{
		m_pTexture->ReleaseObjects();
		delete m_pTexture;
		m_pTexture = nullptr;
	}
	if (m_TESTGameObject)
	{
		m_TESTGameObject->ReleaseObjects();
		delete m_TESTGameObject;
		m_TESTGameObject = nullptr;
	}

	for (int x = 0; x < SKILL_TO_CHOOSE; ++x)
	{
		if (m_UISkillToChoose[x])
		{ 
			m_UISkillToChoose[x]->ReleaseObjects();
			delete m_UISkillToChoose[x];
			m_UISkillToChoose[x] = nullptr;
		}
	}
	for (int x = 0; x < SKILL_SELECTED; ++x)
	{
		if (m_UISkillSelected[x])
		{
			m_UISkillSelected[x]->ReleaseObjects();
			delete m_UISkillSelected[x];
			m_UISkillSelected[x] = nullptr;
		}
	}
	if (m_GameStartButton)
	{
		m_GameStartButton->ReleaseObjects();
		delete m_GameStartButton;
		m_GameStartButton = nullptr;
	}
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

void SkillSelectScene::FinishSkillSelect()
{
	for (int x = 0; x < SKILL_SELECTED; ++x)
	{
		PlayerSkillMgr::GetInstance()->SetSkill(SkillStg::GetInstance()->GetSkill(m_SelectedIndex[x]), x);
	}
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


	bool isReadySkill = true;
	for (int x = 0; x < SKILL_SELECTED; ++x)
	{
		if (m_SelectedIndex[x] == SKILL_TO_CHOOSE)
		{
			isReadySkill = false;
		}
	}
	if (isReadySkill)
	{
		if(m_GameStartButton) m_GameStartButton->Render(pd3dCommandList);
	}
}

void SkillSelectScene::RenderForShadow(ID3D12GraphicsCommandList * pd3dCommandList)
{
}

void SkillSelectScene::ReleaseUploadBuffers()
{  
	if(m_pTexture) m_pTexture->ReleaseUploadBuffers();
	

	for (int x = 0; x < SKILL_TO_CHOOSE; ++x)
	{
		if (m_UISkillToChoose[x])
		{
			m_UISkillToChoose[x]->ReleaseUploadBuffers();
		}
	}
	for (int x = 0; x < SKILL_SELECTED; ++x)
	{
		if (m_UISkillSelected[x])
		{
			m_UISkillSelected[x]->ReleaseUploadBuffers();
		}
	}
	m_GameStartButton->ReleaseUploadBuffers();
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
		// 만약 이미 선택되었다면 넘어감
		bool isAlreadySelected = false;
		for (int y = 0; y < SKILL_SELECTED; ++y)
		{
			if (m_SelectedIndex[y] == x)
			{
				isAlreadySelected = true;
			}
		}
		if (isAlreadySelected) continue;


		if (((choosePoint[x].x - 75) < cursor.x && cursor.x < (choosePoint[x].x + 75)) &&
			((choosePoint[x].y - 75) < cursor.y && cursor.y < (choosePoint[x].y + 75)))
		{
			for (int y = 0; y < SKILL_SELECTED; ++y)
			{
				if (m_SelectedIndex[y] == SKILL_TO_CHOOSE) // 맨마지막. 즉 BLACK
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
			m_SelectedIndex[x] = SKILL_TO_CHOOSE; // SKILL_TO_CHOOSE: 아무것도 선택하지 않음으로 변경
		}
	}
}

