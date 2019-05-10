#include "stdafx.h"
#include "d3dUtil.h"
 
//// Skill header //////////////////////////
#include "WideareaMagic.h"
#include "Sniping.h" 
//// Skill header //////////////////////////

//// GameObject header //////////////////////////
#include "SkyBox.h"
#include "Widget.h"
//// GameObject header //////////////////////////

//// Manager header //////////////////////////
#include "ModelStorage.h"
#include "LightManager.h"
#include "MeshRenderer.h"
#include "ShaderManager.h"
#include "StaticObjectStorage.h" 
//// Manager header //////////////////////////

#include "GameScreen.h"
 
#include "MyBOBox.h"
#include "Collision.h"
#include "Object.h" //�������ڵ� 
#include "Texture.h"
#include "CubeMesh.h"
#include "FollowCam.h"
#include "Transform.h"
#include "MyStructs.h"
#include "Shader.h"
#include "Terrain.h"
#include "MyFrustum.h"
#include "GameInput.h"
#include "GameScreen.h"
#include "Player.h"
#include "CameraObject.h"
#include "QuadTreeTerrain.h"
#include "BasicCam.h"
 

#include "GameScene.h"

ID3D12DescriptorHeap*		GameScene::m_pd3dCbvSrvDescriptorHeap;

D3D12_CPU_DESCRIPTOR_HANDLE	GameScene::m_d3dCbvCPUDescriptorStartHandle;
D3D12_GPU_DESCRIPTOR_HANDLE	GameScene::m_d3dCbvGPUDescriptorStartHandle;
D3D12_CPU_DESCRIPTOR_HANDLE	GameScene::m_d3dSrvCPUDescriptorStartHandle;
D3D12_GPU_DESCRIPTOR_HANDLE	GameScene::m_d3dSrvGPUDescriptorStartHandle;

D3D12_CPU_DESCRIPTOR_HANDLE	GameScene::m_d3dCbvCPUDescriptorNextHandle;
D3D12_GPU_DESCRIPTOR_HANDLE	GameScene::m_d3dCbvGPUDescriptorNextHandle;
D3D12_CPU_DESCRIPTOR_HANDLE	GameScene::m_d3dSrvCPUDescriptorNextHandle;
D3D12_GPU_DESCRIPTOR_HANDLE	GameScene::m_d3dSrvGPUDescriptorNextHandle;
 
GameScene::GameScene() 
{

}

GameScene::~GameScene()
{

}
void GameScene::CreateCbvSrvDescriptorHeaps(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, int nConstantBufferViews, int nShaderResourceViews)
{
	D3D12_DESCRIPTOR_HEAP_DESC d3dDescriptorHeapDesc;
	d3dDescriptorHeapDesc.NumDescriptors = nConstantBufferViews + nShaderResourceViews; //CBVs + SRVs 
	d3dDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	d3dDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	d3dDescriptorHeapDesc.NodeMask = 0;
	pd3dDevice->CreateDescriptorHeap(&d3dDescriptorHeapDesc, __uuidof(ID3D12DescriptorHeap), (void **)&m_pd3dCbvSrvDescriptorHeap);

	m_d3dCbvCPUDescriptorNextHandle = m_d3dCbvCPUDescriptorStartHandle = m_pd3dCbvSrvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	m_d3dCbvGPUDescriptorNextHandle = m_d3dCbvGPUDescriptorStartHandle = m_pd3dCbvSrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart();
	m_d3dSrvCPUDescriptorNextHandle.ptr = m_d3dSrvCPUDescriptorStartHandle.ptr = m_d3dCbvCPUDescriptorStartHandle.ptr + (d3dUtil::gnCbvSrvDescriptorIncrementSize * nConstantBufferViews);
	m_d3dSrvGPUDescriptorNextHandle.ptr = m_d3dSrvGPUDescriptorStartHandle.ptr = m_d3dCbvGPUDescriptorStartHandle.ptr + (d3dUtil::gnCbvSrvDescriptorIncrementSize * nConstantBufferViews);
}

D3D12_GPU_DESCRIPTOR_HANDLE GameScene::CreateConstantBufferViews(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, int nConstantBufferViews, ID3D12Resource * pd3dConstantBuffers, UINT nStride)
{
	D3D12_GPU_DESCRIPTOR_HANDLE d3dCbvGPUDescriptorHandle = m_d3dCbvGPUDescriptorNextHandle;
	D3D12_GPU_VIRTUAL_ADDRESS d3dGpuVirtualAddress = pd3dConstantBuffers->GetGPUVirtualAddress();
	D3D12_CONSTANT_BUFFER_VIEW_DESC d3dCBVDesc;
	d3dCBVDesc.SizeInBytes = nStride;
	for (int j = 0; j < nConstantBufferViews; j++)
	{
		d3dCBVDesc.BufferLocation = d3dGpuVirtualAddress + (nStride * j);
		m_d3dCbvCPUDescriptorNextHandle.ptr = m_d3dCbvCPUDescriptorNextHandle.ptr + d3dUtil::gnCbvSrvDescriptorIncrementSize;
		pd3dDevice->CreateConstantBufferView(&d3dCBVDesc, m_d3dCbvCPUDescriptorNextHandle);
		m_d3dCbvGPUDescriptorNextHandle.ptr = m_d3dCbvGPUDescriptorNextHandle.ptr + d3dUtil::gnCbvSrvDescriptorIncrementSize;
	}
	return(d3dCbvGPUDescriptorHandle);
}

D3D12_GPU_DESCRIPTOR_HANDLE GameScene::CreateShaderResourceViews(ID3D12Device * pd3dDevice, Texture * pTexture, UINT nRootParameter, bool bAutoIncrement)
{
	assert(!(pTexture == nullptr));

	D3D12_GPU_DESCRIPTOR_HANDLE d3dSrvGPUDescriptorHandle = m_d3dSrvGPUDescriptorNextHandle;
	if (pTexture)
	{
		int nTextures = pTexture->GetTextures();
		int nTextureType = pTexture->GetTextureType();
		for (int i = 0; i < nTextures; i++)
		{
			ID3D12Resource *pShaderResource = pTexture->GetTexture(i);
			D3D12_RESOURCE_DESC d3dResourceDesc = pShaderResource->GetDesc();
			D3D12_SHADER_RESOURCE_VIEW_DESC d3dShaderResourceViewDesc = d3dUtil::GetShaderResourceViewDesc(d3dResourceDesc, nTextureType);
			pd3dDevice->CreateShaderResourceView(pShaderResource, &d3dShaderResourceViewDesc, m_d3dSrvCPUDescriptorNextHandle);
			m_d3dSrvCPUDescriptorNextHandle.ptr += d3dUtil::gnCbvSrvDescriptorIncrementSize;

			pTexture->SetRootArgument(i, (bAutoIncrement) ? (nRootParameter + i) : nRootParameter, m_d3dSrvGPUDescriptorNextHandle);
			m_d3dSrvGPUDescriptorNextHandle.ptr += d3dUtil::gnCbvSrvDescriptorIncrementSize;
		}
	}
	return(d3dSrvGPUDescriptorHandle);
}

bool GameScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return false;
}

bool GameScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam, float ElapsedTime)
{
	switch (nMessageID)
	{
	case WM_KEYUP:
		switch (wParam)
		{
		case VK_F1:
			// �÷��̾� 1���� ����
			break;
		case VK_F2:
			// �÷��̾� 2���� ����
			break;
		case VK_F3:
			m_isSkyMode = !m_isSkyMode;
			break;

		}
		break;
	case WM_KEYDOWN:
		switch (wParam) {
		case 'Z':
			// ī�޶� �Ÿ��� �ɾ�����.
			m_pMainCamera->GetCamera()->GetOffset();
			break;

		case 'X':
			// ī�޶� �Ÿ��� ���������.

			break;

		case VK_RIGHT:
			// at�� �ű��.
			break;
		
		case VK_LEFT:
			// at�� �ű��.

			break;

		case VK_UP:
			// at�� �ű��.
			m_pMainCamera->GetCamera()->GetOffset();
			break;

		case VK_DOWN:
			break;

		case 'E': 
			m_pPlayer->SubstractHP(100);
			break;

		case '1': // ��ų ���ڷ�
			m_pPlayer->UseSkill_Broom();
			break;

		case '2': // ��ų ��������
			if (m_Sniping->GetisUsing())
			{
				m_Sniping->DoNotUse();
			}
			else
			{
				m_Sniping->DoUse();
			}
			break;

		case '3': // ��ų ���� ����
			// m_WideareaMagic->DoUse(); 
			// m_WideareaMagic->SetPosition(m_pPlayer->GetTransform().GetPosition());
			break;
		case 'W':
		case 'w':  
			break;
		case 'S':
		case 's':  
			break;
		default:
			break;
		}
		break;
	}

	return false;
}

void GameScene::BuildObjects(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
	BuildLightsAndMaterials(pd3dDevice, pd3dCommandList);

	//��Ʈ �ñ׳��ĸ� �����Ѵ�.
	if (!m_pd3dGraphicsRootSignature)
	{
		CreateRootSignature(pd3dDevice);
	}
	// ��ũ���� �� ����
	GameScene::CreateCbvSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, 0, 3);
	 
	m_AimPoint = new AimPoint("AimPoint", pd3dDevice, pd3dCommandList, POINT{ int(GameScreen::GetWidth()) / 2, int(GameScreen::GetHeight()) / 2 }, 100.f, 100.f, L"Image/AimPoint.dds");
	m_WideareaMagic = new WideareaMagic(pd3dDevice, pd3dCommandList);

	//// ��ī�� �ڽ� ����
	m_SkyBox = new SkyBox(pd3dDevice, pd3dCommandList, 3000.F, 3000.F, 3000.F);

	// �ͷ��� ���� 
	XMFLOAT3 xmf3Scale(39.0625f * 3.f, 1.0f, 39.0625f * 3.f);
	// XMFLOAT3 xmf3Scale(1.f, 1.0f, 1.f);
	XMFLOAT4 xmf4Color(0.0f, 0.5f, 0.0f, 0.0f);
	m_Terrain = new Terrain("Terrain", pd3dDevice, pd3dCommandList, L"Image/HeightMap.raw", 257, 257, 257, 257, xmf3Scale, xmf4Color);
	
	// �׽�Ʈ�� �� ������Ʈ
	 m_pPlayer = new Player("Player", pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_pOtherPlayer = new Player("OtherPlayer", pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_SkyBox->SetpPlayerTransform(&m_pPlayer->GetTransform());

	//// �׽�Ʈ ����Ʈ�� �ͷ��� ���� 
	m_pQuadtreeTerrain = new QuadtreeTerrain(pd3dDevice, pd3dCommandList, 257, 257, xmf3Scale, xmf4Color, m_Terrain->GetHeightMapImage());

	// ī�޶�
	m_pMainCamera = new CameraObject("Camera");  
	m_Sniping = new Sniping(m_pMainCamera, m_pPlayer, pd3dDevice, pd3dCommandList);
	m_pPlayer->SetSniping(m_Sniping);
	m_pMainCamera->ChangeCamera(m_Sniping->GetBaseCamera());
	 
	 m_pSkyCameraObj = new CameraObject("SkyCamera");
	 m_pSkyCamera = new BasicCam(m_pSkyCameraObj);
	 m_pSkyCamera->CreateShaderVariables(pd3dDevice, pd3dCommandList);
	 m_pSkyCamera->SetViewport(0, 0, GameScreen::GetWidth(), GameScreen::GetHeight(), 0.0f, 1.0f);
	 m_pSkyCamera->SetScissorRect(0, 0, GameScreen::GetWidth(), GameScreen::GetHeight());
	 m_pSkyCamera->GenerateProjectionMatrix(0.01f, 40000.F, float(GameScreen::GetWidth()) / float(GameScreen::GetHeight()), 60.0f);
	 m_pSkyCamera->SetAt(XMFLOAT3(15000.f, 0.f, 15000.f));
	 m_pSkyCamera->Rotate(90.F, 0.F, 0.F);
	 m_pSkyCamera->SetOffset(XMFLOAT3(0.f, -30000.f, 0.f));
	 m_pSkyCameraObj->ChangeCamera(m_pSkyCamera);

#ifdef CHECK_SUBVIEWS
	m_lookAboveCamera = new CameraObject("LookAboveCamera");

	D3D12_VIEWPORT	GBuffer_Viewport{ 0, GameScreen::GetHeight(),  GameScreen::GetAnotherWidth(), GameScreen::GetAnotherHeight(), 0.0f, 1.0f };
	D3D12_RECT		ScissorRect{ 0 , GameScreen::GetHeight(), GameScreen::GetWidth() , GameScreen::GetHeight() + GameScreen::GetAnotherHeight() };

	m_lookAboveCamera->GetCamera()->CreateShaderVariables(pd3dDevice, pd3dCommandList);
	m_lookAboveCamera->GetCamera()->SetScissorRect(ScissorRect);
	m_lookAboveCamera->GetCamera()->SetViewport(GBuffer_Viewport);
	m_lookAboveCamera->GetCamera()->GenerateProjectionMatrix(0.01f, CAMERA_FAR, float(GameScreen::GetWidth()) / float(GameScreen::GetHeight()), 60.0f);
	//m_lookAboveCamera->GetCamera()->SetAt(XMFLOAT3(xmf3Scale.x * 257 / 2, 0.f, xmf3Scale.y * 257 / 2)); 
	m_lookAboveCamera->GetCamera()->SetAt(XMFLOAT3(xmf3Scale.x * 257 / 2, 2000.f, xmf3Scale.z * 257 / 2)); 
	m_lookAboveCamera->GetCamera()->SetOffset(XMFLOAT3(0.0f, 0.f, 10.f));
	m_lookAboveCamera->GetCamera()->Rotate(90.f, 0.f, 0.f);
#endif 

	// ���ҽ� �� ����
	GameScene::CreateShaderResourceViews(pd3dDevice, m_Terrain->GetTexture(), ROOTPARAMETER_TEXTUREBASE, true);  
}

void GameScene::ReleaseObjects()
{  
	if (m_pd3dCbvSrvDescriptorHeap)
	{
		m_pd3dCbvSrvDescriptorHeap->Release();
		m_pd3dCbvSrvDescriptorHeap = nullptr;
	}

	delete LightManager::m_pLights;
	LightManager::m_pLights = nullptr;

	delete m_pMaterials;
	m_pMaterials = nullptr;

	if (m_pd3dcbLights)
	{
		m_pd3dcbLights->Unmap(0, NULL);
		m_pd3dcbLights->Release();
		m_pd3dcbLights = NULL;
	}

	if (m_pd3dcbMaterials)
	{
		m_pd3dcbMaterials->Unmap(0, NULL);
		m_pd3dcbMaterials->Release();
		m_pd3dcbMaterials = NULL;
	}
	if (m_pSkyCamera)
	{
		m_pSkyCamera->ReleaseObjects();
		delete m_pSkyCamera;
		m_pSkyCamera = nullptr;
	}
	if (m_pSkyCameraObj)
	{
		m_pSkyCameraObj->ReleaseObjects();
		delete m_pSkyCameraObj;
		m_pSkyCameraObj = nullptr;
	}
	if (m_SkyBox)
	{
		m_SkyBox->ReleaseObjects();
		delete m_SkyBox;
		m_SkyBox = nullptr;
	}
	if (m_PlayerTerrainIndex)
	{
		delete[] m_PlayerTerrainIndex;
		m_PlayerTerrainIndex = nullptr;
	} 
	if (m_Sniping)
	{ 
		m_Sniping->ReleaseObjects(); 
		delete m_Sniping;
		m_Sniping = nullptr;
	}
	if (m_WideareaMagic)
	{
		m_WideareaMagic->ReleaseObjects();
		delete m_WideareaMagic;
		m_WideareaMagic = nullptr;
	}
	if (m_AimPoint)
	{ 
		m_AimPoint->ReleaseObjects();
		delete m_AimPoint;
		m_AimPoint = nullptr;
	}
	if (m_pOtherPlayer)
	{
		m_pOtherPlayer->ReleaseObjects(); 
		delete m_pOtherPlayer;
		m_pOtherPlayer = nullptr;
	}
	if (m_pPlayer)
	{
		m_pPlayer->ReleaseObjects();
		delete m_pPlayer;
		m_pPlayer = nullptr;
	}
#ifdef CHECK_SUBVIEWS
	if (m_lookAboveCamera)
	{
		m_lookAboveCamera->ReleaseObjects();
		delete m_lookAboveCamera;
		m_lookAboveCamera = nullptr;
	}
#endif
	if (m_pMainCamera)
	{ 
		m_pMainCamera->ReleaseObjects();
		delete m_pMainCamera;
		m_pMainCamera = nullptr;
	}
	if (m_Terrain)
	{ 
		m_Terrain->ReleaseObjects(); 
		delete m_Terrain;
		m_Terrain = nullptr;
	} 
	if (m_pQuadtreeTerrain)
	{  
		m_pQuadtreeTerrain->ReleaseObjects();
		delete m_pQuadtreeTerrain;
		m_pQuadtreeTerrain = nullptr;
	}
}

bool GameScene::ProcessInput(HWND hWnd, float fElapsedTime)
{ 
	// �÷��̾� �̵��� ���� ó�� (��Ȯ���� �̵��� �ƴ϶� ���ӵ�)
	m_pPlayer->ProcessInput(fElapsedTime);
	// m_pOtherPlayer->ProcessInput(fElapsedTime);
	// m_pOtherPlayer->Rotate(0.0f, 10.f, 0.f);

	// �÷��̾� ȸ���� ���� ó��
	if ((GameInput::GetDeltaX() != 0.0f) || (GameInput::GetDeltaY() != 0.0f))
	{
		if (GameInput::GetDeltaX() || GameInput::GetDeltaY())
		{ 
			// �÷��̾�� ī�޶� �Ȱ��� rotate...
			// ���� �������̹Ƿ� ���� ����
			m_pMainCamera->GetCamera()->Rotate(GameInput::GetDeltaY(), GameInput::GetDeltaX(), 0.0f);
			m_pPlayer->Rotate(0.0f, GameInput::GetDeltaX(), 0.0f);
			m_Sniping->Rotate(0.0f, GameInput::GetDeltaX(), 0.0f);
		} 
	}
	
	// ��ŷ ó��
	ProcessPicking(fElapsedTime);

	return true;
}

// ProcessInput�� ���� right, up, look, pos �� ���庯ȯ ��Ŀ� �����Ѵ�.
void GameScene::Update(float fElapsedTime)
{ 
	//// ���� ���� X ////
	UpdateCollision(fElapsedTime);

	if(m_pPlayer) m_pPlayer->Update(fElapsedTime); //Velocity�� ���� pos �̵�
	if(m_pOtherPlayer) m_pOtherPlayer->Update(fElapsedTime);

	if(m_SkyBox) m_SkyBox->Update(fElapsedTime); 
	if(m_WideareaMagic) m_WideareaMagic->Update(fElapsedTime);
	//// ���� ���� X ////

	// light update
	if(m_pcbMappedLights) ::memcpy(m_pcbMappedLights, LightManager::m_pLights, sizeof(LIGHTS));
	// material update
	::memcpy(m_pcbMappedMaterials, m_pMaterials, sizeof(MATERIAL));
}

void GameScene::LastUpdate(float fElapsedTime)
{
#ifdef CHECK_SUBVIEWS
	if (m_lookAboveCamera)
	{
		m_lookAboveCamera->LastUpdate(fElapsedTime);
	}
#endif // CHECK_SUBVIEWS
	 
	// player update ���Ŀ� camera update
	// ��������X
	if (m_pMainCamera) m_pMainCamera->LastUpdate(fElapsedTime);
	if (m_pSkyCameraObj) m_pSkyCameraObj->LastUpdate(fElapsedTime);

	// ī�޶� �������Ұ� ����Ʈ�� ���� ������ üũ
	if (m_pMainCamera && m_pQuadtreeTerrain)
	{
		m_pMainCamera->GetCamera()->GetFrustum()->CheckRendering(m_pQuadtreeTerrain->GetRootNode());
	}
	if (m_pQuadtreeTerrain) m_pQuadtreeTerrain->LastUpdate(fElapsedTime);
	// ��������X 

	// �̵��� �÷��̾� ������ üũ
	//if (Collision::isCollide(m_WideareaMagic->GetBSphere(), m_pPlayer->GetBOBox()))
	//{

	//} 
}

void GameScene::TESTSetRootDescriptor(ID3D12GraphicsCommandList * pd3dCommandList)
{
	pd3dCommandList->SetDescriptorHeaps(1, &m_pd3dCbvSrvDescriptorHeap);
	m_Terrain->UpdateShaderVariables(pd3dCommandList);
}

void GameScene::AnimateObjects(float fTimeElapsed)
{ 
	if (m_pOtherPlayer) m_pOtherPlayer->Animate(fTimeElapsed);
	if (m_pPlayer) m_pPlayer->Animate(fTimeElapsed);
}

void GameScene::Render(ID3D12GraphicsCommandList *pd3dCommandList)
{ 
	RenderShadowMap(pd3dCommandList);

	// ������
	extern MeshRenderer gMeshRenderer;

	// �׷��� ��Ʈ �ñ״�ó ����
	pd3dCommandList->SetGraphicsRootSignature(m_pd3dGraphicsRootSignature);

	// Ŭ�� ȭ�� ����	
	if (m_isSkyMode)
	{
		m_pSkyCameraObj->SetViewportsAndScissorRects(pd3dCommandList);
		m_pSkyCameraObj->GetCamera()->UpdateShaderVariables(pd3dCommandList, ROOTPARAMETER_CAMERA);
	}
	else
	{
		m_pMainCamera->SetViewportsAndScissorRects(pd3dCommandList);
		m_pMainCamera->GetCamera()->UpdateShaderVariables(pd3dCommandList, ROOTPARAMETER_CAMERA);
	}

	// ��ī�̹ڽ� ����
	if(m_SkyBox) m_SkyBox->Render(pd3dCommandList);
	 
	//  ����
	D3D12_GPU_VIRTUAL_ADDRESS d3dcbLightsGpuVirtualAddress = m_pd3dcbLights->GetGPUVirtualAddress();
	pd3dCommandList->SetGraphicsRootConstantBufferView(ROOTPARAMETER_LIGHTS, d3dcbLightsGpuVirtualAddress); //Lights

	D3D12_GPU_VIRTUAL_ADDRESS d3dcbMaterialsGpuVirtualAddress = m_pd3dcbMaterials->GetGPUVirtualAddress();
	pd3dCommandList->SetGraphicsRootConstantBufferView(ROOTPARAMETER_MATERIALS, d3dcbMaterialsGpuVirtualAddress);

	pd3dCommandList->SetDescriptorHeaps(1, &m_pd3dCbvSrvDescriptorHeap);

	if (m_pPlayer) m_pPlayer->Render(pd3dCommandList);
	if (m_pOtherPlayer) m_pOtherPlayer->Render(pd3dCommandList);

	// �ͷ���
	if (m_Terrain)
	{ 
		if (m_isSkyMode)
		{ 
			Mesh* terrainMesh = m_Terrain->GetComponent<Mesh>("TerrainMesh");
			m_pQuadtreeTerrain->Render(pd3dCommandList, m_Terrain, m_pd3dCbvSrvDescriptorHeap);
		}
		else
		{
			pd3dCommandList->SetDescriptorHeaps(1, &m_pd3dCbvSrvDescriptorHeap);
			pd3dCommandList->SetGraphicsRoot32BitConstants(ROOTPARAMETER_WORLD, 16, &Matrix4x4::Identity(), 0);
			m_Terrain->UpdateShaderVariables(pd3dCommandList);

			// TerrainMesh Render
			Mesh* terrainMesh = m_Terrain->GetComponent<Mesh>("TerrainMesh");
			m_pQuadtreeTerrain->Render(pd3dCommandList);
		}
	}
	if(m_WideareaMagic) m_WideareaMagic->Render(pd3dCommandList);


#ifdef CHECK_SUBVIEWS
	m_lookAboveCamera->SetViewportsAndScissorRects(pd3dCommandList); 
	m_lookAboveCamera->GetCamera()->UpdateShaderVariables(pd3dCommandList, ROOTPARAMETER_CAMERA);

	m_pQuadtreeTerrain->Render(m_pQuadtreeTerrain->GetRootNode(), pd3dCommandList);
#endif

	////////////////////////////// Model Render
	// PSO ����
	 

	//// Aim point Render 
	if (!m_isSkyMode)
	{
		if (m_pPlayer) m_pPlayer->RenderHpStatus(pd3dCommandList);
		if(m_AimPoint) m_AimPoint->Render(pd3dCommandList);
	}

}

void GameScene::ReleaseUploadBuffers()
{   
	if (m_SkyBox) m_SkyBox->ReleaseUploadBuffers();
	if (m_AimPoint)m_AimPoint->ReleaseUploadBuffers();
	if (m_Sniping) m_Sniping->ReleaseUploadBuffers();
	if (m_WideareaMagic) m_WideareaMagic->ReleaseUploadBuffers();
	if (m_SkyBox) m_SkyBox->ReleaseUploadBuffers();
	if (m_pOtherPlayer) m_pOtherPlayer->ReleaseUploadBuffers();
	if (m_pPlayer) m_pPlayer->ReleaseUploadBuffers();
	if (m_Terrain) m_Terrain->ReleaseUploadBuffers();
	if (m_pQuadtreeTerrain) m_pQuadtreeTerrain->ReleaseUploadBuffers();
}


ID3D12RootSignature* GameScene::CreateGraphicsRootSignature(ID3D12Device *pd3dDevice)
{
	HRESULT hResult = S_FALSE;
	ID3D12RootSignature* pd3dGraphicsRootSignature = nullptr;

	//// ��Ʈ �з����� ///////////////////////////////////////////////////////////////////// 
	D3D12_ROOT_PARAMETER pRootParameters[ROOTPATAMETER_COUNT];

	pRootParameters[ROOTPARAMETER_WORLD] = d3dUtil::CreateRootParameterConstants(16, 0); // b0: World
	pRootParameters[ROOTPARAMETER_CAMERA] = d3dUtil::CreateRootParameterCBV(1);          // b1: Camera
	pRootParameters[ROOTPARAMETER_MATERIALS] = d3dUtil::CreateRootParameterCBV(2);       // b2: Materials
	pRootParameters[ROOTPARAMETER_LIGHTS] = d3dUtil::CreateRootParameterCBV(3);          // b3: Lights
	pRootParameters[ROOTPARAMETER_PICKINGPOINT] = d3dUtil::CreateRootParameterConstants(2, 4);  // b4: Color
	pRootParameters[ROOTPARAMETER_HPPERCENTAGE] = d3dUtil::CreateRootParameterConstants(1, 5);  // b5: HP percentage
	
	D3D12_DESCRIPTOR_RANGE pTextureDescriptorRanges[3];
	pTextureDescriptorRanges[0] = d3dUtil::CreateDescriptorRangeSRV(1, 0); //t0: gtxtTexture
	pTextureDescriptorRanges[1] = d3dUtil::CreateDescriptorRangeSRV(1, 1); //t1: gtxtTerrainBaseTexture
	pTextureDescriptorRanges[2] = d3dUtil::CreateDescriptorRangeSRV(1, 2); //t2: gtxtTerrainDetailTexture	

	// �ٽ� ���� �з����� 
	pRootParameters[ROOTPARAMETER_TEXTURE] = d3dUtil::CreateRootParameterTable(1, &pTextureDescriptorRanges[0], D3D12_SHADER_VISIBILITY_PIXEL);
	pRootParameters[ROOTPARAMETER_TEXTUREBASE] = d3dUtil::CreateRootParameterTable(1, &pTextureDescriptorRanges[1], D3D12_SHADER_VISIBILITY_PIXEL);
	pRootParameters[ROOTPARAMETER_TEXTUREDETAIL] = d3dUtil::CreateRootParameterTable(1, &pTextureDescriptorRanges[2], D3D12_SHADER_VISIBILITY_PIXEL);
	
	D3D12_DESCRIPTOR_RANGE pd3dDescriptorRanges[8];
	pd3dDescriptorRanges[0] = d3dUtil::CreateDescriptorRangeSRV(1, 6);  //t6: gtxtAlbedoTexture
	pd3dDescriptorRanges[1] = d3dUtil::CreateDescriptorRangeSRV(1, 7);  //t7: gtxtSpecularTexture
	pd3dDescriptorRanges[2] = d3dUtil::CreateDescriptorRangeSRV(1, 8);  //t8: gtxtNormalTexture
	pd3dDescriptorRanges[3] = d3dUtil::CreateDescriptorRangeSRV(1, 9);  //t9: gtxtMetallicTexture
	pd3dDescriptorRanges[4] = d3dUtil::CreateDescriptorRangeSRV(1, 10); //t10: gtxtEmissionTexture
	pd3dDescriptorRanges[5] = d3dUtil::CreateDescriptorRangeSRV(1, 11); //t11: gtxtEmissionTexture
	pd3dDescriptorRanges[6] = d3dUtil::CreateDescriptorRangeSRV(1, 12); //t12: gtxtEmissionTexture
	pd3dDescriptorRanges[7] = d3dUtil::CreateDescriptorRangeSRV(1, 13); //t13: gtxtSkyBoxTexture
	
	pRootParameters[ROOTPARAMETER_ALBEDO] = d3dUtil::CreateRootParameterTable(1, &pd3dDescriptorRanges[0], D3D12_SHADER_VISIBILITY_PIXEL);
	pRootParameters[ROOTPARAMETER_SPECULAR] = d3dUtil::CreateRootParameterTable(1, &pd3dDescriptorRanges[1], D3D12_SHADER_VISIBILITY_PIXEL);
	pRootParameters[ROOTPARAMETER_NORMAL] = d3dUtil::CreateRootParameterTable(1, &pd3dDescriptorRanges[2], D3D12_SHADER_VISIBILITY_PIXEL);
	pRootParameters[ROOTPARAMETER_METALLIC] = d3dUtil::CreateRootParameterTable(1, &pd3dDescriptorRanges[3], D3D12_SHADER_VISIBILITY_PIXEL);
	pRootParameters[ROOTPARAMETER_EMISSION_1] = d3dUtil::CreateRootParameterTable(1, &pd3dDescriptorRanges[4], D3D12_SHADER_VISIBILITY_PIXEL);
	pRootParameters[ROOTPARAMETER_EMISSION_2] = d3dUtil::CreateRootParameterTable(1, &pd3dDescriptorRanges[5], D3D12_SHADER_VISIBILITY_PIXEL);
	pRootParameters[ROOTPARAMETER_EMISSION_3] = d3dUtil::CreateRootParameterTable(1, &pd3dDescriptorRanges[6], D3D12_SHADER_VISIBILITY_PIXEL);
	 
	pRootParameters[ROOTPARAMETER_SKINNEDBONEOFFSET] = d3dUtil::CreateRootParameterCBV(7, 0, D3D12_SHADER_VISIBILITY_VERTEX);
	pRootParameters[ROOTPARAMETER_SKINNEDBONETRANSFORM] = d3dUtil::CreateRootParameterCBV(8, 0, D3D12_SHADER_VISIBILITY_VERTEX);

	pRootParameters[ROOTPARAMETER_INSTANCING] = d3dUtil::CreateRootParameterSRV(14, 0, D3D12_SHADER_VISIBILITY_VERTEX); //t14: Instacing World
	pRootParameters[ROOTPARAMETER_SKYBOX] = d3dUtil::CreateRootParameterTable(1, &pd3dDescriptorRanges[7], D3D12_SHADER_VISIBILITY_PIXEL); //t15: SKYBOX

	//// ��Ʈ �з����� ///////////////////////////////////////////////////////////////////// 


	//// ���÷� ///////////////////////////////////////////////////////////////////// 
	D3D12_STATIC_SAMPLER_DESC d3dSamplerDesc[2];
	::ZeroMemory(&d3dSamplerDesc, sizeof(D3D12_STATIC_SAMPLER_DESC));
	d3dSamplerDesc[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	d3dSamplerDesc[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	d3dSamplerDesc[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	d3dSamplerDesc[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	d3dSamplerDesc[0].MipLODBias = 0;
	d3dSamplerDesc[0].MaxAnisotropy = 1;
	d3dSamplerDesc[0].ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	d3dSamplerDesc[0].MinLOD = 0;
	d3dSamplerDesc[0].MaxLOD = D3D12_FLOAT32_MAX;
	d3dSamplerDesc[0].ShaderRegister = 0;
	d3dSamplerDesc[0].RegisterSpace = 0;
	d3dSamplerDesc[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	 
	d3dSamplerDesc[1].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	d3dSamplerDesc[1].AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	d3dSamplerDesc[1].AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	d3dSamplerDesc[1].AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	d3dSamplerDesc[1].MipLODBias = 0;
	d3dSamplerDesc[1].MaxAnisotropy = 1;
	d3dSamplerDesc[1].ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	d3dSamplerDesc[1].MinLOD = 0;
	d3dSamplerDesc[1].MaxLOD = D3D12_FLOAT32_MAX;
	d3dSamplerDesc[1].ShaderRegister = 1;
	d3dSamplerDesc[1].RegisterSpace = 0;
	d3dSamplerDesc[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	//// ���÷� ///////////////////////////////////////////////////////////////////// 

	D3D12_ROOT_SIGNATURE_FLAGS d3dRootSignatureFlags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS  ;
		// D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;
		//D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS; 

	D3D12_ROOT_SIGNATURE_DESC d3dRootSignatureDesc;
	::ZeroMemory(&d3dRootSignatureDesc, sizeof(D3D12_ROOT_SIGNATURE_DESC));
	d3dRootSignatureDesc.NumParameters = _countof(pRootParameters);
	d3dRootSignatureDesc.pParameters = pRootParameters;
	d3dRootSignatureDesc.NumStaticSamplers = _countof(d3dSamplerDesc);
	d3dRootSignatureDesc.pStaticSamplers = d3dSamplerDesc;
	d3dRootSignatureDesc.Flags = d3dRootSignatureFlags;

	ID3DBlob *pd3dSignatureBlob = nullptr;
	ID3DBlob *pd3dErrorBlob = nullptr;
	hResult = ::D3D12SerializeRootSignature(&d3dRootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &pd3dSignatureBlob, &pd3dErrorBlob);
	assert(hResult == S_OK); 

	hResult = pd3dDevice->CreateRootSignature(0, pd3dSignatureBlob->GetBufferPointer(), pd3dSignatureBlob->GetBufferSize(), IID_PPV_ARGS(&pd3dGraphicsRootSignature));
	assert(hResult == S_OK); 

	if (pd3dSignatureBlob) pd3dSignatureBlob->Release();
	if (pd3dErrorBlob) pd3dErrorBlob->Release();

	return (pd3dGraphicsRootSignature);
}

void GameScene::UpdateCollision(float fElapsedTime)
{
	// �浹üũ ///////////////////////// 
	BoundingOrientedBox AlreadyPlayerBBox = m_pPlayer->CalculateAlreadyBoundingBox(fElapsedTime);
	XMFLOAT3 AlreadyPositon{ AlreadyPlayerBBox.Center.x, AlreadyPlayerBBox.Center.y, AlreadyPlayerBBox.Center.z };
	
	// �ܰ�ó��
	MyBOBox outside_box[4]{ 
		{XMFLOAT3(-100, 0, 15000), XMFLOAT3(100, 3000, 30000)},
		{XMFLOAT3(15000, 0, 30100), XMFLOAT3(30000, 3000, 200)},
		{XMFLOAT3(30100, 0, 15000), XMFLOAT3(100, 3000, 30000)},
		{XMFLOAT3(15000, 0, 100), XMFLOAT3(30000, 3000, 100)},
	};

	for (int i = 0; i < 4; ++i)
	{  
		// �̵��� �ڽ��� ���� �浹�Ѵ�.
		bool isAlreadyCollide = Collision::isCollide(AlreadyPlayerBBox, outside_box[i].GetBOBox());
		if (isAlreadyCollide)
		{
			for (int x = 0; x < 4; ++x) //  plane 4 �鿡 ���� üũ�Ѵ�..
			{
				XMFLOAT3 intersectionPoint;
				// ���⼭ d��... ������ ������ �Ÿ��� �ǹ��Ѵ�. (���/����)
				float d = outside_box[i].GetPlane(x).w;
				// �ð��� ���� �̵��� �������� �ϹǷ� velocity �� ������ �ð��� ���Ѵ�.
				bool isFront = Plane::IsFront(outside_box[i].GetPlaneNormal(x), d, m_pPlayer->GetTransform().GetPosition()); // ������Ʈ ���� ��ġ
				// ���� ������ ��鿡 �����ߴٸ�...
				if (isFront)
				{
					bool isIntersect = Plane::Intersect(outside_box[i].GetPlaneNormal(x), d, m_pPlayer->GetTransform().GetPosition(), Vector3::ScalarProduct(m_pPlayer->GetVelocity(), fElapsedTime, false), intersectionPoint);

					if (isIntersect)
					{
						// �ش� �������� �������ȿ� �����ϴ��� Ȯ���Ѵ�.
						// outside_box[i].IsIn(x, intersectionPoint)
						// 
						if (Collision::isIn(outside_box[i].GetBOBox(), intersectionPoint)) //isIn �Լ� ���߿� �����ؾ���
						{
							m_pPlayer->SetVelocity
							(
								Vector3::Sliding(outside_box[i].GetPlaneNormal(x), m_pPlayer->GetVelocity())
							);
						}
					}
				}
			}
		}
	}

	XMINT4 IDs = m_pQuadtreeTerrain->GetIDs(AlreadyPositon);
	int TerrainCount = m_pQuadtreeTerrain->GetTerrainPieceCount();

	// Ti: Terrain Index
	for (int Ti = 0; Ti < TerrainCount; ++Ti)
	{ 
		int TerrainIndex = Ti;
		//if (Ti == 0) TerrainIndex = IDs.x;
		//else if (Ti == 1) TerrainIndex = IDs.y;
		//else if (Ti == 2) TerrainIndex = IDs.z;
		//else if (Ti == 3) TerrainIndex = IDs.w;

		//if (TerrainIndex == -1) continue;

		for (const auto& name : ModelStorage::GetInstance()->m_NameList)
		{
			MyBOBox* box = ModelStorage::GetInstance()->GetBOBox(name);
			if (!box) continue; // �浹�ڽ��� ���ٸ� �ٸ� ������Ʈ�� �˻�����.

			XMFLOAT4X4* pWorldMatrix = StaticObjectStorage::GetInstance(m_pQuadtreeTerrain)->GetpWorldMatrixs(TerrainIndex, name);

			// Ʈ���� ���� ���� ������Ʈ ������ŭ �浹 üũ
			for (int i = 0; i < StaticObjectStorage::GetInstance(m_pQuadtreeTerrain)->GetObjectCount(TerrainIndex, name); ++i)
			{
				//���� ��� ����´�.

				// �� �浹�ڽ��� ������� ���Ѵ�. �ϴ� ����� ���������� �̵�
				MyBOBox worldBox = *box;
				worldBox.Move(XMFLOAT3(pWorldMatrix[i]._41, 0, pWorldMatrix[i]._43));

				// �̵��� �ڽ��� ���� �浹�Ѵ�.
				bool isAlreadyCollide = Collision::isCollide(AlreadyPlayerBBox, worldBox.GetBOBox());
				if (isAlreadyCollide)
				{
					for (int x = 0; x < 4; ++x) //  plane 4 �鿡 ���� üũ�Ѵ�..
					{
						XMFLOAT3 intersectionPoint;
						// ���⼭ d��... ������ ������ �Ÿ��� �ǹ��Ѵ�. (���/����)
						float d = Plane::CaculateD(box->GetPlane(x), worldBox.GetPosOnPlane(x));
						// �ð��� ���� �̵��� �������� �ϹǷ� velocity �� ������ �ð��� ���Ѵ�.
						bool isFront = Plane::IsFront(box->GetPlaneNormal(x), d, m_pPlayer->GetTransform().GetPosition()); // ������Ʈ ���� ��ġ
						// ���� ������ ��鿡 �����ߴٸ�...
						if (isFront)
						{
							bool isIntersect = Plane::Intersect(box->GetPlaneNormal(x), d, m_pPlayer->GetTransform().GetPosition(), Vector3::ScalarProduct(m_pPlayer->GetVelocity(), fElapsedTime, false), intersectionPoint);

							if (isIntersect)
							{
								// �ش� �������� �������ȿ� �����ϴ��� Ȯ���Ѵ�.
								// worldBox.IsIn(x, intersectionPoint)
								// 
								if (Collision::isIn(worldBox.GetBOBox(), intersectionPoint)) //isIn �Լ� ���߿� �����ؾ���
								{
									m_pPlayer->SetVelocity
									(
										Vector3::Sliding(box->GetPlaneNormal(x), m_pPlayer->GetVelocity())
									);
								}
							}
						}
					}

				}
			}
		}
	}
}

void GameScene::ProcessPicking(float fElapsedTime)
{
	if (GameInput::GetDragMode()) // ���� �巡�׷� ȸ���Ѵٸ�...
	{
		if (!GameInput::IsKeydownE()) return; // e�� ������ �ʾҴٸ� �ƹ��͵� �������� �ʴ´�.
		if (m_pPlayer->IsAttacking()) return;
		
		m_pPlayer->Attack();

		// ��ŷ ray�� �����.
		RAY pickRay = RAY::GeneratePickingRay(m_AimPoint->GetPickingPoint(), m_pMainCamera->GetCamera());

		// �ٸ� ������ ����� Ȯ���Ѵ�.
		float Playerdist;
		bool isCollide = Collision::isCollide(m_pOtherPlayer->GetBOBox()->GetBOBox(), pickRay.origin, pickRay.direction, Playerdist);
		 
		// ���� �ʴ´ٸ� �����Ѵ�.
		if (!isCollide) return;
		if (Playerdist > 3000.f)
		{
			std::cout << Playerdist << " ... " << std::endl;
			return;
		}

		m_pOtherPlayer->SubstractHP(10);
		
		//// ��ŷ ray�� ��� ������Ʈ�� �ִ� �� Ȯ���Ѵ�.
		//float dist = FLT_MAX;
		//float temp;
		//bool isCollideObject;
		//for (const auto& name : ModelStorage::GetInstance()->m_NameList)
		//{
		//	MyBOBox* box = ModelStorage::GetInstance()->GetBOBox(name);
		//	if (!box) continue; // �浹�ڽ��� ���ٸ� �ٸ� ������Ʈ�� �˻�����.

		//	XMFLOAT4X4* pWorldMatrix = StaticObjectStorage::GetInstance(m_pQuadtreeTerrain)->GetpWorldMatrixs(0, name);

		//	// Ʈ���� ���� ���� ������Ʈ ������ŭ �浹 üũ
		//	for (int i = 0; i < StaticObjectStorage::GetInstance(m_pQuadtreeTerrain)->GetObjectCount(0, name); ++i)
		//	{ 
		//		// �� �浹�ڽ��� ������� ���Ѵ�. �ϴ� ����� ���������� �̵�
		//		MyBOBox worldBox = *box;
		//		worldBox.Move(XMFLOAT3(pWorldMatrix[i]._41, 0, pWorldMatrix[i]._43));

		//		if (Collision::isCollide(worldBox.GetBOBox(), pickRay.origin, pickRay.direction, temp))
		//		{
		//			std::cout << "������Ʈ�� �ε���" << std::endl;
		//			dist = fminf(temp, dist);
		//			isCollideObject = true;
		//		}
		//		
		//	}
		//}


		//// ���� ª�� �Ÿ���?
		//// ���� �װ� �ٸ� ������� �ش� ������ hp�� ���ҽ�Ų��.
		//if (Playerdist < dist)
		//{
		//	std::cout << "hp ����" << std::endl;
		//	m_pOtherPlayer->SubstractHP(10);
		//} 
	}
	else // �巡�׷� ȸ������ �ʴ´ٸ�...
	{
		if (!GameInput::isNowClick()) return; // Ŭ������ �ʾҴٸ� �ƹ��͵� �������� �ʴ´�.
		 

	}
}

void GameScene::BuildLightsAndMaterials(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
	//////////////////////////// ����
	LightManager::m_pLights = new LIGHTS;
	::ZeroMemory(static_cast<void*>(LightManager::m_pLights), sizeof(LIGHTS));
	for (int x = 0; x < MAX_LIGHTS; ++x)
	{
		LightManager::m_pLights->m_pLights[x].bEnable = false;
	}

	LightManager::m_pLights->m_xmf4GlobalAmbient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);

	LightManager::m_pLights->m_pLights[0].bEnable = false;
	LightManager::m_pLights->m_pLights[0].nType = LIGHT_TYPE::POINT_LIGHT;
	LightManager::m_pLights->m_pLights[0].fRange = 500.0f;
	LightManager::m_pLights->m_pLights[0].Ambient = XMFLOAT4(0.1f, 0.0f, 0.0f, 1.0f);
	LightManager::m_pLights->m_pLights[0].Diffuse = XMFLOAT4(0.8f, 0.0f, 0.0f, 1.0f);
	LightManager::m_pLights->m_pLights[0].Specular = XMFLOAT4(0.1f, 0.1f, 0.1f, 0.0f);
	LightManager::m_pLights->m_pLights[0].Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	LightManager::m_pLights->m_pLights[0].Direction = XMFLOAT3(0.0f, 0.0f, 0.0f);
	LightManager::m_pLights->m_pLights[0].Attenuation = XMFLOAT3(1.0f, 0.001f, 0.0001f);
	
	LightManager::m_pLights->m_pLights[1].bEnable = false;
	LightManager::m_pLights->m_pLights[1].nType = LIGHT_TYPE::SPOT_LIGHT;
	LightManager::m_pLights->m_pLights[1].fRange = 50.0f;
	LightManager::m_pLights->m_pLights[1].Ambient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	LightManager::m_pLights->m_pLights[1].Diffuse = XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	LightManager::m_pLights->m_pLights[1].Specular = XMFLOAT4(0.1f, 0.1f, 0.1f, 0.0f);
	LightManager::m_pLights->m_pLights[1].Position = XMFLOAT3(-50.0f, 20.0f, -5.0f);
	LightManager::m_pLights->m_pLights[1].Direction = XMFLOAT3(0.0f, 0.0f, 1.0f);
	LightManager::m_pLights->m_pLights[1].Attenuation = XMFLOAT3(1.0f, 0.01f, 0.0001f);
	LightManager::m_pLights->m_pLights[1].fFalloff = 8.0f;
	LightManager::m_pLights->m_pLights[1].fPhi = (float)cos(XMConvertToRadians(40.0f));
	LightManager::m_pLights->m_pLights[1].fTheta = (float)cos(XMConvertToRadians(20.0f));

	LightManager::m_pLights->m_pLights[2].bEnable = true;
	LightManager::m_pLights->m_pLights[2].nType = LIGHT_TYPE::DIRECTIONAL_LIGHT;
	LightManager::m_pLights->m_pLights[2].Ambient = XMFLOAT4(1.f, 0.8f, 0.8f, 1.0f);
	LightManager::m_pLights->m_pLights[2].Diffuse = XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	LightManager::m_pLights->m_pLights[2].Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	LightManager::m_pLights->m_pLights[2].Direction = XMFLOAT3(0.0f, -1.0f, 0.0f);
	
	LightManager::m_pLights->m_pLights[3].bEnable = false;
	LightManager::m_pLights->m_pLights[3].nType = LIGHT_TYPE::SPOT_LIGHT;
	LightManager::m_pLights->m_pLights[3].fRange = 60.0f;
	LightManager::m_pLights->m_pLights[3].Ambient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	LightManager::m_pLights->m_pLights[3].Diffuse = XMFLOAT4(0.5f, 0.0f, 0.0f, 1.0f);
	LightManager::m_pLights->m_pLights[3].Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	LightManager::m_pLights->m_pLights[3].Position = XMFLOAT3(-150.0f, 30.0f, 30.0f);
	LightManager::m_pLights->m_pLights[3].Direction = XMFLOAT3(0.0f, 1.0f, 1.0f);
	LightManager::m_pLights->m_pLights[3].Attenuation = XMFLOAT3(1.0f, 0.01f, 0.0001f);
	LightManager::m_pLights->m_pLights[3].fFalloff = 8.0f;
	LightManager::m_pLights->m_pLights[3].fPhi = (float)cos(XMConvertToRadians(90.0f));
	LightManager::m_pLights->m_pLights[3].fTheta = (float)cos(XMConvertToRadians(30.0f));

	 
	UINT ncbElementBytes = ((sizeof(LIGHTS) + 255) & ~255); //256�� ���
	m_pd3dcbLights = d3dUtil::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbElementBytes, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);

	m_pd3dcbLights->Map(0, NULL, (void **)&m_pcbMappedLights);
	//////////////////////////// ����

	//////////////////////////// ����
	m_pMaterials = new MATERIAL;
	::ZeroMemory(m_pMaterials, sizeof(MATERIAL));

	*m_pMaterials = { XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 5.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) };
	
	CreateConstantBuffer(pd3dDevice, pd3dCommandList, m_pd3dcbMaterials, sizeof(MATERIAL), (void **)&m_pcbMappedMaterials);

	UINT ncbMaterialBytes = ((sizeof(MATERIAL) + 255) & ~255); //256�� ���
	m_pd3dcbMaterials = d3dUtil::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbMaterialBytes, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);

	m_pd3dcbMaterials->Map(0, NULL, (void **)&m_pcbMappedMaterials);
	//////////////////////////// ����
}

void GameScene::RenderShadowMap(ID3D12GraphicsCommandList * pd3dCommandList)
{
}
 
