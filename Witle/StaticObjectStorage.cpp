#include "stdafx.h"
#include "d3dUtil.h"

#include "StaticObject.h"
#include "ModelStorage.h"
#include "ShaderManager.h"
#include "Shader.h"

#include "QuadTreeTerrainMesh.h"
#include "MapInfoLoader.h"
#include "Object.h" 
#include "StaticObjectStorage.h"

using namespace FileRead;

StaticObjectStorage* StaticObjectStorage::m_Instance{ nullptr };
 
#define TREE "ReflexTree"
#define ROCK "Rock"
#define PILLAR "Pillar"
#define SUNFLOWER "Sunflower"
#define ALTER "Altar"

void StaticObjectStorage::UpdateShaderVariables(ID3D12GraphicsCommandList * pd3dCommandList, int count, XMFLOAT4X4 * transforms)
{ 
	pd3dCommandList->SetGraphicsRootShaderResourceView(ROOTPARAMETER_INSTANCING, m_pd3dcbGameObjects->GetGPUVirtualAddress());

	for (UINT x = 0; x < count; x++)
	{
		XMStoreFloat4x4(&m_pcbMappedGameObjects[x].m_xmf4x4Transform, XMMatrixTranspose(XMLoadFloat4x4(&transforms[x])));
	}
}

void StaticObjectStorage::LoadTerrainObjectFromFile(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, const char * pstrFileName, const QuadTreeTerrainMesh const * pTerrain)
{
	FILE *pInFile = NULL;
	::fopen_s(&pInFile, pstrFileName, "rb");
	::rewind(pInFile);

	char pstrToken[64] = { '\0' };

	// ���� �����ϱ� ���� �ͷ��� ���� ������ ���߾� ������ �����Ѵ�.
	// �̸��� ���߾� �����ؾ��ϹǷ� �ϵ��ڵ��� �ؾ��Ѵ�.
	TerrainPieceCount = pTerrain->GetTerrainPieceCount();
	m_StaticObjectStorage[TREE] = new TerrainObjectInfo[TerrainPieceCount];
	m_StaticObjectStorage[ROCK] = new TerrainObjectInfo[TerrainPieceCount];
	m_StaticObjectStorage[PILLAR] = new TerrainObjectInfo[TerrainPieceCount];
	m_StaticObjectStorage[SUNFLOWER] = new TerrainObjectInfo[TerrainPieceCount];
	m_StaticObjectStorage[ALTER] = new TerrainObjectInfo[TerrainPieceCount];

	for (; ; )
	{
		if (::ReadStringFromFile(pInFile, pstrToken))
		{
			if (!strcmp(pstrToken, "<Hierarchy>:"))
			{
				int nFrame = ::ReadIntegerFromFile(pInFile);
				for (int i = 0; i < nFrame; ++i)
				{
					::ReadStringFromFile(pInFile, pstrToken);
					if (!strcmp(pstrToken, "<Frame>:"))
					{
						LoadNameAndPositionFromFile(pd3dDevice, pd3dCommandList, pInFile, pTerrain);
					}
				}
			}
			else if (!strcmp(pstrToken, "</Hierarchy>"))
			{
				break;
			}
		}
		else
		{
			break;
		}
	}

#ifdef _WITH_DEBUG_FRAME_HIERARCHY
	TCHAR pstrDebug[256] = { 0 };
	_stprintf_s(pstrDebug, 256, _T("Frame Hierarchy\n"));
	OutputDebugString(pstrDebug);

	LoadObject::PrintFrameInfo(pLoadedModel->m_pModelRootObject, NULL);
#endif

	::fclose(pInFile);
}

void StaticObjectStorage::LoadNameAndPositionFromFile(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, FILE * pInFile, const QuadTreeTerrainMesh const * pTerrain)
{
	char pstrToken[64] = { '\0' };
	UINT nReads = 0;

	char name[64];

	int nFrame = ::ReadIntegerFromFile(pInFile);
	::ReadStringFromFile(pInFile, name);
	 
	for (; ; )
	{
		::ReadStringFromFile(pInFile, pstrToken);
		if (!strcmp(pstrToken, "<GlobalTransform>:"))
		{
			TerrainObjectCount += 1;
			XMFLOAT4X4 temp;
			nReads = (UINT)::fread(&temp, sizeof(XMFLOAT4X4), 1, pInFile);
			XMFLOAT4X4 transform = Matrix4x4::Identity();
			transform._41 = -temp._41;
			transform._42 = temp._42;
			transform._43 = -temp._43;

			// ����� ���� ���° ���̵����� �� ����� ������忡 �����ϴ� ��ġ���� �˾Ƴ���...
			// fbx sdk ���� �����ö� ���� ������ �ִ��� x, z��ǥ�� -��ȣ �ٿ����� ...
			
			//if (!strcmp(name, TREE))
			//{
			//	TerrainObjectCount += 1;
			//	for (int x = 0; x < QUAD; ++x)
			//	{
			//		if (terrainIDs[x] == -1) continue;
			//		m_StaticObjectStorage[TREE][terrainIDs[x]].TerrainObjectCount += 1;

			//		LoadObject* TestObject = ModelStorage::GetInstance()->GetRootObject(TREE);
			//		TestObject->SetPosition(XMFLOAT3{ transform._41, transform._42, transform._43 });
			//		TestObject->UpdateTransform(NULL);
			//		 
			//		m_StaticObjectStorage[TREE][terrainIDs[x]].TransformList.emplace_back(TestObject->m_pChild->m_xmf4x4World);
			//		delete TestObject; 
			//	}
			//}
			//else if (!strcmp(name, ROCK))
			//{
			//	TerrainObjectCount += 1;
			//	for (int x = 0; x < QUAD; ++x)
			//	{
			//		if (terrainIDs[x] == -1) continue;
			//		m_StaticObjectStorage[ROCK][terrainIDs[x]].TerrainObjectCount += 1;

			//		LoadObject* TestObject = ModelStorage::GetInstance()->GetRootObject(ROCK);
			//		TestObject->SetPosition(XMFLOAT3{ transform._41, transform._42, transform._43 });
			//		TestObject->UpdateTransform(NULL);

			//		m_StaticObjectStorage[ROCK][terrainIDs[x]].TransformList.emplace_back(TestObject->m_pChild->m_xmf4x4World);
			//		delete TestObject;

			//	}
			//}
			//else if (!strcmp(name, PILLAR))
			//{
			//	TerrainObjectCount += 1;

			//	for (int x = 0; x < QUAD; ++x)
			//	{
			//		if (terrainIDs[x] == -1) continue;
			//		m_StaticObjectStorage[PILLAR][terrainIDs[x]].TerrainObjectCount += 1;

			//		LoadObject* TestObject = ModelStorage::GetInstance()->GetRootObject(PILLAR);
			//		TestObject->SetPosition(XMFLOAT3{ transform._41, transform._42, transform._43 });
			//		TestObject->UpdateTransform(NULL);

			//		m_StaticObjectStorage[PILLAR][terrainIDs[x]].TransformList.emplace_back(TestObject->m_pChild->m_xmf4x4World);
			//		delete TestObject;

			//	} 
			//}
			if (!strcmp(name, SUNFLOWER))
			{
				XMFLOAT3 position{ transform._41, transform._42, transform._43 };
				const int* terrainIDs = pTerrain->GetIDs(position);

#ifdef _WITH_DEBUG_TERRAIN_MAX_POS
				TCHAR pstrDebug[256] = { 0 };
				_stprintf_s(pstrDebug, 256, L"(position: %f, %f, %f) node: %d %d %d %d\n", position.x, position.y, position.z, terrainIDs[0], terrainIDs[1], terrainIDs[2], terrainIDs[3]);
				OutputDebugString(pstrDebug);
#endif 
				for (int x = 0; x < QUAD; ++x)
				{
					if (terrainIDs[x] == -1) continue;
					m_StaticObjectStorage[SUNFLOWER][terrainIDs[x]].TerrainObjectCount += 1;

					LoadObject* TestObject = ModelStorage::GetInstance()->GetRootObject(SUNFLOWER);
					TestObject->SetPosition(XMFLOAT3{ transform._41, transform._42, transform._43 });
					TestObject->UpdateTransform(NULL);

					m_StaticObjectStorage[SUNFLOWER][terrainIDs[x]].TransformList.emplace_back(TestObject->m_pChild->m_xmf4x4World);
					delete TestObject;
				} 

				delete terrainIDs;
			}
			//else if (!strcmp(name, ALTER))
			//{
			//	TerrainObjectCount += 1;

			//	for (int x = 0; x < QUAD; ++x)
			//	{
			//		if (terrainIDs[x] == -1) continue;
			//		//m_StaticObjectStorage[ALTER][terrainIDs[x]].TerrainObjectCount += 1;

			//		//LoadObject* TestObject = ModelStorage::GetInstance()->GetRootObject(ALTER);
			//		//TestObject->SetPosition(XMFLOAT3{ transform._41, transform._42, transform._43 });
			//		//TestObject->UpdateTransform(NULL);

			//		//m_StaticObjectStorage[ALTER][terrainIDs[x]].TransformList.emplace_back(TestObject->m_pChild->m_xmf4x4World);
			//		//delete TestObject; 
			//	}
			//}
			else
			{
			}

		}
		else if (!strcmp(pstrToken, "<Children>:"))
		{
			int nChilds = ::ReadIntegerFromFile(pInFile);
			if (nChilds > 0)
			{
				for (int i = 0; i < nChilds; i++)
				{
					::ReadStringFromFile(pInFile, pstrToken);
					if (!strcmp(pstrToken, "<Frame>:"))
					{
						LoadNameAndPositionFromFile(pd3dDevice, pd3dCommandList, pInFile, pTerrain);
#ifdef _WITH_DEBUG_FRAME_HIERARCHY
						TCHAR pstrDebug[256] = { 0 };
						_stprintf_s(pstrDebug, 256, _T("(Frame: %p) (Parent: %p)\n"), pChild, pGameObject);
						OutputDebugString(pstrDebug);
#endif
					}
				}
			}
		}
		else if (!strcmp(pstrToken, "</Frame>"))
		{
			break;
		}
	}
}

void StaticObjectStorage::CreateShaderVariables(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
{
	//�ν��Ͻ� ������ ������ ���� ���۸� ���ε� �� �������� �����Ѵ�.
	m_pd3dcbGameObjects = d3dUtil::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL,
		sizeof(VS_SRV_INSTANCEINFO) * TerrainObjectCount, D3D12_HEAP_TYPE_UPLOAD,
		D3D12_RESOURCE_STATE_GENERIC_READ, NULL);

	//���� ����(���ε� ��)�� ���� �����͸� �����Ѵ�.
	m_pd3dcbGameObjects->Map(0, NULL, (void **)&m_pcbMappedGameObjects);
}

StaticObjectStorage * StaticObjectStorage::GetInstance(const QuadTreeTerrainMesh const * pTerrain)
{
	if (!m_Instance)
	{
		m_Instance = new StaticObjectStorage();
	}

	return m_Instance;
}

void StaticObjectStorage::CreateInfo(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, const QuadTreeTerrainMesh const * pTerrain)
{
	if (m_isCreate) return;

	// ��ġ ������ �о�´�.
	LoadTerrainObjectFromFile(pd3dDevice, pd3dCommandList, "Information/Terrain.bin", pTerrain);
	
	// �о�� ��ġ ������ ������� ���̴� ������ �����Ѵ�.
	CreateShaderVariables(pd3dDevice, pd3dCommandList);

	// �ӽ÷� ����
	m_TestObject = ModelStorage::GetInstance()->GetRootObject(SUNFLOWER);

	m_isCreate = true;
}
 
void StaticObjectStorage::Render(ID3D12GraphicsCommandList * pd3dCommandList, int terrainID)
{
	pd3dCommandList->SetPipelineState(ShaderManager::GetInstance()->GetShader("InstancingStandardShader")->GetPSO());
	UpdateShaderVariables(pd3dCommandList, m_StaticObjectStorage[SUNFLOWER][terrainID].TerrainObjectCount, m_StaticObjectStorage[SUNFLOWER][terrainID].TransformList.begin()._Ptr);
	m_TestObject->RenderInstancing(pd3dCommandList, m_StaticObjectStorage[SUNFLOWER][terrainID].TerrainObjectCount);
}
