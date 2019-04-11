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
	//�׷��� ��Ʈ �ñ׳��ĸ� �����Ѵ�.
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

	//////////////////////////////////////  ����

	ID3D12Resource				*m_pd3dcbLights{ nullptr };
	LIGHTS						*m_pcbMappedLights{ nullptr };

	MATERIAL					*m_pMaterials{ nullptr };

	ID3D12Resource				*m_pd3dcbMaterials{ nullptr };
	MATERIAL					*m_pcbMappedMaterials{ nullptr };
	//////////////////////////////////////  ����


	///////////////////////////////////////////////////////////////////////// Player
	GameObject *m_TESTQuadGameobject;
	QuadTreeTerrainMesh         *m_TESTQuadTree{ nullptr };

public:
	SOCKET socket;

	// ���� ���� ����
	BOOL mUsingID;			// ������ id ���� ���Ǵ��� �Ǵ�
	BOOL m_isLobby;			// ������ �κ� �ִ��� �Ǵ� (�� �����)
	BOOL m_isRoom;			// ������ �濡 �ִ��� �Ǵ�
	BOOL m_isGameReady;		// ������ �غ�Ϸ� ����
	BOOL m_isGameAlive;		// ������ �����ʰ� ���� ������ �Ǵ�
	BOOL m_isGameDie;		// ������ �׾����� �Ǵ�
	BOOL m_isGameRisporn;	// ������ �׾����� �Ǵ�
	BOOL m_isGamePlay;		// ������ ����������

	USHORT PlayerID;		// �������� �� Ŭ���̾�Ʈ ID
	INT GameRoomIndex;
	INT InGame_PlayIndex;	//���� �÷��̸� ������ �� �ڽ��� �濡�� ���° �÷��̾��ΰ�
	INT Score;				// �������� ����
	INT Kill_Count;
	INT Death_Count;
	INT Rank;
	INT EndUI_List;
	BOOL Attack_Buff;
	BOOL Defence_Buff;

	FLOAT PosX, PosY, PosZ;	// �÷��̾��� ��ġ
	FLOAT RotY;			// �÷��̾��� ȸ�� ����
	INT Select_model;
	INT HP;
	BOOL Allive;
	BOOL WeaponType;
	INT Combat_Atk;
	INT Fire_Atk;
	INT Defence;
	FLOAT Speed;

	OVERLAPPED_EX Player_OVERLAPPED;

	VOID PlayerConnectInit(UINT id);	// ������ �����ϸ� OVERLAPPED ����ü �ʱ�ȭ
	VOID PlayerDisconnect(UINT id);		// ������ ������ �������� OVERLAPPED ����ü ����
	VOID InitPlayer(UINT id);			// ������ �������� �ʱ�ȭ(���� �÷��̰� ��������)
	VOID GamePlayInit(UINT id);			// ���� �÷��̰� ������ �濡 ó�� �� ���·� ������ֱ�
	///////////////////////////////////////////////////////////////////////// Player
};