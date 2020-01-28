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
				int nRootChild = FileRead::ReadIntegerFromFile(pInFile); // 모든 오브젝트 개수

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

			// 계산을 통해 몇번째 아이디인지 즉 어디의 리프노드에 존재하는 위치인지 알아낸다...
			// fbx sdk 에서 꺼내올때 무슨 문제가 있는지 x, z좌표에 -부호 붙여야함 ...
			// 그리고 위치 차이때문에 

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

	if (!strcmp(name, comp_name) || !strcmp(name, "Cylinder001") /*Floor때문에 하드코딩*/ || !strcmp(name, "Object001"))
	{
		// 만약 name이 절벽인 경우...
		if (!strcmp(name, "Object001"))
		{
			XMFLOAT4X4 newXMFLOAT4X4 = tr;
			newXMFLOAT4X4._42 = 50;
			LoadObject* TestObject = ModelStorage::GetInstance()->GetRootObject(Cliff);
			TestObject->SetTransform(newXMFLOAT4X4); // 여기서 Scale 과 다이렉트 X축에 대한 회전 일어나므로 절대 빼먹으면 안됨..
			TestObject->UpdateTransform(NULL);

			// 절벽 오브젝트는 어차피 모든 터레인 조각에 넣을거니까 충돌체 처리 X  
			MyBOBox temp(MapInfoMgr::GetMapCenter(),
			             XMFLOAT3{ MapInfoMgr::GetMapExtentsX(), 10000.f, MapInfoMgr::GetMapExtentsZ() });

			AddWorldMatrix(temp, Cliff, TestObject->m_pChild->m_xmf4x4World); 
			 
			delete TestObject;
			TestObject = nullptr;

			return true; // 절벽인 경우 만 예외 처리
		}

		for (int Ti = 0; Ti < 4; ++Ti)
		{ 
			if (!strcmp(name, "Cylinder001"))
			{
				// RUIN_FLOOR 예외처리
				// 현재 사용안함 

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

				// 모든 모델은 충돌체를 지니고있지 않다. 한번 확인해준다.
				MyBOBox* pBoBox = ModelStorage::GetInstance()->GetBOBox(comp_name);
				if (pBoBox == nullptr)
				{
					// 충돌체를 지니고 있지 않은 경우 임시로 만든다.
					MyBOBox tempbobox(XMFLOAT3(0, 0, 0), XMFLOAT3(1, 1, 1));
					tempbobox.Move(XMFLOAT3(TestObject->m_pChild->m_xmf4x4World._41, 0, TestObject->m_pChild->m_xmf4x4World._43));

					AddWorldMatrix(tempbobox, comp_name, TestObject->m_pChild->m_xmf4x4World);
				}
				else
				{
					// 충돌체를 갖고 있는 경우 
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
	// 렌더링
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
		// 포지션이 해당 메쉬에 맞는지 확인한다. 
		// x, z 사이에 있는지 검사한다.
		float minX = node->boundingBox.Center.x - node->boundingBox.Extents.x;
		float maxX = node->boundingBox.Center.x + node->boundingBox.Extents.x;
		bool isIntervenedX = (minX <= position.x) && (position.x <= maxX);

		float minZ = node->boundingBox.Center.z - node->boundingBox.Extents.z;
		float maxZ = node->boundingBox.Center.z + node->boundingBox.Extents.z;
		bool isIntervenedZ = (minZ <= position.z)  && (position.z <= maxZ);
		 
		if (isIntervenedX && isIntervenedZ)
		{  
			// 만약 속한다면 해당 ID를 채운다.
			assert(!(pIDs[QUAD - 1] != -1)); // 만약 마지막이 채워져 있다면 오류이다.
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

	if (nBlockWidth == m_widthMin || nBlockLength == m_lengthMin) // 마지막 리프 노드라면..
	{ 
		// 마지막 리프 노드는 아이디를 설정한다. 
		m_ReafNodeCount += 1;

		// 현재 테스트로 바운딩박스의 centerY = 128, externY = 256 으로 설정 
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
	// 만약 자식이 있다면, 즉 리프노드가 아니라면 경고창을 내뱉는다.
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

	// 쿼드 트리의 부모 노드를 만듭니다.
	m_pRootNode = new quadtree::QT_DRAWER_NODE(center, extents);
	 
	// 리프노드의 개수를 구하고, 바운딩박스및 터레인 조각을 생성한다.
	// 순서변경X
	RecursiveCreateTerrain(m_pRootNode, pd3dDevice, pd3dCommandList, 0, 0, m_widthTotal, m_lengthTotal, pContext);
	m_pReafNodes = new quadtree::QT_DRAWER_NODE*[m_ReafNodeCount]; //리프노드를 가리킬 포인터 배열을 생성
	RecursiveInitReafNodes(m_pRootNode);
	// 순서변경X 

	// 위치 정보를 읽어온다.
	LoadTerrainObjectFromFile(pd3dDevice, pd3dCommandList, "Information/Terrain.bin", this);

	// 읽어온 위치 정보의 개수대로 쉐이더 변수를 생성한다.
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
	// 지형 렌더
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

	// 지형 오브젝트 렌더링
	ShaderManager::GetInstance()->SetPSO(pd3dCommandList, "InstancingStandardShader", isGBuffers);

	for (int index = 0; index < GetReafNodeCount(); ++index)
	{ 
		quadtree::QT_DRAWER_NODE* const pReaf = GetpReaf(index);

		// key : std::string으로 이름을 의미합니다.
		// 모든 리프노드를 순회하면서 각 리프노드에 대한 변수를 만듭니다.
		for (auto& info : pReaf->ModelInfoList)
		{ 
			if (info.second.TerrainObjectCount == 0) continue;
			if (!strcmp(info.first.c_str(), Flower)) continue;
			if (!strcmp(info.first.c_str(), RUIN_FLOOR)) continue;

			pd3dCommandList->SetGraphicsRootShaderResourceView(ROOTPARAMETER_INSTANCING, info.second.m_pd3dcbGameObjects->GetGPUVirtualAddress()); // 인스턴싱 쉐이더 리소스 뷰

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
	for (int index = 0; index < GetReafNodeCount(); ++index)
	{
		quadtree::QT_DRAWER_NODE* const pReaf = GetpReaf(index);

		// key : std::string으로 이름을 의미합니다.
		// 모든 리프노드를 순회하면서 각 리프노드에 대한 변수를 만듭니다.
		for (auto& info : pReaf->ModelInfoList)
		{
			if (info.second.TerrainObjectCount == 0) continue;
			if (!strcmp(info.first.c_str(), Flower)) continue;
			if (!strcmp(info.first.c_str(), RUIN_FLOOR)) continue;

			pd3dCommandList->SetGraphicsRootShaderResourceView(ROOTPARAMETER_INSTANCING, info.second.m_pd3dcbGameObjects->GetGPUVirtualAddress()); // 인스턴싱 쉐이더 리소스 뷰
			 
			info.second.pLoadObject->RenderInstancing(pd3dCommandList, info.second.TerrainObjectCount, isGBuffers);
		}
	} 
}

void QtTerrainInstancingDrawer::RenderObjBOBox(ID3D12GraphicsCommandList* pd3dCommandList, int index)
{
	// info.first = 모델 이름
	// info.second = TerrainObjectInfo라는 모델 정보

	//for (auto& info : m_StaticObjectStorage)
	//{
	//	if (info.second[index].TerrainObjectCount == 0) continue;

	//	pd3dCommandList->SetGraphicsRootShaderResourceView(ROOTPARAMETER_INSTANCING, info.second[index].m_pd3dcbGameObjects->GetGPUVirtualAddress()); // 인스턴싱 쉐이더 리소스 뷰

	//	ModelStorage::GetInstance()->RenderBOBoxInstancing(pd3dCommandList, info.first, info.second[index].TerrainObjectCount);
	//}
}
  

XMFLOAT4X4 QtTerrainInstancingDrawer::GetAltarTransform(int index, const std::string& name)
{
	return m_AltarTransformStorage[index];
}

void QtTerrainInstancingDrawer::CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{  
	//인스턴스 정보를 저장할 정점 버퍼를 업로드 힙 유형으로 생성한다. 
	for (int index = 0; index < GetReafNodeCount(); ++index)
	{
		quadtree::QT_DRAWER_NODE* const pReaf = GetpReaf(index);

		// key : std::string으로 이름을 의미합니다.
		// 모든 리프노드를 순회하면서 각 리프노드에 대한 변수를 만듭니다.
		for (auto& info : pReaf->ModelInfoList)
		{
			// 모델 데이티와 텍스쳐 데이터 불러옴
			info.second.pLoadObject = ModelStorage::GetInstance()->GetRootObject(info.first);
			info.second.pTexture = TextureStorage::GetInstance()->GetTexture(info.first);

			// 터레인 오브젝트가 없다면 넘어간다.
			if (info.second.TerrainObjectCount == 0) continue;

			// 쉐이더 변수 생성
			info.second.m_pd3dcbGameObjects =
				d3dUtil::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL,
					sizeof(VS_SRV_INSTANCEINFO) * info.second.TerrainObjectCount, D3D12_HEAP_TYPE_UPLOAD,
					D3D12_RESOURCE_STATE_GENERIC_READ, NULL);

			info.second.m_pd3dcbGameObjects->Map(0, NULL, (void**)&info.second.m_pcbMappedGameObjects);

			// 쉐이더 변수 안에 정보 저장
			// vector 안에 들어있는 모든 정보를 XMMatrixTranspose, 즉 전치행렬도 뒤집어서 하나씩 저장한다.
			for (int i = 0; i < info.second.TerrainObjectCount; ++i)
			{
				XMStoreFloat4x4(
					&info.second.m_pcbMappedGameObjects[i].m_xmf4x4Transform,
					XMMatrixTranspose(XMLoadFloat4x4(&info.second.TransformList[i]
					)));
			} 

			// 이제 info.second.TransformList 은 저장되었으므로 더 이상 쓰지 않는다.
			// 메모리를 비운다.
			info.second.TransformList.clear();
		}
		 
	}
}

void QtTerrainInstancingDrawer::Render(ID3D12GraphicsCommandList *pd3dCommandList, bool isGBuffers)
{ 
	RenderTerrainObjects(pd3dCommandList, isGBuffers); // 지형 오브젝트 렌더

	ShaderManager::GetInstance()->SetPSO(pd3dCommandList, SHADER_TERRAIN, isGBuffers);
	RecursiveRender(m_pRootNode, pd3dCommandList, isGBuffers); // 지형 렌더	 
}

void QtTerrainInstancingDrawer::Init()
{
}

void QtTerrainInstancingDrawer::PrintInfo()
{
}

// 그림자 렌더링을 위한 지형 오브젝트를 렌더합니다.
void QtTerrainInstancingDrawer::RenderTerrainForShadow(ID3D12GraphicsCommandList * pd3dCommandList, Terrain * pTerrain, ID3D12DescriptorHeap* pHeap)
{ 
	ShaderManager::GetInstance()->SetPSO(pd3dCommandList, SHADER_TERRAIN_FORSHADOW, false);

	pd3dCommandList->SetGraphicsRoot32BitConstants(ROOTPARAMETER_WORLD, 16, &Matrix4x4::Identity(), 0);
	pd3dCommandList->SetDescriptorHeaps(1, &pHeap);
	pTerrain->UpdateShaderVariables(pd3dCommandList);

	RecursiveRender(m_pRootNode, pd3dCommandList, false); // 지형 렌더	 
}


// 그림자 렌더링을 위한 지형 오브젝트만 렌더링합니다.
void QtTerrainInstancingDrawer::RenderInstancingObjectsForShadow(ID3D12GraphicsCommandList * pd3dCommandList)
{
	ShaderManager::GetInstance()->SetPSO(pd3dCommandList, SHADER_INSTACINGSTANDARDFORSHADOW, false);

	// 설명자 힙 설정 
	RecursiveRenderTerrainObjectsForShadow(m_pRootNode, pd3dCommandList, false);
}

void QtTerrainInstancingDrawer::Render(ID3D12GraphicsCommandList * pd3dCommandList, Terrain * pTerrain, ID3D12DescriptorHeap* pHeap, bool isGBuffers)
{
	// 지형 렌더
	ShaderManager::GetInstance()->SetPSO(pd3dCommandList, SHADER_TERRAIN, isGBuffers);

	pd3dCommandList->SetGraphicsRoot32BitConstants(ROOTPARAMETER_WORLD, 16, &Matrix4x4::Identity(), 0); 
	
	pTerrain->UpdateShaderVariables(pd3dCommandList);

	RecursiveRender(m_pRootNode, pd3dCommandList, isGBuffers); // 지형 렌더	 

	// 지형 오브젝트 렌더
	RenderTerrainObjects(pd3dCommandList, isGBuffers); 
}

void QtTerrainInstancingDrawer::Render(int index, ID3D12GraphicsCommandList * pd3dCommandList, bool isGBuffers)
{
	// 렌더링
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
