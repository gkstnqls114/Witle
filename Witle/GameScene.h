#pragma once
#include "Scene.h"


//���� ��ü�� ������ ���̴����� �Ѱ��ֱ� ���� ����ü(��� ����)�̴�.
struct CB_OBJECT_INFO // b1
{
	XMFLOAT4 fColor{1.F , 1.F, 1.F, 1.F};
};
 
class MyDescriptorHeap;
class CameraObject;
class Player;
class Terrain;
struct LIGHTS;
struct LIGHT;
struct MATERIAL;
struct MATERIALS;

class GameScene
	: public Scene
{

	bool SaveData();

protected:
	//�׷��� ��Ʈ �ñ׳��ĸ� �����Ѵ�.
	virtual ID3D12RootSignature* CreateGraphicsRootSignature(ID3D12Device *pd3dDevice) override; 

	void BuildLightsAndMaterials(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);

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
	virtual void LastUpdate(float ElapsedTime) override;

protected:
	RootResource* m_RootResource{ nullptr };

	Player* m_GameObject{ nullptr };
	Terrain* m_Terrain{ nullptr };
	MyDescriptorHeap* m_TerrainHeap{ nullptr };
	CameraObject* m_Camera{ nullptr };

	XMFLOAT4X4 matrix{ Matrix4x4::Identity() };

	//////////////////////////////////////  ����
	LIGHTS						*m_pLights = NULL;

	ID3D12Resource				*m_pd3dcbLights = NULL;
	LIGHTS						*m_pcbMappedLights = NULL;

	MATERIALS					*m_pMaterials = NULL;

	ID3D12Resource				*m_pd3dcbMaterials = NULL;
	MATERIAL					*m_pcbMappedMaterials = NULL;
	//////////////////////////////////////  ����

};