#pragma once
#include "Scene.h"

//// Skill ////////////////
class PlayerSkillMgr;
class Sniping;
class WideareaMagic;
//// Skill ////////////////

//// Objects ///////////////
class Player;
class Widget;
class Monster;
class SkyBox;
class CameraObject;
class Terrain;
class QuadtreeTerrain;
class AltarSphere;
//// Objects ///////////////

//// Componenet ///////////////
class Camera;
class CylinderMesh;
class LineSphere;
class HitEffect;
//// Componenet ///////////////

//// UI ///////////////
class UI2DImage;
class AimPoint;
//// UI ///////////////

//// ETC... ///////////////
class Shader;
class CLoadedModelInfo;
class GameObject;
class LoadObject;
class SkinnedShader;
class MyDescriptorHeap;
class Texture;

struct LIGHTS;
struct MATERIAL;
struct MATERIALS;
//// ETC... ///////////////


class GameScene
	: public Scene
{
private:
	static UINT m_ShadowmapCount;
	static std::list<Texture* > m_pConnectedTextureList;
	static ID3D12DescriptorHeap			*m_pd3dCbvSrUavDescriptorHeap; 
	static UINT			m_TextureCount;

private:
	static D3D12_CPU_DESCRIPTOR_HANDLE	m_hCPUShadowmap;
	static D3D12_GPU_DESCRIPTOR_HANDLE	m_hGPUShadowmap;
	static D3D12_CPU_DESCRIPTOR_HANDLE	m_hCPUPlayerShadowmap;
	static D3D12_GPU_DESCRIPTOR_HANDLE	m_hGPUPlayerShadowmap;

	static D3D12_CPU_DESCRIPTOR_HANDLE	m_CbvCPUDescriptorStartHandle;
	static D3D12_GPU_DESCRIPTOR_HANDLE	m_CbvGPUDescriptorStartHandle;
	static D3D12_CPU_DESCRIPTOR_HANDLE	m_SrvCPUDescriptorStartHandle;
	static D3D12_GPU_DESCRIPTOR_HANDLE	m_SrvGPUDescriptorStartHandle;
	static D3D12_CPU_DESCRIPTOR_HANDLE	m_UavCPUDescriptorStartHandle;
	static D3D12_GPU_DESCRIPTOR_HANDLE	m_UavGPUDescriptorStartHandle;

	static D3D12_CPU_DESCRIPTOR_HANDLE	m_CbvCPUDescriptorNextHandle;
	static D3D12_GPU_DESCRIPTOR_HANDLE	m_CbvGPUDescriptorNextHandle;
	static D3D12_CPU_DESCRIPTOR_HANDLE	m_SrvCPUDescriptorNextHandle;
	static D3D12_GPU_DESCRIPTOR_HANDLE	m_SrvGPUDescriptorNextHandle;
	static D3D12_CPU_DESCRIPTOR_HANDLE	m_UavCPUDescriptorNextHandle;
	static D3D12_GPU_DESCRIPTOR_HANDLE	m_UavGPUDescriptorNextHandle;

public: 
	static void ConnectTexture(Texture* pTexture);

	static void CreateCbvSrvDescriptorHeaps(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	static void CreateSrvDescriptorHeapsForPlayerShadowmap(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12Resource* pShadowmap);
	static void CreateSrvDescriptorHeapsForShadowmap(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12Resource* pShadowmap);

protected:
	static void CreateShaderResourceViewsForTextureBase(ID3D12Device * pd3dDevice, Texture * pTexture);

	static D3D12_GPU_DESCRIPTOR_HANDLE CreateConstantBufferViews(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, int nConstantBufferViews, ID3D12Resource *pd3dConstantBuffers, UINT nStride);

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUCbvDescriptorStartHandle() { return(m_CbvCPUDescriptorStartHandle); }
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUCbvDescriptorStartHandle() { return(m_CbvGPUDescriptorStartHandle); }
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUSrvDescriptorStartHandle() { return(m_SrvCPUDescriptorStartHandle); }
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUSrvDescriptorStartHandle() { return(m_SrvGPUDescriptorStartHandle); }

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUCbvDescriptorNextHandle() { return(m_CbvCPUDescriptorNextHandle); }
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUCbvDescriptorNextHandle() { return(m_CbvGPUDescriptorNextHandle); }
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUSrvDescriptorNextHandle() { return(m_SrvCPUDescriptorNextHandle); }
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUSrvDescriptorNextHandle() { return(m_SrvGPUDescriptorNextHandle); }

protected:
	void UpdateCollision(float fElapsedTime);
	void BuildLightsAndMaterials(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	
public:
	GameScene();
	virtual ~GameScene();

	virtual bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) override;
	virtual bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam, float ElapsedTime) override;

	virtual void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList) override;
	virtual void ReleaseObjects() override;

	virtual bool ProcessInput(HWND hWnd, float ElapsedTime) override;

	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, bool isGBuffers) override;
	virtual void RenderForShadow(ID3D12GraphicsCommandList *pd3dCommandList) override;
	virtual void RenderForPlayerShadow(ID3D12GraphicsCommandList *pd3dCommandList) override;

	virtual void ReleaseUploadBuffers() override;

	virtual void UpdatePhysics(float ElapsedTime) override;
	virtual void Update(float ElapsedTime) override;
	virtual void AnimateObjects(float fTimeElapsed) override;
	virtual void LastUpdate(float ElapsedTime) override;

	QuadtreeTerrain* GetQuadtreeTerrain() const { return m_pQuadtreeTerrain; }

public:
	
	// 플레이어 관련 ////////////////////////////
	
	Player*                 m_pPlayer        { nullptr }; 
	Sniping*				m_Sniping        { nullptr }; 
	
	// 플레이어 관련 ////////////////////////////c


	// 몬스터 관련 ////////////////////////////

	int						m_TestMonsterCount{ 10 }; //원래 107마리..
	Monster**				m_TestMonster{ nullptr };
	Monster*				m_AltarMonster{ nullptr };
	
	// 몬스터 관련 ////////////////////////////

	bool					m_isSkyMode{ false };

	AimPoint*				m_AimPoint{ nullptr };
	 
	Terrain*                m_Terrain{ nullptr };
	CameraObject*           m_pMainCamera{ nullptr }; // 실제로 화면에 그려지는 카메라

	CameraObject*           m_pSkyCameraObj{ nullptr };
	Camera*					m_pSkyCamera{ nullptr };
	
	SkyBox*					m_SkyBox{ nullptr };
	  
	Monster*				m_Dragon{ nullptr };

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
	UI2DImage* m_UIPlayer{ nullptr };  // uimap 에서 플레이어 위치

	UI2DImage* m_SampleUISkill1{ nullptr }; // 스킬 1
	UI2DImage* m_SampleUISkill2{ nullptr }; // 스킬 2
	UI2DImage* m_SampleUISkill3{ nullptr }; // 스킬 3
	UI2DImage* m_SampleUISkill4{ nullptr }; // 스킬 4
};