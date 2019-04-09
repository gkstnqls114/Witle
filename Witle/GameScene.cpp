#include "stdafx.h"
#include "d3dUtil.h"

#include "LightManager.h"
#include "MeshRenderer.h"
#include "ShaderManager.h"
#include "GameScreen.h"
#include "MapInfoLoader.h"

#include "StaticObject.h"
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
#include "QuadTreeTerrainMesh.h"
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
		case VK_F2:
		case VK_F3:
			break;

		}
		break;
	case WM_KEYDOWN:
		switch (wParam) {
		case 'A':
			break;
		case 'W':
		case 'w':
			TerrainIndex += 1;
			break;
		case 'S':
		case 's':
			TerrainIndex -= 1;
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

	// �ͷ��� ���� 
	XMFLOAT3 xmf3Scale(39.0625f * 3.f, 1.0f, 39.0625f * 3.f);
	// XMFLOAT3 xmf3Scale(1.f, 1.0f, 1.f);
	XMFLOAT4 xmf4Color(0.0f, 0.5f, 0.0f, 0.0f);
	m_Terrain = new Terrain("Terrain", pd3dDevice, pd3dCommandList, L"Image/HeightMap.raw", 257, 257, 257, 257, xmf3Scale, xmf4Color);


	// �׽�Ʈ�� �� ������Ʈ
	m_pPlayer = new Player("Player", pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_GameObjectDiffuse = new Texture(1, RESOURCE_TEXTURE2D);
	m_GameObjectDiffuse->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Model/Textures/ReflexTree_Diffuse.dds", 0);


	// ������ �÷����� ������Ʈ ��ġ ����
	MapInfoLoader::LoadTerrainObjectFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Information/Terrain.bin");
 
	// SunFlower
	m_SunFlowerCount = SunFlower::m_CountFromMap;
	m_SunFlowers = new SunFlower*[m_SunFlowerCount];
	for (int x = 0; x < m_SunFlowerCount; ++x)
	{
		m_SunFlowers[x] = new SunFlower(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	}
	// Trees
	m_TreeCount = MyReflexTree::m_CountFromMap;
	m_Trees = new MyReflexTree* [m_TreeCount];
	for (int x = 0; x < m_TreeCount; ++x)
	{
		m_Trees[x] = new MyReflexTree(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
 	}
	MyReflexTree::CreateShaderVariables(pd3dDevice, pd3dCommandList);
	m_TreeDiffuse = new Texture(1, RESOURCE_TEXTURE2D); 
	m_TreeDiffuse->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Model/Textures/ReflexTree_Diffuse.dds", 0);
	

	// Trees
	m_PillaCount = Pillar::m_CountFromMap;
	m_Pillas = new Pillar*[m_PillaCount];
	for (int x = 0; x < m_PillaCount; ++x)
	{
		m_Pillas[x] = new Pillar(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	}

	m_RockCount = Rock::m_CountFromMap;
	m_Rocks = new Rock*[m_RockCount];
	for (int x = 0; x < m_RockCount; ++x)
	{
		m_Rocks[x] = new Rock(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	}

	//�׽�Ʈ ����Ʈ�� �ͷ��� ����
	m_TESTQuadGameobject = new GameObject("TESTQuad");
	m_TESTQuadTree = new QuadTreeTerrainMesh(m_TESTQuadGameobject, pd3dDevice, pd3dCommandList, 257, 257, xmf3Scale, xmf4Color, m_Terrain->GetHeightMapImage());

	// ī�޶�
	m_Camera = new CameraObject("Camera");
	// Camera* cameraComponent = new FollowCam(m_Camera, m_pPlayer);
	Camera* cameraComponent = new FollowCam(m_Camera, m_pPlayer);
	GameScreen::SetCamera(cameraComponent);
	cameraComponent->CreateShaderVariables(pd3dDevice, pd3dCommandList);
	cameraComponent->SetOffset(XMFLOAT3(0, 0, 1000.f));
	cameraComponent->SetAt(XMFLOAT3(0, 0, 0));
	// cameraComponent->SetAt(XMFLOAT3(0, m_Terrain->GetHeight(1, 1) + 10, 0));
	cameraComponent->SetViewport(0, 0, GameScreen::GetWidth(), GameScreen::GetHeight(), 0.0f, 1.0f);
	cameraComponent->SetScissorRect(0, 0, GameScreen::GetWidth(), GameScreen::GetHeight());
	cameraComponent->GenerateProjectionMatrix(0.01f, CAMERA_FAR, float(GameScreen::GetWidth()) / float(GameScreen::GetHeight()), 60.0f);
	m_Camera->ChangeCamera(cameraComponent);


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
	GameScene::CreateShaderResourceViews(pd3dDevice, m_GameObjectDiffuse, ROOTPARAMETER_TEXTURE, true);
	// GameScene::CreateShaderResourceViews(pd3dDevice, m_TreeDiffuse, ROOTPARAMETER_TEXTURE, true);
}

void GameScene::ReleaseObjects()
{
	if (m_pPlayer)
	{
		// m_pPlayer->ReleaseObjects(); 
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
	if (m_Camera)
	{
		m_Camera->ReleaseObjects();
		delete m_Camera;
		m_Camera = nullptr;
	}
	if (m_Terrain)
	{
		m_Terrain->ReleaseObjects();
		delete m_Terrain;
		m_Terrain = nullptr;
	} 
	if (m_TESTQuadTree)
	{
		delete m_TESTQuadTree;
		m_TESTQuadTree = nullptr;
	}
}

bool GameScene::ProcessInput(HWND hWnd, float ElapsedTime)
{
	DWORD dwDirection = 0;

	// Ű���� ó��
	if (GameInput::IsKeydownUP())
	{ 
		dwDirection |= DIR_FORWARD;
	}
	if (GameInput::IsKeydownDOWN())
	{ 
		dwDirection |= DIR_BACKWARD;
	}
	if (GameInput::IsKeydownLEFT())
	{
		dwDirection |= DIR_LEFT;
	}
	if (GameInput::IsKeydownRIGHT())
	{
		dwDirection |= DIR_RIGHT;
	}
	if(GameInput::IsKeydownW())
	{
		dwDirection |= DIR_UP;
	}
	if (GameInput::IsKeydownS())
	{
		dwDirection |= DIR_DOWN;
	}

	// ���� Ű���� �����¿� �����δٸ�...
	if (dwDirection != 0)
	{
		// AXIS axis = m_pPlayer->GetTransform().GetCoorAxis();
		AXIS axis = AXIS{ m_pPlayer->GetCoorAxis()};

		XMFLOAT3 xmf3Shift = XMFLOAT3(0.f, 0.f, 0.f); // �̵���

		/*�÷��̾ dwDirection �������� �̵��Ѵ�(�����δ� �ӵ� ���͸� �����Ѵ�). �̵� �Ÿ��� �ð��� ����ϵ��� �Ѵ�.
		�÷��̾��� �̵� �ӷ��� (20m/��)�� �����Ѵ�.*/
		float fDistance = 2000.0f * ElapsedTime; // 1�ʴ� �ִ� �ӷ� 20m���� ����, ���� 1 = 1cm

		if (dwDirection & DIR_FORWARD) xmf3Shift = Vector3::Add(xmf3Shift, axis.look, fDistance);
		if (dwDirection & DIR_BACKWARD) xmf3Shift = Vector3::Add(xmf3Shift, axis.look, -fDistance);
		if (dwDirection & DIR_RIGHT) xmf3Shift = Vector3::Add(xmf3Shift, axis.right, fDistance);
		if (dwDirection & DIR_LEFT) xmf3Shift = Vector3::Add(xmf3Shift, axis.right, -fDistance);
		if (dwDirection & DIR_UP) xmf3Shift = Vector3::Add(xmf3Shift, axis.up, fDistance);
		if (dwDirection & DIR_DOWN) xmf3Shift = Vector3::Add(xmf3Shift, axis.up, -fDistance);

		//�÷��̾��� �̵��� ���͸� xmf3Shift ���͸�ŭ ���Ѵ�. 
		// m_pPlayer->MoveVelocity(xmf3Shift);
		m_pPlayer->MoveVelocity(xmf3Shift); 
	}
	else
	{
		XMFLOAT3 Veclocity = m_pPlayer->GetVelocity();
		if (Vector3::Length(Veclocity) > 0.f)
		{ 
			float fLength = Vector3::Length(Veclocity);
			float fDeceleration = (3000.f * ElapsedTime); //�ش����� Friction
			if (fDeceleration > fLength) fDeceleration = fLength;
			// m_pPlayer->MoveVelocity(Vector3::ScalarProduct(Veclocity, -fDeceleration, true));
			m_pPlayer->MoveVelocity(Vector3::ScalarProduct(Veclocity, -fDeceleration, true));
		} 
	}
	m_pPlayer->SetTrackAnimationSet(dwDirection);
	
	if ((GameInput::GetcDeltaX() != 0.0f) || (GameInput::GetcDeltaY() != 0.0f))
	{
		if (GameInput::GetcDeltaX() || GameInput::GetcDeltaY())
		{
			// �÷��̾�� ī�޶� �Ȱ��� rotate...
			// ���� �������̹Ƿ� ���� ����
			m_Camera->GetCamera()->Rotate(GameInput::GetcDeltaY(), GameInput::GetcDeltaX(), 0.0f);
			m_pPlayer->Rotate(0.0f, GameInput::GetcDeltaX(), 0.0f);
		}
		
	}

	return true;
}

// ProcessInput�� ���� right, up, look, pos �� ���庯ȯ ��Ŀ� �����Ѵ�.
void GameScene::Update(float fElapsedTime)
{ 
	// �浹üũ ///////////////////////// 
	//BoundingOrientedBox AlreadyBBox = m_pPlayer->CalculateAlreadyBoundingBox(fElapsedTime); 
	//XMFLOAT3 AlreadyPositon{ AlreadyBBox.Center.x, AlreadyBBox.Center.y, AlreadyBBox.Center.z };
	//for (int i = 0; i < m_TreeCount; ++i)
	//{
	//	bool isAlreadyCollide = Collision::isCollide(AlreadyBBox, m_Trees[i]->GetBOBox()->GetBOBox());
	//	if (isAlreadyCollide)
	//	{ 
	//		bool isUseSliding = false;
	//		for (int x = 0; x < 4; ++x)
	//		{
	//			bool isIntersect = Plane::Intersect(m_Trees[i]->GetBOBox()->GetPlane(x), AlreadyPositon, m_pPlayer->GetVelocity());
	//			bool isFront = Plane::IsFront(m_Trees[i]->GetBOBox()->GetPlane(x), AlreadyPositon);
	//			if (isIntersect && isFront)
	//			{
	//				std::cout << x << " ... intersect! ";
	//				//�����̵�����

	//				m_pPlayer->SetVelocity(
	//					Vector3::Sliding(m_Trees[i]->GetBOBox()->GetPlane(x), m_pPlayer->GetVelocity())
	//				);

	//				isUseSliding = true; 
	//			}
	//		} 
	//		 
	//		if(!isUseSliding)
	//		{
	//			m_pPlayer->MoveVelocity(Vector3::ScalarProduct(m_pPlayer->GetVelocity(), -1, false));
	//		}
	//		std::cout << std::endl;
	//	}
	//}
	// �浹üũ /////////////////////////

	if (m_pPlayer)
	{
		m_pPlayer->Update(fElapsedTime); //Velocity�� ���� pos �̵�
	}
	 


	// light update
	::memcpy(m_pcbMappedLights, LightManager::m_pLights, sizeof(LIGHTS));
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
	if (m_Camera)
	{
		m_Camera->LastUpdate(fElapsedTime);
	} 

	// ī�޶� �������Ұ� ����Ʈ�� ���� ������ üũ
	//m_Camera->GetFrustum()->CheckRendering(m_TESTQuadTree->GetRootNode());
	m_Camera->GetFrustum()->CheckRenderingAllTRUE(m_TESTQuadTree->GetRootNode());
}

void GameScene::TESTSetRootDescriptor(ID3D12GraphicsCommandList * pd3dCommandList)
{
	pd3dCommandList->SetDescriptorHeaps(1, &m_pd3dCbvSrvDescriptorHeap);
	m_Terrain->UpdateShaderVariables(pd3dCommandList);
}

void GameScene::AnimateObjects(float fTimeElapsed)
{
	// if (m_pHeightMapTerrain) m_pHeightMapTerrain->Animate(fTimeElapsed);
	 
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
	m_Camera->SetViewportsAndScissorRects(pd3dCommandList); 
	m_Camera->GetCamera()->UpdateShaderVariables(pd3dCommandList, ROOTPARAMETER_CAMERA);

	//  ����
	D3D12_GPU_VIRTUAL_ADDRESS d3dcbLightsGpuVirtualAddress = m_pd3dcbLights->GetGPUVirtualAddress();
	pd3dCommandList->SetGraphicsRootConstantBufferView(ROOTPARAMETER_LIGHTS, d3dcbLightsGpuVirtualAddress); //Lights

	D3D12_GPU_VIRTUAL_ADDRESS d3dcbMaterialsGpuVirtualAddress = m_pd3dcbMaterials->GetGPUVirtualAddress();
	pd3dCommandList->SetGraphicsRootConstantBufferView(ROOTPARAMETER_MATERIALS, d3dcbMaterialsGpuVirtualAddress);

	// Terrain PSO
	// pd3dCommandList->SetPipelineState(ShaderManager::GetInstance()->GetShader("Terrain")->GetPSO());
	pd3dCommandList->SetDescriptorHeaps(1, &m_pd3dCbvSrvDescriptorHeap);

	pd3dCommandList->SetGraphicsRoot32BitConstants(ROOTPARAMETER_WORLD,  16, &matrix, 0);
	m_Terrain->UpdateShaderVariables(pd3dCommandList); 

	// TerrainMesh Render
	Mesh* terrainMesh = m_Terrain->GetComponent<Mesh>("TerrainMesh");
	// m_TESTQuadTree->Render(m_TESTQuadTree->GetRootNode(), pd3dCommandList);
	m_TESTQuadTree->Render(TerrainIndex, pd3dCommandList);


#ifdef CHECK_SUBVIEWS
	m_lookAboveCamera->SetViewportsAndScissorRects(pd3dCommandList); 
	m_lookAboveCamera->GetCamera()->UpdateShaderVariables(pd3dCommandList, ROOTPARAMETER_CAMERA);

	m_TESTQuadTree->Render(m_TESTQuadTree->GetRootNode(), pd3dCommandList);
#endif

	////////////////////////////// Model Render
	// PSO ����
	
	// Ŭ�� ȭ�� ����
	m_Camera->SetViewportsAndScissorRects(pd3dCommandList);
	m_Camera->GetCamera()->UpdateShaderVariables(pd3dCommandList, ROOTPARAMETER_CAMERA);
	
	pd3dCommandList->SetDescriptorHeaps(1, &m_pd3dCbvSrvDescriptorHeap);
	m_GameObjectDiffuse->UpdateShaderVariables(pd3dCommandList);
	m_pPlayer->Render(pd3dCommandList);

	//for (int x = 0; x < m_SunFlowerCount; ++x)
	//{
	//	m_SunFlowers[x]->Render(pd3dCommandList);
	//} 

	//for (int x = 0; x < m_PillaCount; ++x)
	//{
	//	m_Pillas[x]->Render(pd3dCommandList);
	//} 

	//for (int x = 0; x < m_RockCount; ++x)
	//{
	//	m_Rocks[x]->Render(pd3dCommandList);
	//}

	//for (int x = 0; x < m_TreeCount; ++x)
	//{
		m_Trees[0]->Render(pd3dCommandList);
	//}
	////////////////////////////// Model Render

}

void GameScene::ReleaseUploadBuffers()
{
	//for (int x = 0; x < m_TreeCount; ++x)
	//{
	//	if (m_Trees[x]) m_Trees[x]->ReleaseUploadBuffers();
	//}
	if (m_pPlayer) m_pPlayer->ReleaseUploadBuffers();
	if (m_Terrain) m_Terrain->ReleaseUploadBuffers();
	if (m_TESTQuadTree) m_TESTQuadTree->ReleaseUploadBuffers();
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
	pRootParameters[ROOTPARAMETER_COLOR] = d3dUtil::CreateRootParameterConstants(3, 4);  // b4: Color
	
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

	//// ��Ʈ �з����� ///////////////////////////////////////////////////////////////////// 


	//// ���÷� ///////////////////////////////////////////////////////////////////// 
	D3D12_STATIC_SAMPLER_DESC d3dSamplerDesc;
	::ZeroMemory(&d3dSamplerDesc, sizeof(D3D12_STATIC_SAMPLER_DESC));
	d3dSamplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	d3dSamplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	d3dSamplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	d3dSamplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	d3dSamplerDesc.MipLODBias = 0;
	d3dSamplerDesc.MaxAnisotropy = 1;
	d3dSamplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	d3dSamplerDesc.MinLOD = 0;
	d3dSamplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
	d3dSamplerDesc.ShaderRegister = 0;
	d3dSamplerDesc.RegisterSpace = 0;
	d3dSamplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
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
	d3dRootSignatureDesc.NumStaticSamplers = 1;
	d3dRootSignatureDesc.pStaticSamplers = &d3dSamplerDesc;
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

void GameScene::BuildLightsAndMaterials(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
	////////////////////////////// ����
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
	LightManager::m_pLights->m_pLights[2].Ambient = XMFLOAT4(0.f, 1.f, 0.2f, 1.0f);
	LightManager::m_pLights->m_pLights[2].Diffuse = XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	LightManager::m_pLights->m_pLights[2].Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	LightManager::m_pLights->m_pLights[2].Direction = XMFLOAT3(0.0f, 0.0f, 1.0f);
	
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
	////////////////////////////// ����

	////////////////////////////// ����
	m_pMaterials = new MATERIAL;
	::ZeroMemory(m_pMaterials, sizeof(MATERIAL));

	*m_pMaterials = { XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 5.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) };
	
	CreateConstantBuffer(pd3dDevice, pd3dCommandList, m_pd3dcbMaterials, sizeof(MATERIAL), (void **)&m_pcbMappedMaterials);

	UINT ncbMaterialBytes = ((sizeof(MATERIAL) + 255) & ~255); //256�� ���
	m_pd3dcbMaterials = d3dUtil::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbMaterialBytes, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);

	m_pd3dcbMaterials->Map(0, NULL, (void **)&m_pcbMappedMaterials);
	////////////////////////////// ����
}

void GameScene::RenderShadowMap(ID3D12GraphicsCommandList * pd3dCommandList)
{
}
