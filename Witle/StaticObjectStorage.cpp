#include "stdafx.h"
#include "d3dUtil.h"
#include "QuadTreeTerrainMesh.h"
#include "MapInfoLoader.h"
#include "Object.h" 
#include "StaticObjectStorage.h"

using namespace FileRead;

StaticObjectStorage* StaticObjectStorage::m_Instance{ nullptr };
 
#define TREE "Tree"
#define ROCK "Rock"
#define PILLAR "Pillar"
#define SUNFLOWER "Sunflower"
#define ALTER "Altar"

void StaticObjectStorage::LoadTerrainObjectFromFile(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, const char * pstrFileName, const QuadTreeTerrainMesh const * pTerrain)
{
	FILE *pInFile = NULL;
	::fopen_s(&pInFile, pstrFileName, "rb");
	::rewind(pInFile);

	char pstrToken[64] = { '\0' };

	// 먼저 시작하기 전에 터레인 개수 조각에 맞추어 인포를 구성한다.
	// 이름에 맞추어 구성해야하므로 하드코딩을 해야한다.
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
			XMFLOAT4X4 transform;
			nReads = (UINT)::fread(&transform, sizeof(XMFLOAT4X4), 1, pInFile);

			// 계산을 통해 몇번째 아이디인지 즉 어디의 리프노드에 존재하는 위치인지 알아낸다...
			const int* terrainIDs = pTerrain->GetIDs(XMFLOAT3{ transform._41, transform._42, transform._43});
			 
			if (!strcmp(name, TREE))
			{
				for (int x = 0; x < QUAD; ++x)
				{
					if (terrainIDs[x] == -1) continue;
					m_StaticObjectStorage[TREE][terrainIDs[x]].TerrainObjectCount += 1;
					m_StaticObjectStorage[TREE][terrainIDs[x]].TransformList.emplace_back(transform);
				}
			}
			else if (!strcmp(name, ROCK))
			{
				for (int x = 0; x < QUAD; ++x)
				{
					if (terrainIDs[x] == -1) continue;
					m_StaticObjectStorage[ROCK][terrainIDs[x]].TerrainObjectCount += 1;
					m_StaticObjectStorage[ROCK][terrainIDs[x]].TransformList.emplace_back(transform);
				}
			}
			else if (!strcmp(name, PILLAR))
			{
				for (int x = 0; x < QUAD; ++x)
				{
					if (terrainIDs[x] == -1) continue;
					m_StaticObjectStorage[PILLAR][terrainIDs[x]].TerrainObjectCount += 1;
					m_StaticObjectStorage[PILLAR][terrainIDs[x]].TransformList.emplace_back(transform);
				} 
			}
			else if (!strcmp(name, SUNFLOWER))
			{
				for (int x = 0; x < QUAD; ++x)
				{
					if (terrainIDs[x] == -1) continue;
					m_StaticObjectStorage[SUNFLOWER][terrainIDs[x]].TerrainObjectCount += 1;
					m_StaticObjectStorage[SUNFLOWER][terrainIDs[x]].TransformList.emplace_back(transform);
				} 
			}
			else if (!strcmp(name, ALTER))
			{
				for (int x = 0; x < QUAD; ++x)
				{
					if (terrainIDs[x] == -1) continue;
					m_StaticObjectStorage[ALTER][terrainIDs[x]].TerrainObjectCount += 1;
					m_StaticObjectStorage[ALTER][terrainIDs[x]].TransformList.emplace_back(transform);
				}
			}
			else
			{
			}

			delete terrainIDs;
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

	LoadTerrainObjectFromFile(pd3dDevice, pd3dCommandList, "Information/Terrain.bin", pTerrain);
	 
	m_isCreate = true;
}
 
void StaticObjectStorage::Render(ID3D12GraphicsCommandList * pd3dCommandList, int terrainID)
{
	
}
