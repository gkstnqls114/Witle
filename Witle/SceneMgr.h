#pragma once

class Scene;
class GameScene;
class MainScene;
class SkillSelectScene;
class LoseScene;
class WinScene;

class SceneMgr
{
	static SceneMgr* m_Instace;
	
	Scene* m_pCurrScene{ nullptr };
	ENUM_SCENE m_CurrSceneType{ ENUM_SCENE::SCENE_NONE };

	// ����ϴ� ��� ��ϵ� //////////////////
	
	GameScene* m_GameScene { nullptr };
	MainScene* m_MainScene { nullptr };
	SkillSelectScene* m_SkillSelectScene{ nullptr };
	LoseScene* m_LoseScene { nullptr };
	WinScene* m_WinScene { nullptr };

	// ����ϴ� ��� ��ϵ� //////////////////

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

	// ����� ��ȯ�ϴ� �Լ���

	void ChangeSceneToGame();
	void ChangeSceneToMain();
	void ChangeSceneToSkillSelect();
	void ChangeSceneToWin();
	void ChangeSceneToLose();

	// ����� ��ȯ�ϴ� �Լ���


	// ���� ����� �ش� ����� �´��� bool �� Ȯ���ϴ� �Լ�

	bool IsGameScene() const;
	bool IsMainScene() const;
	bool IsSkillSelectScene() const;
	bool IsWinScene() const;
	bool IsLoseScene() const;

	// ���� ����� �ش� ����� �´��� bool �� Ȯ���ϴ� �Լ�

};