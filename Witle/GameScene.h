#pragma once
#include "Scene.h"


//게임 객체의 정보를 셰이더에게 넘겨주기 위한 구조체(상수 버퍼)이다.
struct CB_OBJECT_INFO // b1
{
	XMFLOAT4 fColor{1.F , 1.F, 1.F, 1.F};
};
 
class GameObject;

class GameScene
	: public Scene
{

	POINT m_ptOldCursorPos;

	bool SaveData();

protected:
	//그래픽 루트 시그너쳐를 생성한다.
	virtual ComPtr<ID3D12RootSignature> CreateGraphicsRootSignature(ID3D12Device *pd3dDevice) override; 

public:
	GameScene();
	virtual ~GameScene();

	virtual bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) override;
	virtual bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam, float ElapsedTime) override;

	virtual void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList) override;
	virtual void ReleaseObjects() override;

	virtual bool ProcessInput(HWND hWnd, POINT OldCursor, float ElapsedTime) override; 
	 
	virtual void AnimateObjects(float fTimeElapsed) override;
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList) override;

	virtual void ReleaseUploadBuffers() override;

	virtual void Update(float ElapsedTime) override;
	virtual void LastUpdate() override;

protected:
	GameObject* m_GameObject; 
	GameObject* m_Camera;

	//Camera *m_Camera{ nullptr };

	//CPlayer	*m_Player{ nullptr };
	//
	//CMesh	* m_Mesh{ nullptr };
	// 
	///////////////////////////////////////////////////////////////// 지형
	//HeightMapTerrain *m_pHeightMapTerrain{ nullptr };
	//SkyBox *m_SkyBox{ nullptr };
};