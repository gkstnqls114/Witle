#include "stdafx.H"
#include "d3dUtil.h"
#include "StaticObject.h"
#include "MapInfoLoader.h"

using namespace FileRead;


void MapInfoLoader::LoadTerrainObjectFromFile(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ID3D12RootSignature * pd3dGraphicsRootSignature, const char * pstrFileName)
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
				int nFrame = ::ReadIntegerFromFile(pInFile);
				for (int i = 0 ; i < nFrame; ++i)
				{
					::ReadStringFromFile(pInFile, pstrToken);
					if (!strcmp(pstrToken, "<Frame>:"))
					{
						LoadNameAndPositionFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, pInFile);
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

void MapInfoLoader::LoadNameAndPositionFromFile(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ID3D12RootSignature * pd3dGraphicsRootSignature, FILE *pInFile)
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
			XMFLOAT4X4 test;
			nReads = (UINT)::fread(&test, sizeof(XMFLOAT4X4), 1, pInFile);
			 
			if (!strcmp(name, "Tree"))
			{
				MyReflexTree::m_PostionFromMap[MyReflexTree::m_CountFromMap] = (test);
				MyReflexTree::m_CountFromMap += 1;
			}
			else if (!strcmp(name, "Rock"))
			{
				Rock::m_PostionFromMap[Rock::m_CountFromMap] = (test);
				Rock::m_CountFromMap += 1; 
			}
			else if (!strcmp(name, "Pillar"))
			{
				Pillar::m_PostionFromMap[Pillar::m_CountFromMap] = (test);
				Pillar::m_CountFromMap += 1;
			}
			else if (!strcmp(name, "Sunflower"))
			{ 
				SunFlower::m_PostionFromMap[SunFlower::m_CountFromMap] = (test);
				SunFlower::m_CountFromMap += 1;
			}
			else if (!strcmp(name, "Altar"))
			{
				Altar::m_PostionFromMap[Altar::m_CountFromMap] = (test);
				Altar::m_CountFromMap += 1;
			}
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
						LoadNameAndPositionFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, pInFile);
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
