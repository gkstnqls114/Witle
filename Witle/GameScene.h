#pragma once
#include "Scene.h"


//게임 객체의 정보를 셰이더에게 넘겨주기 위한 구조체(상수 버퍼)이다.
struct CB_OBJECT_INFO // b1
{
	XMFLOAT4 fColor{1.F , 1.F, 1.F, 1.F};
};
 
class MyDescriptorHeap;
class CameraObject;
class Player;
class Terrain;
class GameObject;

class QuadTreeTerrainMesh;

struct LIGHTS;
struct LIGHT;
struct MATERIAL;
struct MATERIALS;

class GameScene
	: public Scene
{

	bool SaveData();

protected:
	//그래픽 루트 시그너쳐를 생성한다.
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
	ParameterForm*          m_parameterForm{ nullptr };

	Player*                 m_GameObject{ nullptr };
	Terrain*                m_Terrain{ nullptr };
	MyDescriptorHeap*       m_TerrainHeap{ nullptr };
	CameraObject*           m_Camera{ nullptr };

	XMFLOAT4X4              matrix{ Matrix4x4::Identity() };

	//////////////////////////////////////  조명
	LIGHTS						*m_pLights{ nullptr };

	ID3D12Resource				*m_pd3dcbLights{ nullptr };
	LIGHTS						*m_pcbMappedLights{ nullptr };

	MATERIALS					*m_pMaterials{ nullptr };

	ID3D12Resource				*m_pd3dcbMaterials{ nullptr };
	MATERIAL					*m_pcbMappedMaterials{ nullptr };
	//////////////////////////////////////  조명

	GameObject *m_gameobject;
	QuadTreeTerrainMesh         *m_TESTQuadTree{ nullptr };
};