#include "stdafx.h"
#include "d3dUtil.h"
#include "CubeMesh.h"
#include "FollowCam.h"
#include "MeshRenderer.h"
#include "ShaderManager.h"
#include "Transform.h"
#include "Shader.h"
#include "Terrain.h"
#include "GameInput.h"
#include "Player.h"
#include "CameraObject.h"
#include "MyDescriptorHeap.h"
#include "RootResource.h"
//
//#include "Vertex.h"
//
//#include "ModelShader.h"
//#include "InstancingShader_StructuredBuffer.h"
//#include "Player.h"
//
//#include "InheritMesh.h"
//#include "InheritShader.h"
//#include "AlphaShader.h"
//#include "Camera.h"
//#include "HeightMapTerrain.h"
//
//#include "CameraStorage.h"

#include "GameScene.h"

#define _WITH_PLAYER_TOP

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
			// m_pHeightMapTerrain->changeGS();
			break;
		case VK_F2:
		case VK_F3:
			// ī�޶� ��ȯ
			break;

		}
		break;
	case WM_KEYDOWN:
		switch (wParam) {
//
//#ifdef _DEBUG
//		case KEY_A:
//			m_Camera->Move(Vector3::ScalarProduct(m_Camera->GetRightVector(), -100));
//			break;
//
//		case KEY_D:
//			m_Camera->Move(Vector3::ScalarProduct(m_Camera->GetRightVector(), 100));
//			break;
//
//		case KEY_W:
//			m_Camera->Move(Vector3::ScalarProduct(m_Camera->GetUpVector(), 100));
//			break;
//
//		case KEY_S:
//			m_Camera->Move(Vector3::ScalarProduct(m_Camera->GetUpVector(), -100));
//			break;
//
//		case VK_SPACE:
//			m_Camera->ShowData();
//			break;
//
//		case VK_F1:
//			SaveData();
//			break;
//
//#endif

		default:
			break;
		}
		break;
	}

	return false;
}

void GameScene::BuildObjects(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
	//����Ʈ�� �� �������� Ŭ���̾�Ʈ ���� ��ü�� �����Ѵ�.
	m_Viewport.TopLeftX = 0;
	m_Viewport.TopLeftY = 0;
	m_Viewport.Width = static_cast<float>(FRAME_BUFFER_WIDTH);
	m_Viewport.Height = static_cast<float>(FRAME_BUFFER_HEIGHT);
	m_Viewport.MinDepth = 0.0f;
	m_Viewport.MaxDepth = 1.0f;

	//���� �簢���� �� �������� Ŭ���̾�Ʈ ���� ��ü�� �����Ѵ�.
	m_ScissorRect = { 0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT };

	//��Ʈ �ñ׳��ĸ� �����Ѵ�.
	m_pd3dGraphicsRootSignature = CreateGraphicsRootSignature(pd3dDevice);
	
	// ť��޽� ����
	m_GameObject = new Player("Player");
	ComponentBase* cubemesh = new CubeMesh(m_GameObject, pd3dDevice, pd3dCommandList, 1, 1, 1);
	std::cout << cubemesh->GetFamillyID() << " " << cubemesh->GetComponentID() << std::endl;
	m_GameObject->InsertComponent(cubemesh->GetFamillyID(), cubemesh);

	// �ͷ��� ���� 
	XMFLOAT3 xmf3Scale(8.0f, 2.0f, 8.0f);
	XMFLOAT4 xmf4Color(0.0f, 0.5f, 0.0f, 0.0f);
	m_Terrain = new Terrain("Terrain", pd3dDevice, pd3dCommandList,
		L"Image/HeightMap.raw", 257, 257, 257, 257, xmf3Scale, xmf4Color);

	//// �ش� �ͷ����� �÷��̾� �ݹ����� ����
	m_GameObject->SetPlayerUpdatedContext(m_Terrain);

	m_TerrainHeap = new MyDescriptorHeap();
	m_TerrainHeap->CreateCbvSrvUavDescriptorHeaps(pd3dDevice, pd3dCommandList, 0, 2, 0);
	// m_TerrainHeap.CreateConstantBufferViews(pd3dDevice, pd3dCommandList, 1, m_pd3dcbGameObject, ncbElementBytes);
	m_TerrainHeap->CreateShaderResourceViews(pd3dDevice, pd3dCommandList, m_Terrain->GetTexture(), 3, true);

	// ī�޶�
	Camera* cameraComponent = new FollowCam(m_GameObject, m_GameObject);
	// cameraComponent->SetOffset(XMFLOAT3(0, -50.f, 50.f));
	// cameraComponent->SetOffset(XMFLOAT3(0, -30.f, 50.f));
	
	m_Camera = new CameraObject("Camera");
	m_Camera->ChangeCamera(cameraComponent);

	cameraComponent->SetViewport(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, 0.0f, 1.0f);
	cameraComponent->SetScissorRect(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
	cameraComponent->GenerateProjectionMatrix(0.01f, CAMERA_FAR, float(FRAME_BUFFER_WIDTH) / float(FRAME_BUFFER_HEIGHT), 60.0f);

}

void GameScene::ReleaseObjects()
{
	if (m_RootResource)
	{
		m_RootResource->ReleaseObjects();
		delete m_RootResource;
		m_RootResource = nullptr;
	}
	if (m_GameObject)
	{
		m_GameObject->ReleaseObjects();
		delete m_GameObject;
		m_GameObject = nullptr;
	}
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
	if (m_TerrainHeap)
	{
		delete m_TerrainHeap;
		m_TerrainHeap = nullptr;
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
		AXIS axis = m_GameObject->GetComponent<Transform>("")->GetCoorAxis();

		XMFLOAT3 xmf3Shift = XMFLOAT3(0, 0, 0); // �̵���

		/*�÷��̾ dwDirection �������� �̵��Ѵ�(�����δ� �ӵ� ���͸� �����Ѵ�). �̵� �Ÿ��� �ð��� ����ϵ��� �Ѵ�.
		�÷��̾��� �̵� �ӷ��� (50/��)�� �����Ѵ�.*/
		float fDistance = 50.0f * ElapsedTime;

		if (dwDirection & DIR_FORWARD) xmf3Shift = Vector3::Add(xmf3Shift, axis.look, fDistance);
		if (dwDirection & DIR_BACKWARD) xmf3Shift = Vector3::Add(xmf3Shift, axis.look, -fDistance);
		if (dwDirection & DIR_RIGHT) xmf3Shift = Vector3::Add(xmf3Shift, axis.right, fDistance);
		if (dwDirection & DIR_LEFT) xmf3Shift = Vector3::Add(xmf3Shift, axis.right, -fDistance);
		if (dwDirection & DIR_UP) {
			xmf3Shift = Vector3::Add(xmf3Shift, axis.up, fDistance);
		}
		if (dwDirection & DIR_DOWN) xmf3Shift = Vector3::Add(xmf3Shift, axis.up, -fDistance);

		//�÷��̾��� �̵��� ���͸� xmf3Shift ���͸�ŭ ���Ѵ�.
		m_GameObject->VelocityMove(xmf3Shift);
	}
	

	if ((GameInput::GetcDeltaX() != 0.0f) || (GameInput::GetcDeltaY() != 0.0f))
	{
		if (GameInput::GetcDeltaX() || GameInput::GetcDeltaY())
		{
			// �÷��̾�� ī�޶� �Ȱ��� rotate...
			m_GameObject->GetComponent<Transform>("")->Rotate(0.0f, GameInput::GetcDeltaX(), 0.0f);
			m_Camera->GetCamera()->Rotate(0.0f, GameInput::GetcDeltaX(), 0.0f);
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

// ProcessInput�� ���� right, up, look, pos �� ���庯ȯ ��Ŀ� �����Ѵ�.
void GameScene::Update(float fElapsedTime)
{
	if (m_GameObject)
	{
		// m_GameObject->Update(m_Camera->GetComponent<FollowCam>("Camera")); //Velocity�� ���� pos �̵�
		m_GameObject->Update(fElapsedTime); //Velocity�� ���� pos �̵�
	}
	
	if (m_GameObject)
	{
		m_GameObject->GetComponent<Transform>("")->Update(fElapsedTime); // right, up, look, pos�� ���� ���庯ȯ��� �ٽ� ����
	}
}

void GameScene::LastUpdate(float fElapsedTime)
{
	// player update ���Ŀ� camera update
	if (m_Camera)
	{
		m_Camera->LastUpdate(fElapsedTime);
	} 
}

void GameScene::AnimateObjects(float fTimeElapsed)
{
	//if (m_pHeightMapTerrain) m_pHeightMapTerrain->Animate(fTimeElapsed);
	//if (m_Player) m_Player->OnPrepareRender();
}

void GameScene::Render(ID3D12GraphicsCommandList *pd3dCommandList)
{
	// ������
	extern MeshRenderer gMeshRenderer;

	// �׷��� ��Ʈ �ñ״�ó ����
	pd3dCommandList->SetGraphicsRootSignature(m_pd3dGraphicsRootSignature);

	// Ŭ�� ȭ�� ����
	m_Camera->SetViewportsAndScissorRects(pd3dCommandList);
	m_Camera->UpdateShaderVariables(pd3dCommandList);

	// Terrain PSO
	pd3dCommandList->SetPipelineState(ShaderManager::GetInstance()->GetShader("Terrain")->GetPSO());
	m_TerrainHeap->FirstUpdate(pd3dCommandList);

	m_RootResource->UpdateShaderVariable(pd3dCommandList, m_pd3dGraphicsRootSignature, "World", SourcePtr(&matrix));
	m_Terrain->UpdateShaderVariables(pd3dCommandList); 

	// TerrainMesh Render
	Mesh* terrainMesh = m_Terrain->GetComponent<Mesh>("TerrainMesh");
	gMeshRenderer.Render(pd3dCommandList, terrainMesh);

	// PSO ����
	pd3dCommandList->SetPipelineState(ShaderManager::GetInstance()->GetShader("Cube")->GetPSO());

	// ���̴� ���� ����
	XMFLOAT4X4 xmf4x4World;
	XMStoreFloat4x4(&xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(&m_GameObject->GetComponent<Transform>("")->GetWorldMatrix())));
	m_RootResource->UpdateShaderVariable(pd3dCommandList, m_pd3dGraphicsRootSignature, "World", SourcePtr(&xmf4x4World));

	// CubeMesh Render
	Mesh* mesh = m_GameObject->GetComponent<Mesh>("Mesh");
	gMeshRenderer.Render(pd3dCommandList, mesh);
	 
}

void GameScene::ReleaseUploadBuffers()
{ 
	//if (m_Mesh) m_Mesh->ReleaseUploadBuffers();
	//if (m_pHeightMapTerrain) m_pHeightMapTerrain->ReleaseUploadBuffers();
	//if (m_SkyBox) m_SkyBox->ReleaseUploadBuffers(); 

	//m_Player->SetMesh(0, m_Mesh);
}

bool GameScene::SaveData()
{
	/*std::ofstream out("Data.txt");

	if (out.is_open()) {
		XMFLOAT3 pos = m_Camera->GetPosition();
		XMFLOAT3 right = m_Camera->GetRightVector();
		XMFLOAT3 up = m_Camera->GetUpVector();
		XMFLOAT3 look = m_Camera->GetLookVector();
		XMFLOAT3 at = m_Camera->GetAt();
		XMFLOAT3 offset = m_Camera->GetOffset();

		out << "Camera Data ... " << std::endl;
		out << "Position ... " << pos.x << " " << pos.y << " " << pos.z << std::endl;
		out << "Right ... " << right.x << " " << right.y << " " << right.z << " " << std::endl;
		out << "Up ... " << up.x << " " << up.y << " " << up.z << " " << std::endl;
		out << "Look ... " << look.x << " " << look.y << " " << look.z << " " <<  std::endl;
		out << "At ... " << at.x << " " << at.y << " " << at.z << " " << std::endl;
		out << "Offset ... " << offset.x << " " << offset.y << " " << offset.z << " " << std::endl;

	}
	std::cout << "����Ϸ�" << std::endl;
	*/
	return true;
}

ID3D12RootSignature* GameScene::CreateGraphicsRootSignature(ID3D12Device *pd3dDevice)
{
	HRESULT hResult = S_FALSE;
	ID3D12RootSignature* pd3dGraphicsRootSignature = nullptr;

	D3D12_ROOT_PARAMETER pRootParameters[5];
	m_RootResource = new RootResource(_countof(pRootParameters));

	// ��Ʈ ���
	pRootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	pRootParameters[0].Constants.Num32BitValues = 16;
	pRootParameters[0].Constants.ShaderRegister = 0;
	pRootParameters[0].Constants.RegisterSpace = 0;
	pRootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	m_RootResource->InsertResource(0, "World", pRootParameters[0]);

	pRootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	pRootParameters[1].Constants.Num32BitValues = 32;
	pRootParameters[1].Constants.ShaderRegister = 1;
	pRootParameters[1].Constants.RegisterSpace = 0;
	pRootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	m_RootResource->InsertResource(1, "ViewAndProjection", pRootParameters[0]);

	D3D12_DESCRIPTOR_RANGE pd3dDescriptorRanges[3];
	 
	pd3dDescriptorRanges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges[0].NumDescriptors = 1;
	pd3dDescriptorRanges[0].BaseShaderRegister = 0; //t0: gtxtTexture
	pd3dDescriptorRanges[0].RegisterSpace = 0;
	pd3dDescriptorRanges[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	pd3dDescriptorRanges[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges[1].NumDescriptors = 1;
	pd3dDescriptorRanges[1].BaseShaderRegister = 1; //t1: gtxtTerrainBaseTexture
	pd3dDescriptorRanges[1].RegisterSpace = 0;
	pd3dDescriptorRanges[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	pd3dDescriptorRanges[2].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges[2].NumDescriptors = 1;
	pd3dDescriptorRanges[2].BaseShaderRegister = 2; //t2: gtxtTerrainDetailTexture
	pd3dDescriptorRanges[2].RegisterSpace = 0;
	pd3dDescriptorRanges[2].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	// �ٽ� ���� �з����� 
	pRootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pRootParameters[2].DescriptorTable.NumDescriptorRanges = 1;
	pRootParameters[2].DescriptorTable.pDescriptorRanges = &pd3dDescriptorRanges[0]; //gtxtexture
	pRootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	pRootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pRootParameters[3].DescriptorTable.NumDescriptorRanges = 1;
	pRootParameters[3].DescriptorTable.pDescriptorRanges = &pd3dDescriptorRanges[1];//t4: gtxtTerrainBaseTexture
	pRootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	pRootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pRootParameters[4].DescriptorTable.NumDescriptorRanges = 1;
	pRootParameters[4].DescriptorTable.pDescriptorRanges = &pd3dDescriptorRanges[2];
	pRootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
/*
	pRootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pRootParameters[1].Constants.Num32BitValues = 32;
	pRootParameters[1].Constants.ShaderRegister = 0;
	pRootParameters[1].Constants.RegisterSpace = 1;
	pRootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;*/
	 
	//// ��Ʈ ������
	//// ī�޶� ���� (��, ����)
	//pRootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	//pRootParameters[1].Constants.Num32BitValues = 16 * 2;
	//pRootParameters[1].Constants.ShaderRegister = 1;
	//pRootParameters[1].Constants.RegisterSpace = 0;
	//pRootParameters[1].ShaderVisibility =  D3D12_SHADER_VISIBILITY_ALL;

	//// SRV ���̴� ���ҽ� �� ( �ν��Ͻ��� ���� )
	//pRootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_SRV;
	//pRootParameters[2].Descriptor.ShaderRegister = 0; //t0
	//pRootParameters[2].Descriptor.RegisterSpace = 0;
	//pRootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	/////////////////////////////////////////////////////////////////////////// �ٴ� Terrain
	//D3D12_DESCRIPTOR_RANGE pd3dDescriptorRanges[5];

	//pd3dDescriptorRanges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	//pd3dDescriptorRanges[0].NumDescriptors = 1;
	//pd3dDescriptorRanges[0].BaseShaderRegister = 1; //t1: gtxtTexture
	//pd3dDescriptorRanges[0].RegisterSpace = 0;
	//pd3dDescriptorRanges[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	//pd3dDescriptorRanges[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	//pd3dDescriptorRanges[1].NumDescriptors = 1;
	//pd3dDescriptorRanges[1].BaseShaderRegister = 2; //t2: gtxtTerrainBaseTexture
	//pd3dDescriptorRanges[1].RegisterSpace = 0;
	//pd3dDescriptorRanges[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	//pd3dDescriptorRanges[2].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	//pd3dDescriptorRanges[2].NumDescriptors = 1;
	//pd3dDescriptorRanges[2].BaseShaderRegister = 3; //t3: gtxtTerrainDetailTexture
	//pd3dDescriptorRanges[2].RegisterSpace = 0;
	//pd3dDescriptorRanges[2].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	//pd3dDescriptorRanges[3].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	//pd3dDescriptorRanges[3].NumDescriptors = 1;
	//pd3dDescriptorRanges[3].BaseShaderRegister = 4; //t4: gtxtTerrainAlphaTexture
	//pd3dDescriptorRanges[3].RegisterSpace = 0;
	//pd3dDescriptorRanges[3].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	//pd3dDescriptorRanges[4].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	//pd3dDescriptorRanges[4].NumDescriptors = 1;
	//pd3dDescriptorRanges[4].BaseShaderRegister = 5; //t4: gtxtTerrainDetailTexture_2
	//pd3dDescriptorRanges[4].RegisterSpace = 0;
	//pd3dDescriptorRanges[4].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	//// Terrain �ؽ��ĸ� ���� ���ҽ�(�ؽ��� ��)
	//pRootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	//pRootParameters[3].DescriptorTable.NumDescriptorRanges = 1;
	//pRootParameters[3].DescriptorTable.pDescriptorRanges = &pd3dDescriptorRanges[0]; //t1: gtxtTexture
	//pRootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	//pRootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	//pRootParameters[4].DescriptorTable.NumDescriptorRanges = 1;
	//pRootParameters[4].DescriptorTable.pDescriptorRanges = &pd3dDescriptorRanges[1]; //t2: gtxtTerrainBaseTexture
	//pRootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	//pRootParameters[5].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	//pRootParameters[5].DescriptorTable.NumDescriptorRanges = 1;
	//pRootParameters[5].DescriptorTable.pDescriptorRanges = &pd3dDescriptorRanges[2]; //t3: gtxtTerrainDetailTexture
	//pRootParameters[5].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	//pRootParameters[6].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	//pRootParameters[6].DescriptorTable.NumDescriptorRanges = 1;
	//pRootParameters[6].DescriptorTable.pDescriptorRanges = &pd3dDescriptorRanges[3]; // t4: gtxtTerrainAlphaTexture
	//pRootParameters[6].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	//pRootParameters[7].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	//pRootParameters[7].DescriptorTable.NumDescriptorRanges = 1;
	//pRootParameters[7].DescriptorTable.pDescriptorRanges = &pd3dDescriptorRanges[4]; // t5: gtxtTerrainDetailTexture_2
	//pRootParameters[7].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	///////////////////////////////////////////////////////////////////////// �ٴ� Terrain


	///////////////////////////////////////////////////////////////////////// G buffer
	//D3D12_DESCRIPTOR_RANGE DRange[1];

	//DRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	//DRange[0].NumDescriptors = 3;
	//DRange[0].BaseShaderRegister = 6; //Texture[]
	//DRange[0].RegisterSpace = 0;
	//DRange[0].OffsetInDescriptorsFromTableStart = 0;

	//pRootParameters[8].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	//pRootParameters[8].DescriptorTable.NumDescriptorRanges = 1;
	//pRootParameters[8].DescriptorTable.pDescriptorRanges = &DRange[0]; // t6: GBuffer Texture Array
	//pRootParameters[8].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	///////////////////////////////////////////////////////////////////////// G buffer

	///////////////////////////////////////////////////////////////////////// ���÷�
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
	///////////////////////////////////////////////////////////////////////// ���÷�

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
	// ��Ʈ �ñ״�ó ����
	m_RootResource->SetRootSignature(pd3dGraphicsRootSignature);
#endif // CHECK_ROOT_SIGNATURE

	return (pd3dGraphicsRootSignature);
}

//void GameScene::BuildConstantBuffer(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
//{
//	// PPT ���������� 02 - 68p
//	HRESULT hResult = S_FALSE;
//
//	// �ڿ� ����
//	UINT HardwareSize = d3dUtil::CalcConstantBufferByteSize(sizeof(CB_OBJECT_INFO)); // 256�� ���
//#if defined(_DEBUG)
//	std::cout << "������� �ڿ� ũ��: " << HardwareSize << std::endl;
//#endif
//
//	// �� �Ӽ�
//	D3D12_HEAP_PROPERTIES HeapProperties;
//	::ZeroMemory(&HeapProperties, sizeof(D3D12_HEAP_PROPERTIES));
//	HeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
//	HeapProperties.CreationNodeMask = 1;
//	HeapProperties.VisibleNodeMask = 1;
//
//	D3D12_RESOURCE_DESC ResourceDesc;
//	::ZeroMemory(&ResourceDesc, sizeof(D3D12_RESOURCE_DESC));
//	ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
//	ResourceDesc.Width = HardwareSize; // 256�� ���
//	ResourceDesc.Height = ResourceDesc.SampleDesc.Count = 1;
//	ResourceDesc.DepthOrArraySize = ResourceDesc.MipLevels = 1;
//	ResourceDesc.Format = DXGI_FORMAT_UNKNOWN;
//	ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR; // ��켱
//
//	// �ڿ��� �����Ѵ�.
//	hResult = pd3dDevice->CreateCommittedResource(
//		&HeapProperties,
//		D3D12_HEAP_FLAG_NONE,
//		&ResourceDesc,
//		D3D12_RESOURCE_STATE_GENERIC_READ, // ���ε����� �ʱ����
//		nullptr,
//		IID_PPV_ARGS(&m_pd3dCBUpload));
//	assert(hResult == S_OK);
//#if defined(_DEBUG)
//	OutputDebugString(L"...CreateCommittedResource\n");
//#endif
//
//	hResult = m_pd3dCBUpload->Map(0, nullptr, (void**)&m_pCBMappedObjectInfo);
//	assert(hResult == S_OK);
//#if defined(_DEBUG)
//	OutputDebugString(L"...Constant Buffer Map\n");
//#endif
//	XMFLOAT4 white{1.F, 0.F, 1.F, 1.F};
//	::memcpy(&m_pCBMappedObjectInfo->fColor, &white, sizeof(float) * 4);
//
//	// ������ ���� �����Ѵ�.
//	D3D12_DESCRIPTOR_HEAP_DESC d3dCBVHeap;
//	::ZeroMemory(&d3dCBVHeap, sizeof(D3D12_DESCRIPTOR_HEAP_DESC));
//	d3dCBVHeap.NumDescriptors = 1; // ���� �����ڴ� �Ѱ��� �����Ѵ�.
//	d3dCBVHeap.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV; // ��Ÿ�� CBV , SRV, UAV
//	d3dCBVHeap.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE; //���̴��� �ش� �ڿ��� �� �� �ִ�.
//	d3dCBVHeap.NodeMask = 0;
//	hResult = pd3dDevice->CreateDescriptorHeap(&d3dCBVHeap, IID_PPV_ARGS(&m_pd3dCBVdescriptorHeap));
//	assert(hResult == S_OK);
//#if defined(_DEBUG)
//	OutputDebugString(L"...Create CBV Descriptor Heap\n");
//#endif
//
//	////////////// ������ۺ� ����
//	// ppt ���������� 02 - 69p
//
//	// ������ۺ並 �����ϴ� ���� CPU(�������α׷�), ����ϴ� ���� GPU(���̴�)
//	D3D12_CPU_DESCRIPTOR_HANDLE d3dCPUdescriptorHandle = 
//		m_pd3dCBVdescriptorHeap->GetCPUDescriptorHandleForHeapStart();
//	D3D12_GPU_VIRTUAL_ADDRESS d3dGPUVirtualAddress = 
//		m_pd3dCBUpload->GetGPUVirtualAddress();
//
//	// ������� �� ����ü
//	D3D12_CONSTANT_BUFFER_VIEW_DESC CBVdesc;
//	::ZeroMemory(&CBVdesc, sizeof(D3D12_CONSTANT_BUFFER_VIEW_DESC));
//	CBVdesc.BufferLocation = d3dGPUVirtualAddress;
//	CBVdesc.SizeInBytes = HardwareSize;
//
//	pd3dDevice->CreateConstantBufferView(
//		&CBVdesc,
//		d3dCPUdescriptorHandle // ������ۺ並 �����ϴ� ������ ���� ����
//	);
//}
