#include "stdafx.h"
#include "d3dUtil.h"

#include "ParameterFormManager.h"
#include "LightManager.h"
#include "MeshRenderer.h"
#include "ShaderManager.h"
#include "GameScreen.h"

#include "Object.h" //교수님코드
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
#include "MyDescriptorHeap.h"
#include "ParameterForm.h"

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

	//루트 시그너쳐를 생성한다.
	m_pd3dGraphicsRootSignature = CreateGraphicsRootSignature(pd3dDevice);

	// 디스크립터 힙 설정
	GameScene::CreateCbvSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, 0, 2);

	// 큐브메쉬 생성
	m_GameObject = new Player("Player");
	// 피킹 테스트할 오브젝트 생성, 반드시 순서 유지. gameobject 생성 후 만들어야한다.

	ComponentBase* cubemesh = new CubeMesh(m_GameObject, pd3dDevice, pd3dCommandList, 1.f, 1.f, 1.f);
	m_GameObject->InsertComponent("Mesh", cubemesh);

	// 터레인 생성 
	XMFLOAT3 xmf3Scale(8.0f, 1.0f, 8.0f);
	XMFLOAT4 xmf4Color(0.0f, 0.5f, 0.0f, 0.0f);
	m_Terrain = new Terrain("Terrain", pd3dDevice, pd3dCommandList,
		L"Image/HeightMap.raw", 257, 257, 257, 257, xmf3Scale, xmf4Color);
	// 리소스설정
	GameScene::CreateShaderResourceViews(pd3dDevice, m_Terrain->GetTexture(), 5, true);

	//// 해당 터레인을 플레이어 콜백으로 설정
	m_GameObject->SetPlayerUpdatedContext(m_Terrain);

	// 피킹 테스트 오브젝트들
	m_PickingTESTMeshs = new GameObject*[m_numPickingTESTMeshs];
	m_PickingColors = new XMFLOAT3[m_numPickingTESTMeshs];
	for (int x = 0; x < m_numPickingTESTMeshs; ++x)
	{
		std::string name = "TESTPikcing" + std::to_string(x);
		m_PickingTESTMeshs[x] = new GameObject(name);
		m_PickingColors[x] = XMFLOAT3(1.F, 1.F, 1.F);
		int rand_x = rand() % int(257);
		int rand_z = rand() % int(257);
		m_PickingTESTMeshs[x]->GetTransform().SetPosition(XMFLOAT3(rand_x, m_Terrain->GetHeight(rand_x, rand_z) + 0.5f, rand_z));
		m_PickingTESTMeshs[x]->InsertComponent("Mesh", cubemesh);
	}

	
	//////////////////////////////////////////////////// 테스트할 모델 빌드
	CAngrybotObject *pAngrybotModel = new CAngrybotObject(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, nullptr, 0);
	m_TESTModel = pAngrybotModel;
	// m_TESTModel->SetChild(pAngrybotModel, true);
	m_TESTModel->SetPosition(.0f, m_Terrain->GetHeight(400.0f, 700.0f), 0.0f);
	// m_TESTModel->SetScale(1.0f, 1.0f, 1.0f);
	//////////////////////////////////////////////////// 테스트할 모델 빌드


	// 카메라
	m_Camera = new CameraObject("Camera");
	Camera* cameraComponent = new FollowCam(m_Camera, m_GameObject);
	GameScreen::SetCamera(cameraComponent);
	cameraComponent->CreateShaderVariables(pd3dDevice, pd3dCommandList);
	cameraComponent->SetOffset(XMFLOAT3(0, -5.f, 10.f));
	cameraComponent->SetViewport(0, 0, GameScreen::GetWidth(), GameScreen::GetHeight(), 0.0f, 1.0f);
	cameraComponent->SetScissorRect(0, 0, GameScreen::GetWidth(), GameScreen::GetHeight());
	cameraComponent->GenerateProjectionMatrix(0.01f, CAMERA_FAR, float(GameScreen::GetWidth()) / float(GameScreen::GetHeight()), 60.0f);

	m_Camera->ChangeCamera(cameraComponent);

#ifdef CHECK_ANOTHER_CAMERA
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
	//테스트 쿼드트리 터레인 생성
	m_TESTQuadGameobject = new GameObject("TESTQuad");
	m_TESTQuadTree = new QuadTreeTerrainMesh(m_TESTQuadGameobject, pd3dDevice, pd3dCommandList, 257, 257,
		xmf3Scale, xmf4Color, m_Terrain->GetHeightMapImage());
}

void GameScene::ReleaseObjects()
{
	if (m_parameterForm)
	{
		m_parameterForm->ReleaseObjects();
		delete m_parameterForm;
		m_parameterForm = nullptr;
	}
	if (m_GameObject)
	{
		m_GameObject->ReleaseObjects();
		delete m_GameObject;
		m_GameObject = nullptr;
	}
#ifdef CHECK_ANOTHER_CAMERA
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
	if (m_PickingTESTMeshs)
	{
		for (int x = 0; x < m_numPickingTESTMeshs; ++x)
		{
			// m_PickingTESTMeshs[x]->ReleaseObjects();
			delete m_PickingTESTMeshs[x];
			m_PickingTESTMeshs[x] = nullptr;
		}
		delete[] m_PickingTESTMeshs;
		m_PickingTESTMeshs = nullptr;
	}
	if (m_PickingColors)
	{
		delete[] m_PickingColors;
		m_PickingColors = nullptr;
	}
}

bool GameScene::ProcessInput(HWND hWnd, float ElapsedTime)
{
	DWORD dwDirection = 0;

	// 키보드 처리
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

	// 만약 키보드 상하좌우 움직인다면...
	if (dwDirection != 0)
	{
		AXIS axis = m_GameObject->GetTransform().GetCoorAxis();

		XMFLOAT3 xmf3Shift = XMFLOAT3(0, 0, 0); // 이동량

		/*플레이어를 dwDirection 방향으로 이동한다(실제로는 속도 벡터를 변경한다). 이동 거리는 시간에 비례하도록 한다.
		플레이어의 이동 속력은 (50/초)로 가정한다.*/
		float fDistance = 50.0f * ElapsedTime;

		if (dwDirection & DIR_FORWARD) xmf3Shift = Vector3::Add(xmf3Shift, axis.look, fDistance);
		if (dwDirection & DIR_BACKWARD) xmf3Shift = Vector3::Add(xmf3Shift, axis.look, -fDistance);
		if (dwDirection & DIR_RIGHT) xmf3Shift = Vector3::Add(xmf3Shift, axis.right, fDistance);
		if (dwDirection & DIR_LEFT) xmf3Shift = Vector3::Add(xmf3Shift, axis.right, -fDistance);
		if (dwDirection & DIR_UP) {
			xmf3Shift = Vector3::Add(xmf3Shift, axis.up, fDistance);
		}
		if (dwDirection & DIR_DOWN) xmf3Shift = Vector3::Add(xmf3Shift, axis.up, -fDistance);

		//플레이어의 이동량 벡터를 xmf3Shift 벡터만큼 더한다.
		m_GameObject->VelocityMove(xmf3Shift);
	}
	

	if ((GameInput::GetcDeltaX() != 0.0f) || (GameInput::GetcDeltaY() != 0.0f))
	{
		if (GameInput::GetcDeltaX() || GameInput::GetcDeltaY())
		{
			// 플레이어와 카메라 똑같이 rotate...
			// 순서 의존적이므로 변경 금지
			m_Camera->GetCamera()->Rotate(GameInput::GetcDeltaY(), GameInput::GetcDeltaX(), 0.0f);
			m_GameObject->GetTransform().Rotate(0.0f, GameInput::GetcDeltaX(), 0.0f);
		}
		
	}

	// 피킹을 테스트한다.
	Camera* pCamera = m_Camera->GetCamera();
	RAY pickRay;
	bool isPick = GameInput::GenerateRayforPicking(m_Camera->GetTransform().GetPosition(), pCamera->GetViewMatrix(), pCamera->GetProjectionMatrix(), pickRay);
	if (isPick)
	{
		if (m_PickingTESTMeshs)
		{
			XMFLOAT3* pickColor;
			float dist;
			for (int x = 0; x < m_numPickingTESTMeshs; ++x)
			{
				auto world = m_PickingTESTMeshs[x]->GetTransform().GetWorldMatrix();
				BoundingBox box{ XMFLOAT3(0.F, 0.F, 0.F),  XMFLOAT3(0.5F, 0.5F, 0.5F) };
				box.Transform(box, XMLoadFloat4x4(&world));
				if (box.Intersects(XMLoadFloat3(&pickRay.origin), XMLoadFloat3(&pickRay.direction), dist))
				{
					m_PickingColors[x] = XMFLOAT3(1.F, 0.F, 0.F);
				}
			}


		}
	}

	return true;
}

//
//bool GameScene::ProcessMouseWheel(HWND hWnd, short WheelData, float ElapsedTime)
//{
	//if (WheelData == MOUSE_WHEEL_STOP) return false;

	//bool isUpWheel = WheelData == MOUSE_WHEEL_UP;
	//bool isDownWheel = WheelData == MOUSE_WHEEL_DOWN;
	//if (isUpWheel) {
	//	m_Camera->ZoomIn(0.8f);
	//}
	//else if (isDownWheel) {
	//	m_Camera->ZoomOut(1.2f);
	//} 
//
//	return false;
//}

// ProcessInput에 의한 right, up, look, pos 를 월드변환 행렬에 갱신한다.
void GameScene::Update(float fElapsedTime)
{
	if (m_TESTModel)
	{
		m_TESTModel->UpdateTransform();
	}
	if (m_GameObject)
	{
		// m_GameObject->Update(m_Camera->GetComponent<FollowCam>("Camera")); //Velocity를 통해 pos 이동
		m_GameObject->Update(fElapsedTime); //Velocity를 통해 pos 이동
	}
	
	if (m_GameObject)
	{
		m_GameObject->GetTransform().Update(fElapsedTime); // right, up, look, pos에 맞춰 월드변환행렬 다시 설정
	}

	if (m_PickingTESTMeshs)
	{
		for (int x = 0; x < m_numPickingTESTMeshs; ++x)
		{
			m_PickingTESTMeshs[x]->GetTransform().Update(fElapsedTime); // right, up, look, pos에 맞춰 월드변환행렬 다시 설정
		} 
	}

	// light update
	::memcpy(m_pcbMappedLights, LightManager::m_pLights, sizeof(LIGHTS));
	// material update
	::memcpy(m_pcbMappedMaterials, m_pMaterials, sizeof(MATERIAL));
}

void GameScene::LastUpdate(float fElapsedTime)
{
#ifdef CHECK_ANOTHER_CAMERA
	if (m_lookAboveCamera)
	{
		m_lookAboveCamera->LastUpdate(fElapsedTime);
	}
#endif // CHECK_ANOTHER_CAMERA


	// player update 이후에 camera update
	if (m_Camera)
	{
		m_Camera->LastUpdate(fElapsedTime);
	} 

	// 카메라 프러스텀과 쿼드트리 지형 렌더링 체크
	m_Camera->GetFrustum()->TESTCheck(m_TESTQuadTree->GetRootNode());
	// m_Camera->GetFrustum()->TESTCheckAllTRUE(m_TESTQuadTree->GetRootNode());
}

void GameScene::TESTSetRootDescriptor(ID3D12GraphicsCommandList * pd3dCommandList)
{
	pd3dCommandList->SetDescriptorHeaps(1, &m_pd3dCbvSrvDescriptorHeap);
	m_Terrain->UpdateShaderVariables(pd3dCommandList);
}

void GameScene::TESTRenderOnGbuffer(ID3D12GraphicsCommandList * pd3dCommandList, UINT gbufferCount)
{
}

void GameScene::AnimateObjects(float fTimeElapsed)
{
	if (m_TESTModel)
	{
		m_TESTModel->Animate(fTimeElapsed);
	}
	//if (m_pHeightMapTerrain) m_pHeightMapTerrain->Animate(fTimeElapsed);
	//if (m_Player) m_Player->OnPrepareRender();
}

void GameScene::Render(ID3D12GraphicsCommandList *pd3dCommandList)
{
	RenderShadowMap(pd3dCommandList);

	// 렌더링
	extern MeshRenderer gMeshRenderer;

	// 그래픽 루트 시그니처 설정
	pd3dCommandList->SetGraphicsRootSignature(m_pd3dGraphicsRootSignature);

	// 클라 화면 설정
	m_Camera->SetViewportsAndScissorRects(pd3dCommandList); 
	m_Camera->GetCamera()->UpdateShaderVariables(pd3dCommandList, m_parameterForm->GetIndex("Camera"));

	//  조명
	D3D12_GPU_VIRTUAL_ADDRESS d3dcbLightsGpuVirtualAddress = m_pd3dcbLights->GetGPUVirtualAddress();
	pd3dCommandList->SetGraphicsRootConstantBufferView(m_parameterForm->GetIndex("Lights"), d3dcbLightsGpuVirtualAddress); //Lights

	D3D12_GPU_VIRTUAL_ADDRESS d3dcbMaterialsGpuVirtualAddress = m_pd3dcbMaterials->GetGPUVirtualAddress();
	pd3dCommandList->SetGraphicsRootConstantBufferView(m_parameterForm->GetIndex("Materials"), d3dcbMaterialsGpuVirtualAddress);

	// Terrain PSO
	pd3dCommandList->SetPipelineState(ShaderManager::GetInstance()->GetShader("Terrain")->GetPSO());
	pd3dCommandList->SetDescriptorHeaps(1, &m_pd3dCbvSrvDescriptorHeap);

	m_parameterForm->UpdateShaderVariable(pd3dCommandList, m_pd3dGraphicsRootSignature, "World", SourcePtr(&matrix));
	m_Terrain->UpdateShaderVariables(pd3dCommandList); 

	// TerrainMesh Render
	Mesh* terrainMesh = m_Terrain->GetComponent<Mesh>("TerrainMesh");
	//gMeshRenderer.Render(pd3dCommandList, terrainMesh);

	//QuadTreeTerrain test Render
	m_TESTQuadTree->TESTRender(m_TESTQuadTree->GetRootNode(), pd3dCommandList);


#ifdef CHECK_ANOTHER_CAMERA
	m_lookAboveCamera->SetViewportsAndScissorRects(pd3dCommandList); 
	m_lookAboveCamera->GetCamera()->UpdateShaderVariables(pd3dCommandList, m_parameterForm->GetIndex("Camera"));

	m_TESTQuadTree->TESTRender(m_TESTQuadTree->GetRootNode(), pd3dCommandList);
#endif

	// PSO 설정
	pd3dCommandList->SetPipelineState(ShaderManager::GetInstance()->GetShader("Standard")->GetPSO());

	// 클라 화면 설정
	m_Camera->SetViewportsAndScissorRects(pd3dCommandList);
	m_Camera->GetCamera()->UpdateShaderVariables(pd3dCommandList, m_parameterForm->GetIndex("Camera"));

	////////////////////////////// Model Render
	m_TESTModel->Render(pd3dCommandList);
	////////////////////////////// Model Render

	////////////////////////////// CubeMesh Render
	// 쉐이더 변수 설정 
	pd3dCommandList->SetPipelineState(ShaderManager::GetInstance()->GetShader("Cube")->GetPSO());
	Mesh* mesh = m_GameObject->GetComponent<Mesh>("Mesh");

	m_parameterForm->UpdateShaderVariable(pd3dCommandList, m_pd3dGraphicsRootSignature, "World", SourcePtr(&XMMatrixTranspose(XMLoadFloat4x4(&m_GameObject->GetTransform().GetWorldMatrix()))));

	gMeshRenderer.Render(pd3dCommandList, mesh);
	////////////////////////////// CubeMesh Render


	for (int x = 0; x < m_numPickingTESTMeshs; ++x)
	{
		// 쉐이더 변수 설정 
		m_parameterForm->UpdateShaderVariable(pd3dCommandList, m_pd3dGraphicsRootSignature, "World", SourcePtr(&XMMatrixTranspose(XMLoadFloat4x4(&m_PickingTESTMeshs[x]->GetTransform().GetWorldMatrix()))));
		m_parameterForm->UpdateShaderVariable(pd3dCommandList, m_pd3dGraphicsRootSignature, "Color", SourcePtr(&m_PickingColors[x]));
		// CubeMesh Render
		Mesh* mesh = m_PickingTESTMeshs[x]->GetComponent<Mesh>("Mesh");
		gMeshRenderer.Render(pd3dCommandList, mesh); 
	}
	
}

void GameScene::ReleaseUploadBuffers()
{ 
	if (m_GameObject) m_GameObject->ReleaseUploadBuffers();
	if (m_Terrain) m_Terrain->ReleaseUploadBuffers();
	if (m_TESTQuadTree) m_TESTQuadTree->ReleaseUploadBuffers();
	if (m_PickingTESTMeshs)
	{
		for (int x = 0; x < m_numPickingTESTMeshs; ++x)
		{
			m_PickingTESTMeshs[x]->ReleaseUploadBuffers();
		}
	}
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

ID3D12RootSignature* GameScene::CreateGraphicsRootSignature(ID3D12Device *pd3dDevice)
{
	HRESULT hResult = S_FALSE;
	ID3D12RootSignature* pd3dGraphicsRootSignature = nullptr;

	D3D12_ROOT_PARAMETER pRootParameters[ROOTPATAMETER_COUNT];

	m_parameterForm = new ParameterForm(_countof(pRootParameters));
	ParameterFormManager::GetInstance()->SetCurrForm(m_parameterForm);

	// 루트 상수
	pRootParameters[ROOTPATAMETER_WORLD].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	pRootParameters[ROOTPATAMETER_WORLD].Constants.Num32BitValues = 16;
	pRootParameters[ROOTPATAMETER_WORLD].Constants.ShaderRegister = 0;
	pRootParameters[ROOTPATAMETER_WORLD].Constants.RegisterSpace = 0;
	pRootParameters[ROOTPATAMETER_WORLD].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	m_parameterForm->InsertResource(ROOTPATAMETER_WORLD, "World", pRootParameters[ROOTPATAMETER_WORLD]);

	pRootParameters[ROOTPATAMETER_CAMERA].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pRootParameters[ROOTPATAMETER_CAMERA].Descriptor.ShaderRegister = 1; //Camera
	pRootParameters[ROOTPATAMETER_CAMERA].Descriptor.RegisterSpace = 0;
	pRootParameters[ROOTPATAMETER_CAMERA].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	m_parameterForm->InsertResource(ROOTPATAMETER_CAMERA, "Camera", pRootParameters[ROOTPATAMETER_CAMERA]);

	pRootParameters[ROOTPATAMETER_MATERIAL].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pRootParameters[ROOTPATAMETER_MATERIAL].Descriptor.ShaderRegister = 2; //Materials
	pRootParameters[ROOTPATAMETER_MATERIAL].Descriptor.RegisterSpace = 0;
	pRootParameters[ROOTPATAMETER_MATERIAL].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	m_parameterForm->InsertResource(ROOTPATAMETER_MATERIAL, "Materials", pRootParameters[ROOTPATAMETER_MATERIAL]);

	pRootParameters[ROOTPATAMETER_LIGHT].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pRootParameters[ROOTPATAMETER_LIGHT].Descriptor.ShaderRegister = 3; //Lights
	pRootParameters[ROOTPATAMETER_LIGHT].Descriptor.RegisterSpace = 0;
	pRootParameters[ROOTPATAMETER_LIGHT].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	m_parameterForm->InsertResource(ROOTPATAMETER_LIGHT, "Lights", pRootParameters[ROOTPATAMETER_LIGHT]);

	pRootParameters[ROOTPATAMETER_COLOR].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	pRootParameters[ROOTPATAMETER_COLOR].Constants.Num32BitValues = 3;
	pRootParameters[ROOTPATAMETER_COLOR].Constants.ShaderRegister = 4;
	pRootParameters[ROOTPATAMETER_COLOR].Constants.RegisterSpace = 0;
	pRootParameters[ROOTPATAMETER_COLOR].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	m_parameterForm->InsertResource(ROOTPATAMETER_COLOR, "Color", pRootParameters[ROOTPATAMETER_COLOR]);

	D3D12_DESCRIPTOR_RANGE pTextureDescriptorRanges[3];
	 
	pTextureDescriptorRanges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pTextureDescriptorRanges[0].NumDescriptors = 1;
	pTextureDescriptorRanges[0].BaseShaderRegister = 0; //t0: gtxtTexture
	pTextureDescriptorRanges[0].RegisterSpace = 0;
	pTextureDescriptorRanges[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	pTextureDescriptorRanges[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pTextureDescriptorRanges[1].NumDescriptors = 1;
	pTextureDescriptorRanges[1].BaseShaderRegister = 1; //t1: gtxtTerrainBaseTexture
	pTextureDescriptorRanges[1].RegisterSpace = 0;
	pTextureDescriptorRanges[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	pTextureDescriptorRanges[2].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pTextureDescriptorRanges[2].NumDescriptors = 1;
	pTextureDescriptorRanges[2].BaseShaderRegister = 2; //t2: gtxtTerrainDetailTexture
	pTextureDescriptorRanges[2].RegisterSpace = 0;
	pTextureDescriptorRanges[2].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	// 다시 루프 패러미터 
	pRootParameters[ROOTPATAMETER_TEXTURE].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pRootParameters[ROOTPATAMETER_TEXTURE].DescriptorTable.NumDescriptorRanges = 1;
	pRootParameters[ROOTPATAMETER_TEXTURE].DescriptorTable.pDescriptorRanges = &pTextureDescriptorRanges[0]; //gtxtexture
	pRootParameters[ROOTPATAMETER_TEXTURE].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	
	pRootParameters[ROOTPATAMETER_TEXTUREBASE].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pRootParameters[ROOTPATAMETER_TEXTUREBASE].DescriptorTable.NumDescriptorRanges = 1;
	pRootParameters[ROOTPATAMETER_TEXTUREBASE].DescriptorTable.pDescriptorRanges = &pTextureDescriptorRanges[1];//t4: gtxtTerrainBaseTexture
	pRootParameters[ROOTPATAMETER_TEXTUREBASE].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	
	pRootParameters[ROOTPATAMETER_TEXTUREDETAIL].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pRootParameters[ROOTPATAMETER_TEXTUREDETAIL].DescriptorTable.NumDescriptorRanges = 1;
	pRootParameters[ROOTPATAMETER_TEXTUREDETAIL].DescriptorTable.pDescriptorRanges = &pTextureDescriptorRanges[2]; 
	pRootParameters[ROOTPATAMETER_TEXTUREDETAIL].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	
	D3D12_DESCRIPTOR_RANGE pd3dDescriptorRanges[10];

	pd3dDescriptorRanges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges[0].NumDescriptors = 1;
	pd3dDescriptorRanges[0].BaseShaderRegister = 6; //t6: gtxtAlbedoTexture
	pd3dDescriptorRanges[0].RegisterSpace = 0;
	pd3dDescriptorRanges[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	pd3dDescriptorRanges[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges[1].NumDescriptors = 1;
	pd3dDescriptorRanges[1].BaseShaderRegister = 7; //t7: gtxtSpecularTexture
	pd3dDescriptorRanges[1].RegisterSpace = 0;
	pd3dDescriptorRanges[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	pd3dDescriptorRanges[2].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges[2].NumDescriptors = 1;
	pd3dDescriptorRanges[2].BaseShaderRegister = 8; //t8: gtxtNormalTexture
	pd3dDescriptorRanges[2].RegisterSpace = 0;
	pd3dDescriptorRanges[2].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	pd3dDescriptorRanges[3].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges[3].NumDescriptors = 1;
	pd3dDescriptorRanges[3].BaseShaderRegister = 9; //t9: gtxtMetallicTexture
	pd3dDescriptorRanges[3].RegisterSpace = 0;
	pd3dDescriptorRanges[3].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	pd3dDescriptorRanges[4].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges[4].NumDescriptors = 1;
	pd3dDescriptorRanges[4].BaseShaderRegister = 10; //t10: gtxtEmissionTexture
	pd3dDescriptorRanges[4].RegisterSpace = 0;
	pd3dDescriptorRanges[4].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	pd3dDescriptorRanges[5].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges[5].NumDescriptors = 1;
	pd3dDescriptorRanges[5].BaseShaderRegister = 11; //t11: gtxtEmissionTexture
	pd3dDescriptorRanges[5].RegisterSpace = 0;
	pd3dDescriptorRanges[5].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	pd3dDescriptorRanges[6].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges[6].NumDescriptors = 1;
	pd3dDescriptorRanges[6].BaseShaderRegister = 12; //t12: gtxtEmissionTexture
	pd3dDescriptorRanges[6].RegisterSpace = 0;
	pd3dDescriptorRanges[6].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	pd3dDescriptorRanges[7].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges[7].NumDescriptors = 1;
	pd3dDescriptorRanges[7].BaseShaderRegister = 13; //t13: gtxtSkyBoxTexture
	pd3dDescriptorRanges[7].RegisterSpace = 0;
	pd3dDescriptorRanges[7].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	pd3dDescriptorRanges[8].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges[8].NumDescriptors = 1;
	pd3dDescriptorRanges[8].BaseShaderRegister = 1; //t1: gtxtTerrainBaseTexture
	pd3dDescriptorRanges[8].RegisterSpace = 0;
	pd3dDescriptorRanges[8].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	pd3dDescriptorRanges[9].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges[9].NumDescriptors = 1;
	pd3dDescriptorRanges[9].BaseShaderRegister = 2; //t2: gtxtTerrainDetailTexture
	pd3dDescriptorRanges[9].RegisterSpace = 0;
	pd3dDescriptorRanges[9].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	pRootParameters[7].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pRootParameters[7].DescriptorTable.NumDescriptorRanges = 1;
	pRootParameters[7].DescriptorTable.pDescriptorRanges = &(pd3dDescriptorRanges[0]);
	pRootParameters[7].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	m_parameterForm->InsertResource(7, "Albedo", pRootParameters[7]);  //t6: gtxtAlbedoTexture

	pRootParameters[8].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pRootParameters[8].DescriptorTable.NumDescriptorRanges = 1;
	pRootParameters[8].DescriptorTable.pDescriptorRanges = &(pd3dDescriptorRanges[1]);
	pRootParameters[8].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	m_parameterForm->InsertResource(8, "Specular", pRootParameters[8]); //t7: gtxtSpecularTexture

	pRootParameters[9].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pRootParameters[9].DescriptorTable.NumDescriptorRanges = 1;
	pRootParameters[9].DescriptorTable.pDescriptorRanges = &(pd3dDescriptorRanges[2]);
	pRootParameters[9].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	m_parameterForm->InsertResource(9, "Normal", pRootParameters[9]); //t8: gtxtNormalTexture

	pRootParameters[10].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pRootParameters[10].DescriptorTable.NumDescriptorRanges = 1;
	pRootParameters[10].DescriptorTable.pDescriptorRanges = &(pd3dDescriptorRanges[3]);
	pRootParameters[10].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	m_parameterForm->InsertResource(10, "Metallic", pRootParameters[10]);//t9: gtxtMetallicTexture

	pRootParameters[11].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pRootParameters[11].DescriptorTable.NumDescriptorRanges = 1;
	pRootParameters[11].DescriptorTable.pDescriptorRanges = &(pd3dDescriptorRanges[4]);
	pRootParameters[11].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	m_parameterForm->InsertResource(11, "Emission_1", pRootParameters[11]);//t10: gtxtEmissionTexture

	pRootParameters[12].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pRootParameters[12].DescriptorTable.NumDescriptorRanges = 1;
	pRootParameters[12].DescriptorTable.pDescriptorRanges = &(pd3dDescriptorRanges[5]);
	pRootParameters[12].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	m_parameterForm->InsertResource(12, "Emission_2", pRootParameters[12]);//t12: gtxtEmissionTexture

	pRootParameters[13].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pRootParameters[13].DescriptorTable.NumDescriptorRanges = 1;
	pRootParameters[13].DescriptorTable.pDescriptorRanges = &(pd3dDescriptorRanges[6]);
	pRootParameters[13].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	m_parameterForm->InsertResource(13, "Emission_3", pRootParameters[13]);//t12: gtxtEmissionTexture

	pRootParameters[14].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pRootParameters[14].Descriptor.ShaderRegister = 7; //Skinned Bone Offsets
	pRootParameters[14].Descriptor.RegisterSpace = 0;
	pRootParameters[14].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	m_parameterForm->InsertResource(14, "SkinnedBoneOffset", pRootParameters[14]);

	pRootParameters[15].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pRootParameters[15].Descriptor.ShaderRegister = 8; //Skinned Bone Transforms
	pRootParameters[15].Descriptor.RegisterSpace = 0;
	pRootParameters[15].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	m_parameterForm->InsertResource(15, "SkinnedBoneTransform", pRootParameters[15]);
	/*
	pRootParameters[16].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pRootParameters[16].DescriptorTable.NumDescriptorRanges = 1;
	pRootParameters[16].DescriptorTable.pDescriptorRanges = &(pd3dDescriptorRanges[7]);
	pRootParameters[16].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	m_parameterForm->InsertResource(16, "SkyBox", pRootParameters[16]);*/
/*
	pRootParameters[17].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pRootParameters[17].DescriptorTable.NumDescriptorRanges = 1;
	pRootParameters[17].DescriptorTable.pDescriptorRanges = &(pd3dDescriptorRanges[8]);
	pRootParameters[17].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	m_parameterForm->InsertResource(17, "TerrainDetail", pRootParameters[17]);

	pRootParameters[18].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pRootParameters[18].DescriptorTable.NumDescriptorRanges = 1;
	pRootParameters[18].DescriptorTable.pDescriptorRanges = &(pd3dDescriptorRanges[9]);
	pRootParameters[18].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	m_parameterForm->InsertResource(18, "TerrainDetail", pRootParameters[18]);*/

	///////////////////////////////////////////////////////////////////////// 샘플러
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
	///////////////////////////////////////////////////////////////////////// 샘플러

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
	hResult = ::D3D12SerializeRootSignature(&d3dRootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1,
		&pd3dSignatureBlob, &pd3dErrorBlob);
	assert(hResult == S_OK); 

	hResult = pd3dDevice->CreateRootSignature(0, pd3dSignatureBlob->GetBufferPointer(),
		pd3dSignatureBlob->GetBufferSize(), IID_PPV_ARGS(&pd3dGraphicsRootSignature));
	assert(hResult == S_OK); 

	if (pd3dSignatureBlob) pd3dSignatureBlob->Release();
	if (pd3dErrorBlob) pd3dErrorBlob->Release();

#ifdef CHECK_ROOT_SIGNATURE
	// 루트 시그니처 셋팅
	m_parameterForm->SetRootSignature(pd3dGraphicsRootSignature);
#endif // CHECK_ROOT_SIGNATURE

	return (pd3dGraphicsRootSignature);
}

void GameScene::BuildLightsAndMaterials(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
	////////////////////////////// 조명
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


	// 쉐이더 변수 설정

	UINT ncbElementBytes = ((sizeof(LIGHTS) + 255) & ~255); //256의 배수
	m_pd3dcbLights = d3dUtil::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbElementBytes, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);

	m_pd3dcbLights->Map(0, NULL, (void **)&m_pcbMappedLights);
	////////////////////////////// 조명

	////////////////////////////// 재질
	m_pMaterials = new MATERIAL;
	::ZeroMemory(m_pMaterials, sizeof(MATERIAL));

	*m_pMaterials = { XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 5.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) };
	
	CreateConstantBuffer(pd3dDevice, pd3dCommandList, m_pd3dcbMaterials, sizeof(MATERIAL), (void **)&m_pcbMappedMaterials);

	UINT ncbMaterialBytes = ((sizeof(MATERIAL) + 255) & ~255); //256의 배수
	m_pd3dcbMaterials = d3dUtil::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbMaterialBytes, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);

	m_pd3dcbMaterials->Map(0, NULL, (void **)&m_pcbMappedMaterials);
	////////////////////////////// 재질
}

void GameScene::RenderShadowMap(ID3D12GraphicsCommandList * pd3dCommandList)
{
}
