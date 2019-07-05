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
 
bool MonsterTransformStorage::LoadTransform(char * name, const char * comp_name, const XMFLOAT4X4 & tr)
{ 
	bool result = false;

	if (!strcmp(name, comp_name))
	{
		m_MonsterTransformStorage[comp_name].TerrainObjectCount += 1;

		//LoadObject* TestObject = ModelStorage::GetInstance()->GetRootObject(comp_name);
		//TestObject->SetTransform(tr);
		//TestObject->UpdateTransform(NULL);

		m_MonsterTransformStorage[comp_name].TransformList.emplace_back(tr);

		//delete TestObject;
		//TestObject = nullptr;

		result = true;
	}
	return result;
}

void MonsterTransformStorage::LoadMonsterFromFile(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, const char * pstrFileName)
{
	FILE *pInFile = NULL;
	::fopen_s(&pInFile, pstrFileName, "rb");
	::rewind(pInFile);

	char pstrToken[64] = { '\0' };
	  
	for (; ; )
	{
		if (::ReadStringFromFile(pInFile, pstrToken))
		{
			if (!strcmp(pstrToken, "<Hierarchy>:"))
			{
				int nRootChild = ::ReadIntegerFromFile(pInFile); // 모든 오브젝트 개수

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
						LoadNameAndPositionFromFile(pd3dDevice, pd3dCommandList, pInFile);
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
 
void MonsterTransformStorage::LoadNameAndPositionFromFile(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, FILE * pInFile)
{
	char pstrToken[64] = { '\0' };
	UINT nReads = 0;

	char name[64];

	int nFrame = ::ReadIntegerFromFile(pInFile);
	::ReadStringFromFile(pInFile, name);
	std::cout << name << " 초기위치" << std::endl;
	 
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
			   
			// 계산을 통해 몇번째 아이디인지 즉 어디의 리프노드에 존재하는 위치인지 알아낸다...
			// fbx sdk 에서 꺼내올때 무슨 문제가 있는지 x, z좌표에 -부호 붙여야함 ...
			// 그리고 위치 차이때문에 

			XMFLOAT3 position{ transform._41, transform._42, transform._43 };  

			// 몬스터는 개별로 모델저장소에 저장하여 존재하지 않아서 일단 이렇게 만들어야함 ..
			if (!strcmp(name, SPACECAT_BLUE))
			{ 
				LoadTransform(name, SPACECAT_BLUE, transform);
			}
			else if (!strcmp(name, SPACECAT_PINK))
			{
				LoadTransform(name, SPACECAT_PINK, transform);
			}
			else if (!strcmp(name, SPACECAT_GREEN))
			{
				LoadTransform(name, SPACECAT_GREEN, transform);
			}
			else if (!strcmp(name, CREEPYMONSTER))
			{
				LoadTransform(name, CREEPYMONSTER, transform);
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
						LoadNameAndPositionFromFile(pd3dDevice, pd3dCommandList, pInFile);
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
	for (auto& loadobj : m_MonsterTransformStorage)
	{ 
		// loadobj.first는 이름입니다.

		loadobj.second.TransformList.clear();
	}
	m_MonsterTransformStorage.clear();
}

int MonsterTransformStorage::GetObjectCount(int index, const std::string & name)
{
	return m_MonsterTransformStorage[name].TerrainObjectCount;
}

int MonsterTransformStorage::GetObjectAllCount(int index)
{
	int result = 0;

	for (auto& info : m_MonsterTransformStorage)
	{
		result += info.second.TerrainObjectCount;
	} 

	return result;
}

XMFLOAT4X4 MonsterTransformStorage::GetWorldMatrix(int index, const std::string & name)
{ 
	return m_MonsterTransformStorage[name].TransformList[index]; 
}

XMFLOAT3 MonsterTransformStorage::GetPosition(int index, const std::string & name)
{
	XMFLOAT4X4 tr = m_MonsterTransformStorage[name].TransformList[index];
	return XMFLOAT3(tr._41, tr._42, tr._43);
}

void MonsterTransformStorage::CreateInfo(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
{
	if (m_isCreate) return;

	// 위치 정보를 읽어온다.
	LoadMonsterFromFile(pd3dDevice, pd3dCommandList, "Information/TerrainForMonster.bin");
	
	// 읽어온 위치 정보의 개수대로 쉐이더 변수를 생성한다.
	CreateShaderVariables(pd3dDevice, pd3dCommandList);
	 
	m_isCreate = true;
}
 
