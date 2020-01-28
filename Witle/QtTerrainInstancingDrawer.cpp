#include "stdafx.h"
#include "d3dUtil.h"
#include "TerrainMesh.h" 
#include "ShaderManager.h"
#include "Shader.h"
#include "MeshRenderer.h"
#include "TextureStorage.h" 
#include "HeightMapImage.h"
#include <unordered_set>
#include "MyFrustum.h"
#include "Collision.h"
#include "CameraObject.h"
#include "Terrain.h"
#include "ModelStorage.h"
#include "MyDescriptorHeap.h"
#include "Object.h"
#include "Texture.h"
#include "MapInfoMgr.h"
#include "MyFrustum.h"
#include "CameraObject.h"
#include "Camera.h"
#include "MyFrustum.h"
#include "QtTerrainInstancingDrawer.h"
 
 
void QtTerrainInstancingDrawer::AddDataListOfNode(quadtree::QT_DRAWER_NODE& node, const quadtree::QT_DRAWER_ADDER& adder)
{
	node.ModelInfoList[adder.model_name].TerrainObjectCount += 1;
	node.ModelInfoList[adder.model_name].TransformList.push_back(adder.world);
}

void QtTerrainInstancingDrawer::ProcessDataOfNode(quadtree::QT_DRAWER_NODE& node, GameObject& gameobj)
{
	CameraObject* pCameraObject = dynamic_cast<CameraObject*>(&gameobj);
	if (pCameraObject == nullptr) return;

	// bool isIntersect = pCameraObject->GetCamera()->GetFrustum()->IsIntersect(); 
}

void QtTerrainInstancingDrawer::ReleaseMembers()
{
	
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

			for (const auto& modelname : ModelStorage::GetInstance()->m_NameList)
			{
				bool isLocated = LoadTransform(name, modelname.c_str(), transform);
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

bool QtTerrainInstancingDrawer::LoadTransform(char* name, const char* comp_name, const XMFLOAT4X4& tr)
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

			// ���� ������Ʈ�� ������ ��� �ͷ��� ������ �����Ŵϱ� �浹ü ó�� X  
			MyBOBox temp(MapInfoMgr::GetMapCenter(),
			             XMFLOAT3{ MapInfoMgr::GetMapExtentsX(), 10000.f, MapInfoMgr::GetMapExtentsZ() });

			AddWorldMatrix(temp, Cliff, TestObject->m_pChild->m_xmf4x4World); 
			 
			delete TestObject;
			TestObject = nullptr;

			return true; // ������ ��� �� ���� ó��
		}

		for (int Ti = 0; Ti < 4; ++Ti)
		{ 
			if (!strcmp(name, "Cylinder001"))
			{
				// RUIN_FLOOR ����ó��
				// ���� ������ 

				//LoadObject* TestObject = ModelStorage::GetInstance()->GetRootObject(RUIN_FLOOR);
				//TestObject->SetTransform(tr);
				//TestObject->UpdateTransform(NULL);
				
				//delete TestObject;
				//TestObject = nullptr;
			}
			else
			{ 
				LoadObject* TestObject = ModelStorage::GetInstance()->GetRootObject(comp_name);
				TestObject->SetTransform(tr);
				TestObject->UpdateTransform(NULL);

				// ��� ���� �浹ü�� ���ϰ����� �ʴ�. �ѹ� Ȯ�����ش�.
				MyBOBox* pBoBox = ModelStorage::GetInstance()->GetBOBox(comp_name);
				if (pBoBox == nullptr)
				{
					// �浹ü�� ���ϰ� ���� ���� ��� �ӽ÷� �����.
					MyBOBox tempbobox(XMFLOAT3(0, 0, 0), XMFLOAT3(1, 1, 1));
					tempbobox.Move(XMFLOAT3(TestObject->m_pChild->m_xmf4x4World._41, 0, TestObject->m_pChild->m_xmf4x4World._43));

					AddWorldMatrix(tempbobox, comp_name, TestObject->m_pChild->m_xmf4x4World);
				}
				else
				{
					// �浹ü�� ���� �ִ� ��� 
					MyBOBox mybobox = *pBoBox;
					mybobox.Move(XMFLOAT3(TestObject->m_pChild->m_xmf4x4World._41, 0, TestObject->m_pChild->m_xmf4x4World._43));
					AddWorldMatrix(mybobox, comp_name, TestObject->m_pChild->m_xmf4x4World);
				}

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

void QtTerrainInstancingDrawer::RecursiveRenderTerrainObjects_BOBox(const quadtree::QT_DRAWER_NODE * node, ID3D12GraphicsCommandList * pd3dCommandList)
{
	if (node->isRendering)
	{
		if (node->terrainMesh)
		{
			RenderObjBOBox(pd3dCommandList, node->id);
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

void QtTerrainInstancingDrawer::RecursiveRenderTerrainObjects(const quadtree::QT_DRAWER_NODE * node, ID3D12GraphicsCommandList * pd3dCommandList, bool isGBuffers)
{ 
	if (node->isRendering)
	{
		if (node->terrainMesh)
		{
			RenderObj(pd3dCommandList, node->id, isGBuffers); 
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


void QtTerrainInstancingDrawer::RecursiveRenderTerrainObjectsForShadow(const quadtree::QT_DRAWER_NODE * node, ID3D12GraphicsCommandList * pd3dCommandList, bool isGBuffers)
{
	if (node->isRendering)
	{
		if (node->terrainMesh)
		{
			RenderObjForShadow(pd3dCommandList, node->id, isGBuffers); 
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
 
void QtTerrainInstancingDrawer::RecursiveRender(const quadtree::QT_DRAWER_NODE * node, ID3D12GraphicsCommandList * pd3dCommandList, bool isGBuffers)
{
	// ������
	extern MeshRenderer gMeshRenderer;

	if (node->terrainMesh)
	{ 
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

void QtTerrainInstancingDrawer::RecursiveInitReafNodes(quadtree::QT_DRAWER_NODE * node)
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

void QtTerrainInstancingDrawer::RecursiveReleaseUploadBuffers(quadtree::QT_DRAWER_NODE * node)
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

void QtTerrainInstancingDrawer::RecursiveReleaseObjects(quadtree::QT_DRAWER_NODE * node)
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

void QtTerrainInstancingDrawer::RecursiveCalculateIDs(quadtree::QT_DRAWER_NODE * node, const XMFLOAT3 position, int* pIDs) const
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
 
void QtTerrainInstancingDrawer::RecursiveCreateTerrain(quadtree::QT_DRAWER_NODE * node, ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList,
	int xStart, int zStart, int nBlockWidth, int nBlockLength,
	 HeightMapImage * pContext)
{
	assert(!(m_widthMin < 3));
	assert(!(m_lengthMin < 3));
	assert(!(m_lengthMin != m_widthMin));

	if (nBlockWidth == m_widthMin || nBlockLength == m_lengthMin) // ������ ���� �����..
	{ 
		// ������ ���� ���� ���̵� �����Ѵ�. 
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
		node->terrainMesh = new TerrainMesh(m_EmptyObj, pd3dDevice, pd3dCommandList, xStart, zStart, m_widthMin, m_lengthMin, m_xmf3Scale, m_xmf4Color, pContext);
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

				node->children[index] = new quadtree::QT_DRAWER_NODE(center, extents); 
				RecursiveCreateTerrain(node->children[index], pd3dDevice, pd3dCommandList, New_xStart, New_zStart, Next_BlockWidth, Next_BlockLength, pContext);
				index += 1;
			}
		} 
	}
}

void QtTerrainInstancingDrawer::CreateReafNode(quadtree::QT_DRAWER_NODE* node, ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, HeightMapImage* pContext)
{  
	// ���� �ڽ��� �ִٸ�, �� ������尡 �ƴ϶�� ���â�� ����´�.
	assert(!(node->children[0] != nullptr) && "is not reaf node");

	int xStart = (node->BoBox->GetBOBox().Center.x - float(m_widthMin - 1) / 2.0f * m_xmf3Scale.x) / m_xmf3Scale.x;
	int zStart = (node->BoBox->GetBOBox().Center.z - float(m_lengthMin - 1) / 2.0f * m_xmf3Scale.z) / m_xmf3Scale.z;
	 
	node->terrainMesh = new TerrainMesh(m_EmptyObj, pd3dDevice, pd3dCommandList, xStart, zStart, m_widthMin, m_lengthMin, m_xmf3Scale, m_xmf4Color, pContext);
}
 
QtTerrainInstancingDrawer::QtTerrainInstancingDrawer(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, 
	const XMFLOAT3& center, const XMFLOAT3& extents, float min_size,
	int nWidth, int nLength, XMFLOAT3 xmf3Scale, XMFLOAT4 xmf4Color, HeightMapImage * pContext)
	: Quadtree<quadtree::QT_DRAWER_NODE, quadtree::QT_DRAWER_ADDER>(center, extents, min_size)
{ 
	m_widthTotal = nWidth;
	m_lengthTotal = nLength;
	m_xmf3Scale = xmf3Scale;
	m_xmf4Color = xmf4Color;

	HeightMapImage *pHeightMapImage = (HeightMapImage *)pContext;
	int cxHeightMap = pHeightMapImage->GetHeightMapWidth();
	int czHeightMap = pHeightMapImage->GetHeightMapLength();

	m_EmptyObj = new EmptyGameObject("QtTerrainInstancingDrawer");

	for (int index = 0; index < GetReafNodeCount(); ++index)
	{
		CreateReafNode(GetpReaf(index), pd3dDevice, pd3dCommandList, pContext);
	}

	// ���� Ʈ���� �θ� ��带 ����ϴ�.
	m_pRootNode = new quadtree::QT_DRAWER_NODE(center, extents);
	 
	// ��������� ������ ���ϰ�, �ٿ���ڽ��� �ͷ��� ������ �����Ѵ�.
	// ��������X
	RecursiveCreateTerrain(m_pRootNode, pd3dDevice, pd3dCommandList, 0, 0, m_widthTotal, m_lengthTotal, pContext);
	m_pReafNodes = new quadtree::QT_DRAWER_NODE*[m_ReafNodeCount]; //������带 ����ų ������ �迭�� ����
	RecursiveInitReafNodes(m_pRootNode);
	// ��������X 

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
  
void QtTerrainInstancingDrawer::RenderObjAll(ID3D12GraphicsCommandList* pd3dCommandList, Terrain* pTerrain, bool isGBuffers)
{ 
	// ���� ����
	ShaderManager::GetInstance()->SetPSO(pd3dCommandList, SHADER_TERRAIN, isGBuffers);

	pd3dCommandList->SetGraphicsRoot32BitConstants(ROOTPARAMETER_WORLD, 16, &Matrix4x4::Identity(), 0);

	pTerrain->UpdateShaderVariables(pd3dCommandList);

	extern MeshRenderer gMeshRenderer;

	for (int index = 0; index < GetReafNodeCount(); ++index)
	{
		quadtree::QT_DRAWER_NODE* const pReaf = GetpReaf(index);

		if (pReaf->terrainMesh)
		{
			gMeshRenderer.Render(pd3dCommandList, pReaf->terrainMesh);
		}
	} 

	// ���� ������Ʈ ������
	ShaderManager::GetInstance()->SetPSO(pd3dCommandList, "InstancingStandardShader", isGBuffers);

	for (int index = 0; index < GetReafNodeCount(); ++index)
	{ 
		quadtree::QT_DRAWER_NODE* const pReaf = GetpReaf(index);

		// key : std::string���� �̸��� �ǹ��մϴ�.
		// ��� ������带 ��ȸ�ϸ鼭 �� ������忡 ���� ������ ����ϴ�.
		for (auto& info : pReaf->ModelInfoList)
		{ 
			if (info.second.TerrainObjectCount == 0) continue;
			if (!strcmp(info.first.c_str(), Flower)) continue;
			if (!strcmp(info.first.c_str(), RUIN_FLOOR)) continue;

			pd3dCommandList->SetGraphicsRootShaderResourceView(ROOTPARAMETER_INSTANCING, info.second.m_pd3dcbGameObjects->GetGPUVirtualAddress()); // �ν��Ͻ� ���̴� ���ҽ� ��

			info.second.pTexture->UpdateShaderVariables(pd3dCommandList);

			info.second.pLoadObject->RenderInstancing(pd3dCommandList, info.second.TerrainObjectCount, isGBuffers); 
		}
	}
}

void QtTerrainInstancingDrawer::RenderObj(ID3D12GraphicsCommandList* pd3dCommandList, int index, bool isGBuffers)
{

}

void QtTerrainInstancingDrawer::RenderObjForShadow(ID3D12GraphicsCommandList* pd3dCommandList, int index, bool isGBuffers)
{
	// info.first = �� �̸�
	// info.second = TerrainObjectInfo��� �� ����

	//for (auto& info : m_StaticObjectStorage)
	//{
	//	if (info.second[index].TerrainObjectCount == 0) continue;
	//	if (!strcmp(info.first.c_str(), Flower)) continue;
	//	if (!strcmp(info.first.c_str(), RUIN_FLOOR)) continue;

	//	pd3dCommandList->SetGraphicsRootShaderResourceView(ROOTPARAMETER_INSTANCING, info.second[index].m_pd3dcbGameObjects->GetGPUVirtualAddress()); // �ν��Ͻ� ���̴� ���ҽ� ��

	//	m_StaticObjectModelsStorage[info.first].pLoadObject->RenderInstancing(pd3dCommandList, info.second[index].TerrainObjectCount, isGBuffers);
	//}
}

void QtTerrainInstancingDrawer::RenderObjBOBox(ID3D12GraphicsCommandList* pd3dCommandList, int index)
{
	// info.first = �� �̸�
	// info.second = TerrainObjectInfo��� �� ����

	//for (auto& info : m_StaticObjectStorage)
	//{
	//	if (info.second[index].TerrainObjectCount == 0) continue;

	//	pd3dCommandList->SetGraphicsRootShaderResourceView(ROOTPARAMETER_INSTANCING, info.second[index].m_pd3dcbGameObjects->GetGPUVirtualAddress()); // �ν��Ͻ� ���̴� ���ҽ� ��

	//	ModelStorage::GetInstance()->RenderBOBoxInstancing(pd3dCommandList, info.first, info.second[index].TerrainObjectCount);
	//}
}
  

XMFLOAT4X4 QtTerrainInstancingDrawer::GetAltarTransform(int index, const std::string& name)
{
	return m_AltarTransformStorage[index];
}

void QtTerrainInstancingDrawer::CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{  
	//�ν��Ͻ� ������ ������ ���� ���۸� ���ε� �� �������� �����Ѵ�. 
	for (int index = 0; index < GetReafNodeCount(); ++index)
	{
		quadtree::QT_DRAWER_NODE* const pReaf = GetpReaf(index);

		// key : std::string���� �̸��� �ǹ��մϴ�.
		// ��� ������带 ��ȸ�ϸ鼭 �� ������忡 ���� ������ ����ϴ�.
		for (auto& info : pReaf->ModelInfoList)
		{
			// �� ����Ƽ�� �ؽ��� ������ �ҷ���
			info.second.pLoadObject = ModelStorage::GetInstance()->GetRootObject(info.first);
			info.second.pTexture = TextureStorage::GetInstance()->GetTexture(info.first);

			// �ͷ��� ������Ʈ�� ���ٸ� �Ѿ��.
			if (info.second.TerrainObjectCount == 0) continue;

			// ���̴� ���� ����
			info.second.m_pd3dcbGameObjects =
				d3dUtil::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL,
					sizeof(VS_SRV_INSTANCEINFO) * info.second.TerrainObjectCount, D3D12_HEAP_TYPE_UPLOAD,
					D3D12_RESOURCE_STATE_GENERIC_READ, NULL);

			info.second.m_pd3dcbGameObjects->Map(0, NULL, (void**)&info.second.m_pcbMappedGameObjects);

			// ���̴� ���� �ȿ� ���� ����
			// vector �ȿ� ����ִ� ��� ������ XMMatrixTranspose, �� ��ġ��ĵ� ����� �ϳ��� �����Ѵ�.
			for (int i = 0; i < info.second.TerrainObjectCount; ++i)
			{
				XMStoreFloat4x4(
					&info.second.m_pcbMappedGameObjects[i].m_xmf4x4Transform,
					XMMatrixTranspose(XMLoadFloat4x4(&info.second.TransformList[i]
					)));
			} 

			// ���� info.second.TransformList �� ����Ǿ����Ƿ� �� �̻� ���� �ʴ´�.
			// �޸𸮸� ����.
			info.second.TransformList.clear();
		}
		 
	}
}

void QtTerrainInstancingDrawer::Render(ID3D12GraphicsCommandList *pd3dCommandList, bool isGBuffers)
{ 
	RenderTerrainObjects(pd3dCommandList, isGBuffers); // ���� ������Ʈ ����

	ShaderManager::GetInstance()->SetPSO(pd3dCommandList, SHADER_TERRAIN, isGBuffers);
	RecursiveRender(m_pRootNode, pd3dCommandList, isGBuffers); // ���� ����	 
}

void QtTerrainInstancingDrawer::Init()
{
}

void QtTerrainInstancingDrawer::PrintInfo()
{
}

// �׸��� �������� ���� ���� ������Ʈ�� �����մϴ�.
void QtTerrainInstancingDrawer::RenderTerrainForShadow(ID3D12GraphicsCommandList * pd3dCommandList, Terrain * pTerrain, ID3D12DescriptorHeap* pHeap)
{ 
	ShaderManager::GetInstance()->SetPSO(pd3dCommandList, SHADER_TERRAIN_FORSHADOW, false);

	pd3dCommandList->SetGraphicsRoot32BitConstants(ROOTPARAMETER_WORLD, 16, &Matrix4x4::Identity(), 0);
	pd3dCommandList->SetDescriptorHeaps(1, &pHeap);
	pTerrain->UpdateShaderVariables(pd3dCommandList);

	RecursiveRender(m_pRootNode, pd3dCommandList, false); // ���� ����	 
}


// �׸��� �������� ���� ���� ������Ʈ�� �������մϴ�.
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
	RenderObj(pd3dCommandList, index, isGBuffers);
}
 
void QtTerrainInstancingDrawer::AddWorldMatrix(const MyBOBox& collider, const std::string& model_name, const XMFLOAT4X4& world)
{
	quadtree::QT_DRAWER_ADDER add_data(model_name, world); 
	AddRecursiveDataOfNode(*GetpRoot(), collider, add_data);
}
 
void QtTerrainInstancingDrawer::ReleaseObjects()
{
	ReleaseMembers();
}

void QtTerrainInstancingDrawer::ReleaseUploadBuffers()
{
	ReleaseMemberUploadBuffers();
}
