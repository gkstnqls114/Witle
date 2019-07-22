#pragma once

class Scene;
class GameScene;
class MainScene;
class SkillSelectScene;

class SceneMgr
{
	static SceneMgr* m_Instace;
	
	Scene* m_pCurrScene{ nullptr };

	// ����ϴ� ��� ��ϵ� //////////////////
	
	GameScene* m_GameScene{ nullptr };
	MainScene* m_MainScene{ nullptr };
	SkillSelectScene* m_SkillSelectScene{ nullptr };

	// ����ϴ� ��� ��ϵ� //////////////////

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

	// ����� ��ȯ�ϴ� �Լ���

	void ChangeSceneToGame();
	void ChangeSceneToMain();
	void ChangeSceneToSkillSelect();

	// ����� ��ȯ�ϴ� �Լ���


	// ���� ����� �ش� ����� �´��� bool �� Ȯ���ϴ� �Լ�

	bool IsGameScene() const;
	bool IsMainScene() const;
	bool IsSkillSelectScene() const;

	// ���� ����� �ش� ����� �´��� bool �� Ȯ���ϴ� �Լ�

};