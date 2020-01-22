#include "stdafx.h"
#include "d3dUtil.h"
#include "TerrainMesh.h" 
#include "ShaderManager.h"
#include "Shader.h"
#include "MeshRenderer.h"
#include "TextureStorage.h"
#include "StaticObjectStorage.h"
#include "HeightMapImage.h"
#include <unordered_set>
#include "MyFrustum.h"
#include "Collision.h"
#include "Terrain.h"
#include "ModelStorage.h"
#include "MyDescriptorHeap.h"
#include "Object.h"
#include "Texture.h"
#include "QtTerrainInstancingDrawer.h"

// ó�� ���̵�� 0���� �����Ѵ�.
// ���� ��常 ���̵� �����Ѵ�.
// ���� ���̵�� 0���� �����Ѵ�.
int QtTerrainInstancingDrawer::gTreePieceCount{ 0 }; 
 
void QtTerrainInstancingDrawer::ReleaseMembers()
{
	delete[] m_pReafNodes; 
}

void QtTerrainInstancingDrawer::ReleaseMemberUploadBuffers()
{ 
	RecursiveReleaseUploadBuffers(m_pRootNode);
}


void QtTerrainInstancingDrawer::LoadTerrainObjectFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, const char* pstrFileName, const QtTerrainInstancingDrawer const* pTerrain)
{
	FILE* pInFile = NULL;
	::fopen_s(&pInFile, pstrFileName, "rb");
	::rewind(pInFile);

	char pstrToken[64] = { '\0' };

	// ���� �����ϱ� ���� �ͷ��� ���� ������ ���߾� ������ �����Ѵ�.
	// �̸��� ���߾� �����ؾ��ϹǷ� �ϵ��ڵ��� �ؾ��Ѵ�.
	TerrainPieceCount = pTerrain->GetTerrainPieceCount();

	for (const auto& name : ModelStorage::GetInstance()->m_NameList)
	{
		m_StaticObjectStorage[name] = new TerrainObjectInfo[TerrainPieceCount];
	}

	for (; ; )
	{
		if (FileRead::ReadStringFromFile(pInFile, pstrToken))
		{
			if (!strcmp(pstrToken, "<Hierarchy>:"))
			{
				int nRootChild = FileRead::ReadIntegerFromFile(pInFile); // ��� ������Ʈ ����

				FileRead::ReadStringFromFile(pInFile, pstrToken); //<ObjectCount>: 
				int nObjects = FileRead::ReadIntegerFromFile(pInFile);
				for (int i = 0; i < nObjects; ++i)
				{
					FileRead::ReadStringFromFile(pInFile, pstrToken); // Object Name
					FileRead::ReadIntegerFromFile(pInFile); // Object Count
				}

				for (int i = 0; i < nRootChild; ++i)
				{
					FileRead::ReadStringFromFile(pInFile, pstrToken);
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

void QtTerrainInstancingDrawer::LoadNameAndPositionFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, FILE* pInFile, const QtTerrainInstancingDrawer const* pTerrain)
{
	char pstrToken[64] = { '\0' };
	UINT nReads = 0;

	char name[64];

	int nFrame = FileRead::ReadIntegerFromFile(pInFile);
	FileRead::ReadStringFromFile(pInFile, name);

	for (; ; )
	{
		FileRead::ReadStringFromFile(pInFile, pstrToken);
		if (!strcmp(pstrToken, "<GlobalTransform>:"))
		{
			TerrainObjectAllCount += 1;

			XMFLOAT4X4 temp;
			nReads = (UINT)::fread(&temp, sizeof(XMFLOAT4X4), 1, pInFile);

			FileRead::ReadStringFromFile(pInFile, pstrToken); // <GlobalRotation>:
			XMFLOAT4 rotationXYZ;
			nReads = (UINT)::fread(&rotationXYZ, sizeof(XMFLOAT4), 1, pInFile);
			XMFLOAT4X4 rotate180Y = Matrix4x4::RotateMatrix(0.f, 0.f, 0.f);
			XMFLOAT4X4 rotateInfo = Matrix4x4::RotateMatrix(0.f, rotationXYZ.z, 0.f);

			// XMFLOAT4X4 transform = Matrix4x4::Multiply(rotate180Y, rotateInfo);
			XMFLOAT4X4 transform = Matrix4x4::Identity();

			// XMFLOAT4X4 transform = Matrix4x4::Identity();
			transform._41 = -(temp._41);
			transform._42 = 0;
			if (!strcmp(name, Flower)) transform._42 = temp._42;
			transform._43 = -(temp._43);

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
			int nChilds = FileRead::ReadIntegerFromFile(pInFile);
			if (nChilds > 0)
			{
				for (int i = 0; i < nChilds; i++)
				{
					FileRead::ReadStringFromFile(pInFile, pstrToken);
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

bool QtTerrainInstancingDrawer::LoadTransform(char* name, const char* comp_name, const XMINT4& IDs, const XMFLOAT4X4& tr)
{
	bool result = false;

	if (!strcmp(name, comp_name) || !strcmp(name, "Cylinder001") /*Floor������ �ϵ��ڵ�*/ || !strcmp(name, "Object001"))
	{
		// ���� name�� ������ ���...
		if (!strcmp(name, "Object001"))
		{
			XMFLOAT4X4 newXMFLOAT4X4 = tr;
			newXMFLOAT4X4._42 = 50;
			LoadObject* TestObject = ModelStorage::GetInstance()->GetRootObject(Cliff);
			TestObject->SetTransform(newXMFLOAT4X4); // ���⼭ Scale �� ���̷�Ʈ X�࿡ ���� ȸ�� �Ͼ�Ƿ� ���� �������� �ȵ�..
			TestObject->UpdateTransform(NULL);

			for (int x = 0; x < TerrainPieceCount; ++x)
			{
				m_StaticObjectStorage[Cliff][x].TerrainObjectCount += 1;

				m_StaticObjectStorage[Cliff][x].TransformList.emplace_back(TestObject->m_pChild->m_xmf4x4World);
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

void QtTerrainInstancingDrawer::RecursiveRenderTerrainObjects_BOBox(const quadtree::QUAD_TREE_NODE * node, ID3D12GraphicsCommandList * pd3dCommandList)
{
	if (node->isRendering)
	{
		if (node->terrainMesh)
		{
			StaticObjectStorage::GetInstance(this)->RenderBOBox(pd3dCommandList, node->id);
		}
		else
		{

			if (node->children[0]->isRendering) RecursiveRenderTerrainObjects_BOBox(node->children[0], pd3dCommandList);
			if (node->children[1]->isRendering) RecursiveRenderTerrainObjects_BOBox(node->children[1], pd3dCommandList);
			if (node->children[2]->isRendering) RecursiveRenderTerrainObjects_BOBox(node->children[2], pd3dCommandList);
			if (node->children[3]->isRendering) RecursiveRenderTerrainObjects_BOBox(node->children[3], pd3dCommandList);

		}
	}
}


void QtTerrainInstancingDrawer::RenderTerrainObjects(ID3D12GraphicsCommandList * pd3dCommandList, bool isGBuffers)
{
	ShaderManager::GetInstance()->SetPSO(pd3dCommandList, "InstancingStandardShader", isGBuffers);
	
	RecursiveRenderTerrainObjects(m_pRootNode, pd3dCommandList, isGBuffers);
	  
	ShaderManager::GetInstance()->SetPSO(pd3dCommandList, "InstancingLine", isGBuffers);
	RecursiveRenderTerrainObjects_BOBox(m_pRootNode, pd3dCommandList); 
}

void QtTerrainInstancingDrawer::RecursiveRenderTerrainObjects(const quadtree::QUAD_TREE_NODE * node, ID3D12GraphicsCommandList * pd3dCommandList, bool isGBuffers)
{ 
	if (node->isRendering)
	{
		if (node->terrainMesh)
		{
			StaticObjectStorage::GetInstance(this)->Render(pd3dCommandList, node->id, isGBuffers);

		}
		else
		{

			if (node->children[0]->isRendering) RecursiveRenderTerrainObjects(node->children[0], pd3dCommandList, isGBuffers);
			if (node->children[1]->isRendering) RecursiveRenderTerrainObjects(node->children[1], pd3dCommandList, isGBuffers);
			if (node->children[2]->isRendering) RecursiveRenderTerrainObjects(node->children[2], pd3dCommandList, isGBuffers);
			if (node->children[3]->isRendering) RecursiveRenderTerrainObjects(node->children[3], pd3dCommandList, isGBuffers);

		} 
 	} 
}


void QtTerrainInstancingDrawer::RecursiveRenderTerrainObjectsForShadow(const quadtree::QUAD_TREE_NODE * node, ID3D12GraphicsCommandList * pd3dCommandList, bool isGBuffers)
{
	if (node->isRendering)
	{
		if (node->terrainMesh)
		{
			StaticObjectStorage::GetInstance(this)->RenderForShadow(pd3dCommandList, node->id, isGBuffers);

		}
		else
		{ 
			if (node->children[0]->isRendering) RecursiveRenderTerrainObjects(node->children[0], pd3dCommandList, isGBuffers);
			if (node->children[1]->isRendering) RecursiveRenderTerrainObjects(node->children[1], pd3dCommandList, isGBuffers);
			if (node->children[2]->isRendering) RecursiveRenderTerrainObjects(node->children[2], pd3dCommandList, isGBuffers);
			if (node->children[3]->isRendering) RecursiveRenderTerrainObjects(node->children[3], pd3dCommandList, isGBuffers);
		}
	}
}

static std::unordered_set<int> set;

void QtTerrainInstancingDrawer::RecursiveRender(const quadtree::QUAD_TREE_NODE * node, ID3D12GraphicsCommandList * pd3dCommandList, bool isGBuffers)
{
	// ������
	extern MeshRenderer gMeshRenderer;

	if (node->terrainMesh)
	{
		set.insert(node->id);
		gMeshRenderer.Render(pd3dCommandList, node->terrainMesh); 
	}
	else
	{
		if (node->children[0]->isRendering) RecursiveRender(node->children[0], pd3dCommandList, isGBuffers);
		if (node->children[1]->isRendering) RecursiveRender(node->children[1], pd3dCommandList, isGBuffers);
		if (node->children[2]->isRendering) RecursiveRender(node->children[2], pd3dCommandList, isGBuffers);
		if (node->children[3]->isRendering) RecursiveRender(node->children[3], pd3dCommandList, isGBuffers);
	}
}

void QtTerrainInstancingDrawer::RecursiveInitReafNodes(quadtree::QUAD_TREE_NODE * node)
{ 
	if (node->terrainMesh)
	{
		m_pReafNodes[node->id] = node;
	}
	else
	{
		RecursiveInitReafNodes(node->children[0]);
		RecursiveInitReafNodes(node->children[1]);
		RecursiveInitReafNodes(node->children[2]);
		RecursiveInitReafNodes(node->children[3]);
	}
}

void QtTerrainInstancingDrawer::RecursiveReleaseUploadBuffers(quadtree::QUAD_TREE_NODE * node)
{
	if (node->terrainMesh)
	{
		node->terrainMesh->ReleaseUploadBuffers();
	}
	else
	{
		RecursiveReleaseUploadBuffers(node->children[0]);
		RecursiveReleaseUploadBuffers(node->children[1]);
		RecursiveReleaseUploadBuffers(node->children[2]);
		RecursiveReleaseUploadBuffers(node->children[3]);
	}
}

void QtTerrainInstancingDrawer::RecursiveReleaseObjects(quadtree::QUAD_TREE_NODE * node)
{
	if (node->terrainMesh)
	{
		node->terrainMesh->ReleaseObjects();
		delete node->terrainMesh;
		node->terrainMesh = nullptr;
	}
	else
	{
		RecursiveReleaseObjects(node->children[0]);
		RecursiveReleaseObjects(node->children[1]);
		RecursiveReleaseObjects(node->children[2]);
		RecursiveReleaseObjects(node->children[3]);

		delete node->children[0];
		delete node->children[1];
		delete node->children[2];
		delete node->children[3];

		node->children[0] = nullptr;
		node->children[1] = nullptr;
		node->children[2] = nullptr;
		node->children[3] = nullptr;
	}
}

void QtTerrainInstancingDrawer::RecursiveCalculateIDs(quadtree::QUAD_TREE_NODE * node, const XMFLOAT3 position, int* pIDs) const
{
	if (node->terrainMesh)
	{
		assert(!(node->id == -1));
		// �������� �ش� �޽��� �´��� Ȯ���Ѵ�. 
		// x, z ���̿� �ִ��� �˻��Ѵ�.
		float minX = node->boundingBox.Center.x - node->boundingBox.Extents.x;
		float maxX = node->boundingBox.Center.x + node->boundingBox.Extents.x;
		bool isIntervenedX = (minX <= position.x) && (position.x <= maxX);

		float minZ = node->boundingBox.Center.z - node->boundingBox.Extents.z;
		float maxZ = node->boundingBox.Center.z + node->boundingBox.Extents.z;
		bool isIntervenedZ = (minZ <= position.z)  && (position.z <= maxZ);
		 
		if (isIntervenedX && isIntervenedZ)
		{  
			// ���� ���Ѵٸ� �ش� ID�� ä���.
			assert(!(pIDs[QUAD - 1] != -1)); // ���� �������� ä���� �ִٸ� �����̴�.
			for (int x = 0; x < QUAD; ++x)
			{
				if (pIDs[x] == -1)
				{
					pIDs[x] = node->id; 
					break;
				}
			} 
		}
	}
	else
	{
		RecursiveCalculateIDs(node->children[0], position, pIDs);
		RecursiveCalculateIDs(node->children[1], position, pIDs);
		RecursiveCalculateIDs(node->children[2], position, pIDs);
		RecursiveCalculateIDs(node->children[3], position, pIDs);
	}
}

void QtTerrainInstancingDrawer::CalculateIDs(const XMFLOAT3 position, XMINT4& IDs) const
{
	int num = 0;

	for (int i = 0; i < m_ReafNodeCount; ++i)
	{
		quadtree::QUAD_TREE_NODE* node = m_pReafNodes[i];

		if (Collision::isIn(node->boundingBox, position))
		{
			if (num == 0) IDs.x = i;
			else if (num == 1) IDs.y = i;
			else if (num == 2) IDs.z = i;
			else if (num == 3) IDs.w = i;
			++num;
		} 
		 
	}
}

void QtTerrainInstancingDrawer::CalculateIndex(const XMFLOAT3 position, int * pIndices) const
{
	for (int i = 0; i < m_ReafNodeCount; ++i)
	{
		quadtree::QUAD_TREE_NODE* node = m_pReafNodes[i];
		// �������� �ش� �޽��� �´��� Ȯ���Ѵ�. 
		// x, z ���̿� �ִ��� �˻��Ѵ�.
		float minX = node->boundingBox.Center.x - node->boundingBox.Extents.x;
		float maxX = node->boundingBox.Center.x + node->boundingBox.Extents.x;
		bool isIntervenedX = (minX <= position.x) && (position.x <= maxX);

		float minZ = node->boundingBox.Center.z - node->boundingBox.Extents.z;
		float maxZ = node->boundingBox.Center.z + node->boundingBox.Extents.z;
		bool isIntervenedZ = (minZ <= position.z) && (position.z <= maxZ);

		if (isIntervenedX && isIntervenedZ)
		{
			// ���� ���Ѵٸ� �ش� ID�� ä���.
			assert(!(pIndices[QUAD - 1] != -1)); // ���� �������� ä���� �ִٸ� �����̴�.
			for (int x = 0; x < QUAD; ++x)
			{
				if (pIndices[x] == -1)
				{
					pIndices[x] = i;
					break;
				}
			}
		}
	} 
}

void QtTerrainInstancingDrawer::RecursiveCreateTerrain(quadtree::QUAD_TREE_NODE * node, ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList,
	int xStart, int zStart, int nBlockWidth, int nBlockLength,
	 HeightMapImage * pContext)
{
	assert(!(m_widthMin < 3));
	assert(!(m_lengthMin < 3));
	assert(!(m_lengthMin != m_widthMin));

	if (nBlockWidth == m_widthMin || nBlockLength == m_lengthMin) // ������ ���� �����..
	{ 
		// ������ ���� ���� ���̵� �����Ѵ�.
		node->id = gTreePieceCount++;
		m_ReafNodeCount += 1;

		// ���� �׽�Ʈ�� �ٿ���ڽ��� centerY = 128, externY = 256 ���� ���� 
		XMFLOAT3 center{
			float(xStart) * m_xmf3Scale.x + float(nBlockWidth - 1) / 2.0f * m_xmf3Scale.x ,
			0.0f,
			float(zStart) * m_xmf3Scale.z + float(nBlockLength - 1) / 2.0f * m_xmf3Scale.z };

		XMFLOAT3 extents{
				float(nBlockWidth - 1) / 2.0f * m_xmf3Scale.x,
				1000.f,
				float(nBlockLength - 1) / 2.0f* m_xmf3Scale.z };
		 

		node->boundingBox = BoundingBox( center, extents); 
		node->terrainMesh = new TerrainMesh(this, pd3dDevice, pd3dCommandList, xStart, zStart, m_widthMin, m_lengthMin, m_xmf3Scale, m_xmf4Color, pContext);
	}
	else
	{ 
		int cxQuadsPerBlock = nBlockWidth - 1;
		int czQuadsPerBlock = nBlockLength - 1;

		int Next_BlockWidth = cxQuadsPerBlock / 2 + 1;
		int Next_BlockLength = czQuadsPerBlock / 2 + 1;

		int index = 0;
		 
		for (int z = 0; z < 2; z++)
		{
			for (int x = 0; x < 2; x++)
			{
				int New_xStart = xStart + x * (Next_BlockWidth - 1);
				int New_zStart = zStart + z * (Next_BlockLength - 1);

				XMFLOAT3 center{ 
					float(xStart) * m_xmf3Scale.x + float(nBlockWidth - 1) / 2.0f * m_xmf3Scale.x ,
					0.0f,
					float(zStart) * m_xmf3Scale.z + float(nBlockLength - 1) / 2.0f * m_xmf3Scale.z };
				
				XMFLOAT3 extents{
						float(nBlockWidth - 1) / 2.0f * m_xmf3Scale.x,
						1000.f,
						float(nBlockLength - 1) / 2.0f* m_xmf3Scale.z  };

				node->boundingBox = BoundingBox(center, extents);

				node->children[index] = new quadtree::QUAD_TREE_NODE(); 
				RecursiveCreateTerrain(node->children[index], pd3dDevice, pd3dCommandList, New_xStart, New_zStart, Next_BlockWidth, Next_BlockLength, pContext);
				index += 1;
			}
		} 
	}
}
 
QtTerrainInstancingDrawer::QtTerrainInstancingDrawer(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, int nWidth, int nLength, XMFLOAT3 xmf3Scale, XMFLOAT4 xmf4Color, HeightMapImage * pContext)
	: GameObject("QuadTreeTerrain")
{ 
	m_widthTotal = nWidth;
	m_lengthTotal = nLength;
	m_xmf3Scale = xmf3Scale;
	m_xmf4Color = xmf4Color;

	HeightMapImage *pHeightMapImage = (HeightMapImage *)pContext;
	int cxHeightMap = pHeightMapImage->GetHeightMapWidth();
	int czHeightMap = pHeightMapImage->GetHeightMapLength();

	// ���� Ʈ���� �θ� ��带 ����ϴ�.
	m_pRootNode = new quadtree::QUAD_TREE_NODE;
	 
	// ��������� ������ ���ϰ�, �ٿ���ڽ��� �ͷ��� ������ �����Ѵ�.
	// ��������X
	RecursiveCreateTerrain(m_pRootNode, pd3dDevice, pd3dCommandList, 0, 0, m_widthTotal, m_lengthTotal, pContext);
	m_pReafNodes = new quadtree::QUAD_TREE_NODE*[m_ReafNodeCount]; //������带 ����ų ������ �迭�� ����
	RecursiveInitReafNodes(m_pRootNode);
	// ��������X

	// ����Լ��� ��� �ͷ��� ���� �ε� �Ϸ���...
	// 1. ������ ����ϴ���.
	// StaticObjectStorage::GetInstance(this)->CreateInfo(pd3dDevice, pd3dCommandList, this);

	// 2. StaticObjectStorage ���� �Űܿ°�
	// ��ġ ������ �о�´�.
	LoadTerrainObjectFromFile(pd3dDevice, pd3dCommandList, "Information/Terrain.bin", this);

	// �о�� ��ġ ������ ������� ���̴� ������ �����Ѵ�.
	CreateShaderVariables(pd3dDevice, pd3dCommandList); 
}

QtTerrainInstancingDrawer::~QtTerrainInstancingDrawer()
{
	if (m_pRootNode)
	{
		RecursiveReleaseObjects(m_pRootNode);
		delete m_pRootNode;
		m_pRootNode = nullptr;
	}
}

void QtTerrainInstancingDrawer::Update(float fElapsedTime)
{
}

void QtTerrainInstancingDrawer::LastUpdate(float fElapsedTime)
{
	// �׷����ϴ� ������ �ε��� �ʱ�ȭ

}

XMINT4 const  QtTerrainInstancingDrawer::GetIDs(const XMFLOAT3 & position) const
{
	XMINT4 IDs{ -1, -1, -1, -1 };

	CalculateIDs(position, IDs);

	return IDs;
}

int * const QtTerrainInstancingDrawer::GetIndex(const XMFLOAT3 & position) const
{
	int* pIndeics = new int[QUAD];
	for (int x = 0; x < QUAD; ++x)
	{
		pIndeics[x] = -1; // -1�� ����. -1�̶�� �������� �ʴ� ��.
	}

	CalculateIndex(position, pIndeics);

	return pIndeics;
}

void QtTerrainInstancingDrawer::RenderObjAll(ID3D12GraphicsCommandList* pd3dCommandList, bool isGBuffers)
{
	for (int i = 0; i < TerrainPieceCount; ++i)
	{
		RenderObj(pd3dCommandList, i, isGBuffers);
	}
}

void QtTerrainInstancingDrawer::RenderObj(ID3D12GraphicsCommandList* pd3dCommandList, int index, bool isGBuffers)
{
	// info.first = �� �̸�
// info.second = TerrainObjectInfo��� �� ����

	for (auto& info : m_StaticObjectStorage)
	{
		if (info.second[index].TerrainObjectCount == 0) continue;
		if (!strcmp(info.first.c_str(), Flower)) continue;
		if (!strcmp(info.first.c_str(), RUIN_FLOOR)) continue;

		pd3dCommandList->SetGraphicsRootShaderResourceView(ROOTPARAMETER_INSTANCING, info.second[index].m_pd3dcbGameObjects->GetGPUVirtualAddress()); // �ν��Ͻ� ���̴� ���ҽ� ��

		m_StaticObjectModelsStorage[info.first].pTexture->UpdateShaderVariables(pd3dCommandList);

		m_StaticObjectModelsStorage[info.first].pLoadObject->RenderInstancing(pd3dCommandList, info.second[index].TerrainObjectCount, isGBuffers);
	}
}

void QtTerrainInstancingDrawer::RenderObjForShadow(ID3D12GraphicsCommandList* pd3dCommandList, int index, bool isGBuffers)
{// info.first = �� �̸�
	// info.second = TerrainObjectInfo��� �� ����

	for (auto& info : m_StaticObjectStorage)
	{
		if (info.second[index].TerrainObjectCount == 0) continue;
		if (!strcmp(info.first.c_str(), Flower)) continue;
		if (!strcmp(info.first.c_str(), RUIN_FLOOR)) continue;

		pd3dCommandList->SetGraphicsRootShaderResourceView(ROOTPARAMETER_INSTANCING, info.second[index].m_pd3dcbGameObjects->GetGPUVirtualAddress()); // �ν��Ͻ� ���̴� ���ҽ� ��

		m_StaticObjectModelsStorage[info.first].pLoadObject->RenderInstancing(pd3dCommandList, info.second[index].TerrainObjectCount, isGBuffers);
	}
}

void QtTerrainInstancingDrawer::RenderObjBOBox(ID3D12GraphicsCommandList* pd3dCommandList, int index)
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

int QtTerrainInstancingDrawer::GetObjectCount(int index, const std::string& name)
{
	return m_StaticObjectStorage[name][index].TerrainObjectCount; 
}

int QtTerrainInstancingDrawer::GetObjectAllCount(int index)
{
	int result = 0;

	for (auto& info : m_StaticObjectStorage)
	{
		result += info.second[index].TerrainObjectCount;
	}

	return result;
}

XMFLOAT4X4* QtTerrainInstancingDrawer::GetWorldMatrix(int index, const std::string& name)
{
	return m_StaticObjectStorage[name][index].TransformList.begin()._Ptr; 
}

XMFLOAT4X4 QtTerrainInstancingDrawer::GetAltarTransform(int index, const std::string& name)
{
	return m_AltarTransformStorage[index];
}

void QtTerrainInstancingDrawer::CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{//�ν��Ͻ� ������ ������ ���� ���۸� ���ε� �� �������� �����Ѵ�. 
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

			info.second[iTerrainPiece].m_pd3dcbGameObjects->Map(0, NULL, (void**)&info.second[iTerrainPiece].m_pcbMappedGameObjects);
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

void QtTerrainInstancingDrawer::Render(ID3D12GraphicsCommandList *pd3dCommandList, bool isGBuffers)
{ 
	RenderTerrainObjects(pd3dCommandList, isGBuffers); // ���� ������Ʈ ����

	ShaderManager::GetInstance()->SetPSO(pd3dCommandList, SHADER_TERRAIN, isGBuffers);
	RecursiveRender(m_pRootNode, pd3dCommandList, isGBuffers); // ���� ����	 
}

void QtTerrainInstancingDrawer::RenderTerrainForShadow(ID3D12GraphicsCommandList * pd3dCommandList, Terrain * pTerrain, ID3D12DescriptorHeap* pHeap)
{ 
	ShaderManager::GetInstance()->SetPSO(pd3dCommandList, SHADER_TERRAIN_FORSHADOW, false);

	pd3dCommandList->SetGraphicsRoot32BitConstants(ROOTPARAMETER_WORLD, 16, &Matrix4x4::Identity(), 0);
	pd3dCommandList->SetDescriptorHeaps(1, &pHeap);
	pTerrain->UpdateShaderVariables(pd3dCommandList);

	RecursiveRender(m_pRootNode, pd3dCommandList, false); // ���� ����	 
}

void QtTerrainInstancingDrawer::RenderInstancingObjectsForShadow(ID3D12GraphicsCommandList * pd3dCommandList)
{
	ShaderManager::GetInstance()->SetPSO(pd3dCommandList, SHADER_INSTACINGSTANDARDFORSHADOW, false);

	// ������ �� ���� 
	RecursiveRenderTerrainObjectsForShadow(m_pRootNode, pd3dCommandList, false);
}

void QtTerrainInstancingDrawer::Render(ID3D12GraphicsCommandList * pd3dCommandList, Terrain * pTerrain, ID3D12DescriptorHeap* pHeap, bool isGBuffers)
{
	// ���� ����
	ShaderManager::GetInstance()->SetPSO(pd3dCommandList, SHADER_TERRAIN, isGBuffers);

	pd3dCommandList->SetGraphicsRoot32BitConstants(ROOTPARAMETER_WORLD, 16, &Matrix4x4::Identity(), 0); 
	
	pTerrain->UpdateShaderVariables(pd3dCommandList);

	RecursiveRender(m_pRootNode, pd3dCommandList, isGBuffers); // ���� ����	 

	// ���� ������Ʈ ����
	RenderTerrainObjects(pd3dCommandList, isGBuffers);

}

void QtTerrainInstancingDrawer::Render(int index, ID3D12GraphicsCommandList * pd3dCommandList, bool isGBuffers)
{
	// ������
	extern MeshRenderer gMeshRenderer; 
	if (index < 0 || index >= m_ReafNodeCount) return;
	ShaderManager::GetInstance()->SetPSO(pd3dCommandList, SHADER_TERRAIN, isGBuffers);
	gMeshRenderer.Render(pd3dCommandList, m_pReafNodes[index]->terrainMesh);
	StaticObjectStorage::GetInstance(this)->Render(pd3dCommandList, index, isGBuffers);
}

quadtree::QUAD_TREE_NODE * QtTerrainInstancingDrawer::GetReafNodeByID(int id)
{
	return nullptr;
}
