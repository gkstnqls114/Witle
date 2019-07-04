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
#include "MonsterTransformStorage.h"

using namespace FileRead;

MonsterTransformStorage* MonsterTransformStorage::m_Instance{ nullptr };

void MonsterTransformStorage::UpdateShaderVariables(ID3D12GraphicsCommandList * pd3dCommandList, int count, XMFLOAT4X4 * transforms)
{ 
}
 
bool MonsterTransformStorage::LoadTransform(char * name, const char * comp_name, const XMINT4 & IDs, const XMFLOAT4X4 & tr)
{ 
	bool result = false;

	if (!strcmp(name, comp_name))
	{
		// ���� name�� ������ ���...
		if (!strcmp(name, Cliff))
		{ 

			LoadObject* TestObject = ModelStorage::GetInstance()->GetRootObject(comp_name);
			TestObject->SetTransform(tr); // ���⼭ Scale �� ���̷�Ʈ X�࿡ ���� ȸ�� �Ͼ�Ƿ� ���� �������� �ȵ�..
			TestObject->UpdateTransform(NULL);

			for (int x = 0; x < TerrainPieceCount; ++x)
			{
				m_MonsterTransformStorage[comp_name][x].TerrainObjectCount += 1;

				m_MonsterTransformStorage[comp_name][x].TransformList.emplace_back(TestObject->m_pChild->m_xmf4x4World);
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

			m_MonsterTransformStorage[comp_name][terrainIDs].TerrainObjectCount += 1;

			LoadObject* TestObject = ModelStorage::GetInstance()->GetRootObject(comp_name);
			TestObject->SetTransform(tr);
			TestObject->UpdateTransform(NULL);

			m_MonsterTransformStorage[comp_name][terrainIDs].TransformList.emplace_back(TestObject->m_pChild->m_xmf4x4World);

			delete TestObject;
			TestObject = nullptr;

			result = true;
		}
	}
	return result;
}

void MonsterTransformStorage::LoadTerrainObjectFromFile(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, const char * pstrFileName, const QuadtreeTerrain const * pTerrain)
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
		m_MonsterTransformStorage[name ] = new TerrainObjectInfo[TerrainPieceCount];
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

void MonsterTransformStorage::LoadMonsterTransform(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, const char * pstrFileName)
{
	FILE *pInFile = NULL;
	::fopen_s(&pInFile, pstrFileName, "rb");
	::rewind(pInFile);

	char pstrToken[64] = { '\0' };

	for (const auto& name : ModelStorage::GetInstance()->m_NameList)
	{
		m_MonsterStorage[name] = new MonsterInfo[TerrainPieceCount];
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
						LoadNameAndPositionFromFile(pd3dDevice, pd3dCommandList, pInFile, nullptr);
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

void MonsterTransformStorage::LoadNameAndPositionFromFile(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, FILE * pInFile, const QuadtreeTerrain const * pTerrain)
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
			
			XMFLOAT4X4 transform = Matrix4x4::Multiply(rotate180Y, rotateInfo);
			
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

void MonsterTransformStorage::CreateShaderVariables(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
{
	 
}

MonsterTransformStorage * MonsterTransformStorage::GetInstance(const QuadtreeTerrain const * pTerrain)
{
	if (!m_Instance)
	{
		m_Instance = new MonsterTransformStorage();
	}

	return m_Instance;
}

MonsterTransformStorage * MonsterTransformStorage::GetInstance()
{ 
	if (!m_Instance)
	{
		m_Instance = new MonsterTransformStorage();
	}

	return m_Instance;
}

void MonsterTransformStorage::ReleaseInstance()
{
	if (m_Instance)
	{ 
		delete m_Instance;
		m_Instance = nullptr;
	}
}

void MonsterTransformStorage::ReleaseObjects()
{
	for (auto& model : m_StaticObjectModelsStorage)
	{
		delete model.second.pLoadObject;
		model.second.pLoadObject = nullptr;
	}
	m_StaticObjectModelsStorage.clear();
	 
	for (auto& loadobj : m_MonsterTransformStorage)
	{ 
		// loadobj.first�� �̸��Դϴ�.

		loadobj.second->TransformList.clear();

		if (loadobj.second)
		{
			delete[] loadobj.second;
			loadobj.second = nullptr;
		}
	}
	m_MonsterTransformStorage.clear();
}

int MonsterTransformStorage::GetObjectCount(int index, const std::string & name)
{
	return m_MonsterTransformStorage[name][index].TerrainObjectCount;
}

int MonsterTransformStorage::GetObjectAllCount(int index)
{
	int result = 0;

	for (auto& info : m_MonsterTransformStorage)
	{
		result += info.second[index].TerrainObjectCount;
	} 

	return result;
}

XMFLOAT4X4* MonsterTransformStorage::GetWorldMatrix(int index, const std::string & name)
{ 
	return m_MonsterTransformStorage[name][index].TransformList.begin()._Ptr; 
}

void MonsterTransformStorage::CreateInfo(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, const QuadtreeTerrain const * pTerrain)
{
	if (m_isCreate) return;

	// ��ġ ������ �о�´�.
	LoadTerrainObjectFromFile(pd3dDevice, pd3dCommandList, "Information/Terrain.bin", pTerrain);
	LoadMonsterTransform(pd3dDevice, pd3dCommandList, "Information/TerrainForMonster.bin");

	// �о�� ��ġ ������ ������� ���̴� ������ �����Ѵ�.
	CreateShaderVariables(pd3dDevice, pd3dCommandList);
	 
	m_isCreate = true;
}
 
