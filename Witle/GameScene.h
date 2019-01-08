#pragma once
#include "Scene.h"


//���� ��ü�� ������ ���̴����� �Ѱ��ֱ� ���� ����ü(��� ����)�̴�.
struct CB_OBJECT_INFO // b1
{
	XMFLOAT4 fColor{1.F , 1.F, 1.F, 1.F};
};
 
class GameObject;

class GameScene
	: public CScene
{
	POINT m_ptOldCursorPos;

	bool SaveData();

protected:
	//�׷��� ��Ʈ �ñ׳��ĸ� �����Ѵ�.
	virtual ComPtr<ID3D12RootSignature> CreateGraphicsRootSignature(ID3D12Device *pd3dDevice) override; 

	virtual void Update(float ElapsedTime) override;


public:
	GameScene();
	virtual ~GameScene();

	virtual bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) override;
	virtual bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam, float ElapsedTime) override;

	virtual void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList) override;
	virtual void ReleaseObjects() override;

	virtual bool ProcessInput(HWND hWnd, POINT OldCursor, float ElapsedTime) override;
	virtual bool ProcessMouseWheel(HWND hWnd, short WheelData, float ElapsedTime) override;

	virtual void OnprepareRender();
	virtual void AnimateObjects(float fTimeElapsed) override;
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList) override;

	virtual void ReleaseUploadBuffers() override;

protected:
	GameObject* m_GameObject; 
	GameObject* m_Camera;

	//Camera *m_Camera{ nullptr };

	//CPlayer	*m_Player{ nullptr };
	//
	//CMesh	* m_Mesh{ nullptr };
	// 
	///////////////////////////////////////////////////////////////// ����
	//HeightMapTerrain *m_pHeightMapTerrain{ nullptr };
	//SkyBox *m_SkyBox{ nullptr };
};