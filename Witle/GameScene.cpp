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

//// �Ŵ��� ���� ��� //////////////////////////
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
//// �Ŵ��� ���� ��� //////////////////////////

//// ���� ���� ��� //////////////////////////
#include "SpaceCat.h"
#include "Dragon.h"
#include "CreepyMonster.h"
#include "MonsterStatus.h"
//// ���� ���� ��� //////////////////////////

//// �÷��̾� ���� ��� //////////////////////////
#include "Player.h"
#include "PlayerStatus.h"
#include "PlayerSkillMgr.h"
//// �÷��̾� ���� ��� //////////////////////////

// ���߿� �ڵ� ���� 
#include "SkillSelectScene.h"

#include "MyBOBox.h"
#include "SkillEffect.h"
#include "Collision.h"
#include "Status.h"
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
#include "CameraObject.h"
#include "QuadTreeTerrain.h"
#include"MyDescriptorHeap.h"
#include "BasicCam.h" 

#include "UI2DImage.h"
#include "GameScene.h"

MyDescriptorHeap*		GameScene::m_pd3dCbvSrvDescriptorHeap;

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
	m_pd3dCbvSrvDescriptorHeap = new MyDescriptorHeap();
	m_pd3dCbvSrvDescriptorHeap->CreateCbvSrvUavDescriptorHeaps(pd3dDevice, pd3dCommandList, nConstantBufferViews, nShaderResourceViews, 0);

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

D3D12_GPU_DESCRIPTOR_HANDLE GameScene::CreateShaderResourceViewsForShadow(ID3D12Device * pd3dDevice, ID3D12Resource * pResource, UINT nRootParameter, bool bAutoIncrement, int index)
{
	assert(!(pResource == nullptr));

	D3D12_GPU_DESCRIPTOR_HANDLE d3dSrvGPUDescriptorHandle = m_d3dSrvGPUDescriptorNextHandle;

	int nTextureType = RESOURCE_TEXTURE2D;

	ID3D12Resource *pShaderResource = pResource;
	D3D12_RESOURCE_DESC d3dResourceDesc = pShaderResource->GetDesc();
	d3dResourceDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	D3D12_SHADER_RESOURCE_VIEW_DESC d3dShaderResourceViewDesc = d3dUtil::GetShaderResourceViewDesc(d3dResourceDesc, nTextureType);

	// ���°����
	D3D12_CPU_DESCRIPTOR_HANDLE hCPU = m_pd3dCbvSrvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	for (int i = 0; i < index; ++i)
	{
		hCPU.ptr += d3dUtil::gnCbvSrvDescriptorIncrementSize;
	}

	pd3dDevice->CreateShaderResourceView(pShaderResource, &d3dShaderResourceViewDesc, hCPU);

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
		case VK_F2:
			MyBOBox::CHANGEMODE();
			Monster::CHANGEMODE();
			break;

		case 'L':
			break;

		case 'Z':
			// �ӽ÷� ü�� 100�� ��� ����
			m_pPlayer->SubstractHP(100);
			break;

		case '1': // ��ų ���ڷ�  
			SoundManager::GetInstance()->Play(ENUM_SOUND::BROOM);
			m_pPlayer->UseSkill_Broom();
			// SoundManager::GetInstance()->Stop(ENUM_SOUND::BROOM);
			break;

		case '2':
			// �������� ��ų
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

		case VK_SPACE:
			// 
			break;

		case MYVK_E:
			// �÷��̾� ��ų �Ŵ������� ���̾ ��ų Ȱ��ȭ
			m_PlayerSkillMgr->Activate(m_pPlayer->GetMPStatus(), ENUM_SKILL::SKILL_FIREBALL);
			break;

		case MYVK_R:
			// �÷��̾� ��ų �Ŵ������� ���̾ ��ų Ȱ��ȭ
			m_PlayerSkillMgr->Activate(m_pPlayer->GetMPStatus(), ENUM_SKILL::SKILL_ICEBALL);
			break;

		case MYVK_T:
			// �÷��̾� ��ų �Ŵ������� ���̾ ��ų Ȱ��ȭ
			m_PlayerSkillMgr->Activate(m_pPlayer->GetMPStatus(), ENUM_SKILL::SKILL_ELECTRICBALL);
			break;

		case MYVK_Y:
			// �÷��̾� ��ų �Ŵ������� ���̾ ��ų Ȱ��ȭ
			m_PlayerSkillMgr->Activate(m_pPlayer->GetMPStatus(), ENUM_SKILL::SKILL_DEBUFF);
			break;
		case '4':
			break;

		case '6':
			break;

		case '5':
			// ��� ���� Ȱ��ȭ ��Ű�� ġƮ
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


	// ��ũ���� �� ����
	GameScene::CreateCbvSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, 0, 4);

	m_AimPoint = new AimPoint("AimPoint", pd3dDevice, pd3dCommandList, POINT{ int(GameScreen::GetWidth()) / 2, int(GameScreen::GetHeight()) / 2 }, 100.f, 100.f, L"Image/AimPoint.dds");
	// m_WideareaMagic = new WideareaMagic(pd3dDevice, pd3dCommandList);

	// ���� ���� -> ���߿� ���۾����� �ű��
	// SoundManager::GetInstance()->Play(ENUM_SOUND::SATRT);
	//SoundManager::GetInstance()->Stop(ENUM_SOUND::SATRT);
	// �������
	// SoundManager::GetInstance()->Play(ENUM_SOUND::TITLE);

	//// ��ī�� �ڽ� ����
	m_SkyBox = new SkyBox(pd3dDevice, pd3dCommandList, 3000.F, 3000.F, 3000.F);
	// ��ī�� �ڽ� ���� ///////////////////////////////////

	// �ͷ��� ���� ////////////////////////////////////////
	XMFLOAT3 xmf3Scale(39.0625f * 3.f, 1.0f, 39.0625f * 3.f);
	// XMFLOAT3 xmf3Scale(1.f, 1.0f, 1.f);
	XMFLOAT4 xmf4Color(0.0f, 0.5f, 0.0f, 0.0f);
	m_Terrain = new Terrain("Terrain", pd3dDevice, pd3dCommandList, L"Image/HeightMap.raw", 257, 257, 257, 257, xmf3Scale, xmf4Color);
	// �ͷ��� ���� ////////////////////////////////////////

	// �÷��̾� ���� ////////////////////////////////////////
	m_pPlayer = new Player("Player", pd3dDevice, pd3dCommandList, GraphicsRootSignatureMgr::GetGraphicsRootSignature());
	m_SkyBox->SetpPlayerTransform(&m_pPlayer->GetTransform());
	PlayerManager::SetMainPlayer(m_pPlayer);
	m_PlayerSkillMgr = new PlayerSkillMgr(pd3dDevice, pd3dCommandList);
	// �÷��̾� ���� ////////////////////////////////////////

	// �׽�Ʈ��  
	std::random_device rd;
	std::mt19937 mersenne(rd());
	std::uniform_int_distribution<> die(2000, 15000);
	std::uniform_int_distribution<> monstertype(0, 3);

	m_Dragon = new Dragon("Dragon",
		XMFLOAT3(15000, 0, 15000),
		pd3dDevice, pd3dCommandList, GraphicsRootSignatureMgr::GetGraphicsRootSignature());

	m_TestMonster = new Monster*[m_TestMonsterCount];

	/*
		m_TestMonster[0] = new Dragon("SpaceCat",
			XMFLOAT3(15000, 0, 15000),
			pd3dDevice, pd3dCommandList, GraphicsRootSignatureMgr::GetGraphicsRootSignature());
	*/
	int spacecatblue_count = 0;
	int spacecatgreen_count = 0;
	int spacecatpink_count = 0;
	int creepymonster_count = 0;
	int boss_count = 0;

	MonsterTransformStorage* instance = MonsterTransformStorage::GetInstance();
	instance->CreateInfo(pd3dDevice, pd3dCommandList);

	for (int i = 0; i < m_TestMonsterCount; )
	{
		int value = monstertype(mersenne);

		if (value == ENUM_MONSTER::MONSTER_CREEPYMONSTER)
		{
			if (instance->Count(CREEPYMONSTER) <= creepymonster_count)continue;
			m_TestMonster[i] = new CreepyMonster("CreepyMonster",
				instance->GetPosition(creepymonster_count, CREEPYMONSTER),
				pd3dDevice, pd3dCommandList, GraphicsRootSignatureMgr::GetGraphicsRootSignature());
			creepymonster_count += 1;
		}
		else if (value == ENUM_MONSTER::MONSTER_SPACECATBLUE)
		{
			if (instance->Count(SPACECAT_BLUE) <= spacecatblue_count)continue;
			m_TestMonster[i] = new SpaceCat("SpaceCat",
				instance->GetPosition(spacecatblue_count, SPACECAT_BLUE),
				pd3dDevice, pd3dCommandList, GraphicsRootSignatureMgr::GetGraphicsRootSignature());
			spacecatblue_count += 1;
		}
		else if (value == ENUM_MONSTER::MONSTER_SPACECATPINK)
		{
			if (instance->Count(SPACECAT_PINK) <= spacecatpink_count)continue;
			m_TestMonster[i] = new SpaceCat("SpaceCat",
				instance->GetPosition(spacecatpink_count, SPACECAT_PINK),
				pd3dDevice, pd3dCommandList, GraphicsRootSignatureMgr::GetGraphicsRootSignature());
			spacecatpink_count += 1;
		}
		else if (value == ENUM_MONSTER::MONSTER_SPACECATGREEN)
		{
			if (instance->Count(SPACECAT_GREEN) <= spacecatgreen_count)continue;
			m_TestMonster[i] = new SpaceCat("SpaceCat",
				instance->GetPosition(spacecatgreen_count, SPACECAT_GREEN),
				pd3dDevice, pd3dCommandList, GraphicsRootSignatureMgr::GetGraphicsRootSignature());
			spacecatgreen_count += 1;
		}
		else if (value == ENUM_MONSTER::MONSTER_BOSSMONSTER)
		{
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

	//// �׽�Ʈ ����Ʈ�� �ͷ��� ���� 
	m_pQuadtreeTerrain = new QuadtreeTerrain(pd3dDevice, pd3dCommandList, 257, 257, xmf3Scale, xmf4Color, m_Terrain->GetHeightMapImage());

	// ī�޶�
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

	// ���ҽ� �� ����
	GameScene::CreateShaderResourceViews(pd3dDevice, m_Terrain->GetTexture(), ROOTPARAMETER_TEXTUREBASE, true);

	// UI �̹��� �߰�
	m_TESTGameObject = new EmptyGameObject("Test");

	// �̴ϸ�
	m_SampleUIMap = new UI2DImage(
		m_TESTGameObject, pd3dDevice, pd3dCommandList,
		POINT{ int(GameScreen::GetWidth()) / 2 + 500, int(GameScreen::GetHeight()) / 2 - 220 },
		m_UIMapSize.x,
		m_UIMapSize.y,
		nullptr
	);

	// ��ų 1
	m_SampleUISkill1 = new UI2DImage(
		m_TESTGameObject, pd3dDevice, pd3dCommandList,
		POINT{ int(GameScreen::GetWidth()) / 2 - 300, int(GameScreen::GetHeight()) / 2 + 270 }, 100, 100,
		nullptr
	);

	// ��ų 2
	m_SampleUISkill2 = new UI2DImage(
		m_TESTGameObject, pd3dDevice, pd3dCommandList,
		POINT{ int(GameScreen::GetWidth()) / 2 - 100, int(GameScreen::GetHeight()) / 2 + 270 }, 100, 100,
		nullptr
	);

	// ��ų 3
	m_SampleUISkill3 = new UI2DImage(
		m_TESTGameObject, pd3dDevice, pd3dCommandList,
		POINT{ int(GameScreen::GetWidth()) / 2 + 100, int(GameScreen::GetHeight()) / 2 + 270 }, 100, 100,
		nullptr
	);

	// ��ų 4
	m_SampleUISkill4 = new UI2DImage(
		m_TESTGameObject, pd3dDevice, pd3dCommandList,
		POINT{ int(GameScreen::GetWidth()) / 2 + 300, int(GameScreen::GetHeight()) / 2 + 270 }, 100, 100,
		nullptr
	);

	XMFLOAT4X4 tr;
	tr = StaticObjectStorage::GetInstance()->GetAltarTransform(0, ALTAR_IN);
	XMFLOAT3 pos{ tr._41, 0 , tr._43 }; // ���� ������
	m_AltarSphere[0] = new AltarSphere("AltarSphere0", pos, pd3dDevice, pd3dCommandList, GraphicsRootSignatureMgr::GetGraphicsRootSignature());

	// ui map ���������� �����ϱ� ���� ũ�� ���
	pos.x = pos.x * (float(m_UIMapSize.x) / 30000.f);
	pos.z = -pos.z * (float(m_UIMapSize.y) / 30000.f); // ��ũ�� ��ǥ��� �̵��ϱ� ����

	// ��ũ�� ��ǥ�� ��ġ �̵�
	pos.x += m_SampleUIMap->getRect().left;
	pos.z += m_SampleUIMap->getRect().bottom;

	m_UIAltar_1 = new UI2DImage(
		m_TESTGameObject, pd3dDevice, pd3dCommandList,
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
		m_TESTGameObject, pd3dDevice, pd3dCommandList,
		POINT{ int(pos.x), int(pos.z) },
		20, 20,
		L"Image/Red.dds"
	);

	tr = StaticObjectStorage::GetInstance()->GetAltarTransform(2, ALTAR_IN);
	pos = XMFLOAT3{ tr._41,0 , tr._43 };
	m_AltarSphere[2] = new AltarSphere("AltarSphere0", pos, pd3dDevice, pd3dCommandList, GraphicsRootSignatureMgr::GetGraphicsRootSignature());

	// ui map ���������� �����ϱ� ���� ũ�� ���
	pos.x = pos.x * (float(m_UIMapSize.x) / 30000.f);
	pos.z = -pos.z * (float(m_UIMapSize.y) / 30000.f); // ��ũ�� ��ǥ��� �̵��ϱ� ����

	// ��ũ�� ��ǥ�� ��ġ �̵�
	pos.x += m_SampleUIMap->getRect().left;
	pos.z += m_SampleUIMap->getRect().bottom;
	m_UIAltar_3 = new UI2DImage(
		m_TESTGameObject, pd3dDevice, pd3dCommandList,
		POINT{ int(pos.x), int(pos.z) },
		20, 20,
		L"Image/Red.dds"
	);

	tr = StaticObjectStorage::GetInstance()->GetAltarTransform(3, ALTAR_IN);
	pos = XMFLOAT3{ tr._41, 0 , tr._43 };
	m_AltarSphere[3] = new AltarSphere("AltarSphere0", pos, pd3dDevice, pd3dCommandList, GraphicsRootSignatureMgr::GetGraphicsRootSignature());

	// ui map ���������� �����ϱ� ���� ũ�� ���
	pos.x = pos.x * (float(m_UIMapSize.x) / 30000.f);
	pos.z = -pos.z * (float(m_UIMapSize.y) / 30000.f); // ��ũ�� ��ǥ��� �̵��ϱ� ����

	// ��ũ�� ��ǥ�� ��ġ �̵�
	pos.x += m_SampleUIMap->getRect().left;
	pos.z += m_SampleUIMap->getRect().bottom;
	m_UIAltar_4 = new UI2DImage(
		m_TESTGameObject, pd3dDevice, pd3dCommandList,
		POINT{ int(pos.x), int(pos.z) },
		20, 20,
		L"Image/Red.dds"
	);

	tr = StaticObjectStorage::GetInstance()->GetAltarTransform(4, ALTAR_IN);
	pos = XMFLOAT3{ tr._41, 0 , tr._43 };
	m_AltarSphere[4] = new AltarSphere("AltarSphere0", pos, pd3dDevice, pd3dCommandList, GraphicsRootSignatureMgr::GetGraphicsRootSignature());

	// ui map ���������� �����ϱ� ���� ũ�� ���
	pos.x = pos.x * (float(m_UIMapSize.x) / 30000.f);
	pos.z = -pos.z * (float(m_UIMapSize.y) / 30000.f); // ��ũ�� ��ǥ��� �̵��ϱ� ����

	// ��ũ�� ��ǥ�� ��ġ �̵�
	pos.x += m_SampleUIMap->getRect().left;
	pos.z += m_SampleUIMap->getRect().bottom;
	m_UIAltar_5 = new UI2DImage(
		m_TESTGameObject, pd3dDevice, pd3dCommandList,
		POINT{ int(pos.x), int(pos.z) },
		20, 20,
		L"Image/Red.dds"
	);

	m_UIPlayer = new UI2DImage(
		m_TESTGameObject, pd3dDevice, pd3dCommandList,
		POINT{ 0, 0 },
		10, 10,
		L"Image/Red.dds"
	);

}

void GameScene::ReleaseObjects()
{
	if (m_pd3dCbvSrvDescriptorHeap)
	{
		m_pd3dCbvSrvDescriptorHeap->ReleaseObjects();
		delete m_pd3dCbvSrvDescriptorHeap;
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
	// �÷��̾� �̵��� ���� ó�� (��Ȯ���� �̵��� �ƴ϶� ���ӵ�)
	m_pPlayer->ProcessInput(fElapsedTime);

	// �÷��̾� ȸ���� ���� ó��
	if ((GameInput::GetDeltaX() != 0.0f) || (GameInput::GetDeltaY() != 0.0f))
	{
		if (GameInput::GetDeltaX() || GameInput::GetDeltaY())
		{
			// �÷��̾�� ī�޶� �Ȱ��� rotate...
			// ���� �������̹Ƿ� ���� ����
			m_pMainCamera->GetCamera()->Rotate(GameInput::GetDeltaY(), GameInput::GetDeltaX(), 0.0f);
			m_pPlayer->Rotate(0.0f, GameInput::GetDeltaX(), 0.0f);
		}
	}

	return true;
}

void GameScene::UpdatePhysics(float fElapsedTime)
{
	m_Dragon->UpdateState(fElapsedTime);
	// ��ų ����Ʈ ���ӵ� ó��
	m_PlayerSkillMgr->UpdatePhysics(fElapsedTime);

	for (int i = 0; i < m_TestMonsterCount; ++i) {
		m_TestMonster[i]->UpdateState(fElapsedTime); // State�� ������Ʈ ó��...
	}

	// �ݵ�� UpdatePhysics �������� �浹 ó���� �ؾ���.
	// �׷��� �浹�� �Ϳ� ���� �����̵� ������ ���ӵ� ó���� �� �� ����.
	UpdateCollision(fElapsedTime);
}

// ProcessInput�� ���� right, up, look, pos �� ���庯ȯ ��Ŀ� �����Ѵ�.
void GameScene::Update(float fElapsedTime)
{
	// �÷��̾� ���� , �� �÷��̾�� ���� �浹 üũ
	if (GameInput::GetDragMode()) // ���� �巡�׷� ȸ���Ѵٸ�...
	{
		if (GameInput::IsKeydownE() && !m_pPlayer->IsAttacking() && !m_pPlayer->GetpBroom()->GetisUsing())
		{
			// �÷��̾� �Ϲ� ���Ÿ� ���ݽ� ���Ϳ� �浹üũ ///////////////////////
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
	else // �巡�׷� ȸ������ �ʴ´ٸ�...
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

	// �÷��̾� ��ų �̵� ������Ʈ
	m_PlayerSkillMgr->Update(fElapsedTime);

	for (int x = 0; x < 5; ++x)
	{
		m_AltarSphere[x]->Update(fElapsedTime);
	}

	//// ���� ���� X //// 
	if (m_pPlayer) m_pPlayer->Update(fElapsedTime); //Velocity�� ���� pos �̵�

	if (m_SkyBox) m_SkyBox->Update(fElapsedTime);
	if (m_WideareaMagic) m_WideareaMagic->Update(fElapsedTime);

	m_Dragon->Update(fElapsedTime);
	for (int i = 0; i < m_TestMonsterCount; ++i)
	{
		m_TestMonster[i]->Update(fElapsedTime);
	}
	//// ���� ���� X ////

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

	// player update ���Ŀ� camera update
	// ��������X
	if (m_pMainCamera) m_pMainCamera->LastUpdate(fElapsedTime);
	if (m_pSkyCameraObj) m_pSkyCameraObj->LastUpdate(fElapsedTime);

	// Update�� ��ġ�� ��ų ������ ���Ϳ��� �����ϴ��� Ȯ�� ///////////////////////////
	for (int index = 0; index < m_PlayerSkillMgr->GetCount(); ++index)
	{
		// ��ų Ȱ��ȭ�� �Ǿ����� �ʴٸ� �Ѿ��.
		if (!m_PlayerSkillMgr->GetSkillEffect(index)->isActive) continue;

		MyCollider* skill_collider = m_PlayerSkillMgr->GetSkillEffect(index)->skillEffect->GetCollier();

		// ��� ���� ���� �浹üũ
		for (int i = 0; i < m_TestMonsterCount; ++i)
		{
			// ü���� 0���� ������ �˻����� �ʴ´�.
			if (m_TestMonster[i]->GetStatus()->m_Guage <= 0.f) continue;

			if (Collision::isCollide(m_TestMonster[i]->GetBOBox(), skill_collider))
			{
				std::cout << "��ų�� �ε���" << std::endl;
				m_TestMonster[i]->SubstractHP(5);
				m_PlayerSkillMgr->Deactive(index);
			}

		}

		//////////////////////////////////////////////////////// BOSS
		// ü���� 0���� ������ �˻����� �ʴ´�.
		if (m_Dragon->GetStatus()->m_Guage <= 0.f) continue;

		if (Collision::isCollide(m_Dragon->GetBOBox(), skill_collider))
		{
			std::cout << "���� ��ų�� ����" << std::endl;
			m_Dragon->SubstractHP(5);
			m_PlayerSkillMgr->Deactive(index);
		}
		//////////////////////////////////////////////////////// BOSS
	}

	// ������ �浹�� �� �Ǵ��� Ȯ��
	if (Collision::isCollide(m_pPlayer->GetBOBox()->GetBOBox(), m_Dragon->GetBOBox()->GetBOBox()))
	{
		std::cout << "���� �浹 Ȯ��" << std::endl;
	}

	// Update�� ��ġ�� ���Ͱ� ���� �ÿ� ����/�÷��̾��浹üũ Ȯ�� ///////////////////////////
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

	// ī�޶� �������Ұ� ����Ʈ�� ���� ������ üũ
	if (m_pMainCamera && m_pQuadtreeTerrain)
	{
		m_pMainCamera->GetCamera()->GetFrustum()->CheckRendering(m_pQuadtreeTerrain->GetRootNode());
	}
	if (m_pQuadtreeTerrain) m_pQuadtreeTerrain->LastUpdate(fElapsedTime);
	// ��������X  
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
	// ������
	extern MeshRenderer gMeshRenderer;

	//  ���� ����
	D3D12_GPU_VIRTUAL_ADDRESS d3dcbLightsGpuVirtualAddress = m_pd3dcbLights->GetGPUVirtualAddress();
	pd3dCommandList->SetGraphicsRootConstantBufferView(ROOTPARAMETER_LIGHTS, d3dcbLightsGpuVirtualAddress); //Lights

	D3D12_GPU_VIRTUAL_ADDRESS d3dcbMaterialsGpuVirtualAddress = m_pd3dcbMaterials->GetGPUVirtualAddress();
	pd3dCommandList->SetGraphicsRootConstantBufferView(ROOTPARAMETER_MATERIALS, d3dcbMaterialsGpuVirtualAddress);
	// ���� ����

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

		// ������ �� ���� ���� �� ����
		m_pMainCamera->GetCamera()->UpdateLightShaderVariables(pd3dCommandList, &LightManager::m_pLights->m_pLights[2]);
	}

	// ��ī�̹ڽ� ����
	if (m_SkyBox) m_SkyBox->Render(pd3dCommandList, isGBuffers);

	//pd3dCommandList->SetDescriptorHeaps(1, &m_pd3dCbvSrvDescriptorHeap);

	if (m_pPlayer) m_pPlayer->Render(pd3dCommandList, isGBuffers);

	if (m_WideareaMagic) m_WideareaMagic->Render(pd3dCommandList, isGBuffers);

	if (m_PlayerSkillMgr) m_PlayerSkillMgr->Render(pd3dCommandList, isGBuffers);

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

	// �ͷ���
	if (m_Terrain)
	{
		Mesh* terrainMesh = m_Terrain->GetComponent<Mesh>("TerrainMesh");
		m_pQuadtreeTerrain->Render(pd3dCommandList, m_Terrain, m_pd3dCbvSrvDescriptorHeap->GetpDescriptorHeap(), isGBuffers);
	}



	/// ui map�� ��ų ���� ������..
	ShaderManager::GetInstance()->SetPSO(pd3dCommandList, SHADER_UIMAPFORPLAYER, isGBuffers);
	XMFLOAT2 playerpos{ m_pPlayer->GetTransform().GetPosition().x,  m_pPlayer->GetTransform().GetPosition().z };

	// ui map ���������� �����ϱ� ���� ũ�� ���
	playerpos.x = playerpos.x * (float(m_UIMapSize.x) / 30000.f);
	playerpos.y = -playerpos.y * (float(m_UIMapSize.y) / 30000.f); // ��ũ�� ��ǥ��� �̵��ϱ� ����

	// ��ũ�� ��ǥ�� ��ġ �̵�
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

	SkillSelectScene::m_pTexture->UpdateShaderVariable(pd3dCommandList, 8); // �ӽ÷� ���������� ������
	m_SampleUIMap->Render(pd3dCommandList);


	ShaderManager::GetInstance()->SetPSO(pd3dCommandList, SHADER_SKILLICON, isGBuffers);
	float cooltime = m_PlayerSkillMgr->GetSkillEffect(0)->RemainCoolTimePrecentage;
	pd3dCommandList->SetGraphicsRoot32BitConstants(ROOTPARAMETER_HPPERCENTAGE, 1, &cooltime, 0);
	SkillSelectScene::m_pTexture->UpdateShaderVariable(pd3dCommandList, SkillSelectScene::m_SelectedIndex[0]);
	m_SampleUISkill1->Render(pd3dCommandList);

	SkillSelectScene::m_pTexture->UpdateShaderVariable(pd3dCommandList, SkillSelectScene::m_SelectedIndex[1]);
	cooltime = m_PlayerSkillMgr->GetSkillEffect(1)->RemainCoolTimePrecentage;
	pd3dCommandList->SetGraphicsRoot32BitConstants(ROOTPARAMETER_HPPERCENTAGE, 1, &cooltime, 0);
	m_SampleUISkill2->Render(pd3dCommandList);

	SkillSelectScene::m_pTexture->UpdateShaderVariable(pd3dCommandList, SkillSelectScene::m_SelectedIndex[2]);
	cooltime = m_PlayerSkillMgr->GetSkillEffect(2)->RemainCoolTimePrecentage;
	pd3dCommandList->SetGraphicsRoot32BitConstants(ROOTPARAMETER_HPPERCENTAGE, 1, &cooltime, 0);
	m_SampleUISkill3->Render(pd3dCommandList);

	SkillSelectScene::m_pTexture->UpdateShaderVariable(pd3dCommandList, SkillSelectScene::m_SelectedIndex[3]);
	cooltime = m_PlayerSkillMgr->GetSkillEffect(3)->RemainCoolTimePrecentage;
	pd3dCommandList->SetGraphicsRoot32BitConstants(ROOTPARAMETER_HPPERCENTAGE, 1, &cooltime, 0);
	m_SampleUISkill4->Render(pd3dCommandList);

}

void GameScene::RenderForShadow(ID3D12GraphicsCommandList * pd3dCommandList)
{
	// ������ ������..

	// ���̹��ۿ� ������ ���� ���...

	// ������
	extern MeshRenderer gMeshRenderer;

	// Ŭ�� ȭ�� ����	
	if (m_isSkyMode)
	{

	}
	else
	{
		m_pMainCamera->SetViewportsAndScissorRects(pd3dCommandList);
		m_pMainCamera->GetCamera()->UpdateShaderVariables(pd3dCommandList, 1);
		m_pMainCamera->GetCamera()->UpdateLightShaderVariables(pd3dCommandList, &LightManager::m_pLights->m_pLights[2]);
	}

	m_pPlayer->RenderForShadow(pd3dCommandList);

	for (int i = 0; i < m_TestMonsterCount; ++i)
	{
		if (m_TestMonster[i]) m_TestMonster[i]->RenderForShadow(pd3dCommandList);
	}

	// �ͷ���
	if (m_Terrain)
	{
		// m_pQuadtreeTerrain->RenderTerrainForShadow(pd3dCommandList, m_Terrain, m_pd3dCbvSrvDescriptorHeap);
		m_pQuadtreeTerrain->RenderInstancingObjectsForShadow(pd3dCommandList);
	}

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


void GameScene::UpdateCollision(const BoundingOrientedBox & AlreadyPlayerBBox, float fElapsedTime)
{
}

void GameScene::UpdateCollision(float fElapsedTime)
{
	// �÷��̾� �浹üũ ///////////////////////// 
	BoundingOrientedBox AlreadyPlayerBBox = m_pPlayer->CalculateAlreadyBoundingBox(fElapsedTime);
	XMFLOAT3 AlreadyPositon{ AlreadyPlayerBBox.Center.x, AlreadyPlayerBBox.Center.y, AlreadyPlayerBBox.Center.z };

	// �ܰ�ó��
	MyBOBox outside_box[4]{
		{XMFLOAT3(-100, 0, 15000), XMFLOAT3(100, 3000, 20000)},
		{XMFLOAT3(15000, 0, 30100), XMFLOAT3(20000, 3000, 100)},
		{XMFLOAT3(30100, 0, 15000), XMFLOAT3(100, 3000, 20000)},
		{XMFLOAT3(15000, 0, -100), XMFLOAT3(30000, 3000, 100)},
	};

	// �ܰ��κ� ������ ���ϵ��� �浹üũ
	for (int i = 0; i < 4; ++i)
	{
		XMFLOAT3 slideVector{ 0.f, 0.f, 0.f };

		// �̵��� �ڽ��� ���� �浹�Ѵ�.
		bool isSlide = Collision::ProcessCollision(
			AlreadyPlayerBBox,
			outside_box[i],
			m_pPlayer->GetTransform().GetPosition(),
			m_pPlayer->GetVelocity(),
			fElapsedTime,
			false,
			slideVector);

		if (isSlide)
		{
			m_pPlayer->SetVelocity(slideVector);
		}
	}

	// �÷��̾�� �������� �浹üũ ///////////////////////// 
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

			XMFLOAT4X4* pWorldMatrix = StaticObjectStorage::GetInstance(m_pQuadtreeTerrain)->GetWorldMatrix(TerrainIndex, name);

			// Ʈ���� ���� ���� ������Ʈ ������ŭ �浹 üũ
			for (int i = 0; i < StaticObjectStorage::GetInstance(m_pQuadtreeTerrain)->GetObjectCount(TerrainIndex, name); ++i)
			{
				//���� ��� ����´�.

				// �� �浹�ڽ��� ������� ���Ѵ�. �ϴ� ����� ���������� �̵�
				MyBOBox worldBox = *box;
				worldBox.Move(XMFLOAT3(pWorldMatrix[i]._41, 0, pWorldMatrix[i]._43));

				XMFLOAT3 slideVector{ 0.f, 0.f, 0.f };

				bool isSlide = Collision::ProcessCollision(
					AlreadyPlayerBBox,
					worldBox,
					m_pPlayer->GetTransform().GetPosition(),
					m_pPlayer->GetVelocity(),
					fElapsedTime,
					true,
					slideVector);

				if (isSlide)
				{
					m_pPlayer->SetVelocity(slideVector);
				}

			}
		}
	}
	// �÷��̾�� �������� �浹üũ ///////////////////////// 



	// ���� �浹üũ ///////////////////////// 

	// ���� �浹üũ ///////////////////////// 
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
