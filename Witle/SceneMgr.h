#pragma once

class Scene;
class GameScene;
class MainScene;
class SkillSelectScene;

class SceneMgr
{
	static SceneMgr* m_Instace;
	
	Scene* m_pCurrScene{ nullptr };

	// 사용하는 장면 목록들 //////////////////
	
	GameScene* m_GameScene{ nullptr };
	MainScene* m_MainScene{ nullptr };
	SkillSelectScene* m_SkillSelectScene{ nullptr };

	// 사용하는 장면 목록들 //////////////////

public:
	SceneMgr();
	~SceneMgr();

	static SceneMgr* GetInstacne()
	{
		if (m_Instace == nullptr)
		{
			m_Instace = new SceneMgr;
		}
		return m_Instace;
	}

	void ReleaseObjects();
	void ReleaseUploadBuffers();
	void BuildObjects(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);

	Scene* GetCurrScene() const { return m_pCurrScene; }

	// 장면을 전환하는 함수들

	void ChangeSceneToGame();
	void ChangeSceneToMain();
	void ChangeSceneToSkillSelect();

	// 장면을 전환하는 함수들


	// 현재 장면이 해당 장면이 맞는지 bool 로 확인하는 함수

	bool IsGameScene() const;
	bool IsMainScene() const;
	bool IsSkillSelectScene() const;

	// 현재 장면이 해당 장면이 맞는지 bool 로 확인하는 함수

};