#include "stdafx.h"
#include "d3dUtil.h"
#include "CubeMesh.h"
#include "FollowCam.h"
#include "MeshRenderer.h"
#include "ShaderManager.h"
#include "Transform.h"
#include "Shader.h"
#include "Terrain.h"
#include "GameObject.h"
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
			// 카메라 변환
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
	//뷰포트를 주 윈도우의 클라이언트 영역 전체로 설정한다.
	m_Viewport.TopLeftX = 0;
	m_Viewport.TopLeftY = 0;
	m_Viewport.Width = static_cast<float>(FRAME_BUFFER_WIDTH);
	m_Viewport.Height = static_cast<float>(FRAME_BUFFER_HEIGHT);
	m_Viewport.MinDepth = 0.0f;
	m_Viewport.MaxDepth = 1.0f;

	//씨저 사각형을 주 윈도우의 클라이언트 영역 전체로 설정한다.
	m_ScissorRect = { 0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT };

	//루트 시그너쳐를 생성한다.
	m_pd3dGraphicsRootSignature = CreateGraphicsRootSignature(pd3dDevice);
	
	// 큐브메쉬 생성
	ComponentBase* cubemesh = new CubeMesh(pd3dDevice, pd3dCommandList, 0.5, 0.5, 0.5);
	std::cout << cubemesh->GetFamillyID() << " " << cubemesh->GetComponentID() << std::endl;
	m_GameObject = new GameObject("Player");
	m_GameObject->InsertComponent(cubemesh->GetFamillyID(), cubemesh);

	// 터레인 생성 
	XMFLOAT3 xmf3Scale(8.0f, 1.0f, 8.0f);
	XMFLOAT4 xmf4Color(1.f,1.f, 1.f, 0.0f); 
	m_Terrain = new Terrain("Terrain", pd3dDevice, pd3dCommandList,
		L"Image/HeightMap.raw", 257, 257, 257, 257, xmf3Scale, xmf4Color);

	m_TerrainHeap.CreateCbvSrvUavDescriptorHeaps(pd3dDevice, pd3dCommandList, 0, 2, 0);
	// m_TerrainHeap.CreateConstantBufferViews(pd3dDevice, pd3dCommandList, 1, m_pd3dcbGameObject, ncbElementBytes);
	m_TerrainHeap.CreateShaderResourceViews(pd3dDevice, pd3dCommandList, m_Terrain->GetTexture(), 3, true);

	// 카메라
	Camera* cameraComponent = new FollowCam(m_GameObject);
	cameraComponent->Teleport(XMFLOAT3(0, 0, -5.f));
	m_Camera = new GameObject("Camera");
	m_Camera->InsertComponent(cameraComponent->GetFamillyID(), cameraComponent);

	cameraComponent->SetViewport(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, 0.0f, 1.0f);
	cameraComponent->SetScissorRect(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
	cameraComponent->GenerateProjectionMatrix(0.01f, CAMERA_FAR, float(FRAME_BUFFER_WIDTH) / float(FRAME_BUFFER_HEIGHT), 60.0f);

	// cameraComponent->CreateShaderVariables(pd3dDevice, pd3dCommandList);
	// cameraComponent->SetTarget(m_Player);

	//// 스카이 박스 생성
	//m_SkyBox = new SkyBox(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature.Get());

	//// Terrain 바닥 생성
	////XMFLOAT3 xmf3Scale(8.0f, 2.0f, 8.0f);
	//XMFLOAT3 xmf3Scale(100.0f, 10.0f, 100.0f);
	//int nWidth = 257;
	//int nLength = 257;
	//int nBlockWidth =  257;
	//int nBlockLength = 257;

	//XMFLOAT4 xmf4Color(0.0f, 1.f, 0.0f, 1.0f);
	////XMFLOAT4 xmf4Color(0.0f, 0.5f, 0.0f, 0.0f);
	//m_pHeightMapTerrain = new HeightMapTerrain(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature.Get(), _T("Image/HeightMap.raw"), 
	//	nWidth, nLength, nBlockWidth, nBlockLength, xmf3Scale, xmf4Color);
	
//  
//	
//	// 메쉬 생성
//#if defined(_WITH_TEXT_MESH) && !defined(_WITH_OBJ_MESH)
//	m_Mesh = new CCylinderDiffuse(pd3dDevice, pd3dCommandList);
//#elif !defined(_WITH_TEXT_MESH) && defined(_WITH_OBJ_MESH)
//	m_Mesh = new CMesh(pd3dDevice, pd3dCommandList, "metric_1M.obj");
//#endif
//
//	m_Player = new CPlayer();
//	m_Player->SetPosition(XMFLOAT3(nWidth/2.F * xmf3Scale.x, 1000.f, nLength/2.F * xmf3Scale.z));
//
//	Camera* m_tempCamera = new Camera();
//	
//	XMFLOAT3 CameraAt = m_Player->GetPosition();
//	XMFLOAT3 CameraOffset = XMFLOAT3(0.f, 0.f, 500.f);
//	m_tempCamera->SetAt(CameraAt);
//	m_tempCamera->SetOffset(CameraOffset);
//	m_tempCamera->SetPosition(XMFLOAT3(0.f, 0.f, -500.f));
//
//	m_tempCamera->SetViewport(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, 0.0f, 1.0f);
//	m_tempCamera->SetScissorRect(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
//	m_tempCamera->GenerateProjectionMatrix(1.01f, CAMERA_FAR, float(FRAME_BUFFER_WIDTH) / float(FRAME_BUFFER_HEIGHT), 60.0f);
//	m_tempCamera->CreateShaderVariables(pd3dDevice, pd3dCommandList);
//	m_tempCamera->SetTarget(m_Player);
//
//	m_Camera = m_tempCamera;
//
//	CameraStorage::GetInstance()->SetCamera(m_Camera);
}

void GameScene::ReleaseObjects()
{ 
	if (m_GameObject) {
		delete m_GameObject;
		m_GameObject = nullptr;
	}
	//if (m_Player) {
	//	delete m_Player;
	//	m_Player = nullptr;
	//}
	//if (m_Camera) {
	//	delete m_Camera;
	//	m_Camera = nullptr;
	//}
	//if (m_pHeightMapTerrain) {
	//	delete m_pHeightMapTerrain;
	//	m_pHeightMapTerrain = nullptr;
	//}
	//if (m_SkyBox) {
	//	delete m_SkyBox;
	//	m_SkyBox = nullptr;
	//} 
}

bool GameScene::ProcessInput(HWND hWnd, POINT OldCursor, float ElapsedTime)
{
	static UCHAR pKeyBuffer[256]; // 키 input을 위한 변수

	DWORD dwDirection = 0;
	// 키보드의 상태 정보를 반환한다.
	// 화살표 키(‘→’, ‘←’, ‘↑’, ‘↓’)를 누르면 플레이어를 오른쪽/왼쪽(로컬 x-축), 앞/
	// 뒤(로컬 z-축)로 이동한다. ‘Page Up’과 ‘Page Down’ 키를 누르면 플레이어를 위/아래(로컬 y-축)로 이동한다.*/
	if (::GetKeyboardState(pKeyBuffer))
	{
		if (pKeyBuffer[VK_UP] & 0xF0) dwDirection |= DIR_FORWARD;
		if (pKeyBuffer[VK_DOWN] & 0xF0) dwDirection |= DIR_BACKWARD;
		if (pKeyBuffer[VK_LEFT] & 0xF0) dwDirection |= DIR_LEFT;
		if (pKeyBuffer[VK_RIGHT] & 0xF0) dwDirection |= DIR_RIGHT;
		/*if (pKeyBuffer[VK_RIGHT] & 0xF0) {
			dwDirection |= DIR_UP;
		}
		if (pKeyBuffer[VK_RIGHT] & 0xF0) {
			dwDirection |= DIR_DOWN;
		}*/
		//if (pKeyBuffer[KEY_Z] & 0xF0) dwDirection |= DIR_SPECIALITEM;
		//if (pKeyBuffer[VK_CONTROL] & 0xF0) dwDirection |= DIR_SHOOT;
	}
	
	float cxDelta = 0.0f, cyDelta = 0.0f;
	POINT ptCursorPos;
	/*마우스를 캡쳐했으면 마우스가 얼마만큼 이동하였는 가를 계산한다. 마우스 왼쪽 또는 오른쪽 버튼이 눌러질 때의
	메시지(WM_LBUTTONDOWN, WM_RBUTTONDOWN)를 처리할 때 마우스를 캡쳐하였다. 그러므로 마우스가 캡쳐된
	것은 마우스 버튼이 눌려진 상태를 의미한다. 마우스 버튼이 눌려진 상태에서 마우스를 좌우 또는 상하로 움직이면 플
	레이어를 x-축 또는 y-축으로 회전한다.*/
	if (::GetCapture() == hWnd)
	{
		//마우스 커서를 화면에서 없앤다(보이지 않게 한다).
		::SetCursor(NULL);
		//현재 마우스 커서의 위치를 가져온다.
		::GetCursorPos(&ptCursorPos);
		//마우스 버튼이 눌린 상태에서 마우스가 움직인 양을 구한다.
		cxDelta = (float)(ptCursorPos.x - OldCursor.x) / 3.0f;
		cyDelta = (float)(ptCursorPos.y - OldCursor.y) / 3.0f;

		//마우스 커서의 위치를 마우스가 눌려졌던 위치로 설정한다.
		::SetCursorPos(OldCursor.x, OldCursor.y);
	}

	//마우스 또는 키 입력이 있으면 플레이어를 이동하거나(dwDirection) 회전한다(cxDelta 또는 cyDelta).
	if ((dwDirection != 0) || (cxDelta != 0.0f) || (cyDelta != 0.0f))
	{
		if (cxDelta || cyDelta)
		{
			m_GameObject->GetComponent<Transform>("")->Rotate(cyDelta, cxDelta, 0.0f);
			m_Camera->GetComponent<Camera>("Camera")->Rotate(cyDelta, cxDelta, 0.0f);

			// m_Camera->GetComponent<Camera>("Camera")->Rotate(cyDelta, cxDelta, 0.0f);

			/*cxDelta는 y-축의 회전을 나타내고 cyDelta는 x-축의 회전을 나타낸다. 오른쪽 마우스 버튼이 눌려진 경우
			cxDelta는 z-축의 회전을 나타낸다.*/
			//if (pKeyBuffer[VK_RBUTTON] & 0xF0) { 
			//	Transform* tr = m_GameObject->GetComponent<Transform>("Transform");
			//	tr->Rotate(cyDelta, 0.0f, -cxDelta);
			//}
			//else {
			//	// static_cast<Transform*>(m_GameObject->GetComponent("Transform"))->Rotate(cyDelta, cxDelta, 0.0f);

			//	// m_Camera->Rotate(cyDelta, cxDelta, 0.0f);
			//}
		}
		/*플레이어를 dwDirection 방향으로 이동한다(실제로는 속도 벡터를 변경한다). 이동 거리는 시간에 비례하도록 한다.
		플레이어의 이동 속력은 (50/초)로 가정한다.*/
		//if (dwDirection) m_GameObject->Move(dwDirection, 50.0f * ElapsedTime, true);
		if (dwDirection)
		{
			m_GameObject->GetComponent<Transform>("")->Move(dwDirection, 50.0f * ElapsedTime, false);
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

void GameScene::LastUpdate()
{
	// player update 이후에 camera update
	if (m_Camera) {
		m_Camera->GetComponent<Camera>("Camera")->Update();
	} 
}

void GameScene::AnimateObjects(float fTimeElapsed)
{
	//if (m_pHeightMapTerrain) m_pHeightMapTerrain->Animate(fTimeElapsed);
	//if (m_Player) m_Player->OnPrepareRender();
}

void GameScene::Render(ID3D12GraphicsCommandList *pd3dCommandList)
{
	// 그래픽 루트 시그니처 설정
	pd3dCommandList->SetGraphicsRootSignature(m_pd3dGraphicsRootSignature);

	// 클라 화면 설정
	D3D12_VIEWPORT GBuffer_Viewport = D3D12_VIEWPORT{ 0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT };
	D3D12_RECT ScissorRect = D3D12_RECT{ 0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT };
	pd3dCommandList->RSSetViewports(1, &GBuffer_Viewport);
	pd3dCommandList->RSSetScissorRects(1, &ScissorRect);

	// PSO 설정
	pd3dCommandList->SetPipelineState(ShaderManager::GetInstance()->GetShader("Cube")->GetPSO());
	
	// 쉐이더 변수 설정
	pd3dCommandList->SetGraphicsRoot32BitConstants(0, 16, &m_GameObject->GetComponent<Transform>("")->GetWorldMatrix(), 0);
	//pd3dCommandList->SetGraphicsRoot32BitConstants(0, 16, &matrix, 16);
	//pd3dCommandList->SetGraphicsRoot32BitConstants(0, 16, &matrix, 32);

	m_Camera->GetComponent<Camera>("Camera")->UpdateShaderVariables(pd3dCommandList);
	
	// 렌더링
	extern MeshRenderer gMeshRenderer;

	Mesh* mesh = m_GameObject->GetComponent<Mesh>("Mesh");
	gMeshRenderer.Render(pd3dCommandList, mesh);

	

	// Terrain PSO 설정 
	pd3dCommandList->SetPipelineState(ShaderManager::GetInstance()->GetShader("Terrain")->GetPSO());
	m_TerrainHeap.FirstUpdate(pd3dCommandList);

	XMFLOAT4X4 matrix = Matrix4x4::Identity();
	pd3dCommandList->SetGraphicsRoot32BitConstants(0, 16, &matrix, 0);
	m_Terrain->UpdateShaderVariables(pd3dCommandList);

	Mesh* terrainMesh = m_Terrain->GetComponent<Mesh>("TerrainMesh");
	gMeshRenderer.Render(pd3dCommandList, terrainMesh);

	//if (m_SkyBox) m_SkyBox->Render(pd3dCommandList, m_Camera);

	////if (m_Player) m_Player->Render(pd3dCommandList, m_Camera);

	//if (m_pHeightMapTerrain) m_pHeightMapTerrain->Render(pd3dCommandList, m_Camera);

	//if (m_TEST_BlendObject) m_TEST_BlendObject->Render(pd3dCommandList, m_Camera); 
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
	std::cout << "저장완료" << std::endl;
	*/
	return true;
}

ID3D12RootSignature* GameScene::CreateGraphicsRootSignature(ID3D12Device *pd3dDevice)
{
	HRESULT hResult = S_FALSE;
	ID3D12RootSignature* pd3dGraphicsRootSignature = nullptr;

	D3D12_ROOT_PARAMETER pRootParameters[5];

	// 루트 상수
	pRootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	pRootParameters[0].Constants.Num32BitValues = 16;
	pRootParameters[0].Constants.ShaderRegister = 0;
	pRootParameters[0].Constants.RegisterSpace = 0;
	pRootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pRootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	pRootParameters[1].Constants.Num32BitValues = 32;
	pRootParameters[1].Constants.ShaderRegister = 1;
	pRootParameters[1].Constants.RegisterSpace = 0;
	pRootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;

	D3D12_DESCRIPTOR_RANGE pd3dDescriptorRanges[3];
	 
	pd3dDescriptorRanges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges[0].NumDescriptors = 1;
	pd3dDescriptorRanges[0].BaseShaderRegister = 0; //t0: gtxtTexture
	pd3dDescriptorRanges[0].RegisterSpace = 0;
	pd3dDescriptorRanges[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	pd3dDescriptorRanges[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges[1].NumDescriptors = 1;
	pd3dDescriptorRanges[1].BaseShaderRegister = 1; //t4: gtxtTerrainBaseTexture
	pd3dDescriptorRanges[1].RegisterSpace = 0;
	pd3dDescriptorRanges[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	pd3dDescriptorRanges[2].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges[2].NumDescriptors = 1;
	pd3dDescriptorRanges[2].BaseShaderRegister = 2; //t5: gtxtTerrainDetailTexture
	pd3dDescriptorRanges[2].RegisterSpace = 0;
	pd3dDescriptorRanges[2].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	// 다시 루프 패러미터 
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
	 
	//// 루트 서술자
	//// 카메라 정보 (뷰, 투영)
	//pRootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	//pRootParameters[1].Constants.Num32BitValues = 16 * 2;
	//pRootParameters[1].Constants.ShaderRegister = 1;
	//pRootParameters[1].Constants.RegisterSpace = 0;
	//pRootParameters[1].ShaderVisibility =  D3D12_SHADER_VISIBILITY_ALL;

	//// SRV 쉐이더 리소스 뷰 ( 인스턴싱을 위함 )
	//pRootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_SRV;
	//pRootParameters[2].Descriptor.ShaderRegister = 0; //t0
	//pRootParameters[2].Descriptor.RegisterSpace = 0;
	//pRootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	/////////////////////////////////////////////////////////////////////////// 바닥 Terrain
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

	//// Terrain 텍스쳐를 위한 리소스(텍스쳐 뷰)
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
	///////////////////////////////////////////////////////////////////////// 바닥 Terrain


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
#if defined(_DEBUG)
	OutputDebugString(L"...D3D12SerializeRootSignature\n");
#endif

	hResult = pd3dDevice->CreateRootSignature(0, pd3dSignatureBlob->GetBufferPointer(),
		pd3dSignatureBlob->GetBufferSize(), IID_PPV_ARGS(&pd3dGraphicsRootSignature));
	assert(hResult == S_OK);
#if defined(_DEBUG)
	OutputDebugString(L"...CreateRootSignature\n");
#endif

	if (pd3dSignatureBlob) pd3dSignatureBlob->Release();
	if (pd3dErrorBlob) pd3dErrorBlob->Release();

	return (pd3dGraphicsRootSignature);
}

//void GameScene::BuildConstantBuffer(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
//{
//	// PPT 파이프라인 02 - 68p
//	HRESULT hResult = S_FALSE;
//
//	// 자원 생성
//	UINT HardwareSize = d3dUtil::CalcConstantBufferByteSize(sizeof(CB_OBJECT_INFO)); // 256의 배수
//#if defined(_DEBUG)
//	std::cout << "상수버퍼 자원 크기: " << HardwareSize << std::endl;
//#endif
//
//	// 힙 속성
//	D3D12_HEAP_PROPERTIES HeapProperties;
//	::ZeroMemory(&HeapProperties, sizeof(D3D12_HEAP_PROPERTIES));
//	HeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
//	HeapProperties.CreationNodeMask = 1;
//	HeapProperties.VisibleNodeMask = 1;
//
//	D3D12_RESOURCE_DESC ResourceDesc;
//	::ZeroMemory(&ResourceDesc, sizeof(D3D12_RESOURCE_DESC));
//	ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
//	ResourceDesc.Width = HardwareSize; // 256의 배수
//	ResourceDesc.Height = ResourceDesc.SampleDesc.Count = 1;
//	ResourceDesc.DepthOrArraySize = ResourceDesc.MipLevels = 1;
//	ResourceDesc.Format = DXGI_FORMAT_UNKNOWN;
//	ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR; // 행우선
//
//	// 자원을 생성한다.
//	hResult = pd3dDevice->CreateCommittedResource(
//		&HeapProperties,
//		D3D12_HEAP_FLAG_NONE,
//		&ResourceDesc,
//		D3D12_RESOURCE_STATE_GENERIC_READ, // 업로드힙의 초기상태
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
//	// 서술자 힙을 생성한다.
//	D3D12_DESCRIPTOR_HEAP_DESC d3dCBVHeap;
//	::ZeroMemory(&d3dCBVHeap, sizeof(D3D12_DESCRIPTOR_HEAP_DESC));
//	d3dCBVHeap.NumDescriptors = 1; // 현재 서술자는 한개만 연결한다.
//	d3dCBVHeap.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV; // 힙타입 CBV , SRV, UAV
//	d3dCBVHeap.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE; //쉐이더가 해당 자원을 볼 수 있다.
//	d3dCBVHeap.NodeMask = 0;
//	hResult = pd3dDevice->CreateDescriptorHeap(&d3dCBVHeap, IID_PPV_ARGS(&m_pd3dCBVdescriptorHeap));
//	assert(hResult == S_OK);
//#if defined(_DEBUG)
//	OutputDebugString(L"...Create CBV Descriptor Heap\n");
//#endif
//
//	////////////// 상수버퍼뷰 생성
//	// ppt 파이프라인 02 - 69p
//
//	// 상수버퍼뷰를 생성하는 것은 CPU(응용프로그램), 사용하는 것은 GPU(쉐이더)
//	D3D12_CPU_DESCRIPTOR_HANDLE d3dCPUdescriptorHandle = 
//		m_pd3dCBVdescriptorHeap->GetCPUDescriptorHandleForHeapStart();
//	D3D12_GPU_VIRTUAL_ADDRESS d3dGPUVirtualAddress = 
//		m_pd3dCBUpload->GetGPUVirtualAddress();
//
//	// 상수버퍼 뷰 구조체
//	D3D12_CONSTANT_BUFFER_VIEW_DESC CBVdesc;
//	::ZeroMemory(&CBVdesc, sizeof(D3D12_CONSTANT_BUFFER_VIEW_DESC));
//	CBVdesc.BufferLocation = d3dGPUVirtualAddress;
//	CBVdesc.SizeInBytes = HardwareSize;
//
//	pd3dDevice->CreateConstantBufferView(
//		&CBVdesc,
//		d3dCPUdescriptorHandle // 상수버퍼뷰를 포함하는 서술자 힙의 시작
//	);
//}

void GameScene::Update(float ElapsedTime)
{
	
}
