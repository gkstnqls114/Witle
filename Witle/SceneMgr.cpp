#include "stdafx.h"
#include "GameScene.h"
#include "MainScene.h"
#include "SkillSelectScene.h"
#include "WinScene.h"
#include "LoseScene.h"
#include "SceneMgr.h"

static SceneMgr* m_Instace{ nullptr };

SceneMgr::SceneMgr()
{
	m_GameScene = new GameScene;
	m_MainScene = new MainScene;
	m_SkillSelectScene = new SkillSelectScene;
	m_WinScene = new WinScene;
	m_LoseScene = new LoseScene;

	ChangeSceneToMain();
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
}

void SceneMgr::BuildHeap(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
{
	GameScene::CreateCbvSrvDescriptorHeaps(pd3dDevice, pd3dCommandList);
}

void SceneMgr::ChangeSceneToGame()
{
	m_pCurrScene = m_GameScene;
	m_CurrSceneType = ENUM_SCENE::SCENE_GAME;
}

void SceneMgr::ChangeSceneToMain()
{
	m_pCurrScene = m_MainScene;
	m_CurrSceneType = ENUM_SCENE::SCENE_MAIN;
}

void SceneMgr::ChangeSceneToSkillSelect()
{
	m_pCurrScene = m_SkillSelectScene;
	m_CurrSceneType = ENUM_SCENE::SCENE_SKILLSELECT;
}

void SceneMgr::ChangeSceneToWin()
{
	m_pCurrScene = m_WinScene;
	m_CurrSceneType = ENUM_SCENE::SCENE_WIN;
}

void SceneMgr::ChangeSceneToLose()
{
	m_pCurrScene = m_LoseScene;
	m_CurrSceneType = ENUM_SCENE::SCENE_LOSE;
}

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
