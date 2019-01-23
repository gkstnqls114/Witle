#pragma once
#include "MyDescriptorHeap.h"
#include "Scene.h"


//���� ��ü�� ������ ���̴����� �Ѱ��ֱ� ���� ����ü(��� ����)�̴�.
struct CB_OBJECT_INFO // b1
{
	XMFLOAT4 fColor{1.F , 1.F, 1.F, 1.F};
};
 
class GameObject;
class Player;
class Terrain;

class GameScene
	: public Scene
{

	bool SaveData();

protected:
	//�׷��� ��Ʈ �ñ׳��ĸ� �����Ѵ�.
	virtual ID3D12RootSignature* CreateGraphicsRootSignature(ID3D12Device *pd3dDevice) override; 

public:
	GameScene();
	virtual ~GameScene();

	virtual bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) override;
	virtual bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam, float ElapsedTime) override;

	virtual void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList) override;
	virtual void ReleaseObjects() override;

	virtual bool ProcessInput(HWND hWnd, float ElapsedTime) override; 
	 
	virtual void AnimateObjects(float fTimeElapsed) override;
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList) override;

	virtual void ReleaseUploadBuffers() override;

	virtual void Update(float ElapsedTime) override;
	virtual void LastUpdate() override;

protected:
	Player* m_GameObject;
	Terrain* m_Terrain;
	MyDescriptorHeap m_TerrainHeap;
	GameObject* m_Camera;

	XMFLOAT4X4 matrix = Matrix4x4::Identity();

	//Camera *m_Camera{ nullptr };

	//CPlayer	*m_Player{ nullptr };
	//
	//CMesh	* m_Mesh{ nullptr };
	// 
	///////////////////////////////////////////////////////////////// ����
	//HeightMapTerrain *m_pHeightMapTerrain{ nullptr };
	//SkyBox *m_SkyBox{ nullptr };
};