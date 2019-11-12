#include "stdafx.h"
#include "GameScene.h"
#include "MainScene.h"
#include "SkillSelectScene.h"
#include "PostprocessingSetting.h"
#include "WinScene.h"
#include "LoseScene.h"
#include "GameInput.h"
#include "SceneMgr.h"

#include "SoundManager.h"

SceneMgr* SceneMgr::m_Instace{ nullptr };

SceneMgr::SceneMgr()
{
	m_MainScene = new MainScene;
	m_SkillSelectScene = new SkillSelectScene;
	m_GameScene = new GameScene;
	m_WinScene = new WinScene;
	m_LoseScene = new LoseScene;
}

SceneMgr::~SceneMgr()
{
}

void SceneMgr::ReleaseObjects()
{
	if (m_GameScene)
	{
		m_GameScene->ReleaseObjects();
		delete m_GameScene;
		m_GameScene = nullptr;
	}

	if (m_MainScene)
	{
		m_MainScene->ReleaseObjects();
		delete m_MainScene;
		m_MainScene = nullptr;
	}

	if (m_SkillSelectScene)
	{
		m_SkillSelectScene->ReleaseObjects();
		delete m_SkillSelectScene;
		m_SkillSelectScene = nullptr;
	}

	if (m_WinScene)
	{
		m_WinScene->ReleaseObjects();
		delete m_WinScene;
		m_WinScene = nullptr;
	}

	if (m_LoseScene)
	{
		m_LoseScene->ReleaseObjects();
		delete m_LoseScene;
		m_LoseScene = nullptr;
	}
}

void SceneMgr::ReleaseUploadBuffers()
{
	if (m_GameScene) m_GameScene->ReleaseUploadBuffers();
	if (m_MainScene) m_MainScene->ReleaseUploadBuffers();
	if (m_SkillSelectScene) m_SkillSelectScene->ReleaseUploadBuffers();
	if (m_WinScene) m_WinScene->ReleaseUploadBuffers();
	if (m_LoseScene) m_LoseScene->ReleaseUploadBuffers();
}

void SceneMgr::BuildObjects(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
	if (m_GameScene) m_GameScene->BuildObjects(pd3dDevice, pd3dCommandList);
	if (m_MainScene) m_MainScene->BuildObjects(pd3dDevice, pd3dCommandList);
	if (m_SkillSelectScene) m_SkillSelectScene->BuildObjects(pd3dDevice, pd3dCommandList);
	if (m_WinScene) m_WinScene->BuildObjects(pd3dDevice, pd3dCommandList);
	if (m_LoseScene) m_LoseScene->BuildObjects(pd3dDevice, pd3dCommandList);

	ChangeSceneToMain();
}

void SceneMgr::BuildHeap(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
{
	GameScene::CreateCbvSrvDescriptorHeaps(pd3dDevice, pd3dCommandList);
}

// 장면 //////////////////////////////////////////////////////////////////

// 게임
void SceneMgr::ChangeSceneToGame()
{
	if (!m_GameScene) return;

	bool isNot = false;
	for (int i = 0; i < 4; ++i)
	{
		if (m_SkillSelectScene->m_SelectedIndex[i] == 6)
		{
			isNot = true;
		}
	}

	if (isNot) return;
	
	// 사운드 ///////////////////////////////////////////////////////////
	SoundManager::GetInstance()->Stop(ENUM_SOUND::MAIN_SOUND);
	SoundManager::GetInstance()->Stop(ENUM_SOUND::WIN_SOUND);
	SoundManager::GetInstance()->Stop(ENUM_SOUND::LOSE_SOUND);
	SoundManager::GetInstance()->Stop(ENUM_SOUND::SKILLPAGE_SOUND);
	SoundManager::GetInstance()->Play(ENUM_SOUND::GAME_SOUND);
	// 사운드 ///////////////////////////////////////////////////////////

#ifdef _DEBUG
	GameInput::ChagneMoveMode();
	// GameInput::ChagneDragMode();
#else
	GameInput::ChagneMoveMode();
#endif // DEBUG

	PostprocessingSetting::ActiveBloom();
	PostprocessingSetting::ActiveTonecurve();

	m_SkillSelectScene->FinishSkillSelect(); // Skill Select에서 설정된 스킬 연결
	m_pCurrScene = m_GameScene;
	m_GameScene->Init(); //테스트
	m_CurrSceneType = ENUM_SCENE::SCENE_GAME;
}

// 메인
void SceneMgr::ChangeSceneToMain()
{
	if (!m_MainScene) return;

	// 사운드 ///////////////////////////////////////////////////////////
	SoundManager::GetInstance()->Stop(ENUM_SOUND::WIN_SOUND);
	SoundManager::GetInstance()->Stop(ENUM_SOUND::LOSE_SOUND);
	SoundManager::GetInstance()->Stop(ENUM_SOUND::SKILLPAGE_SOUND);
	SoundManager::GetInstance()->Stop(ENUM_SOUND::GAME_SOUND);
	SoundManager::GetInstance()->Play(ENUM_SOUND::MAIN_SOUND);
	// 사운드 ///////////////////////////////////////////////////////////

	PostprocessingSetting::DeactiveBloom();
	PostprocessingSetting::DeactiveTonecurve();

	GameInput::ChagneDragMode();
	m_pCurrScene = m_MainScene;
	m_CurrSceneType = ENUM_SCENE::SCENE_MAIN;
}

// 스킬 선택
void SceneMgr::ChangeSceneToSkillSelect()
{
	if (!m_SkillSelectScene) return;

	// 사운드 ///////////////////////////////////////////////////////////
	SoundManager::GetInstance()->Stop(ENUM_SOUND::WIN_SOUND);
	SoundManager::GetInstance()->Stop(ENUM_SOUND::LOSE_SOUND);
	SoundManager::GetInstance()->Stop(ENUM_SOUND::GAME_SOUND);
	SoundManager::GetInstance()->Stop(ENUM_SOUND::MAIN_SOUND);
	SoundManager::GetInstance()->Play(ENUM_SOUND::SKILLPAGE_SOUND);
	// 사운드 ///////////////////////////////////////////////////////////

	PostprocessingSetting::DeactiveBloom();
	PostprocessingSetting::DeactiveTonecurve();

	GameInput::ChagneDragMode();
	m_pCurrScene = m_SkillSelectScene;
	m_CurrSceneType = ENUM_SCENE::SCENE_SKILLSELECT;
}

// 이김
void SceneMgr::ChangeSceneToWin()
{
	if (!m_WinScene) return;

	// 사운드 ///////////////////////////////////////////////////////////
	SoundManager::GetInstance()->Stop(ENUM_SOUND::LOSE_SOUND);
	SoundManager::GetInstance()->Stop(ENUM_SOUND::GAME_SOUND);
	SoundManager::GetInstance()->Stop(ENUM_SOUND::MAIN_SOUND);
	SoundManager::GetInstance()->Stop(ENUM_SOUND::SKILLPAGE_SOUND);
	SoundManager::GetInstance()->Play(ENUM_SOUND::WIN_SOUND);
	// 사운드 ///////////////////////////////////////////////////////////

	PostprocessingSetting::DeactiveBloom();
	PostprocessingSetting::DeactiveTonecurve();

	GameInput::ChagneDragMode();
	m_pCurrScene = m_WinScene;
	m_CurrSceneType = ENUM_SCENE::SCENE_WIN;
}

// 짐
void SceneMgr::ChangeSceneToLose()
{
	if (!m_LoseScene) return;

	// 사운드 ///////////////////////////////////////////////////////////
	SoundManager::GetInstance()->Stop(ENUM_SOUND::GAME_SOUND);
	SoundManager::GetInstance()->Stop(ENUM_SOUND::MAIN_SOUND);
	SoundManager::GetInstance()->Stop(ENUM_SOUND::SKILLPAGE_SOUND);
	SoundManager::GetInstance()->Stop(ENUM_SOUND::WIN_SOUND);
	SoundManager::GetInstance()->Play(ENUM_SOUND::LOSE_SOUND);
	// 사운드 ///////////////////////////////////////////////////////////

	PostprocessingSetting::DeactiveBloom();
	PostprocessingSetting::DeactiveTonecurve();

	GameInput::ChagneDragMode();
	m_pCurrScene = m_LoseScene;
	m_CurrSceneType = ENUM_SCENE::SCENE_LOSE;
}

// 장면 //////////////////////////////////////////////////////////////////

bool SceneMgr::IsGameScene() const
{
	return m_CurrSceneType == ENUM_SCENE::SCENE_GAME;
}

bool SceneMgr::IsMainScene() const
{
	return m_CurrSceneType == ENUM_SCENE::SCENE_MAIN;
}

bool SceneMgr::IsSkillSelectScene() const
{
	return m_CurrSceneType == ENUM_SCENE::SCENE_SKILLSELECT;
}

bool SceneMgr::IsWinScene() const
{
	return m_CurrSceneType == ENUM_SCENE::SCENE_WIN;

}

bool SceneMgr::IsLoseScene() const
{
	return m_CurrSceneType == ENUM_SCENE::SCENE_LOSE;

}
