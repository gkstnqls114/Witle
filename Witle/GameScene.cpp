#include "stdafx.h"
#include "d3dUtil.h"

//// Game Base ////////////////////////////
#include "GameScreen.h"
#include "GameTimer.h"
//// Game Base ////////////////////////////

//// Skill header //////////////////////////
#include "WideareaMagic.h"
#include "Sniping.h" 
#include "Broom.h"
#include "CylinderMesh.h"
#include "LineSphere.h"
//// Skill header //////////////////////////

//// GameObject header //////////////////////////
#include "SkyBox.h"
#include "Widget.h"
#include "AltarSphere.h"
//// GameObject header //////////////////////////

//// 매니저 관련 헤더 //////////////////////////
#include "MainCameraMgr.h"
#include "GraphicsRootSignatureMgr.h"
#include "MonsterTransformStorage.h"
#include "ModelStorage.h"
#include "LightManager.h"
#include "MeshRenderer.h"
#include "ShaderManager.h"
#include "StaticObjectStorage.h" 
#include "PlayerManager.h"
#include "MainCameraMgr.h"
#include "SoundManager.h"
//// 매니저 관련 헤더 //////////////////////////

//// 몬스터 관련 헤더 //////////////////////////
#include "SpaceCat.h"
#include "Mushroom.h"
#include "Dragon.h"
#include "CreepyMonster.h"
#include "MonsterStatus.h"
//// 몬스터 관련 헤더 //////////////////////////

//// 플레이어 관련 헤더 //////////////////////////
#include "Player.h"
#include "PlayerStatus.h"
#include "PlayerSkillMgr.h"
//// 플레이어 관련 헤더 //////////////////////////

// 나중에 코드 수정 
#include "SkillSelectScene.h"

#include "MyBOBox.h"
#include "SkillEffect.h"
#include "Collision.h"
#include "Status.h"
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
#include "CameraObject.h"
#include "QuadTreeTerrain.h"
#include"MyDescriptorHeap.h"
#include "BasicCam.h" 

#include "UI2DImage.h"
#include "GameScene.h"

UINT				    	GameScene::m_ShadowmapCount{ 2 };
std::list<Texture*>         GameScene::m_pConnectedTextureList;
ID3D12DescriptorHeap*		GameScene::m_pd3dCbvSrUavDescriptorHeap { nullptr };
UINT	   		            GameScene::m_TextureCount{ 0 };

D3D12_CPU_DESCRIPTOR_HANDLE	GameScene::m_hCPUShadowmap;
D3D12_GPU_DESCRIPTOR_HANDLE	GameScene::m_hGPUShadowmap;
D3D12_CPU_DESCRIPTOR_HANDLE	GameScene::m_hCPUPlayerShadowmap;
D3D12_GPU_DESCRIPTOR_HANDLE	GameScene::m_hGPUPlayerShadowmap;

D3D12_CPU_DESCRIPTOR_HANDLE	GameScene::m_CbvCPUDescriptorStartHandle;
D3D12_GPU_DESCRIPTOR_HANDLE	GameScene::m_CbvGPUDescriptorStartHandle;
D3D12_CPU_DESCRIPTOR_HANDLE	GameScene::m_SrvCPUDescriptorStartHandle;
D3D12_GPU_DESCRIPTOR_HANDLE	GameScene::m_SrvGPUDescriptorStartHandle;
D3D12_CPU_DESCRIPTOR_HANDLE	GameScene::m_UavCPUDescriptorStartHandle;
D3D12_GPU_DESCRIPTOR_HANDLE	GameScene::m_UavGPUDescriptorStartHandle;

D3D12_CPU_DESCRIPTOR_HANDLE	GameScene::m_CbvCPUDescriptorNextHandle;
D3D12_GPU_DESCRIPTOR_HANDLE	GameScene::m_CbvGPUDescriptorNextHandle;
D3D12_CPU_DESCRIPTOR_HANDLE	GameScene::m_SrvCPUDescriptorNextHandle;
D3D12_GPU_DESCRIPTOR_HANDLE	GameScene::m_SrvGPUDescriptorNextHandle;
D3D12_CPU_DESCRIPTOR_HANDLE	GameScene::m_UavCPUDescriptorNextHandle;
D3D12_GPU_DESCRIPTOR_HANDLE	GameScene::m_UavGPUDescriptorNextHandle;

GameScene::GameScene()
{

}

GameScene::~GameScene()
{

}

void GameScene::ConnectTexture(Texture * pTexture)
{
	assert(pTexture != nullptr);

	m_pConnectedTextureList.push_back(pTexture);
	m_TextureCount += pTexture->GetTextures();
}

void GameScene::CreateCbvSrvDescriptorHeaps(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
{
	// 연결된 텍스쳐 개수만큼 힙 설정
	UINT nConstantBufferViews = 0;
	UINT nShaderResourceViews = m_TextureCount + m_ShadowmapCount;
	UINT nUnorderedAcessViews = 0;

	D3D12_DESCRIPTOR_HEAP_DESC d3dDescriptorHeapDesc;
	d3dDescriptorHeapDesc.NumDescriptors = nConstantBufferViews + nShaderResourceViews + nUnorderedAcessViews; //CBVs + SRVs + UAVs 
	d3dDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	d3dDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	d3dDescriptorHeapDesc.NodeMask = 0;
	pd3dDevice->CreateDescriptorHeap(&d3dDescriptorHeapDesc, __uuidof(ID3D12DescriptorHeap), (void **)&m_pd3dCbvSrUavDescriptorHeap);

	m_CbvCPUDescriptorStartHandle = m_pd3dCbvSrUavDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	m_CbvGPUDescriptorStartHandle = m_pd3dCbvSrUavDescriptorHeap->GetGPUDescriptorHandleForHeapStart();

	m_SrvCPUDescriptorStartHandle.ptr = m_CbvCPUDescriptorStartHandle.ptr + (d3dUtil::gnCbvSrvDescriptorIncrementSize * nConstantBufferViews);
	m_SrvGPUDescriptorStartHandle.ptr = m_CbvGPUDescriptorStartHandle.ptr + (d3dUtil::gnCbvSrvDescriptorIncrementSize * nConstantBufferViews);
	
	m_UavCPUDescriptorStartHandle.ptr = m_SrvCPUDescriptorStartHandle.ptr + (d3dUtil::gnCbvSrvDescriptorIncrementSize * nShaderResourceViews);
	m_UavGPUDescriptorStartHandle.ptr = m_SrvGPUDescriptorStartHandle.ptr + (d3dUtil::gnCbvSrvDescriptorIncrementSize * nShaderResourceViews);
	
	m_CbvCPUDescriptorNextHandle = m_CbvCPUDescriptorStartHandle;
	m_CbvGPUDescriptorNextHandle = m_CbvGPUDescriptorStartHandle;
	m_SrvCPUDescriptorNextHandle = m_SrvCPUDescriptorStartHandle;
	m_SrvGPUDescriptorNextHandle = m_SrvGPUDescriptorStartHandle;
	m_UavCPUDescriptorNextHandle = m_UavCPUDescriptorStartHandle;
	m_UavGPUDescriptorNextHandle = m_UavGPUDescriptorStartHandle;
	// 연결된 텍스쳐 개수만큼 힙 설정

	// 연결된 텍스쳐에 뷰 설정
	int index = 0;
	for (auto& pTexture : m_pConnectedTextureList)
	{
#if _DEBUG
		std::cout << index++ << "  :  ";
#endif // _DEBUG

		CreateShaderResourceViewsForTextureBase(pd3dDevice, pTexture);
	}
}

void GameScene::CreateSrvDescriptorHeapsForPlayerShadowmap(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ID3D12Resource* pShadowmap)
{ 
	D3D12_CPU_DESCRIPTOR_HANDLE d3dSrvCPUDescriptorHandle = m_SrvCPUDescriptorStartHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE d3dSrvGPUDescriptorHandle = m_SrvGPUDescriptorStartHandle;

	d3dSrvCPUDescriptorHandle.ptr = d3dSrvCPUDescriptorHandle.ptr + (m_TextureCount + m_ShadowmapCount - 1) * d3dUtil::gnCbvSrvDescriptorIncrementSize;
	d3dSrvGPUDescriptorHandle.ptr = d3dSrvGPUDescriptorHandle.ptr + (m_TextureCount + m_ShadowmapCount - 1) * d3dUtil::gnCbvSrvDescriptorIncrementSize;

	ID3D12Resource *pShaderResource = pShadowmap;
	D3D12_RESOURCE_DESC d3dResourceDesc = pShaderResource->GetDesc();
	d3dResourceDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	D3D12_SHADER_RESOURCE_VIEW_DESC d3dShaderResourceViewDesc = d3dUtil::GetShaderResourceViewDesc(d3dResourceDesc, RESOURCE_TEXTURE2D);
	pd3dDevice->CreateShaderResourceView(pShaderResource, &d3dShaderResourceViewDesc, d3dSrvCPUDescriptorHandle);

	m_hCPUPlayerShadowmap = d3dSrvCPUDescriptorHandle;
	m_hGPUPlayerShadowmap = d3dSrvGPUDescriptorHandle;
}

void GameScene::CreateSrvDescriptorHeapsForShadowmap(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ID3D12Resource* pShadowmap)
{
	D3D12_CPU_DESCRIPTOR_HANDLE d3dSrvCPUDescriptorHandle = m_SrvCPUDescriptorStartHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE d3dSrvGPUDescriptorHandle = m_SrvGPUDescriptorStartHandle;

	d3dSrvCPUDescriptorHandle.ptr = d3dSrvCPUDescriptorHandle.ptr + (m_TextureCount + m_ShadowmapCount - 2) * d3dUtil::gnCbvSrvDescriptorIncrementSize;
	d3dSrvGPUDescriptorHandle.ptr = d3dSrvGPUDescriptorHandle.ptr + (m_TextureCount + m_ShadowmapCount - 2) * d3dUtil::gnCbvSrvDescriptorIncrementSize;

	ID3D12Resource *pShaderResource = pShadowmap;
	D3D12_RESOURCE_DESC d3dResourceDesc = pShaderResource->GetDesc();
	d3dResourceDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	D3D12_SHADER_RESOURCE_VIEW_DESC d3dShaderResourceViewDesc = d3dUtil::GetShaderResourceViewDesc(d3dResourceDesc, RESOURCE_TEXTURE2D);
	pd3dDevice->CreateShaderResourceView(pShaderResource, &d3dShaderResourceViewDesc, d3dSrvCPUDescriptorHandle);

	m_hCPUShadowmap = d3dSrvCPUDescriptorHandle;
	m_hGPUShadowmap = d3dSrvGPUDescriptorHandle;
}
 
void GameScene::CreateShaderResourceViewsForTextureBase(ID3D12Device * pd3dDevice, Texture * pTexture)
{ 
	pTexture->ShowPath();

	D3D12_CPU_DESCRIPTOR_HANDLE d3dSrvCPUDescriptorHandle = m_SrvCPUDescriptorNextHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE d3dSrvGPUDescriptorHandle = m_SrvGPUDescriptorNextHandle;
	 
	int nTextures = pTexture->GetTextures();
	int nTextureType = pTexture->GetTextureType();
	for (int i = 0; i < nTextures; i++)
	{
		ID3D12Resource *pShaderResource = pTexture->GetTexture(i);
		D3D12_RESOURCE_DESC d3dResourceDesc = pShaderResource->GetDesc();
		D3D12_SHADER_RESOURCE_VIEW_DESC d3dShaderResourceViewDesc = d3dUtil::GetShaderResourceViewDesc(d3dResourceDesc, nTextureType);
		pd3dDevice->CreateShaderResourceView(pShaderResource, &d3dShaderResourceViewDesc, d3dSrvCPUDescriptorHandle);

		pTexture->SethGPU(i, d3dSrvGPUDescriptorHandle);

		d3dSrvCPUDescriptorHandle.ptr += d3dUtil::gnCbvSrvDescriptorIncrementSize;
		d3dSrvGPUDescriptorHandle.ptr += d3dUtil::gnCbvSrvDescriptorIncrementSize;
	}

	m_SrvCPUDescriptorNextHandle = d3dSrvCPUDescriptorHandle;
	m_SrvGPUDescriptorNextHandle = d3dSrvGPUDescriptorHandle;
}
 

D3D12_GPU_DESCRIPTOR_HANDLE GameScene::CreateConstantBufferViews(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, int nConstantBufferViews, ID3D12Resource * pd3dConstantBuffers, UINT nStride)
{
	D3D12_GPU_DESCRIPTOR_HANDLE d3dCbvGPUDescriptorHandle = m_CbvGPUDescriptorNextHandle;
	D3D12_GPU_VIRTUAL_ADDRESS d3dGpuVirtualAddress = pd3dConstantBuffers->GetGPUVirtualAddress();
	D3D12_CONSTANT_BUFFER_VIEW_DESC d3dCBVDesc;
	d3dCBVDesc.SizeInBytes = nStride;
	for (int j = 0; j < nConstantBufferViews; j++)
	{
		d3dCBVDesc.BufferLocation = d3dGpuVirtualAddress + (nStride * j);
		m_CbvCPUDescriptorNextHandle.ptr = m_CbvCPUDescriptorNextHandle.ptr + d3dUtil::gnCbvSrvDescriptorIncrementSize;
		pd3dDevice->CreateConstantBufferView(&d3dCBVDesc, m_CbvCPUDescriptorNextHandle);
		m_CbvGPUDescriptorNextHandle.ptr = m_CbvGPUDescriptorNextHandle.ptr + d3dUtil::gnCbvSrvDescriptorIncrementSize;
	}
	return(d3dCbvGPUDescriptorHandle);
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
			// 플레이어 1으로 변경
			break;
		case VK_F2:
			// 플레이어 2으로 변경
			break;
		case VK_F3:
			m_isSkyMode = !m_isSkyMode;
			break;

		}
		break;
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_F2:
			MyBOBox::CHANGEMODE();
			Monster::CHANGEMODE();
			break;

		case 'L':
			break;

		case 'Z':
			// 임시로 체력 100씩 닳게 설정
			m_pPlayer->SubstractHP(100);
			break;

		case '1': // 스킬 빗자루  
			SoundManager::GetInstance()->Play(ENUM_SOUND::BROOM);
			m_pPlayer->UseSkill_Broom();
			// SoundManager::GetInstance()->Stop(ENUM_SOUND::BROOM);
			break;

		case '2':
			// 스나이핑 스킬
			if (m_Sniping->GetisUsing())
			{
				m_Sniping->DoNotUse();
			}
			else
			{
				m_Sniping->DoUse();
			}
			break;

		case '3': // 스킬 광역 공격
			// m_WideareaMagic->DoUse(); 
			// m_WideareaMagic->SetPosition(m_pPlayer->GetTransform().GetPosition());
			break;

		case VK_SPACE:
			// 
			break;

		case MYVK_E:
			// 플레이어 스킬 매니저에서 파이어볼 스킬 활성화
			PlayerSkillMgr::GetInstance()->Activate(m_pPlayer->GetMPStatus(), 0);
			break;

		case MYVK_R:
			// 플레이어 스킬 매니저에서 파이어볼 스킬 활성화
			PlayerSkillMgr::GetInstance()->Activate(m_pPlayer->GetMPStatus(), 1);
			break;

		case MYVK_T:
			// 플레이어 스킬 매니저에서 파이어볼 스킬 활성화
			PlayerSkillMgr::GetInstance()->Activate(m_pPlayer->GetMPStatus(), 2);
			break;

		case MYVK_Y:
			// 플레이어 스킬 매니저에서 파이어볼 스킬 활성화
			PlayerSkillMgr::GetInstance()->Activate(m_pPlayer->GetMPStatus(), 3);
			break;
		case '4':
			break;

		case '6':
			break;

		case '5':
			// 모든 제단 활성화 시키는 치트
			for (int x = 0; x < 5; ++x)
			{
				m_AltarSphere[x]->SetisActive(true);
			}
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
	 
	m_AimPoint = new AimPoint("AimPoint", pd3dDevice, pd3dCommandList, POINT{ int(GameScreen::GetWidth()) / 2, int(GameScreen::GetHeight()) / 2 }, 100.f, 100.f, L"Image/AimPoint.dds");
	// m_WideareaMagic = new WideareaMagic(pd3dDevice, pd3dCommandList);

	// 시작 음악 -> 나중에 시작씬으로 옮기기
	// SoundManager::GetInstance()->Play(ENUM_SOUND::SATRT);
	//SoundManager::GetInstance()->Stop(ENUM_SOUND::SATRT);
	// 배경음악
	// SoundManager::GetInstance()->Play(ENUM_SOUND::TITLE);

	//// 스카이 박스 생성
	m_SkyBox = new SkyBox(pd3dDevice, pd3dCommandList, 3000.F, 3000.F, 3000.F);
	// 스카이 박스 생성 ///////////////////////////////////

	// 터레인 생성 ////////////////////////////////////////
	XMFLOAT3 xmf3Scale(39.0625f * 3.f, 1.0f, 39.0625f * 3.f);
	// XMFLOAT3 xmf3Scale(1.f, 1.0f, 1.f);
	XMFLOAT4 xmf4Color(0.0f, 0.5f, 0.0f, 0.0f);
	m_Terrain = new Terrain("Terrain", pd3dDevice, pd3dCommandList, L"Image/HeightMap.raw", 257, 257, 257, 257, xmf3Scale, xmf4Color);
	// 터레인 생성 ////////////////////////////////////////

	// 플레이어 관련 ////////////////////////////////////////
	m_pPlayer = new Player("Player", pd3dDevice, pd3dCommandList, GraphicsRootSignatureMgr::GetGraphicsRootSignature());
	m_SkyBox->SetpPlayerTransform(&m_pPlayer->GetTransform());
	PlayerManager::SetMainPlayer(m_pPlayer);
	// 플레이어 관련 ////////////////////////////////////////

	// 테스트용  
	std::random_device rd;
	std::mt19937 mersenne(rd());
	std::uniform_int_distribution<> die(2000, 15000);
	std::uniform_int_distribution<> monstertype(0, 5);

	m_Dragon = new Dragon("Dragon",
		XMFLOAT3(15000, 0, 15000),
		pd3dDevice, pd3dCommandList, GraphicsRootSignatureMgr::GetGraphicsRootSignature());

	m_TestMonster = new Monster*[m_TestMonsterCount];
	 
	int spacecatblue_count = 0;
	int spacecatgreen_count = 0;
	int spacecatpink_count = 0;
	int creepymonster_count = 0;
	int mushroom_count = 0;
	int boss_count = 0;

	MonsterTransformStorage* instance = MonsterTransformStorage::GetInstance();
	instance->CreateInfo(pd3dDevice, pd3dCommandList);

	for (int i = 0; i < m_TestMonsterCount; )
	{
		int value = monstertype(mersenne);

		if (value == ENUM_MONSTER::MONSTER_CREEPYMONSTER)
		{
#ifdef _DEBUG
			std::cout << instance->Count(CREEPYMONSTER) << "마리가 최대입니다. 현재 " << creepymonster_count << "마리 "<< std::endl;
#endif // _DEBUG

			if (instance->Count(CREEPYMONSTER) <= creepymonster_count)continue;
			m_TestMonster[i] = new CreepyMonster("CreepyMonster",
				instance->GetPosition(creepymonster_count, CREEPYMONSTER),
				pd3dDevice, pd3dCommandList, GraphicsRootSignatureMgr::GetGraphicsRootSignature());
			creepymonster_count += 1;
		}
		else if (value == ENUM_MONSTER::MONSTER_MUSHROOM)
		{

#ifdef _DEBUG
			std::cout << instance->Count(MUSHROOM) << "마리가 최대입니다. 현재 " << mushroom_count << "마리 " << std::endl;
#endif // _DEBUG
			if (instance->Count(MUSHROOM) <= mushroom_count)continue;

			m_TestMonster[i] = new Mushroom("Mushroom",
				instance->GetPosition(mushroom_count, MUSHROOM),
				pd3dDevice, pd3dCommandList, GraphicsRootSignatureMgr::GetGraphicsRootSignature());
			mushroom_count += 1;
		}
		else if (value == ENUM_MONSTER::MONSTER_SPACECATBLUE)
		{ 
#ifdef _DEBUG
			std::cout << instance->Count(SPACECAT_BLUE) << "마리가 최대입니다. 현재 " << spacecatblue_count << "마리 " << std::endl;
#endif // _DEBUG
			if (instance->Count(SPACECAT_BLUE) <= spacecatblue_count)continue;
			m_TestMonster[i] = new SpaceCat("SpaceCat",
				instance->GetPosition(spacecatblue_count, SPACECAT_BLUE),
				pd3dDevice, pd3dCommandList, GraphicsRootSignatureMgr::GetGraphicsRootSignature());
			spacecatblue_count += 1;
		}
		else if (value == ENUM_MONSTER::MONSTER_SPACECATPINK)
		{ 
#ifdef _DEBUG
			std::cout << instance->Count(SPACECAT_PINK) << "마리가 최대입니다. 현재 " << spacecatpink_count << "마리 " << std::endl;
#endif // _DEBUG
			if (instance->Count(SPACECAT_PINK) <= spacecatpink_count)continue;
			m_TestMonster[i] = new SpaceCat("SpaceCat",
				instance->GetPosition(spacecatpink_count, SPACECAT_PINK),
				pd3dDevice, pd3dCommandList, GraphicsRootSignatureMgr::GetGraphicsRootSignature());
			spacecatpink_count += 1;
		}
		else if (value == ENUM_MONSTER::MONSTER_SPACECATGREEN)
		{ 
#ifdef _DEBUG
			std::cout << instance->Count(SPACECAT_GREEN) << "마리가 최대입니다. 현재 " << spacecatgreen_count << "마리 " << std::endl;
#endif // _DEBUG
			if (instance->Count(SPACECAT_GREEN) <= spacecatgreen_count)continue;
			m_TestMonster[i] = new SpaceCat("SpaceCat",
				instance->GetPosition(spacecatgreen_count, SPACECAT_GREEN),
				pd3dDevice, pd3dCommandList, GraphicsRootSignatureMgr::GetGraphicsRootSignature());
			spacecatgreen_count += 1;
		}
		else if (value == ENUM_MONSTER::MONSTER_BOSSMONSTER)
		{ 
#ifdef _DEBUG
			std::cout << instance->Count(DRAGON) << "마리가 최대입니다. 현재 " << boss_count << "마리 " << std::endl;
#endif // _DEBUG
			if (instance->Count(DRAGON) <= boss_count)continue;
			m_TestMonster[i] = new Dragon("Dragon",
				instance->GetPosition(boss_count, DRAGON),
				pd3dDevice, pd3dCommandList, GraphicsRootSignatureMgr::GetGraphicsRootSignature());
			boss_count += 1;

			if (boss_count == 1)
			{
				continue;
			}
		}
		else
		{
			assert(false);
		}

		i += 1;
	}

	//// 테스트 쿼드트리 터레인 생성 
	m_pQuadtreeTerrain = new QuadtreeTerrain(pd3dDevice, pd3dCommandList, 257, 257, xmf3Scale, xmf4Color, m_Terrain->GetHeightMapImage());

	// 카메라
	m_pMainCamera = new CameraObject("Camera");
	m_Sniping = new Sniping(m_pMainCamera, m_pPlayer, pd3dDevice, pd3dCommandList);
	m_pPlayer->SetSniping(m_Sniping);
	m_pMainCamera->ChangeCamera(m_Sniping->GetBaseCamera());
	MainCameraMgr::SetMainPlayer(m_pMainCamera);

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
	 
	// UI 이미지 추가
	m_TESTGameObject = new EmptyGameObject("Test");

	// 미니맵
	m_SampleUIMap = new UI2DImage(
		m_TESTGameObject, ENUM_SCENE::SCENE_GAME, pd3dDevice, pd3dCommandList,
		POINT{ int(GameScreen::GetWidth()) / 2 + 500, int(GameScreen::GetHeight()) / 2 - 220 },
		m_UIMapSize.x,
		m_UIMapSize.y,
		nullptr
	);

	// 스킬 1
	m_SampleUISkill1 = new UI2DImage( 
		m_TESTGameObject, ENUM_SCENE::SCENE_GAME, pd3dDevice, pd3dCommandList,
		POINT{ int(GameScreen::GetWidth()) / 2 - 300, int(GameScreen::GetHeight()) / 2 + 270 }, 100, 100,
		nullptr
	);

	// 스킬 2
	m_SampleUISkill2 = new UI2DImage(
		m_TESTGameObject, ENUM_SCENE::SCENE_GAME, pd3dDevice, pd3dCommandList,
		POINT{ int(GameScreen::GetWidth()) / 2 - 100, int(GameScreen::GetHeight()) / 2 + 270 }, 100, 100,
		nullptr
	);

	// 스킬 3
	m_SampleUISkill3 = new UI2DImage(
		m_TESTGameObject, ENUM_SCENE::SCENE_GAME, pd3dDevice, pd3dCommandList,
		POINT{ int(GameScreen::GetWidth()) / 2 + 100, int(GameScreen::GetHeight()) / 2 + 270 }, 100, 100,
		nullptr
	);

	// 스킬 4
	m_SampleUISkill4 = new UI2DImage(
		m_TESTGameObject, ENUM_SCENE::SCENE_GAME, pd3dDevice, pd3dCommandList,
		POINT{ int(GameScreen::GetWidth()) / 2 + 300, int(GameScreen::GetHeight()) / 2 + 270 }, 100, 100,
		nullptr
	);

	XMFLOAT4X4 tr;
	tr = StaticObjectStorage::GetInstance()->GetAltarTransform(0, ALTAR_IN);
	XMFLOAT3 pos{ tr._41, 0 , tr._43 }; // 월드 포지션
	m_AltarSphere[0] = new AltarSphere("AltarSphere0", pos, pd3dDevice, pd3dCommandList, GraphicsRootSignatureMgr::GetGraphicsRootSignature());

	// ui map 포지션으로 변경하기 위해 크기 축소
	pos.x = pos.x * (float(m_UIMapSize.x) / 30000.f);
	pos.z = -pos.z * (float(m_UIMapSize.y) / 30000.f); // 스크린 좌표계로 이동하기 위해

	// 스크린 좌표계 위치 이동
	pos.x += m_SampleUIMap->getRect().left;
	pos.z += m_SampleUIMap->getRect().bottom;

	m_UIAltar_1 = new UI2DImage(
		m_TESTGameObject, ENUM_SCENE::SCENE_GAME, pd3dDevice, pd3dCommandList,
		POINT{ int(pos.x), int(pos.z) },
		20, 20,
		L"Image/Red.dds"
	);

	tr = StaticObjectStorage::GetInstance()->GetAltarTransform(1, ALTAR_IN);
	pos = XMFLOAT3{ tr._41, 0 , tr._43 };
	m_AltarSphere[1] = new AltarSphere("AltarSphere0", pos, pd3dDevice, pd3dCommandList, GraphicsRootSignatureMgr::GetGraphicsRootSignature());

	pos.x = pos.x * (float(m_UIMapSize.x) / 30000.f);
	pos.z = -pos.z * (float(m_UIMapSize.y) / 30000.f);
	pos.x += m_SampleUIMap->getRect().left;
	pos.z += m_SampleUIMap->getRect().bottom;
	m_UIAltar_2 = new UI2DImage(
		m_TESTGameObject, ENUM_SCENE::SCENE_GAME, pd3dDevice, pd3dCommandList,
		POINT{ int(pos.x), int(pos.z) },
		20, 20,
		L"Image/Red.dds"
	);

	tr = StaticObjectStorage::GetInstance()->GetAltarTransform(2, ALTAR_IN);
	pos = XMFLOAT3{ tr._41,0 , tr._43 };
	m_AltarSphere[2] = new AltarSphere("AltarSphere0", pos, pd3dDevice, pd3dCommandList, GraphicsRootSignatureMgr::GetGraphicsRootSignature());

	// ui map 포지션으로 변경하기 위해 크기 축소
	pos.x = pos.x * (float(m_UIMapSize.x) / 30000.f);
	pos.z = -pos.z * (float(m_UIMapSize.y) / 30000.f); // 스크린 좌표계로 이동하기 위해

	// 스크린 좌표계 위치 이동
	pos.x += m_SampleUIMap->getRect().left;
	pos.z += m_SampleUIMap->getRect().bottom;
	m_UIAltar_3 = new UI2DImage(
		m_TESTGameObject, ENUM_SCENE::SCENE_GAME, pd3dDevice, pd3dCommandList,
		POINT{ int(pos.x), int(pos.z) },
		20, 20,
		L"Image/Red.dds"
	);

	tr = StaticObjectStorage::GetInstance()->GetAltarTransform(3, ALTAR_IN);
	pos = XMFLOAT3{ tr._41, 0 , tr._43 };
	m_AltarSphere[3] = new AltarSphere("AltarSphere0", pos, pd3dDevice, pd3dCommandList, GraphicsRootSignatureMgr::GetGraphicsRootSignature());

	// ui map 포지션으로 변경하기 위해 크기 축소
	pos.x = pos.x * (float(m_UIMapSize.x) / 30000.f);
	pos.z = -pos.z * (float(m_UIMapSize.y) / 30000.f); // 스크린 좌표계로 이동하기 위해

	// 스크린 좌표계 위치 이동
	pos.x += m_SampleUIMap->getRect().left;
	pos.z += m_SampleUIMap->getRect().bottom;
	m_UIAltar_4 = new UI2DImage(
		m_TESTGameObject, ENUM_SCENE::SCENE_GAME, pd3dDevice, pd3dCommandList,
		POINT{ int(pos.x), int(pos.z) },
		20, 20,
		L"Image/Red.dds"
	);

	tr = StaticObjectStorage::GetInstance()->GetAltarTransform(4, ALTAR_IN);
	pos = XMFLOAT3{ tr._41, 0 , tr._43 };
	m_AltarSphere[4] = new AltarSphere("AltarSphere0", pos, pd3dDevice, pd3dCommandList, GraphicsRootSignatureMgr::GetGraphicsRootSignature());

	// ui map 포지션으로 변경하기 위해 크기 축소
	pos.x = pos.x * (float(m_UIMapSize.x) / 30000.f);
	pos.z = -pos.z * (float(m_UIMapSize.y) / 30000.f); // 스크린 좌표계로 이동하기 위해

	// 스크린 좌표계 위치 이동
	pos.x += m_SampleUIMap->getRect().left;
	pos.z += m_SampleUIMap->getRect().bottom;
	m_UIAltar_5 = new UI2DImage(
		m_TESTGameObject, ENUM_SCENE::SCENE_GAME, pd3dDevice, pd3dCommandList,
		POINT{ int(pos.x), int(pos.z) },
		20, 20,
		L"Image/Red.dds"
	);

	m_UIPlayer = new UI2DImage(
		m_TESTGameObject, ENUM_SCENE::SCENE_GAME, pd3dDevice, pd3dCommandList,
		POINT{ 0, 0 },
		10, 10,
		L"Image/Red.dds"
	);

}

void GameScene::ReleaseObjects()
{
	if (m_pd3dCbvSrUavDescriptorHeap)
	{
		m_pd3dCbvSrUavDescriptorHeap->Release(); 
		m_pd3dCbvSrUavDescriptorHeap = nullptr;
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

	if (m_TestMonster)
	{
		for (int i = 0; i < m_TestMonsterCount; ++i)
		{
			m_TestMonster[i]->ReleaseObjects();
			delete m_TestMonster[i];
			m_TestMonster[i] = nullptr;
		}

		delete m_TestMonster;
		m_TestMonster = nullptr;
	}
}

bool GameScene::ProcessInput(HWND hWnd, float fElapsedTime)
{
	// 플레이어 이동에 대한 처리 (정확히는 이동이 아니라 가속도)
	m_pPlayer->ProcessInput(fElapsedTime);

	// 플레이어 회전에 대한 처리
	if ((GameInput::GetDeltaX() != 0.0f) || (GameInput::GetDeltaY() != 0.0f))
	{
		if (GameInput::GetDeltaX() || GameInput::GetDeltaY())
		{
			// 플레이어와 카메라 똑같이 rotate...
			// 순서 의존적이므로 변경 금지
			m_pMainCamera->GetCamera()->Rotate(GameInput::GetDeltaY(), GameInput::GetDeltaX(), 0.0f);
			m_pPlayer->Rotate(0.0f, GameInput::GetDeltaX(), 0.0f);
		}
	}

	return true;
}

void GameScene::UpdatePhysics(float fElapsedTime)
{
	m_Dragon->UpdateState(fElapsedTime);
	// 스킬 이펙트 가속도 처리
	PlayerSkillMgr::GetInstance()->UpdatePhysics(fElapsedTime);

	for (int i = 0; i < m_TestMonsterCount; ++i) {
		m_TestMonster[i]->UpdateState(fElapsedTime); // State와 업데이트 처리...
	}

	// 반드시 UpdatePhysics 마지막에 충돌 처리를 해야함.
	// 그래야 충돌된 것에 따라 슬라이딩 벡터의 가속도 처리를 할 수 있음.
	UpdateCollision(fElapsedTime);
}

// ProcessInput에 의한 right, up, look, pos 를 월드변환 행렬에 갱신한다.
void GameScene::Update(float fElapsedTime)
{
	// 플레이어 공격 , 즉 플레이어와 몬스터 충돌 체크
	if (GameInput::GetDragMode()) // 만약 드래그로 회전한다면...
	{
		if (GameInput::IsKeydownE() && !m_pPlayer->IsAttacking() && !m_pPlayer->GetpBroom()->GetisUsing())
		{
			// 플레이어 일반 원거리 공격시 몬스터와 충돌체크 ///////////////////////
			//for (int i = 0; i < m_TestMonsterCount; ++i) 
			//{
			//	m_pPlayer->Attack(
			//		static_cast<Status*>(
			//			m_TestMonster[i]->GetHPStatus()),
			//		m_TestMonster[i]->GetBOBox(),
			//		m_AimPoint->GetPickingPoint(),
			//		m_pMainCamera->GetCamera());
			//} 
			// SoundManager::GetInstance()->Play(ENUM_SOUND::MAGIC_MISIL);
			//
		}
	}
	else // 드래그로 회전하지 않는다면...
	{

	}

	if (GameInput::IsKeydownSpace())
	{
		for (int x = 0; x < 5; ++x)
		{
			if (Collision::isCollide(m_pPlayer->GetBOBox()->GetBOBox(), m_AltarSphere[x]->GetBOBox()->GetBOBox()))
			{
				m_AltarSphere[x]->SetisEnguaged(true);
			}
		}
	}
	else
	{
		for (int x = 0; x < 5; ++x)
		{
			m_AltarSphere[x]->SetisEnguaged(false);
		}
	}

	for (int x = 0; x < 5; ++x)
	{
		m_AltarSphere[x]->Update(fElapsedTime);
	}

	//// 순서 변경 X //// 
	if (m_pPlayer) m_pPlayer->Update(fElapsedTime); //Velocity를 통해 pos 이동

	if (m_SkyBox) m_SkyBox->Update(fElapsedTime);
	if (m_WideareaMagic) m_WideareaMagic->Update(fElapsedTime);

	m_Dragon->Update(fElapsedTime);
	for (int i = 0; i < m_TestMonsterCount; ++i)
	{
		m_TestMonster[i]->Update(fElapsedTime);
	}

	// 플레이어 스킬 이동 업데이트.. 플레이어 포지션에 맞춰야 하므로 반드시 Player Update이후에 호출해야함.
	PlayerSkillMgr::GetInstance()->Update(fElapsedTime);
	//// 순서 변경 X ////

	for (int x = 0; x < 5; ++x)
	{
		m_AltarSphere[x]->Update(fElapsedTime);
	}

	// light update
	if (m_pcbMappedLights) ::memcpy(m_pcbMappedLights, LightManager::m_pLights, sizeof(LIGHTS));
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

	// player update 이후에 camera update
	// 순서변경X
	if (m_pMainCamera) m_pMainCamera->LastUpdate(fElapsedTime);
	if (m_pSkyCameraObj) m_pSkyCameraObj->LastUpdate(fElapsedTime);

	// Update한 위치로 스킬 공격이 몬스터에게 접촉하는지 확인 ///////////////////////////
	for (int index = 0; index < SKILL_SELECTED; ++index)
	{
		// 스킬 활성화가 되어있지 않다면 넘어간다.
		if (!PlayerSkillMgr::GetInstance()->GetSkillEffect(index)->isActive) continue;
		
		// 만약 스킬이 ATTACK 타입이 아니라면 넘어간다.
		if (PlayerSkillMgr::GetInstance()->GetSkillEffect(index)->m_skillEffect->m_Skilltype != SKILLTYPE_ATTACK) continue;
		 
		MyCollider* skill_collider = PlayerSkillMgr::GetInstance()->GetSkillEffect(index)->m_skillEffect->GetCollier();

		// 모든 몬스터 끼리 충돌체크
		for (int i = 0; i < m_TestMonsterCount; ++i)
		{
			// 체력이 0보다 적으면 검사하지 않는다.
			if (m_TestMonster[i]->GetStatus()->m_Guage <= 0.f) continue;

			if (Collision::isCollide(m_TestMonster[i]->GetBOBox(), skill_collider))
			{ 
				m_TestMonster[i]->SubstractHP(5);
				PlayerSkillMgr::GetInstance()->Deactive(index);
			}

		}

		//////////////////////////////////////////////////////// BOSS를 때림
		// 체력이 0보다 적으면 검사하지 않는다.
		if (m_Dragon->GetStatus()->m_Guage <= 0.f) continue;

		if (Collision::isCollide(m_Dragon->GetBOBox(), skill_collider))
		{
			std::cout << "보스가 스킬에 맞음" << std::endl;
			m_Dragon->SubstractHP(5);
			PlayerSkillMgr::GetInstance()->Deactive(index);
		}
		//////////////////////////////////////////////////////// BOSS를 때림
	}

	// if (m_Dragon->GetisAttacking())
	// {
	// 	if (Collision::isCollide(m_pPlayer->GetBOBox()->GetBOBox(), m_Dragon->GetBOBox()->GetBOBox()))
	// 	{
	// 		m_pPlayer->SubstractHP(5);
	// 	}
	// }

	// Update한 위치로 몬스터가 공격 시에 몬스터/플레이어충돌체크 확인 ///////////////////////////
	//for (int i = 0; i < m_TestMonsterCount; ++i)
	//{
	//	if (m_TestMonster[i]->GetisAttacking())
	//	{
	//		if (Collision::isCollide(m_pPlayer->GetBOBox()->GetBOBox(), m_TestMonster[i]->GetBOBox()->GetBOBox()))
	//		{
	//			m_pPlayer->SubstractHP(5);
	//		}
	//	}
	//}

	// 보스 부딫힘
	if (Collision::isCollide(m_pPlayer->GetBOBox()->GetBOBox(), m_Dragon->GetBOBox()->GetBOBox()))
	{
		// 드래곤과 플레이어 부딪히는 것 테스트
	}
	// 보스 부딫힘

	// 카메라 프러스텀과 쿼드트리 지형 렌더링 체크
	if (m_pMainCamera && m_pQuadtreeTerrain)
	{
		m_pMainCamera->GetCamera()->GetFrustum()->CheckRendering(m_pQuadtreeTerrain->GetRootNode());
	}
	if (m_pQuadtreeTerrain) m_pQuadtreeTerrain->LastUpdate(fElapsedTime);
	// 순서변경X  
}

void GameScene::AnimateObjects(float fTimeElapsed)
{
	if (m_pPlayer) m_pPlayer->Animate(fTimeElapsed);

	for (int x = 0; x < 5; ++x)
	{
		if (m_AltarSphere[x])
		{
			if (m_AltarSphere[x]->GetisActive()) m_AltarSphere[x]->Animate(fTimeElapsed);
		}
	}

	m_Dragon->Animate(fTimeElapsed);

	for (int i = 0; i < m_TestMonsterCount; ++i)
	{
		if (m_TestMonster[i]) m_TestMonster[i]->Animate(fTimeElapsed);
	}
}

void GameScene::Render(ID3D12GraphicsCommandList *pd3dCommandList, bool isGBuffers)
{
	// 렌더링
	extern MeshRenderer gMeshRenderer;

	// 힙 설정
	pd3dCommandList->SetDescriptorHeaps(1, &m_pd3dCbvSrUavDescriptorHeap);
	
	// 쉐도우 맵 연결
	pd3dCommandList->SetGraphicsRootDescriptorTable(ROOTPARAMETER_SHADOWTEXTURE, m_hGPUShadowmap);
	pd3dCommandList->SetGraphicsRootDescriptorTable(ROOTPARAMETER_PLAYERSHADOWTEXTURE, m_hGPUPlayerShadowmap);

	//  조명 설정
	D3D12_GPU_VIRTUAL_ADDRESS d3dcbLightsGpuVirtualAddress = m_pd3dcbLights->GetGPUVirtualAddress();
	pd3dCommandList->SetGraphicsRootConstantBufferView(ROOTPARAMETER_LIGHTS, d3dcbLightsGpuVirtualAddress); //Lights

	D3D12_GPU_VIRTUAL_ADDRESS d3dcbMaterialsGpuVirtualAddress = m_pd3dcbMaterials->GetGPUVirtualAddress();
	pd3dCommandList->SetGraphicsRootConstantBufferView(ROOTPARAMETER_MATERIALS, d3dcbMaterialsGpuVirtualAddress);
	// 조명 설정

	// 클라 화면 설정	
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
	 
	if (m_SkyBox) m_SkyBox->Render(pd3dCommandList, isGBuffers);

	if (m_pPlayer) m_pPlayer->Render(pd3dCommandList, isGBuffers);

	if (m_WideareaMagic) m_WideareaMagic->Render(pd3dCommandList, isGBuffers);

#ifdef CHECK_SUBVIEWS
	m_lookAboveCamera->SetViewportsAndScissorRects(pd3dCommandList);
	m_lookAboveCamera->GetCamera()->UpdateShaderVariables(pd3dCommandList, ROOTPARAMETER_CAMERA);

	m_pQuadtreeTerrain->Render(m_pQuadtreeTerrain->GetRootNode(), pd3dCommandList);
#endif

	////////////////////////////// Model Render
	// PSO 설정


	//// Aim point Render 
	if (!m_isSkyMode)
	{
		if (m_pPlayer) m_pPlayer->RenderStatus(pd3dCommandList, isGBuffers);

		if (!m_pPlayer->GetBroom()->GetisPrepare() && !m_pPlayer->GetBroom()->GetisUsing())
		{
			if (m_AimPoint) m_AimPoint->Render(pd3dCommandList, isGBuffers);
		}
	}

	m_Dragon->Render(pd3dCommandList, isGBuffers);
	
	for (int i = 0; i < m_TestMonsterCount; ++i)
	{
		if (m_TestMonster[i]) m_TestMonster[i]->Render(pd3dCommandList, isGBuffers);
	}

	bool isAllAtive = false;
	for (int i = 0; i < 5; ++i)
	{
		if (m_AltarSphere[i]->GetisActive())
			isAllAtive = true;
	}

	for (int x = 0; x < 5; ++x)
	{
		if (m_AltarSphere[x])m_AltarSphere[x]->Render(pd3dCommandList, isGBuffers);
	}

 
	PlayerSkillMgr::GetInstance()->Render(pd3dCommandList, isGBuffers);


	// 터레인
	if (m_Terrain)
	{ 
		m_pQuadtreeTerrain->Render(pd3dCommandList, m_Terrain, nullptr /*딱히 의미없음*/, isGBuffers);
	}

	/// ui map과 스킬 관련 렌더링..
	ShaderManager::GetInstance()->SetPSO(pd3dCommandList, SHADER_UIMAPFORPLAYER, isGBuffers);
	XMFLOAT2 playerpos{ m_pPlayer->GetTransform().GetPosition().x,  m_pPlayer->GetTransform().GetPosition().z };

	// ui map 포지션으로 변경하기 위해 크기 축소
	playerpos.x = playerpos.x * (float(m_UIMapSize.x) / 30000.f);
	playerpos.y = -playerpos.y * (float(m_UIMapSize.y) / 30000.f); // 스크린 좌표계로 이동하기 위해

	// 스크린 좌표계 위치 이동
	playerpos.x += m_SampleUIMap->getRect().left;
	playerpos.y += m_SampleUIMap->getRect().bottom;

	pd3dCommandList->SetGraphicsRoot32BitConstants(ROOTPARAMETER_PICKINGPOINT, 2, &playerpos, 0);
	m_UIPlayer->Render(pd3dCommandList);

	ShaderManager::GetInstance()->SetPSO(pd3dCommandList, SHADER_UISCREEN, isGBuffers);

	m_UIAltar_1->Render(pd3dCommandList);
	m_UIAltar_2->Render(pd3dCommandList);
	m_UIAltar_3->Render(pd3dCommandList);
	m_UIAltar_4->Render(pd3dCommandList);
	m_UIAltar_5->Render(pd3dCommandList);

	SkillSelectScene::m_pHeap->UpdateShaderVariable(pd3dCommandList);

	SkillSelectScene::m_pTexture->UpdateShaderVariable(pd3dCommandList, SKILL_TO_CHOOSE); // 임시로 검은색으로 렌더링
	m_SampleUIMap->Render(pd3dCommandList);

	ShaderManager::GetInstance()->SetPSO(pd3dCommandList, SHADER_SKILLICON, isGBuffers); 
	float cooltime = PlayerSkillMgr::GetInstance()->GetSkillEffect(0)->RemainCoolTimePrecentage;
	pd3dCommandList->SetGraphicsRoot32BitConstants(ROOTPARAMETER_HPPERCENTAGE, 1, &cooltime, 0);
	SkillSelectScene::m_pTexture->UpdateShaderVariable(pd3dCommandList, SkillSelectScene::m_SelectedIndex[0]);
	m_SampleUISkill1->Render(pd3dCommandList);

	SkillSelectScene::m_pTexture->UpdateShaderVariable(pd3dCommandList, SkillSelectScene::m_SelectedIndex[1]);  
	cooltime = PlayerSkillMgr::GetInstance()->GetSkillEffect(1)->RemainCoolTimePrecentage;
	pd3dCommandList->SetGraphicsRoot32BitConstants(ROOTPARAMETER_HPPERCENTAGE, 1, &cooltime, 0);
	m_SampleUISkill2->Render(pd3dCommandList);

	SkillSelectScene::m_pTexture->UpdateShaderVariable(pd3dCommandList, SkillSelectScene::m_SelectedIndex[2]);  
	cooltime = PlayerSkillMgr::GetInstance()->GetSkillEffect(2)->RemainCoolTimePrecentage;
	pd3dCommandList->SetGraphicsRoot32BitConstants(ROOTPARAMETER_HPPERCENTAGE, 1, &cooltime, 0);
	m_SampleUISkill3->Render(pd3dCommandList);

	SkillSelectScene::m_pTexture->UpdateShaderVariable(pd3dCommandList, SkillSelectScene::m_SelectedIndex[3]); 
	cooltime = PlayerSkillMgr::GetInstance()->GetSkillEffect(3)->RemainCoolTimePrecentage;
	pd3dCommandList->SetGraphicsRoot32BitConstants(ROOTPARAMETER_HPPERCENTAGE, 1, &cooltime, 0);
	m_SampleUISkill4->Render(pd3dCommandList);
}

void GameScene::RenderForShadow(ID3D12GraphicsCommandList * pd3dCommandList)
{  
	// 클라 화면 설정	
	if (m_isSkyMode)
	{

	}
	else
	{
		m_pMainCamera->SetViewportsAndScissorRects(pd3dCommandList);
		m_pMainCamera->GetCamera()->UpdateShaderVariables(pd3dCommandList, 1);
		m_pMainCamera->GetCamera()->UpdateLightShaderVariables(pd3dCommandList, &LightManager::m_pLights->m_pLights[2]);
	} 

	for (int i = 0; i < m_TestMonsterCount; ++i)
	{
		if (m_TestMonster[i]) m_TestMonster[i]->RenderForShadow(pd3dCommandList);
	}

	// 터레인
	if (m_Terrain)
	{
		m_pQuadtreeTerrain->RenderInstancingObjectsForShadow(pd3dCommandList);
	} 
}

void GameScene::RenderForPlayerShadow(ID3D12GraphicsCommandList * pd3dCommandList)
{
	// 클라 화면 설정	
	if (m_isSkyMode)
	{

	}
	else
	{
		MainCameraMgr::GetMainCamera()->GetCamera()->SetViewportsAndScissorRects(pd3dCommandList);
		MainCameraMgr::GetMainCamera()->GetCamera()->UpdateShaderVariables(pd3dCommandList, 1);
		MainCameraMgr::GetMainCamera()->GetCamera()->UpdateLightShaderVariables(pd3dCommandList, &LightManager::m_pLights->m_pLights[2]);
	}

	m_pPlayer->RenderForShadow(pd3dCommandList);  
}

void GameScene::ReleaseUploadBuffers()
{
	if (m_SkyBox) m_SkyBox->ReleaseUploadBuffers();
	if (m_AimPoint)m_AimPoint->ReleaseUploadBuffers();
	if (m_Sniping) m_Sniping->ReleaseUploadBuffers();
	if (m_WideareaMagic) m_WideareaMagic->ReleaseUploadBuffers();
	if (m_SkyBox) m_SkyBox->ReleaseUploadBuffers();
	if (m_pPlayer) m_pPlayer->ReleaseUploadBuffers();
	if (m_Terrain) m_Terrain->ReleaseUploadBuffers();
	if (m_pQuadtreeTerrain) m_pQuadtreeTerrain->ReleaseUploadBuffers();
	for (int i = 0; i < m_TestMonsterCount; ++i)
	{
		m_TestMonster[i]->ReleaseUploadBuffers();
	}
}

void GameScene::UpdateCollision(float fElapsedTime)
{
	// 외곽처리
	MyBOBox outside_box[4]{
		{XMFLOAT3(-100, 0, 15000), XMFLOAT3(100, 3000, 20000)},
		{XMFLOAT3(15000, 0, 30100), XMFLOAT3(20000, 3000, 100)},
		{XMFLOAT3(30100, 0, 15000), XMFLOAT3(100, 3000, 20000)},
		{XMFLOAT3(15000, 0, -100), XMFLOAT3(30000, 3000, 100)},
	};

	// 외곽 4 부분과 충돌처리 확인한다.
	Collision::ProcessCollideEdge(m_pPlayer, 4, outside_box, fElapsedTime);

	// 플레이어와 지형지물 충돌을 확인한다. 
	Collision::ProcessCollideTerrainObject(m_pPlayer, m_pQuadtreeTerrain, fElapsedTime);


	// // 플레이어와 보스 충돌체크
	// MyBOBox bossbox = result_pos;
	// XMFLOAT3 slideVector{ 0.f, 0.f, 0.f };
	// 
	// // 이동한 박스를 통해 충돌한다.
	// bool isSlide = Collision::ProcessCollision(
	// 	AlreadyPlayerBBox,
	// 	bossbox,
	// 	m_pPlayer->GetTransform().GetPosition(),
	// 	m_pPlayer->GetVelocity(),
	// 	fElapsedTime,
	// 	false,
	// 	slideVector);
	// 
	// if (isSlide)
	// {
	// 	m_pPlayer->SetVelocity(slideVector);
	// }
	// // 플레이어와 보스 충돌체크


	// 몬스터 충돌체크 ///////////////////////// 
	for (int x = 0; x < m_TestMonsterCount; ++x)
	{
		Collision::ProcessCollideTerrainObject(m_TestMonster[x], m_pQuadtreeTerrain, fElapsedTime);
	}

	// 몬스터 충돌체크 ///////////////////////// 
}

void GameScene::BuildLightsAndMaterials(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
	//////////////////////////// 조명
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
	LightManager::m_pLights->m_pLights[2].Position = XMFLOAT3(15000, 1000.0f, 15000.0f);
	LightManager::m_pLights->m_pLights[2].Ambient = XMFLOAT4(1.f, 0.8f, 0.8f, 1.0f);
	LightManager::m_pLights->m_pLights[2].Diffuse = XMFLOAT4(1.0f, 0.4f, 0.4f, 1.0f);
	LightManager::m_pLights->m_pLights[2].Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	LightManager::m_pLights->m_pLights[2].Direction = Vector3::Normalize(XMFLOAT3(0.0f, -1.0f, 1.f));

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


	UINT ncbElementBytes = ((sizeof(LIGHTS) + 255) & ~255); //256의 배수
	m_pd3dcbLights = d3dUtil::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbElementBytes, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);

	m_pd3dcbLights->Map(0, NULL, (void **)&m_pcbMappedLights);
	//////////////////////////// 조명

	//////////////////////////// 재질
	m_pMaterials = new MATERIAL;
	::ZeroMemory(m_pMaterials, sizeof(MATERIAL));

	*m_pMaterials = { XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 5.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) };

	CreateConstantBuffer(pd3dDevice, pd3dCommandList, m_pd3dcbMaterials, sizeof(MATERIAL), (void **)&m_pcbMappedMaterials);

	UINT ncbMaterialBytes = ((sizeof(MATERIAL) + 255) & ~255); //256의 배수
	m_pd3dcbMaterials = d3dUtil::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbMaterialBytes, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);

	m_pd3dcbMaterials->Map(0, NULL, (void **)&m_pcbMappedMaterials);
	//////////////////////////// 재질
}
