#pragma once
#include "Scene.h"


class MyDescriptorHeap;
class CameraObject;
class Player;
class Terrain;
class GameObject;

class QuadTreeTerrainMesh;

struct LIGHTS;
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
	ParameterForm*          m_parameterForm{ nullptr };
	

	float PiekcingDist;
	XMFLOAT3*				m_PickingColors{ nullptr };
	GameObject**			m_PickingTESTMeshs{ nullptr };
	int						m_numPickingTESTMeshs{ 100 };

	Player*                 m_GameObject{ nullptr };
	Terrain*                m_Terrain{ nullptr };
	MyDescriptorHeap*       m_TerrainHeap{ nullptr };
	CameraObject*           m_Camera{ nullptr };

#ifdef CHECK_ANOTHER_CAMERA
	CameraObject*			m_lookAboveCamera{ nullptr };
#endif

	XMFLOAT4X4              matrix{ Matrix4x4::Identity() };

	//////////////////////////////////////  ����

	ID3D12Resource				*m_pd3dcbLights{ nullptr };
	LIGHTS						*m_pcbMappedLights{ nullptr };

	MATERIAL					*m_pMaterials{ nullptr };

	ID3D12Resource				*m_pd3dcbMaterials{ nullptr };
	MATERIAL					*m_pcbMappedMaterials{ nullptr };
	//////////////////////////////////////  ����

	GameObject *m_gameobject;
	QuadTreeTerrainMesh         *m_TESTQuadTree{ nullptr };
};