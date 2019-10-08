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
#include "BossSkillMgr.h"
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
#include "HitEffectMgr.h"
//// �Ŵ��� ���� ��� //////////////////////////

//// ���� ���� ��� //////////////////////////
#include "SpaceCat.h"
#include "Mushroom.h"
#include "Dragon.h"
#include "CreepyMonster.h"
#include "MonsterStatus.h"
#include "MonsterMovement.h"
#include "BossMonsterActionMgr.h"
//// ���� ���� ��� //////////////////////////

//// �÷��̾� ���� ��� //////////////////////////
#include "Player.h"
#include "PlayerStatus.h"
#include "PlayerSkillMgr.h"
#include "PlayerActionMgr.h"
//// �÷��̾� ���� ��� //////////////////////////

// ���߿� �ڵ� ���� 
#include "SkillSelectScene.h"

#include "MyBSphere.h"
#include "HitEffect.h"
#include "SkillEffect.h"
#include "MyBOBox.h" 
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

UINT				    	GameScene::m_ShadowmapCount{ 2 };
std::list<Texture*>         GameScene::m_pConnectedTextureList;
UINT	   		            GameScene::m_TextureCount{ 0 };
ID3D12DescriptorHeap*		GameScene::m_pd3dCbvSrUavDescriptorHeap{ nullptr };

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
	// ����� �ؽ��� ������ŭ �� ����
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
	// ����� �ؽ��� ������ŭ �� ����

	// ����� �ؽ��Ŀ� �� ����
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

void GameScene::SetDescriptorHeap(ID3D12GraphicsCommandList * pd3dCommandList)
{
	// �� ����
	pd3dCommandList->SetDescriptorHeaps(1, &m_pd3dCbvSrUavDescriptorHeap);
}

void GameScene::ReleaseHeaps()
{
	if (m_pd3dCbvSrUavDescriptorHeap)
	{
		m_pd3dCbvSrUavDescriptorHeap->Release();
		m_pd3dCbvSrUavDescriptorHeap = nullptr;
	}
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
	switch (nMessageID)
	{
	case WM_MOUSEWHEEL:
	case WM_MOUSEHWHEEL: 
		break;
	case WM_LBUTTONDOWN:
		// �÷��̾� ����
		if (!GameInput::GetDragMode())
		{
			if (!m_pPlayer->GetPlayerActionMgr()->Is_StandardAttackAction() && !m_pPlayer->GetPlayerActionMgr()->isBroomMode() && !m_pPlayer->GetPlayerActionMgr()->Is_BroomPrepareAction())
			{
				SoundManager::GetInstance()->Play(ENUM_SOUND::PLAYER_MAGIC_MISIL);

				bool isAttackDragon = m_pPlayer->Attack(
					static_cast<Status*>(
						m_Dragon->GetStatus()),
					m_Dragon->GetBOBox(),
					m_AimPoint->GetPickingPoint(),
					m_pMainCamera->GetCamera(), true);

				if (isAttackDragon)
				{
					HitEffectMgr::GetInstance()->AddNormalHitEffectPosition(m_Dragon->GetBOBox()->GetBOBox().Center);
					return false;
				}

				for (int i = 0; i < m_TestMonsterCount; ++i)
				{
					bool isAttack = m_pPlayer->Attack(
						static_cast<Status*>(
							m_TestMonster[i]->GetStatus()),
						m_TestMonster[i]->GetBOBox(),
						m_AimPoint->GetPickingPoint(),
						m_pMainCamera->GetCamera(),false);
					 
					if (isAttack)
					{
						HitEffectMgr::GetInstance()->AddNormalHitEffectPosition(m_TestMonster[i]->GetBOBox()->GetBOBox().Center);
						break;
					} 
				}
			}
		}
		break;
	case WM_RBUTTONDOWN:
		break;
	case WM_LBUTTONUP: 
		break;
	case WM_RBUTTONUP:
		break;
	case WM_MOUSEMOVE: 
	{
		XMFLOAT2 delta = GameInput::MouseMoveForGameScene(hWnd, this);
		if ((delta.x != 0.0f) || (delta.y != 0.0f))
		{
			m_pMainCamera->GetCamera()->Rotate(delta.y, delta.x, 0.0f);
			m_pPlayer->Rotate(0.0f, delta.x, 0.0f); 
		}
	}
		break;

	default:
		break;
	}

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

		case MYVK_Q: 
			// �÷��̾� ����
			if (GameInput::GetDragMode())
			{
				 
				if (!m_pPlayer->GetPlayerActionMgr()->Is_StandardAttackAction() && !m_pPlayer->GetPlayerActionMgr()->isBroomMode() && !m_pPlayer->GetPlayerActionMgr()->Is_BroomPrepareAction())
				{
					SoundManager::GetInstance()->Play(ENUM_SOUND::PLAYER_MAGIC_MISIL);

					bool isAttackDragon = m_pPlayer->Attack(
						static_cast<Status*>(
							m_Dragon->GetStatus()),
						m_Dragon->GetBOBox(),
						m_AimPoint->GetPickingPoint(),
						m_pMainCamera->GetCamera(), true);

					if (isAttackDragon)
					{
						HitEffectMgr::GetInstance()->AddNormalHitEffectPosition(m_Dragon->GetBOBox()->GetBOBox().Center);
						return false;
					}

					for (int i = 0; i < m_TestMonsterCount; ++i)
					{
						bool isAttack = m_pPlayer->Attack(
							static_cast<Status*>(
								m_TestMonster[i]->GetStatus()),
							m_TestMonster[i]->GetBOBox(),
							m_AimPoint->GetPickingPoint(),
							m_pMainCamera->GetCamera(), false);

						if (isAttack)
						{
							HitEffectMgr::GetInstance()->AddNormalHitEffectPosition(m_TestMonster[i]->GetBOBox()->GetBOBox().Center);
							break;
						}
					}
				}
/*
				bool isAttackDragon = m_pPlayer->Attack(
					static_cast<Status*>(
						m_Dragon->GetStatus()),
					m_Dragon->GetBOBox(),
					m_AimPoint->GetPickingPoint(),
					m_pMainCamera->GetCamera(), true);
				if (isAttackDragon)
				{
					HitEffectMgr::GetInstance()->AddNormalHitEffectPosition(m_Dragon->GetBOBox()->GetBOBox().Center);
					return false;
				}

				if (!m_pPlayer->GetPlayerActionMgr()->Is_StandardAttackAction() && !m_pPlayer->GetpBroom()->GetisUsing())
				{
					for (int i = 0; i < m_TestMonsterCount; ++i)
					{
						bool isAttack = m_pPlayer->Attack(
							static_cast<Status*>(
								m_TestMonster[i]->GetStatus()),
							m_TestMonster[i]->GetBOBox(),
							m_AimPoint->GetPickingPoint(),
							m_pMainCamera->GetCamera(), false);

						if (isAttack)
						{
							HitEffectMgr::GetInstance()->AddNormalHitEffectPosition(m_TestMonster[i]->GetBOBox()->GetBOBox().Center);
							break;
						}
					} 
				}*/
			}
			break;

		case 'Z':
			// �ӽ÷� ü�� 100�� ��� ����
			m_pPlayer->SubstractHP(100);
			SoundManager::GetInstance()->Stop(ENUM_SOUND::PLAYER_DAMAGE);
			break;

		case '1': // ��ų ���ڷ�  
			m_pPlayer->UseSkill_Broom(); 
			m_pPlayer->GetPlayerActionMgr()->ChangeActionToBroomPrepare();
			// SoundManager::GetInstance()->Play(ENUM_SOUND::PLAYER_BROOM);
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
			break;

		case VK_SPACE:
			// 
			break;

		case MYVK_E: 
			m_pPlayer->GetPlayerActionMgr()->ChangeActionToStandardAttack(); 
			PlayerSkillMgr::GetInstance()->Activate(m_pPlayer->GetMPStatus(), 0);
			break;

		case MYVK_R:
			m_pPlayer->GetPlayerActionMgr()->ChangeActionToStandardAttack(); 
			PlayerSkillMgr::GetInstance()->Activate(m_pPlayer->GetMPStatus(), 1);
			break;

		case MYVK_T:
			m_pPlayer->GetPlayerActionMgr()->ChangeActionToStandardAttack(); 
			PlayerSkillMgr::GetInstance()->Activate(m_pPlayer->GetMPStatus(), 2);
			break;

		case MYVK_Y: 			
			m_pPlayer->GetPlayerActionMgr()->ChangeActionToStandardAttack(); 
			PlayerSkillMgr::GetInstance()->Activate(m_pPlayer->GetMPStatus(), 3);
			break;
		case '4':
			break;

		case '5':
			// ��� ���� Ȱ��ȭ ��Ű�� ġƮ
			for (int x = 0; x < 5; ++x)
			{
				m_AltarSphere[x]->SetisActive(true);
				m_Dragon->IsNotStone();
			}
			break;

		case '6':
			// ���� ���� �״� ġƮ
			m_Dragon->SubstractHP(100);
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
	// �÷��̾� ���� ////////////////////////////////////////

	// �׽�Ʈ��  
	std::random_device rd;
	std::mt19937 mersenne(rd());
	std::uniform_int_distribution<> die(2000, 15000);
	std::uniform_int_distribution<> monstertype(0, 5);
	 
	m_Dragon = new Dragon("Dragon",
		XMFLOAT3(14500, 0, 15000),
		pd3dDevice, pd3dCommandList, GraphicsRootSignatureMgr::GetGraphicsRootSignature());
	 
	m_TestMonster = new Monster*[m_TestMonsterCount];
	for (int x = 0; x < m_TestMonsterCount; ++x)
	{
		m_TestMonster[x] = nullptr;
	}

	int spacecatblue_count = 0;
	int spacecatgreen_count = 0;
	int spacecatpink_count = 0;
	int creepymonster_count = 0;
	int mushroom_count = 0;
	int boss_count = 0;

	MonsterTransformStorage*const instance = MonsterTransformStorage::GetInstance();
	instance->CreateInfo(pd3dDevice, pd3dCommandList);

	for (int i = 0; i < m_TestMonsterCount; )
	{
		int value = monstertype(mersenne);

		if (value == ENUM_MONSTER::MONSTER_CREEPYMONSTER)
		{
#ifdef _DEBUG
			std::cout << instance->Count(CREEPYMONSTER) << "������ �ִ��Դϴ�. ���� " << creepymonster_count << "���� " << std::endl;
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
			std::cout << instance->Count(MUSHROOM) << "������ �ִ��Դϴ�. ���� " << mushroom_count << "���� " << std::endl;
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
			std::cout << instance->Count(SPACECAT_BLUE) << "������ �ִ��Դϴ�. ���� " << spacecatblue_count << "���� " << std::endl;
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
			std::cout << instance->Count(SPACECAT_PINK) << "������ �ִ��Դϴ�. ���� " << spacecatpink_count << "���� " << std::endl;
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
			std::cout << instance->Count(SPACECAT_GREEN) << "������ �ִ��Դϴ�. ���� " << spacecatgreen_count << "���� " << std::endl;
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
			std::cout << instance->Count(DRAGON) << "������ �ִ��Դϴ�. ���� " << boss_count << "���� " << std::endl;
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
	 
	// UI �̹��� �߰�
	m_TESTGameObject = new EmptyGameObject("Test");
	 
	// �̴ϸ�
	m_SampleUIMap = new UI2DImage(
		m_TESTGameObject, ENUM_SCENE::SCENE_GAME, pd3dDevice, pd3dCommandList,
		POINT{ int(GameScreen::GetWidth()) / 2 + 500, int(GameScreen::GetHeight()) / 2 - 220 },
		m_UIMapSize.x,
		m_UIMapSize.y,
		""
	);

	// ��ų 1
	m_SampleUISkill1 = new UI2DImage(
		m_TESTGameObject, ENUM_SCENE::SCENE_GAME, pd3dDevice, pd3dCommandList,
		POINT{ int(GameScreen::GetWidth()) / 2 - 300, int(GameScreen::GetHeight()) / 2 + 270 }, 100, 100,
		""
	);

	// ��ų 2
	m_SampleUISkill2 = new UI2DImage(
		m_TESTGameObject, ENUM_SCENE::SCENE_GAME, pd3dDevice, pd3dCommandList,
		POINT{ int(GameScreen::GetWidth()) / 2 - 100, int(GameScreen::GetHeight()) / 2 + 270 }, 100, 100,
		""
	);

	// ��ų 3
	m_SampleUISkill3 = new UI2DImage(
		m_TESTGameObject, ENUM_SCENE::SCENE_GAME, pd3dDevice, pd3dCommandList,
		POINT{ int(GameScreen::GetWidth()) / 2 + 100, int(GameScreen::GetHeight()) / 2 + 270 }, 100, 100,
		""
	);

	// ��ų 4
	m_SampleUISkill4 = new UI2DImage(
		m_TESTGameObject, ENUM_SCENE::SCENE_GAME, pd3dDevice, pd3dCommandList,
		POINT{ int(GameScreen::GetWidth()) / 2 + 300, int(GameScreen::GetHeight()) / 2 + 270 }, 100, 100,
		""
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
		m_TESTGameObject, ENUM_SCENE::SCENE_GAME, pd3dDevice, pd3dCommandList,
		POINT{ int(pos.x), int(pos.z) },
		20, 20,
		"Blue"
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
		"Blue"
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
		m_TESTGameObject, ENUM_SCENE::SCENE_GAME, pd3dDevice, pd3dCommandList,
		POINT{ int(pos.x), int(pos.z) },
		20, 20,
		"Blue"
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
		m_TESTGameObject, ENUM_SCENE::SCENE_GAME, pd3dDevice, pd3dCommandList,
		POINT{ int(pos.x), int(pos.z) },
		20, 20,
		"Blue"
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
		m_TESTGameObject, ENUM_SCENE::SCENE_GAME, pd3dDevice, pd3dCommandList,
		POINT{ int(pos.x), int(pos.z) },
		20, 20,
		"Blue"
	);

	m_UIPlayer = new UI2DImage(
		m_TESTGameObject, ENUM_SCENE::SCENE_GAME, pd3dDevice, pd3dCommandList,
		POINT{ 0, 0 },
		10, 10,
		"Red"
	);

	m_UIBossMonster = new UI2DImage(
		m_TESTGameObject, ENUM_SCENE::SCENE_GAME, pd3dDevice, pd3dCommandList,
		POINT{ 0, 0 },
		10, 10,
		"Yellow"
	);
}

void GameScene::ReleaseObjects()
{  
	MonsterTransformStorage::ReleaseInstance();
	
	ReleaseHeaps();
	 
	if (LightManager::m_pLights)
	{
		delete LightManager::m_pLights;
		LightManager::m_pLights = nullptr;
	}  
	if (m_pMaterials)
	{ 
		delete m_pMaterials;
		m_pMaterials = nullptr;
	} 
	if (m_pd3dcbLights)
	{
		m_pd3dcbLights->Unmap(0, NULL);
		m_pd3dcbLights->Release();
		m_pd3dcbLights = nullptr;
	}

	if (m_pd3dcbMaterials)
	{
		m_pd3dcbMaterials->Unmap(0, NULL);
		m_pd3dcbMaterials->Release();
		m_pd3dcbMaterials = nullptr;
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
	if (m_AimPoint)
	{
		m_AimPoint->ReleaseObjects();
		delete m_AimPoint;
		m_AimPoint = nullptr;
	}
	if (m_Dragon)
	{ 
		m_Dragon->ReleaseObjects();
		delete m_Dragon;
		m_Dragon = nullptr;
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
	if (m_TESTGameObject)
	{
		m_TESTGameObject->ReleaseObjects();
		delete m_TESTGameObject;
		m_TESTGameObject = nullptr;
	}
	if (m_SampleUIMap)
	{  
		m_SampleUIMap->ReleaseObjects();
		delete m_SampleUIMap;
		m_SampleUIMap = nullptr;
	}
	if (m_UIAltar_1)
	{ 
		m_UIAltar_1->ReleaseObjects();
		delete m_UIAltar_1;
		m_UIAltar_1 = nullptr;
	} 
	if (m_UIAltar_2)
	{ 
		m_UIAltar_2->ReleaseObjects();
		delete m_UIAltar_2;
		m_UIAltar_2 = nullptr;
	} 
	if (m_UIAltar_3)
	{ 
		m_UIAltar_3->ReleaseObjects();
		delete m_UIAltar_3;
		m_UIAltar_3 = nullptr;
	} 
	if (m_UIAltar_4)
	{ 
		m_UIAltar_4->ReleaseObjects();
		delete m_UIAltar_4;
		m_UIAltar_4 = nullptr;
	}
	if (m_UIAltar_5)
	{ 
		m_UIAltar_5->ReleaseObjects();
		delete m_UIAltar_5;
		m_UIAltar_5 = nullptr;
	}
	if (m_UIPlayer)
	{ 
		m_UIPlayer->ReleaseObjects();
		delete m_UIPlayer;
		m_UIPlayer = nullptr;
	} 
	if (m_UIBossMonster)
	{ 
		m_UIBossMonster->ReleaseObjects();
		delete m_UIBossMonster;
		m_UIBossMonster = nullptr;
	}
	if (m_SampleUISkill1)
	{ 
		m_SampleUISkill1->ReleaseObjects();
		delete m_SampleUISkill1;
		m_SampleUISkill1 = nullptr;
	}
	if (m_SampleUISkill2)
	{ 
		m_SampleUISkill2->ReleaseObjects();
		delete m_SampleUISkill2;
		m_SampleUISkill2 = nullptr;
	}
	if (m_SampleUISkill3)
	{ 
		m_SampleUISkill3->ReleaseObjects();
		delete m_SampleUISkill3;
		m_SampleUISkill3 = nullptr;
	}
	if (m_SampleUISkill4)
	{ 
		m_SampleUISkill4->ReleaseObjects();
		delete m_SampleUISkill4;
		m_SampleUISkill4 = nullptr;
	} 
	for (int x = 0; x < 5; ++x)
	{
		if (m_AltarSphere[x])
		{
			m_AltarSphere[x]->ReleaseObjects();
			delete m_AltarSphere[x];
			m_AltarSphere[x] = nullptr;
		}
	}

	if (m_TestMonster)
	{
		for (int i = 0; i < m_TestMonsterCount; ++i)
		{
			if (m_TestMonster[i])
			{
				m_TestMonster[i]->ReleaseObjects();
				delete m_TestMonster[i];
				m_TestMonster[i] = nullptr;
			}
		}

		delete[] m_TestMonster;
		m_TestMonster = nullptr;
	}
}

bool GameScene::ProcessInput(HWND hWnd, float fElapsedTime)
{
	// �÷��̾� �̵��� ���� ó�� (��Ȯ���� �̵��� �ƴ϶� ���ӵ�)
	m_pPlayer->ProcessInput(fElapsedTime);

	//// �÷��̾� ȸ���� ���� ó��
	//float deltaX = GameInput::GetDeltaX();
	//float deltaY = GameInput::GetDeltaY();

	//if ((deltaX != 0.0f) || (deltaY != 0.0f))
	//{
	//	if (deltaX || deltaY)
	//	{
	//		// �÷��̾�� ī�޶� �Ȱ��� rotate...
	//		// ���� �������̹Ƿ� ���� ����
	//		m_pMainCamera->GetCamera()->Rotate(deltaY, deltaX, 0.0f);
	//		m_pPlayer->Rotate(0.0f, deltaX, 0.0f);
	//	}
	//}

	return true;
}

void GameScene::UpdatePhysics(float fElapsedTime)
{
	m_Dragon->UpdateState(fElapsedTime);

	// ��ų ����Ʈ ���ӵ� ó��
	PlayerSkillMgr::GetInstance()->UpdatePhysics(fElapsedTime);
	BossSkillMgr::GetInstance()->UpdatePhysics(fElapsedTime);

	for (int i = 0; i < m_TestMonsterCount; ++i) 
	{
		if(m_TestMonster[i]) m_TestMonster[i]->UpdateState(fElapsedTime); // State�� ������Ʈ ó��...
	}

	// �ݵ�� UpdatePhysics �������� �浹 ó���� �ؾ���.
	// �׷��� �浹�� �Ϳ� ���� �����̵� ������ ���ӵ� ó���� �� �� ����.
	UpdateCollision(fElapsedTime);
}

// ProcessInput�� ���� right, up, look, pos �� ���庯ȯ ��Ŀ� �����Ѵ�.
void GameScene::Update(float fElapsedTime)
{
	HitEffectMgr::GetInstance()->Update(fElapsedTime);
	 
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
	 
	//// ���� ���� X //// 
	if (m_pPlayer) m_pPlayer->Update(fElapsedTime); //Velocity�� ���� pos �̵�

	if (m_SkyBox) m_SkyBox->Update(fElapsedTime); 

	m_Dragon->Update(fElapsedTime);
	for (int i = 0; i < m_TestMonsterCount; ++i)
	{
		m_TestMonster[i]->Update(fElapsedTime);
	}

	// �÷��̾� ��ų �̵� ������Ʈ.. �÷��̾� �����ǿ� ����� �ϹǷ� �ݵ�� Player Update���Ŀ� ȣ���ؾ���.
	PlayerSkillMgr::GetInstance()->Update(fElapsedTime);

	//// ���� ���� X ////
	BossSkillMgr::GetInstance()->Update(fElapsedTime); // ��ų ����Ʈ �̵�

	for (int x = 0; x < 5; ++x)
	{
		m_AltarSphere[x]->Update(fElapsedTime);
	}

	// light update
	if (m_pcbMappedLights) ::memcpy(m_pcbMappedLights, LightManager::m_pLights, sizeof(LIGHTS));
	// material update
	if (m_pcbMappedMaterials) ::memcpy(m_pcbMappedMaterials, m_pMaterials, sizeof(MATERIAL));
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
	for (int index = 0; index < SKILL_SELECTED; ++index)
	{
		// ��ų Ȱ��ȭ�� �Ǿ����� �ʴٸ� �Ѿ��.
		if (!PlayerSkillMgr::GetInstance()->GetpSelectableSkill(index)->isActive) continue;
		
		// ���� ��ų�� ATTACK Ÿ���� �ƴ϶�� �Ѿ��.
		if (PlayerSkillMgr::GetInstance()->GetpSelectableSkill(index)->m_skillEffect->m_Skilltype != SKILLTYPE_ATTACK) continue;
		 
		MyCollider* skill_collider = PlayerSkillMgr::GetInstance()->GetpSelectableSkill(index)->m_skillEffect->GetCollier();

		// ��� ���� ���� �浹üũ
		for (int i = 0; i < m_TestMonsterCount; ++i)
		{
			// ü���� 0���� ������ �˻����� �ʴ´�.
			if (m_TestMonster[i]->GetStatus()->GetGuage() <= 0.f) continue;

			if (Collision::isCollide(m_TestMonster[i]->GetBOBox(), skill_collider))
			{
				auto effect = PlayerSkillMgr::GetInstance()->GetpSelectableSkill(index)->m_skillEffect->GetCollier();

				XMFLOAT3 pos{ -100, -100, -100 };
				switch (effect->GetType())
				{
					case
					BOUNDING_BOX:
						pos = static_cast<MyBOBox*>(effect)->GetBOBox().Center;
						break;
					case BOUNDING_SPHERE:
						pos = static_cast<MyBSphere*>(effect)->GetBSphere()->Center;
						break;
					default:
						break;
				}

				HitEffectMgr::GetInstance()->AddPlayerSkillHitEffectPosition(PlayerSkillMgr::GetInstance()->GetpSelectableSkill(index)->GetSelectableSkillType(), pos);
				m_TestMonster[i]->SubstractHP(5);
				PlayerSkillMgr::GetInstance()->Deactive(index);
			}

		}

		//////////////////////////////////////////////////////// BOSS�� ����
		// ü���� 0���� ������ �˻����� �ʴ´�.
		if (m_Dragon->GetStatus()->GetGuage() <= 0.f) continue;

		if (Collision::isCollide(m_Dragon->GetBOBox(), skill_collider))
		{

#ifdef _DEBUG
			std::cout << "������ ��ų�� ����" << std::endl;
#endif // _DEBUG
			auto effect = PlayerSkillMgr::GetInstance()->GetpSelectableSkill(index)->m_skillEffect->GetCollier();
			
			XMFLOAT3 pos{ -100, -100, -100 };
			switch (effect->GetType())
			{
			case
			BOUNDING_BOX:
				pos = static_cast<MyBOBox*>(effect)->GetBOBox().Center;
				break;
			case BOUNDING_SPHERE:
				pos = static_cast<MyBSphere*>(effect)->GetBSphere()->Center;
				break;
			default:
				break;
			}

			HitEffectMgr::GetInstance()->AddPlayerSkillHitEffectPosition(
				PlayerSkillMgr::GetInstance()->GetpSelectableSkill(index)->GetSelectableSkillType(),
				pos);
			m_Dragon->SubstractHP(5);
			PlayerSkillMgr::GetInstance()->Deactive(index);
		}
		//////////////////////////////////////////////////////// BOSS�� ����
	}
	  
	// update�� ��ġ�� ������ ��ų������ �÷��̾�� �����ϴ��� Ȯ��
	for (int index = 0; index < 10; ++index)
	{
		if (m_pPlayer->GetHPStatus()->GetGuage() <= 0.f) break;

		// ��ų Ȱ��ȭ�� �Ǿ����� �ʴٸ� �Ѿ��.
		if (!BossSkillMgr::GetInstance()->GetpSelectableSkill(index)->isActive) continue;

		// ���� ��ų�� ATTACK Ÿ���� �ƴ϶�� �Ѿ��.
		if (BossSkillMgr::GetInstance()->GetpSelectableSkill(index)->m_skillEffect->m_Skilltype != SKILLTYPE_ATTACK) continue;

		MyCollider* skill_collider = BossSkillMgr::GetInstance()->GetpSelectableSkill(index)->m_skillEffect->GetCollier();

		if (Collision::isCollide(m_pPlayer->GetBOBox(), skill_collider))
		{
			auto effect = BossSkillMgr::GetInstance()->GetpSelectableSkill(index)->m_skillEffect->GetCollier();

			XMFLOAT3 pos{ -100, -100, -100 };
			switch (effect->GetType())
			{
				case
				BOUNDING_BOX:
					pos = static_cast<MyBOBox*>(effect)->GetBOBox().Center;
					break;
				case BOUNDING_SPHERE:
					pos = static_cast<MyBSphere*>(effect)->GetBSphere()->Center;
					break;
				default:
					break;
			}

			HitEffectMgr::GetInstance()->AddPlayerSkillHitEffectPosition(
				BossSkillMgr::GetInstance()->GetpSelectableSkill(index)->GetSelectableSkillType(),
				pos);

			m_pPlayer->SubstractHP(15);
			BossSkillMgr::GetInstance()->Deactive(index);
		}
	}


	// Update�� ��ġ�� ���Ͱ� ���� �ÿ� ����/�÷��̾��浹üũ Ȯ�� ///////////////////////////
	for (int i = 0; i < m_TestMonsterCount; ++i)
	{
		if (!m_TestMonster[i]->GetisAttacking()) continue;
		if (m_TestMonster[i]->GetisFinishAttack()) continue;

		if (Collision::isCollide(m_pPlayer->GetBOBox()->GetBOBox(), m_TestMonster[i]->GetBOBox()->GetBOBox()))
		{
			auto playercenterpos = m_pPlayer->GetBOBox()->GetBOBox().Center;
			auto monstercenterpos = m_TestMonster[i]->GetBOBox()->GetBOBox().Center;
			auto effectpos = Vector3::Add(playercenterpos, Vector3::ScalarProduct(Vector3::Subtract(playercenterpos, monstercenterpos), 0.5, false));

			HitEffectMgr::GetInstance()->AddMonsterHitEffectPosition(effectpos);

			m_TestMonster[i]->FinishAttack();
			m_pPlayer->SubstractHP(5);
		} 
	}
	 
	// ������ �÷��� �ε���
	if (Collision::isCollide(m_pPlayer->GetBOBox() , m_Dragon->GetBOBox()) 
		&& m_Dragon->GetisAttacking()
		&& !m_Dragon->GetisFinishAttack())
	{ 
		m_Dragon->FinishAttack();
		BossMonsterActionMgr* actionmgr = static_cast<BossMonsterActionMgr*>(m_Dragon->GetActionMgr());

		XMFLOAT3 effectpos = m_pPlayer->GetBOBox()->GetBOBox().Center;
		 
		if (actionmgr->Is_BossSkillDownStroke())
		{ 
			HitEffectMgr::GetInstance()->AddMonsterHitEffectPosition(effectpos);
			m_pPlayer->SubstractHP(5);
		}
		else if (actionmgr->Is_BossSkillTailAttack())
		{ 
			HitEffectMgr::GetInstance()->AddMonsterHitEffectPosition(effectpos); 
			m_pPlayer->SubstractHP(15);
		}
		else if (actionmgr->Is_BossSkillRush())
		{ 
			HitEffectMgr::GetInstance()->AddMonsterHitEffectPosition(effectpos); 
			m_pPlayer->SubstractHP(20); 
		}
	}
	// ������ �÷��� �ε���

	// ī�޶� �������Ұ� ����Ʈ�� ���� ������ üũ
	if (m_pMainCamera && m_pQuadtreeTerrain)
	{
		m_pMainCamera->GetCamera()->GetFrustum()->CheckRendering(m_pQuadtreeTerrain->GetRootNode());
	}
	if (m_pQuadtreeTerrain) m_pQuadtreeTerrain->LastUpdate(fElapsedTime);
	// ��������X  
}

void GameScene::Init()
{    
	for (int x = 0; x < 5; ++x)
	{
		m_AltarSphere[x]->SetisActive(false);
		m_AltarSphere[x]->SetisEnguaged(false);
		m_AltarSphere[x]->SetGuage(0.f);
		m_AltarSphere[x]->SetAnimationID(ALTARSPHERE_IDLE.ID);
	}

	PlayerSkillMgr::GetInstance()->Init();
	 
	m_pPlayer->Init();
	m_Dragon->Init();

	m_pMainCamera->GetTransform().SetIdentity();
	m_Sniping->Init();

	for (int i = 0; i < m_TestMonsterCount; ++i)
	{ 
		if(m_TestMonster[i]) m_TestMonster[i]->Init();
	} 
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

	// �� ����
	pd3dCommandList->SetDescriptorHeaps(1, &m_pd3dCbvSrUavDescriptorHeap);

	// ������ �� ����
	pd3dCommandList->SetGraphicsRootDescriptorTable(ROOTPARAMETER_SHADOWTEXTURE, m_hGPUShadowmap);
	pd3dCommandList->SetGraphicsRootDescriptorTable(ROOTPARAMETER_PLAYERSHADOWTEXTURE, m_hGPUPlayerShadowmap);

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
	}

	if (m_SkyBox) m_SkyBox->Render(pd3dCommandList, isGBuffers);

	if (m_pPlayer) m_pPlayer->Render(pd3dCommandList, isGBuffers);
	 

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

 
	HitEffectMgr::GetInstance()->Render(pd3dCommandList);

	// �ͷ���
	if (m_Terrain)
	{
		m_pQuadtreeTerrain->Render(pd3dCommandList, m_Terrain, nullptr /*���� �ǹ̾���*/, isGBuffers);
	}

	BossSkillMgr::GetInstance()->Render(pd3dCommandList, isGBuffers); // ��ų ����Ʈ ����
	PlayerSkillMgr::GetInstance()->Render(pd3dCommandList, isGBuffers);

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

	if (!m_Dragon->GetisStone())
	{
		XMFLOAT2 bossmonsterpos{ m_Dragon->GetTransform().GetPosition().x,  m_Dragon->GetTransform().GetPosition().z };

		// ui map ���������� �����ϱ� ���� ũ�� ���
		bossmonsterpos.x = bossmonsterpos.x * (float(m_UIMapSize.x) / 30000.f);
		bossmonsterpos.y = -bossmonsterpos.y * (float(m_UIMapSize.y) / 30000.f); // ��ũ�� ��ǥ��� �̵��ϱ� ����

		// ��ũ�� ��ǥ�� ��ġ �̵�
		bossmonsterpos.x += m_SampleUIMap->getRect().left;
		bossmonsterpos.y += m_SampleUIMap->getRect().bottom;
		pd3dCommandList->SetGraphicsRoot32BitConstants(ROOTPARAMETER_PICKINGPOINT, 2, &bossmonsterpos, 0);
		m_UIBossMonster->Render(pd3dCommandList);
	}

	ShaderManager::GetInstance()->SetPSO(pd3dCommandList, SHADER_UISCREEN, isGBuffers);

	m_UIAltar_1->Render(pd3dCommandList);
	m_UIAltar_2->Render(pd3dCommandList);
	m_UIAltar_3->Render(pd3dCommandList);
	m_UIAltar_4->Render(pd3dCommandList);
	m_UIAltar_5->Render(pd3dCommandList);

	SkillSelectScene::m_pHeap->UpdateShaderVariable(pd3dCommandList);

	SkillSelectScene::m_pTexture->UpdateShaderVariable(pd3dCommandList, SKILL_TO_CHOOSE); // �ӽ÷� ���������� ������
	m_SampleUIMap->Render(pd3dCommandList);

	ShaderManager::GetInstance()->SetPSO(pd3dCommandList, SHADER_SKILLICON, isGBuffers);
	float cooltime = PlayerSkillMgr::GetInstance()->GetpSelectableSkill(0)->RemainCoolTimePrecentage;
	pd3dCommandList->SetGraphicsRoot32BitConstants(ROOTPARAMETER_HPPERCENTAGE, 1, &cooltime, 0);
	SkillSelectScene::m_pTexture->UpdateShaderVariable(pd3dCommandList, SkillSelectScene::m_SelectedIndex[0]);
	m_SampleUISkill1->Render(pd3dCommandList);

	SkillSelectScene::m_pTexture->UpdateShaderVariable(pd3dCommandList, SkillSelectScene::m_SelectedIndex[1]);
	cooltime = PlayerSkillMgr::GetInstance()->GetpSelectableSkill(1)->RemainCoolTimePrecentage;
	pd3dCommandList->SetGraphicsRoot32BitConstants(ROOTPARAMETER_HPPERCENTAGE, 1, &cooltime, 0);
	m_SampleUISkill2->Render(pd3dCommandList);

	SkillSelectScene::m_pTexture->UpdateShaderVariable(pd3dCommandList, SkillSelectScene::m_SelectedIndex[2]);
	cooltime = PlayerSkillMgr::GetInstance()->GetpSelectableSkill(2)->RemainCoolTimePrecentage;
	pd3dCommandList->SetGraphicsRoot32BitConstants(ROOTPARAMETER_HPPERCENTAGE, 1, &cooltime, 0);
	m_SampleUISkill3->Render(pd3dCommandList);

	SkillSelectScene::m_pTexture->UpdateShaderVariable(pd3dCommandList, SkillSelectScene::m_SelectedIndex[3]);
	cooltime = PlayerSkillMgr::GetInstance()->GetpSelectableSkill(3)->RemainCoolTimePrecentage;
	pd3dCommandList->SetGraphicsRoot32BitConstants(ROOTPARAMETER_HPPERCENTAGE, 1, &cooltime, 0);
	m_SampleUISkill4->Render(pd3dCommandList);
}

void GameScene::RenderForShadow(ID3D12GraphicsCommandList * pd3dCommandList)
{
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

	m_Dragon->RenderForShadow(pd3dCommandList);

	for (int i = 0; i < m_TestMonsterCount; ++i)
	{
		if (m_TestMonster[i]) m_TestMonster[i]->RenderForShadow(pd3dCommandList);
	}

	// �ͷ���
	if (m_Terrain)
	{
		m_pQuadtreeTerrain->RenderInstancingObjectsForShadow(pd3dCommandList);
	}
}

void GameScene::RenderForPlayerShadow(ID3D12GraphicsCommandList * pd3dCommandList)
{
	// Ŭ�� ȭ�� ����	
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
	if (m_SkyBox) m_SkyBox->ReleaseUploadBuffers();
	if (m_pPlayer) m_pPlayer->ReleaseUploadBuffers();
	if (m_Terrain) m_Terrain->ReleaseUploadBuffers();
	if (m_pQuadtreeTerrain) m_pQuadtreeTerrain->ReleaseUploadBuffers();
	if (m_Dragon) m_Dragon->ReleaseUploadBuffers();
	if (m_SampleUIMap) m_SampleUIMap->ReleaseUploadBuffers(); 
	if (m_UIAltar_1) m_UIAltar_1->ReleaseUploadBuffers(); 
	if (m_UIAltar_2) m_UIAltar_2->ReleaseUploadBuffers(); 
	if (m_UIAltar_3) m_UIAltar_3->ReleaseUploadBuffers();
	if (m_UIAltar_4) m_UIAltar_4->ReleaseUploadBuffers(); 
	if (m_UIAltar_5)  m_UIAltar_5->ReleaseUploadBuffers(); 
	if (m_UIPlayer) m_UIPlayer->ReleaseUploadBuffers();
	if (m_UIBossMonster) m_UIBossMonster->ReleaseUploadBuffers(); 
	if (m_SampleUISkill1) m_SampleUISkill1->ReleaseUploadBuffers();
	if (m_SampleUISkill2) m_SampleUISkill2->ReleaseUploadBuffers();
	if (m_SampleUISkill3) m_SampleUISkill3->ReleaseUploadBuffers();
	if (m_SampleUISkill4) m_SampleUISkill4->ReleaseUploadBuffers();
	 
	if (m_TestMonster)
	{
		for (int i = 0; i < m_TestMonsterCount; ++i)
		{
			if (m_TestMonster[i]) m_TestMonster[i]->ReleaseUploadBuffers();
		}
	}

	for (int i = 0; i < 5; ++i)
	{
		if (m_AltarSphere[i]) m_AltarSphere[i]->ReleaseUploadBuffers();
	}
	 
}

void GameScene::UpdateCollision(float fElapsedTime)
{
	// �ܰ�ó��
	MyBOBox outside_box[4]{
		{XMFLOAT3(-100, 0, 15000), XMFLOAT3(100, 3000, 20000)},
		{XMFLOAT3(15000, 0, 30100), XMFLOAT3(20000, 3000, 100)},
		{XMFLOAT3(30100, 0, 15000), XMFLOAT3(100, 3000, 20000)},
		{XMFLOAT3(15000, 0, -100), XMFLOAT3(30000, 3000, 100)},
	};

	// �ܰ� 4 �κа� �浹ó�� Ȯ���Ѵ�.
	Collision::ProcessCollideEdge(m_pPlayer, 4, outside_box, fElapsedTime);

	// �÷��̾�� �������� �浹�� Ȯ���Ѵ�. 
	Collision::ProcessCollideTerrainObject(m_pPlayer, m_pQuadtreeTerrain, fElapsedTime);
	 
	// ������ �������� �浹�� Ȯ���Ѵ�. 
	Collision::ProcessCollideTerrainObject(m_Dragon, m_pQuadtreeTerrain, fElapsedTime);
	  
	// ���� �浹üũ ///////////////////////// 
	for (int x = 0; x < m_TestMonsterCount; ++x)
	{
		Collision::ProcessCollideTerrainObject(m_TestMonster[x], m_pQuadtreeTerrain, fElapsedTime);
	} 
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

	//CreateConstantBuffer(pd3dDevice, pd3dCommandList, m_pd3dcbMaterials, sizeof(MATERIAL), (void **)&m_pcbMappedMaterials);

	UINT ncbMaterialBytes = ((sizeof(MATERIAL) + 255) & ~255); //256�� ���
	m_pd3dcbMaterials = d3dUtil::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbMaterialBytes, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);

	m_pd3dcbMaterials->Map(0, NULL, (void **)&m_pcbMappedMaterials);
	//////////////////////////// ����
}
