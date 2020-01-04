#include "stdafx.h"

#include "SoundManager.h"

#include "GameScene.h"
#include "MainScene.h"
#include "SkillSelectScene.h"
#include "PostprocessingSetting.h"
#include "WinScene.h"
#include "LoseScene.h"
#include "GameInput.h"
#include "TestScene.h"

#include "SceneMgr.h"

SceneMgr* SceneMgr::m_Instace{ nullptr };

SceneMgr::SceneMgr()
{
	// �׽�Ʈ�� ���� �ּ� ó��
	//m_MainScene = new MainScene;
	//m_SkillSelectScene = new SkillSelectScene;
	//m_GameScene = new GameScene;
	//m_WinScene = new WinScene;
	//m_LoseScene = new LoseScene;

	m_TestScene = new TestScene;

	// ����, �ٿ��ֱ������ʰ� �ѹ��� Build, Release �ϱ����� list�� �ֽ��ϴ�.
	m_SceneList.push_back(m_MainScene);
	m_SceneList.push_back(m_SkillSelectScene);
	m_SceneList.push_back(m_GameScene);
	m_SceneList.push_back(m_WinScene);
	m_SceneList.push_back(m_LoseScene);
	m_SceneList.push_back(m_TestScene);
}

SceneMgr::~SceneMgr()
{
}

void SceneMgr::ReleaseObjects()
{
	for (auto& scene : m_SceneList)
	{
		if (scene != nullptr)
		{
			scene->ReleaseObjects();
			delete scene;
			scene = nullptr;
		}
	} 
}

void SceneMgr::ReleaseUploadBuffers()
{
	for (auto& scene : m_SceneList)
	{
		if (scene != nullptr)
		{
			scene->ReleaseUploadBuffers(); 
		}
	} 
}

void SceneMgr::BuildObjects(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
	for (auto& scene : m_SceneList)
	{
		if (scene != nullptr)
		{
			scene->BuildObjects(pd3dDevice, pd3dCommandList);
		}
	} 

	// �׽�Ʈ�� ���� �ּ� ó��
	// ChangeSceneToMain();
	ChangeSceneToTest();
}

void SceneMgr::BuildHeap(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
{
	GameScene::CreateCbvSrvDescriptorHeaps(pd3dDevice, pd3dCommandList);
}

// ��� //////////////////////////////////////////////////////////////////

// ����
void SceneMgr::ChangeSceneToGame()
{
	if (m_GameScene == nullptr)
	{
#ifdef _DEBUG
		std::cout << "m_GameScene is nullptr" << std::endl;
#endif // _DEBUG 
		return;
	} 

	bool isNot = false;
	for (int i = 0; i < 4; ++i)
	{
		if (m_SkillSelectScene->m_SelectedIndex[i] == 6)
		{
			isNot = true;
		}
	}

	if (isNot) return;
	
	// ���� ///////////////////////////////////////////////////////////
	SoundManager::GetInstance()->Stop(ENUM_SOUND::MAIN_SOUND);
	SoundManager::GetInstance()->Stop(ENUM_SOUND::WIN_SOUND);
	SoundManager::GetInstance()->Stop(ENUM_SOUND::LOSE_SOUND);
	SoundManager::GetInstance()->Stop(ENUM_SOUND::SKILLPAGE_SOUND);
	SoundManager::GetInstance()->Play(ENUM_SOUND::GAME_SOUND);
	// ���� ///////////////////////////////////////////////////////////

#ifdef _DEBUG
	GameInput::ChagneMoveMode();
	// GameInput::ChagneDragMode();
#else
	GameInput::ChagneMoveMode();
#endif // DEBUG

	PostprocessingSetting::ActiveBloom();
	PostprocessingSetting::ActiveTonecurve();

	m_SkillSelectScene->FinishSkillSelect(); // Skill Select���� ������ ��ų ����
	m_pCurrScene = m_GameScene;
	m_GameScene->Init(); //�׽�Ʈ
	m_CurrSceneType = ENUM_SCENE::SCENE_GAME;
}

// ����
void SceneMgr::ChangeSceneToMain()
{
	if (m_MainScene == nullptr)
	{
#ifdef _DEBUG
		std::cout << "m_MainScene is nullptr" << std::endl;
#endif // _DEBUG 
		return;
	} 

	// ���� ///////////////////////////////////////////////////////////
	SoundManager::GetInstance()->Stop(ENUM_SOUND::WIN_SOUND);
	SoundManager::GetInstance()->Stop(ENUM_SOUND::LOSE_SOUND);
	SoundManager::GetInstance()->Stop(ENUM_SOUND::SKILLPAGE_SOUND);
	SoundManager::GetInstance()->Stop(ENUM_SOUND::GAME_SOUND);
	SoundManager::GetInstance()->Play(ENUM_SOUND::MAIN_SOUND);
	// ���� ///////////////////////////////////////////////////////////

	PostprocessingSetting::DeactiveBloom();
	PostprocessingSetting::DeactiveTonecurve();

	GameInput::ChagneDragMode();
	m_pCurrScene = m_MainScene;
	m_CurrSceneType = ENUM_SCENE::SCENE_MAIN;
}

// ��ų ����
void SceneMgr::ChangeSceneToSkillSelect()
{
	if (m_SkillSelectScene == nullptr)
	{
#ifdef _DEBUG
		std::cout << "m_SkillSelectScene is nullptr" << std::endl;
#endif // _DEBUG 
		return;
	}

	// ���� ///////////////////////////////////////////////////////////
	SoundManager::GetInstance()->Stop(ENUM_SOUND::WIN_SOUND);
	SoundManager::GetInstance()->Stop(ENUM_SOUND::LOSE_SOUND);
	SoundManager::GetInstance()->Stop(ENUM_SOUND::GAME_SOUND);
	SoundManager::GetInstance()->Stop(ENUM_SOUND::MAIN_SOUND);
	SoundManager::GetInstance()->Play(ENUM_SOUND::SKILLPAGE_SOUND);
	// ���� ///////////////////////////////////////////////////////////

	PostprocessingSetting::DeactiveBloom();
	PostprocessingSetting::DeactiveTonecurve();

	GameInput::ChagneDragMode();
	m_pCurrScene = m_SkillSelectScene;
	m_CurrSceneType = ENUM_SCENE::SCENE_SKILLSELECT;
}

// �̱�
void SceneMgr::ChangeSceneToWin()
{
	if (!m_WinScene) return;

	// ���� ///////////////////////////////////////////////////////////
	SoundManager::GetInstance()->Stop(ENUM_SOUND::LOSE_SOUND);
	SoundManager::GetInstance()->Stop(ENUM_SOUND::GAME_SOUND);
	SoundManager::GetInstance()->Stop(ENUM_SOUND::MAIN_SOUND);
	SoundManager::GetInstance()->Stop(ENUM_SOUND::SKILLPAGE_SOUND);
	SoundManager::GetInstance()->Play(ENUM_SOUND::WIN_SOUND);
	// ���� ///////////////////////////////////////////////////////////

	PostprocessingSetting::DeactiveBloom();
	PostprocessingSetting::DeactiveTonecurve();

	GameInput::ChagneDragMode();
	m_pCurrScene = m_WinScene;
	m_CurrSceneType = ENUM_SCENE::SCENE_WIN;
}

// ��
void SceneMgr::ChangeSceneToLose()
{
	if (m_LoseScene == nullptr)
	{
#ifdef _DEBUG
		std::cout << "m_LoseScene is nullptr" << std::endl;
#endif // _DEBUG 
		return;
	}

	// ���� ///////////////////////////////////////////////////////////
	SoundManager::GetInstance()->Stop(ENUM_SOUND::GAME_SOUND);
	SoundManager::GetInstance()->Stop(ENUM_SOUND::MAIN_SOUND);
	SoundManager::GetInstance()->Stop(ENUM_SOUND::SKILLPAGE_SOUND);
	SoundManager::GetInstance()->Stop(ENUM_SOUND::WIN_SOUND);
	SoundManager::GetInstance()->Play(ENUM_SOUND::LOSE_SOUND);
	// ���� ///////////////////////////////////////////////////////////

	PostprocessingSetting::DeactiveBloom();
	PostprocessingSetting::DeactiveTonecurve();

	GameInput::ChagneDragMode();
	m_pCurrScene = m_LoseScene;
	m_CurrSceneType = ENUM_SCENE::SCENE_LOSE;
}

void SceneMgr::ChangeSceneToTest()
{
	if (m_TestScene == nullptr)
	{
#ifdef _DEBUG
		std::cout << "m_TestScene is nullptr" << std::endl;
#endif // _DEBUG 
		return;
	}

	GameInput::ChagneDragMode();
	m_pCurrScene = m_TestScene;
	m_CurrSceneType = ENUM_SCENE::SCENE_TEST;
}

// ��� //////////////////////////////////////////////////////////////////

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

bool SceneMgr::IsTestScene() const
{
	return m_CurrSceneType == ENUM_SCENE::SCENE_TEST;
}
