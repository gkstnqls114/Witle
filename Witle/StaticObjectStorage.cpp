#include "stdafx.h"
#include "d3dUtil.h"

#include "StaticObject.h"
#include "ModelStorage.h"
#include "ShaderManager.h"
#include "Shader.h"
 
#include "QuadTreeTerrain.h" 
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
}

void StaticObjectStorage::LoadTerrainObjectFromFile(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, const char * pstrFileName, const QuadtreeTerrain const * pTerrain)
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
	// m_StaticObjectStorage[ALTER] = new TerrainObjectInfo[TerrainPieceCount];

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

void StaticObjectStorage::LoadNameAndPositionFromFile(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, FILE * pInFile, const QuadtreeTerrain const * pTerrain)
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
			TerrainObjectAllCount += 1;

			XMFLOAT4X4 temp;
			nReads = (UINT)::fread(&temp, sizeof(XMFLOAT4X4), 1, pInFile);
			XMFLOAT4X4 transform = Matrix4x4::Identity();
			transform._41 = -temp._41;
			transform._42 = temp._42;
			transform._43 = -temp._43;

			// 계산을 통해 몇번째 아이디인지 즉 어디의 리프노드에 존재하는 위치인지 알아낸다...
			// fbx sdk 에서 꺼내올때 무슨 문제가 있는지 x, z좌표에 -부호 붙여야함 ...

			XMFLOAT3 position{ transform._41, transform._42, transform._43 };
			const int* terrainIDs = pTerrain->GetIDs(position);

			if (!strcmp(name, "Tree"/*임시*/))
			{ 
				for (int x = 0; x < QUAD; ++x)
				{
					if (terrainIDs[x] == -1) continue;
					m_StaticObjectStorage[TREE][terrainIDs[x]].TerrainObjectCount += 1;

					LoadObject* TestObject = ModelStorage::GetInstance()->GetRootObject(TREE);
					TestObject->SetPosition(XMFLOAT3{ transform._41, transform._42, transform._43 });
					TestObject->UpdateTransform(NULL);
					 
					m_StaticObjectStorage[TREE][terrainIDs[x]].TransformList.emplace_back(TestObject->m_pChild->m_xmf4x4World);
					delete TestObject; 
				}
			}
			else if (!strcmp(name, ROCK))
			{ 
				for (int x = 0; x < QUAD; ++x)
				{
					if (terrainIDs[x] == -1) continue;
					m_StaticObjectStorage[ROCK][terrainIDs[x]].TerrainObjectCount += 1;

					LoadObject* TestObject = ModelStorage::GetInstance()->GetRootObject(ROCK);
					TestObject->SetPosition(XMFLOAT3{ transform._41, transform._42, transform._43 });
					TestObject->UpdateTransform(NULL);

					m_StaticObjectStorage[ROCK][terrainIDs[x]].TransformList.emplace_back(TestObject->m_pChild->m_xmf4x4World);
					delete TestObject;

				}
			}
			else if (!strcmp(name, PILLAR))
			{ 
				for (int x = 0; x < QUAD; ++x)
				{
					if (terrainIDs[x] == -1) continue;
					m_StaticObjectStorage[PILLAR][terrainIDs[x]].TerrainObjectCount += 1;

					LoadObject* TestObject = ModelStorage::GetInstance()->GetRootObject(PILLAR);
					TestObject->SetPosition(XMFLOAT3{ transform._41, transform._42, transform._43 });
					TestObject->UpdateTransform(NULL);

					m_StaticObjectStorage[PILLAR][terrainIDs[x]].TransformList.emplace_back(TestObject->m_pChild->m_xmf4x4World);
					delete TestObject;

				} 
			}
			else if (!strcmp(name, SUNFLOWER))
			{ 
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

void StaticObjectStorage::CreateShaderVariables(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
{
	//인스턴스 정보를 저장할 정점 버퍼를 업로드 힙 유형으로 생성한다. 
	for (auto& info : m_StaticObjectStorage)
	{ 
		m_StaticObjectModelsStorage[info.first] = ModelStorage::GetInstance()->GetRootObject(info.first);
		
		for (int iTerrainPiece = 0; iTerrainPiece < TerrainPieceCount; ++iTerrainPiece)
		{
			if (info.second[iTerrainPiece].TerrainObjectCount == 0) continue;
			
			// 쉐이더 변수 생성
			info.second[iTerrainPiece].m_pd3dcbGameObjects =
				d3dUtil::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL,
					sizeof(VS_SRV_INSTANCEINFO) * info.second[iTerrainPiece].TerrainObjectCount, D3D12_HEAP_TYPE_UPLOAD,
					D3D12_RESOURCE_STATE_GENERIC_READ, NULL);
		
			info.second[iTerrainPiece].m_pd3dcbGameObjects->Map(0, NULL, (void **)&info.second[iTerrainPiece].m_pcbMappedGameObjects);
		}
	} 

	// 정보 저장 
	for (auto& info : m_StaticObjectStorage)
	{
		for (int iTerrainPiece = 0; iTerrainPiece < TerrainPieceCount; ++iTerrainPiece)
		{
			if (info.second[iTerrainPiece].TerrainObjectCount == 0) continue;
			for (int i = 0; i < info.second[iTerrainPiece].TerrainObjectCount; ++i)
			{
				XMStoreFloat4x4(
					&info.second[iTerrainPiece].m_pcbMappedGameObjects[i].m_xmf4x4Transform,
					XMMatrixTranspose(XMLoadFloat4x4(&info.second[iTerrainPiece].TransformList[i]
					)));
			} 
		}
	}
}

StaticObjectStorage * StaticObjectStorage::GetInstance(const QuadtreeTerrain const * pTerrain)
{
	if (!m_Instance)
	{
		m_Instance = new StaticObjectStorage();
	}

	return m_Instance;
}

int StaticObjectStorage::GetObjectCount(int index, const std::string & name)
{
	return m_StaticObjectStorage[name][index].TerrainObjectCount;
}

int StaticObjectStorage::GetObjectAllCount(int index)
{
	int result = 0;

	for (auto& info : m_StaticObjectStorage)
	{
		result += info.second[index].TerrainObjectCount;
	} 

	return result;
}

XMFLOAT4X4* StaticObjectStorage::GetpWorldMatrixs(int index, const std::string & name)
{ 
	return m_StaticObjectStorage[name][index].TransformList.begin()._Ptr; 
}

void StaticObjectStorage::CreateInfo(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, const QuadtreeTerrain const * pTerrain)
{
	if (m_isCreate) return;

	// 위치 정보를 읽어온다.
	LoadTerrainObjectFromFile(pd3dDevice, pd3dCommandList, "Information/Terrain.bin", pTerrain);
	
	// 읽어온 위치 정보의 개수대로 쉐이더 변수를 생성한다.
	CreateShaderVariables(pd3dDevice, pd3dCommandList);
	 
	m_isCreate = true;
}
 
void StaticObjectStorage::Render(ID3D12GraphicsCommandList * pd3dCommandList, int index)
{  
	for (auto& info : m_StaticObjectStorage)
	{
		if (info.second[index].TerrainObjectCount == 0) continue;

		pd3dCommandList->SetGraphicsRootShaderResourceView(ROOTPARAMETER_INSTANCING, info.second[index].m_pd3dcbGameObjects->GetGPUVirtualAddress());
		 
		m_StaticObjectModelsStorage[info.first]->RenderInstancing(pd3dCommandList, info.second[index].TerrainObjectCount);
	}  
}
