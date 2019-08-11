#include "stdafx.h"
#include "GameScene.h"
#include "MainScene.h"
#include "SkillSelectScene.h"
#include "WinScene.h"
#include "LoseScene.h"
#include "GameInput.h"
#include "SceneMgr.h"

#include "SoundManager.h"

static SceneMgr* m_Instace{ nullptr };

SceneMgr::SceneMgr()
{
	m_GameScene = new GameScene;
	m_MainScene = new MainScene;
	m_SkillSelectScene = new SkillSelectScene;
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
	m_GameScene->BuildObjects(pd3dDevice, pd3dCommandList);
	m_MainScene->BuildObjects(pd3dDevice, pd3dCommandList);
	m_SkillSelectScene->BuildObjects(pd3dDevice, pd3dCommandList);
	m_WinScene->BuildObjects(pd3dDevice, pd3dCommandList);
	m_LoseScene->BuildObjects(pd3dDevice, pd3dCommandList);

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
	// 사운드 ///////////////////////////////////////////////////////////
	SoundManager::GetInstance()->Stop(ENUM_SOUND::MAIN_SOUND);
	SoundManager::GetInstance()->Stop(ENUM_SOUND::WIN_SOUND);
	SoundManager::GetInstance()->Stop(ENUM_SOUND::LOSE_SOUND);
	SoundManager::GetInstance()->Stop(ENUM_SOUND::SKILLPAGE_SOUND);
	SoundManager::GetInstance()->Play(ENUM_SOUND::GAME_SOUND);
	// 사운드 ///////////////////////////////////////////////////////////

	GameInput::ChagneMoveMode();
	m_SkillSelectScene->FinishSkillSelect(); // Skill Select에서 설정된 스킬 연결
	m_pCurrScene = m_GameScene;
	m_GameScene->Init(); //테스트
	m_CurrSceneType = ENUM_SCENE::SCENE_GAME;
}

// 메인
void SceneMgr::ChangeSceneToMain()
{
	// 사운드 ///////////////////////////////////////////////////////////
	SoundManager::GetInstance()->Stop(ENUM_SOUND::WIN_SOUND);
	SoundManager::GetInstance()->Stop(ENUM_SOUND::LOSE_SOUND);
	SoundManager::GetInstance()->Stop(ENUM_SOUND::SKILLPAGE_SOUND);
	SoundManager::GetInstance()->Stop(ENUM_SOUND::GAME_SOUND);
	SoundManager::GetInstance()->Play(ENUM_SOUND::MAIN_SOUND);
	// 사운드 ///////////////////////////////////////////////////////////

	GameInput::ChagneDragMode();
	m_pCurrScene = m_MainScene;
	m_CurrSceneType = ENUM_SCENE::SCENE_MAIN;
}

// 스킬 선택
void SceneMgr::ChangeSceneToSkillSelect()
{
	// 사운드 ///////////////////////////////////////////////////////////
	SoundManager::GetInstance()->Stop(ENUM_SOUND::WIN_SOUND);
	SoundManager::GetInstance()->Stop(ENUM_SOUND::LOSE_SOUND);
	SoundManager::GetInstance()->Stop(ENUM_SOUND::GAME_SOUND);
	SoundManager::GetInstance()->Stop(ENUM_SOUND::MAIN_SOUND);
	SoundManager::GetInstance()->Play(ENUM_SOUND::SKILLPAGE_SOUND);
	// 사운드 ///////////////////////////////////////////////////////////

	GameInput::ChagneDragMode();
	m_pCurrScene = m_SkillSelectScene;
	m_CurrSceneType = ENUM_SCENE::SCENE_SKILLSELECT;
}

// 이김
void SceneMgr::ChangeSceneToWin()
{
	// 사운드 ///////////////////////////////////////////////////////////
	SoundManager::GetInstance()->Stop(ENUM_SOUND::LOSE_SOUND);
	SoundManager::GetInstance()->Stop(ENUM_SOUND::GAME_SOUND);
	SoundManager::GetInstance()->Stop(ENUM_SOUND::MAIN_SOUND);
	SoundManager::GetInstance()->Stop(ENUM_SOUND::SKILLPAGE_SOUND);
	SoundManager::GetInstance()->Play(ENUM_SOUND::WIN_SOUND);
	// 사운드 ///////////////////////////////////////////////////////////

	GameInput::ChagneDragMode();
	m_pCurrScene = m_WinScene;
	m_CurrSceneType = ENUM_SCENE::SCENE_WIN;
}

// 짐
void SceneMgr::ChangeSceneToLose()
{
	// 사운드 ///////////////////////////////////////////////////////////
	SoundManager::GetInstance()->Stop(ENUM_SOUND::GAME_SOUND);
	SoundManager::GetInstance()->Stop(ENUM_SOUND::MAIN_SOUND);
	SoundManager::GetInstance()->Stop(ENUM_SOUND::SKILLPAGE_SOUND);
	SoundManager::GetInstance()->Stop(ENUM_SOUND::WIN_SOUND);
	SoundManager::GetInstance()->Play(ENUM_SOUND::LOSE_SOUND);
	// 사운드 ///////////////////////////////////////////////////////////

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
