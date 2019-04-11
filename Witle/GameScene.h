#pragma once
#include "Scene.h"

class Player;
class CPlayer;
class Shader;
class MyReflexTree;
class ReflexTree;
class CLoadedModelInfo;

class CameraObject;
class Terrain;
class GameObject;
class LoadObject;
class SkinnedStandardShader;

class QuadTreeTerrainMesh;

struct LIGHTS;
struct MATERIAL;
struct MATERIALS;

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
	//그래픽 루트 시그너쳐를 생성한다.
	virtual ID3D12RootSignature* CreateGraphicsRootSignature(ID3D12Device *pd3dDevice) override; 

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
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList) override;

	virtual void ReleaseUploadBuffers() override;

	virtual void Update(float ElapsedTime) override;
	virtual void LastUpdate(float ElapsedTime) override;

	void TESTSetRootDescriptor(ID3D12GraphicsCommandList *pd3dCommandList);
	
protected:
	// fbx test
	Texture*				m_GameObjectDiffuse{ nullptr };
	Player*                 m_pPlayer{ nullptr };

	CLoadedModelInfo *m_pTreeModel{ nullptr };
	UINT					m_TreeCount{ 100 };
	Texture*				m_TreeDiffuse{ nullptr };
	MyReflexTree**			m_Trees{ nullptr }; 
	// fbx test

	Terrain*                m_Terrain{ nullptr };
	CameraObject*           m_Camera{ nullptr };

#ifdef CHECK_SUBVIEWS
	CameraObject*			m_lookAboveCamera{ nullptr };
#endif

	XMFLOAT4X4              matrix{ Matrix4x4::Identity() };

	//////////////////////////////////////  조명

	ID3D12Resource				*m_pd3dcbLights{ nullptr };
	LIGHTS						*m_pcbMappedLights{ nullptr };

	MATERIAL					*m_pMaterials{ nullptr };

	ID3D12Resource				*m_pd3dcbMaterials{ nullptr };
	MATERIAL					*m_pcbMappedMaterials{ nullptr };
	//////////////////////////////////////  조명


	///////////////////////////////////////////////////////////////////////// Player
	GameObject *m_TESTQuadGameobject;
	QuadTreeTerrainMesh         *m_TESTQuadTree{ nullptr };

public:
	SOCKET socket;

	// 유저 정보 변수
	BOOL mUsingID;			// 유저의 id 값이 사용되는지 판단
	BOOL m_isLobby;			// 유저가 로비에 있는지 판단 (방 만들기)
	BOOL m_isRoom;			// 유저가 방에 있는지 판단
	BOOL m_isGameReady;		// 유저의 준비완료 여부
	BOOL m_isGameAlive;		// 유저가 죽지않고 게임 중인지 판단
	BOOL m_isGameDie;		// 유저가 죽었는지 판단
	BOOL m_isGameRisporn;	// 유저가 죽었는지 판단
	BOOL m_isGamePlay;		// 유저가 게임중인지

	USHORT PlayerID;		// 유저들의 각 클라이언트 ID
	INT GameRoomIndex;
	INT InGame_PlayIndex;	//게임 플레이를 시작할 때 자신이 방에서 몇번째 플레이어인가
	INT Score;				// 유저들의 점수
	INT Kill_Count;
	INT Death_Count;
	INT Rank;
	INT EndUI_List;
	BOOL Attack_Buff;
	BOOL Defence_Buff;

	FLOAT PosX, PosY, PosZ;	// 플레이어의 위치
	FLOAT RotY;			// 플레이어의 회전 각도
	INT Select_model;
	INT HP;
	BOOL Allive;
	BOOL WeaponType;
	INT Combat_Atk;
	INT Fire_Atk;
	INT Defence;
	FLOAT Speed;

	OVERLAPPED_EX Player_OVERLAPPED;

	VOID PlayerConnectInit(UINT id);	// 유저가 접속하면 OVERLAPPED 구조체 초기화
	VOID PlayerDisconnect(UINT id);		// 유저가 게임을 나갔을때 OVERLAPPED 구조체 설정
	VOID InitPlayer(UINT id);			// 유저의 게임정보 초기화(게임 플레이가 끝났을때)
	VOID GamePlayInit(UINT id);			// 게임 플레이가 끝나면 방에 처음 들어간 상태로 만들어주기
	///////////////////////////////////////////////////////////////////////// Player
};