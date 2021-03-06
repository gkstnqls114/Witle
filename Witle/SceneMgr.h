#pragma once

class Scene;
class GameScene;
class MainScene;
class SkillSelectScene;
class LoseScene;
class WinScene;
class TestScene;

class SceneMgr
{
	static SceneMgr* m_Instace;
	
	std::list<Scene*> m_SceneList;

	Scene* m_pCurrScene{ nullptr };
	ENUM_SCENE m_CurrSceneType{ ENUM_SCENE::SCENE_NONE };

	// 사용하는 장면 목록들 //////////////////
	
	GameScene* m_GameScene { nullptr };
	MainScene* m_MainScene { nullptr };
	SkillSelectScene* m_SkillSelectScene{ nullptr };
	LoseScene* m_LoseScene { nullptr };
	WinScene* m_WinScene { nullptr };

	// 사용하는 장면 목록들 //////////////////


	// 테스트를 위해 사용할 장면
	TestScene* m_TestScene{ nullptr };

	SceneMgr();
	~SceneMgr();

public:
	static SceneMgr* GetInstance()
	{
		if (m_Instace == nullptr)
		{
			m_Instace = new SceneMgr;
		}
		return m_Instace;
	}

	static void ReleaseInstance()
	{
		if (m_Instace)
		{
			m_Instace->ReleaseObjects();
			delete m_Instace;
			m_Instace = nullptr;
		}
	}

	void ReleaseObjects();
	void ReleaseUploadBuffers();
	void BuildObjects(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	void BuildHeap(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);

	Scene* GetCurrScene() const { return m_pCurrScene; }

	// 장면을 전환하는 함수들

	void ChangeSceneToGame();
	void ChangeSceneToMain();
	void ChangeSceneToSkillSelect();
	void ChangeSceneToWin();
	void ChangeSceneToLose();
	void ChangeSceneToTest();

	// 장면을 전환하는 함수들


	// 현재 장면이 해당 장면이 맞는지 bool 로 확인하는 함수

	bool IsGameScene() const;
	bool IsMainScene() const;
	bool IsSkillSelectScene() const;
	bool IsWinScene() const;
	bool IsLoseScene() const;
	bool IsTestScene() const;

	// 현재 장면이 해당 장면이 맞는지 bool 로 확인하는 함수

};