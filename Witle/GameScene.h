#pragma once
#include "Scene.h"

//// Skill ////////////////
class Sniping;
class WideareaMagic;
//// Skill ////////////////

//// Objects ///////////////
class Player;
class Widget;

//// Objects ///////////////

class Monster;
class SkyBox;
class Shader;
class CLoadedModelInfo;

class CameraObject;
class Terrain;
class GameObject;
class LoadObject;
class SkinnedShader;

class QuadtreeTerrain;

struct LIGHTS;
struct MATERIAL;
struct MATERIALS;

class AimPoint;
class CylinderMesh;
class LineSphere;

class AltarSphere;
class UI2DImage;

class GameScene
	: public Scene
{
private:
	static ID3D12DescriptorHeap			*m_pd3dCbvSrvDescriptorHeap;

	static D3D12_CPU_DESCRIPTOR_HANDLE	m_d3dCbvCPUDescriptorStartHandle;
	static D3D12_GPU_DESCRIPTOR_HANDLE	m_d3dCbvGPUDescriptorStartHandle;
	static D3D12_CPU_DESCRIPTOR_HANDLE	m_d3dSrvCPUDescriptorStartHandle;
	static D3D12_GPU_DESCRIPTOR_HANDLE	m_d3dSrvGPUDescriptorStartHandle;

	static D3D12_CPU_DESCRIPTOR_HANDLE	m_d3dCbvCPUDescriptorNextHandle;
	static D3D12_GPU_DESCRIPTOR_HANDLE	m_d3dCbvGPUDescriptorNextHandle;
	static D3D12_CPU_DESCRIPTOR_HANDLE	m_d3dSrvCPUDescriptorNextHandle;
	static D3D12_GPU_DESCRIPTOR_HANDLE	m_d3dSrvGPUDescriptorNextHandle;

protected:
	static void CreateCbvSrvDescriptorHeaps(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, int nConstantBufferViews, int nShaderResourceViews);

	static D3D12_GPU_DESCRIPTOR_HANDLE CreateConstantBufferViews(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, int nConstantBufferViews, ID3D12Resource *pd3dConstantBuffers, UINT nStride);
	static D3D12_GPU_DESCRIPTOR_HANDLE CreateShaderResourceViews(ID3D12Device *pd3dDevice, Texture *pTexture, UINT nRootParameter, bool bAutoIncrement);

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUCbvDescriptorStartHandle() { return(m_d3dCbvCPUDescriptorStartHandle); }
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUCbvDescriptorStartHandle() { return(m_d3dCbvGPUDescriptorStartHandle); }
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUSrvDescriptorStartHandle() { return(m_d3dSrvCPUDescriptorStartHandle); }
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUSrvDescriptorStartHandle() { return(m_d3dSrvGPUDescriptorStartHandle); }

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUCbvDescriptorNextHandle() { return(m_d3dCbvCPUDescriptorNextHandle); }
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUCbvDescriptorNextHandle() { return(m_d3dCbvGPUDescriptorNextHandle); }
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUSrvDescriptorNextHandle() { return(m_d3dSrvCPUDescriptorNextHandle); }
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUSrvDescriptorNextHandle() { return(m_d3dSrvGPUDescriptorNextHandle); }

protected:
	void UpdateCollision(const BoundingOrientedBox& AlreadyPlayerBBox, float fElapsedTime);
	void UpdateCollision(float fElapsedTime);
	void BuildLightsAndMaterials(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	void RenderShadowMap(ID3D12GraphicsCommandList *pd3dCommandList);

public:
	GameScene();
	virtual ~GameScene();

	virtual bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) override;
	virtual bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam, float ElapsedTime) override;

	virtual void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList) override;
	virtual void ReleaseObjects() override;

	virtual bool ProcessInput(HWND hWnd, float ElapsedTime) override;

	virtual void AnimateObjects(float fTimeElapsed) override;
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, bool isGBuffers) override;

	virtual void ReleaseUploadBuffers() override;

	virtual void UpdatePhysics(float ElapsedTime) override; // 정확히는 가속도 변경을 실행한다.
	virtual void Update(float ElapsedTime) override;
	virtual void LastUpdate(float ElapsedTime) override;

	void TESTSetRootDescriptor(ID3D12GraphicsCommandList *pd3dCommandList);

protected:
	bool					m_isSkyMode{ false };

	AimPoint*				m_AimPoint{ nullptr };

	int*					m_PlayerTerrainIndex{ nullptr };
	Sniping*				m_Sniping{ nullptr };
	WideareaMagic*          m_WideareaMagic{ nullptr };
	Player*                 m_pPlayer{ nullptr };
	
	Monster* 				m_AltarMonster{ nullptr };
	 
	Terrain*                m_Terrain{ nullptr };
	CameraObject*           m_pMainCamera{ nullptr }; // 실제로 화면에 그려지는 카메라

	CameraObject*           m_pSkyCameraObj{ nullptr };
	Camera*					m_pSkyCamera{ nullptr };
	//
	SkyBox*					m_SkyBox{ nullptr };

	int						m_TestMonsterCount{ 100 };
	Monster**				m_TestMonster{ nullptr };

	AltarSphere*			m_AltarSphere[5];

#ifdef CHECK_SUBVIEWS
	CameraObject*			m_lookAboveCamera{ nullptr };
#endif
	//
   //////////////////////////////////////  조명
	ID3D12Resource				*m_pd3dcbLights{ nullptr };
	LIGHTS						*m_pcbMappedLights{ nullptr };

	MATERIAL					*m_pMaterials{ nullptr };

	ID3D12Resource				*m_pd3dcbMaterials{ nullptr };
	MATERIAL					*m_pcbMappedMaterials{ nullptr };
	//////////////////////////////////////  조명

	QuadtreeTerrain			    *m_pQuadtreeTerrain{ nullptr };

private:
	GameObject* m_TESTGameObject{ nullptr };

	POINT m_UIMapSize{ 200, 200 };
	UI2DImage* m_SampleUIMap{ nullptr }; //맵
	UI2DImage* m_UIAltar_1{ nullptr }; // uimap 에서 제단 위치
	UI2DImage* m_UIAltar_2{ nullptr }; // uimap 에서 제단 위치
	UI2DImage* m_UIAltar_3{ nullptr }; // uimap 에서 제단 위치
	UI2DImage* m_UIAltar_4{ nullptr }; // uimap 에서 제단 위치
	UI2DImage* m_UIAltar_5{ nullptr }; // uimap 에서 제단 위치
	UI2DImage* m_UIPlayer{ nullptr }; // uimap 에서 플레이어 위치

	UI2DImage* m_SampleUISkill1{ nullptr }; // 스킬 1
	UI2DImage* m_SampleUISkill2{ nullptr }; // 스킬 2
	UI2DImage* m_SampleUISkill3{ nullptr }; // 스킬 3
	UI2DImage* m_SampleUISkill4{ nullptr }; // 스킬 4
};