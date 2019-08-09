#include "stdafx.h"
#include "d3dUtil.h"

#include "StaticObject.h"
#include "Texture.h"
#include "ModelStorage.h"
#include "TextureStorage.h"
#include "ShaderManager.h"
#include "Shader.h"
 
#include "QuadTreeTerrain.h" 
#include "Object.h" 
#include "StaticObjectStorage.h"

using namespace FileRead;

StaticObjectStorage* StaticObjectStorage::m_Instance{ nullptr };

void StaticObjectStorage::UpdateShaderVariables(ID3D12GraphicsCommandList * pd3dCommandList, int count, XMFLOAT4X4 * transforms)
{ 
}
 
bool StaticObjectStorage::LoadTransform(char * name, const char * comp_name, const XMINT4 & IDs, const XMFLOAT4X4 & tr)
{ 
	bool result = false;
	 
	if (!strcmp(name, comp_name) || !strcmp(name, "Cylinder001") /*Floor������ �ϵ��ڵ�*/)
	{
		// ���� name�� ������ ���...
		if (!strcmp(name, Cliff))
		{ 
			XMFLOAT4X4 newXMFLOAT4X4 = tr;
			newXMFLOAT4X4._42 = 50;
			LoadObject* TestObject = ModelStorage::GetInstance()->GetRootObject(comp_name);
			TestObject->SetTransform(newXMFLOAT4X4); // ���⼭ Scale �� ���̷�Ʈ X�࿡ ���� ȸ�� �Ͼ�Ƿ� ���� �������� �ȵ�..
			TestObject->UpdateTransform(NULL);

			for (int x = 0; x < TerrainPieceCount; ++x)
			{
				m_StaticObjectStorage[comp_name][x].TerrainObjectCount += 1;

				m_StaticObjectStorage[comp_name][x].TransformList.emplace_back(TestObject->m_pChild->m_xmf4x4World);
			}

			delete TestObject;
			TestObject = nullptr;

			return true; // ������ ��� �� ���� ó��
		}

		for (int Ti = 0; Ti < 4; ++Ti)
		{
			int terrainIDs = -1;
			if (Ti == 0) terrainIDs = IDs.x;
			else if (Ti == 1) terrainIDs = IDs.y;
			else if (Ti == 2) terrainIDs = IDs.z;
			else if (Ti == 3) terrainIDs = IDs.w;

			if (terrainIDs == -1) continue;

			if (!strcmp(name, "Cylinder001"))
			{ 
				m_StaticObjectStorage[RUIN_FLOOR][terrainIDs].TerrainObjectCount += 1;

				LoadObject* TestObject = ModelStorage::GetInstance()->GetRootObject(RUIN_FLOOR);
				TestObject->SetTransform(tr);
				TestObject->UpdateTransform(NULL);

				m_StaticObjectStorage[RUIN_FLOOR][terrainIDs].TransformList.emplace_back(TestObject->m_pChild->m_xmf4x4World);
				 
				delete TestObject;
				TestObject = nullptr;
			}
			else
			{

				m_StaticObjectStorage[comp_name][terrainIDs].TerrainObjectCount += 1;

				LoadObject* TestObject = ModelStorage::GetInstance()->GetRootObject(comp_name);
				TestObject->SetTransform(tr);
				TestObject->UpdateTransform(NULL);

				m_StaticObjectStorage[comp_name][terrainIDs].TransformList.emplace_back(TestObject->m_pChild->m_xmf4x4World);
				if (!strcmp(comp_name, ALTAR_IN))
				{
					m_AltarTransformStorage.emplace_back(tr);
				}

				delete TestObject;
				TestObject = nullptr;
			}

			result = true;
		}
	}
	return result;
}

void StaticObjectStorage::LoadTerrainObjectFromFile(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, const char * pstrFileName, const QuadtreeTerrain const * pTerrain)
{
	FILE *pInFile = NULL;
	::fopen_s(&pInFile, pstrFileName, "rb");
	::rewind(pInFile);

	char pstrToken[64] = { '\0' };

	// ���� �����ϱ� ���� �ͷ��� ���� ������ ���߾� ������ �����Ѵ�.
	// �̸��� ���߾� �����ؾ��ϹǷ� �ϵ��ڵ��� �ؾ��Ѵ�.
	TerrainPieceCount = pTerrain->GetTerrainPieceCount();
	 
	for (const auto& name : ModelStorage::GetInstance()->m_NameList)
	{
		m_StaticObjectStorage[name ] = new TerrainObjectInfo[TerrainPieceCount];
	}

	for (; ; )
	{
		if (::ReadStringFromFile(pInFile, pstrToken))
		{
			if (!strcmp(pstrToken, "<Hierarchy>:"))
			{
				int nRootChild = ::ReadIntegerFromFile(pInFile); // ��� ������Ʈ ����

				::ReadStringFromFile(pInFile, pstrToken); //<ObjectCount>: 
				int nObjects = ::ReadIntegerFromFile(pInFile);
				for (int i = 0; i < nObjects; ++i)
				{  
					::ReadStringFromFile(pInFile, pstrToken); // Object Name
					::ReadIntegerFromFile(pInFile); // Object Count
				}

				for (int i = 0; i < nRootChild; ++i)
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

			::ReadStringFromFile(pInFile, pstrToken); // <GlobalRotation>:
			XMFLOAT4 rotationXYZ;
			nReads = (UINT)::fread(&rotationXYZ, sizeof(XMFLOAT4), 1, pInFile);
			XMFLOAT4X4 rotate180Y = Matrix4x4::RotateMatrix(0.f, 0.f, 0.f);
			XMFLOAT4X4 rotateInfo = Matrix4x4::RotateMatrix(0.f, rotationXYZ.z, 0.f);
			
			// XMFLOAT4X4 transform = Matrix4x4::Multiply(rotate180Y, rotateInfo);
			XMFLOAT4X4 transform = Matrix4x4::Identity();
			
			// XMFLOAT4X4 transform = Matrix4x4::Identity();
			transform._41 =  -(temp._41);
			transform._42 = 0;
			if (!strcmp(name, Flower)) transform._42 = temp._42; 
			transform._43 =  -(temp._43);
			 
			assert(!(temp._41 >= 40000));
			assert(!(temp._43 >= 40000));
			   
			// ����� ���� ���° ���̵����� �� ����� ������忡 �����ϴ� ��ġ���� �˾Ƴ���...
			// fbx sdk ���� �����ö� ���� ������ �ִ��� x, z��ǥ�� -��ȣ �ٿ����� ...
			// �׸��� ��ġ ���̶����� 

			XMFLOAT3 position{ transform._41, transform._42, transform._43 };
			XMINT4 terrainIDs = pTerrain->GetIDs(position);
		 
			for (const auto& modelname : ModelStorage::GetInstance()->m_NameList)
			{  
				bool isLocated = LoadTransform(name, modelname.c_str(), terrainIDs, transform);
				if (isLocated) break;
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
	for (auto& info : m_StaticObjectStorage)
	{ 
		m_StaticObjectModelsStorage[info.first].pLoadObject = ModelStorage::GetInstance()->GetRootObject(info.first);
		m_StaticObjectModelsStorage[info.first].pTexture = TextureStorage::GetInstance()->GetTexture(info.first);
		
		for (int iTerrainPiece = 0; iTerrainPiece < TerrainPieceCount; ++iTerrainPiece)
		{
			if (info.second[iTerrainPiece].TerrainObjectCount == 0) continue;
			
			// ���̴� ���� ����
			info.second[iTerrainPiece].m_pd3dcbGameObjects =
				d3dUtil::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL,
					sizeof(VS_SRV_INSTANCEINFO) * info.second[iTerrainPiece].TerrainObjectCount, D3D12_HEAP_TYPE_UPLOAD,
					D3D12_RESOURCE_STATE_GENERIC_READ, NULL);
		
			info.second[iTerrainPiece].m_pd3dcbGameObjects->Map(0, NULL, (void **)&info.second[iTerrainPiece].m_pcbMappedGameObjects);
		}
	} 

	// ���� ���� 
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

StaticObjectStorage * StaticObjectStorage::GetInstance()
{ 
	if (!m_Instance)
	{
		m_Instance = new StaticObjectStorage();
	}

	return m_Instance;
}

void StaticObjectStorage::ReleaseInstance()
{
	if (m_Instance)
	{ 
		delete m_Instance;
		m_Instance = nullptr;
	}
}

void StaticObjectStorage::ReleaseObjects()
{
	for (auto& model : m_StaticObjectModelsStorage)
	{
		delete model.second.pLoadObject;
		model.second.pLoadObject = nullptr;
	}
	m_StaticObjectModelsStorage.clear();
	 
	for (auto& loadobj : m_StaticObjectStorage)
	{ 
		// loadobj.first�� �̸��Դϴ�.

		if (loadobj.second->m_pd3dcbGameObjects)
		{
			loadobj.second->m_pd3dcbGameObjects->Unmap(0, NULL);
			loadobj.second->m_pd3dcbGameObjects = nullptr;
		}

		//if (loadobj.second->m_pcbMappedGameObjects)
		//{
		//	delete[] loadobj.second->m_pcbMappedGameObjects;
		//	loadobj.second->m_pcbMappedGameObjects = nullptr; 
		//}

		loadobj.second->TransformList.clear();

		if (loadobj.second)
		{
			delete[] loadobj.second;
			loadobj.second = nullptr;
		}
	}
	m_StaticObjectStorage.clear();
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

XMFLOAT4X4* StaticObjectStorage::GetWorldMatrix(int index, const std::string & name)
{ 
	return m_StaticObjectStorage[name][index].TransformList.begin()._Ptr; 
}

XMFLOAT4X4 StaticObjectStorage::GetAltarTransform(int index, const std::string & name)
{
	return m_AltarTransformStorage[index];
}

void StaticObjectStorage::CreateInfo(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, const QuadtreeTerrain const * pTerrain)
{
	if (m_isCreate) return;

	// ��ġ ������ �о�´�.
	LoadTerrainObjectFromFile(pd3dDevice, pd3dCommandList, "Information/Terrain.bin", pTerrain);
	
	// �о�� ��ġ ������ ������� ���̴� ������ �����Ѵ�.
	CreateShaderVariables(pd3dDevice, pd3dCommandList);
	 
	m_isCreate = true;
}
 
void StaticObjectStorage::RenderAll(ID3D12GraphicsCommandList * pd3dCommandList, bool isGBuffers)
{
	for (int i = 0; i < TerrainPieceCount; ++i)
	{
		Render(pd3dCommandList, i, isGBuffers);
	}
}

void StaticObjectStorage::Render(ID3D12GraphicsCommandList * pd3dCommandList, int index, bool isGBuffers)
{  
	// info.first = �� �̸�
	// info.second = TerrainObjectInfo��� �� ����
	 
	for (auto& info : m_StaticObjectStorage)
	{ 
		if (info.second[index].TerrainObjectCount == 0) continue;

		pd3dCommandList->SetGraphicsRootShaderResourceView(ROOTPARAMETER_INSTANCING, info.second[index].m_pd3dcbGameObjects->GetGPUVirtualAddress()); // �ν��Ͻ� ���̴� ���ҽ� ��
		  
		m_StaticObjectModelsStorage[info.first].pTexture->UpdateShaderVariables(pd3dCommandList);
		
		m_StaticObjectModelsStorage[info.first].pLoadObject->RenderInstancing(pd3dCommandList, info.second[index].TerrainObjectCount, isGBuffers);
	}  
}

void StaticObjectStorage::RenderForShadow(ID3D12GraphicsCommandList * pd3dCommandList, int index, bool isGBuffers)
{
	// info.first = �� �̸�
	// info.second = TerrainObjectInfo��� �� ����

	for (auto& info : m_StaticObjectStorage)
	{
		if (info.second[index].TerrainObjectCount == 0) continue;
		if (!strcmp(info.first.c_str(), Flower)) continue;

		pd3dCommandList->SetGraphicsRootShaderResourceView(ROOTPARAMETER_INSTANCING, info.second[index].m_pd3dcbGameObjects->GetGPUVirtualAddress()); // �ν��Ͻ� ���̴� ���ҽ� ��

		m_StaticObjectModelsStorage[info.first].pLoadObject->RenderInstancing(pd3dCommandList, info.second[index].TerrainObjectCount, isGBuffers);
	}
}

void StaticObjectStorage::RenderBOBox(ID3D12GraphicsCommandList * pd3dCommandList, int index)
{
	// info.first = �� �̸�
	// info.second = TerrainObjectInfo��� �� ����

	for (auto& info : m_StaticObjectStorage)
	{
		if (info.second[index].TerrainObjectCount == 0) continue;

		pd3dCommandList->SetGraphicsRootShaderResourceView(ROOTPARAMETER_INSTANCING, info.second[index].m_pd3dcbGameObjects->GetGPUVirtualAddress()); // �ν��Ͻ� ���̴� ���ҽ� ��
		 
		ModelStorage::GetInstance()->RenderBOBoxInstancing(pd3dCommandList, info.first, info.second[index].TerrainObjectCount);
	}
}

