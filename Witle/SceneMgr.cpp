#include "stdafx.h"
#include "GameScene.h"
#include "MainScene.h"
#include "SkillSelectScene.h"
#include "SceneMgr.h"

static SceneMgr* m_Instace{ nullptr };

SceneMgr::SceneMgr()
{
	m_GameScene = new GameScene;
	m_MainScene = new MainScene;
	m_SkillSelectScene = new SkillSelectScene;
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
}

void SceneMgr::ReleaseUploadBuffers()
{
	if (m_GameScene) m_GameScene->ReleaseUploadBuffers();  

	if (m_MainScene) m_MainScene->ReleaseUploadBuffers();  
}

void SceneMgr::BuildObjects(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
	m_GameScene->BuildObjects(pd3dDevice, pd3dCommandList);
	m_MainScene->BuildObjects(pd3dDevice, pd3dCommandList);
}

void SceneMgr::ChangeSceneToGame()
{
	m_pCurrScene = m_GameScene;
}

void SceneMgr::ChangeSceneToMain()
{
	m_pCurrScene = m_MainScene;
}

void SceneMgr::ChangeSceneToSkillSelect()
{
}

bool SceneMgr::IsGameScene() const
{
	return m_pCurrScene == m_GameScene;
}

bool SceneMgr::IsMainScene() const
{
	return m_pCurrScene == m_MainScene;
}

bool SceneMgr::IsSkillSelectScene() const
{
	return m_pCurrScene == m_SkillSelectScene;
}
