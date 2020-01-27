#pragma once
#include "QuadtreeNode.h"
#include "GameObject.h"
#include "Quadtree.h"
 
const int MAX_TRIANGLES = 10000;
const int QUAD = 4;

class TerrainMesh;
class HeightMapImage;
class MyFrustum;
class Mesh;
class Terrain;
class MyDescriptorHeap;
class LoadObject;
class Texture;
class GameObject;

/*
    쿼드트리와 인스턴싱을 이용하여 지형과 지형 오브젝트를 렌더링을 하는 클래스
*/
class QtTerrainInstancingDrawer
	: public Quadtree<quadtree::QT_DRAWER_NODE, quadtree::QT_DRAWER_ADDER>
{
	// StaticObjectStorage 에서 가져옴 //////////////////////
	int TerrainPieceCount = 0; // 터레인 제일 작은 조각이 몇개 있는가?
	int TerrainObjectAllCount = 0; //모든 터레인 오브젝트는 몇개가 있는가?

	struct TerrainObjectInfo
	{
		ID3D12Resource* m_pd3dcbGameObjects{ nullptr };         // 인스턴싱을 위해 사용되는 정보
		VS_SRV_INSTANCEINFO* m_pcbMappedGameObjects{ nullptr }; // 인스턴싱을 위해 사용되는 정보
		int         TerrainObjectCount{ 0 };					// 터레인 조각 위에 배치되는 오브젝트의 개수
		std::vector<XMFLOAT4X4> TransformList;                  // 터레인 조각 위에 배치되는 오브젝트의 월드 행렬
	};

	struct RenderInfo // 그림을 그리기 위해 필요한 모델/텍스쳐정보
	{
		LoadObject* pLoadObject{ nullptr };
		Texture* pTexture{ nullptr };
	};

	std::map<std::string, RenderInfo> m_StaticObjectModelsStorage; // 모델 이름은 반드시 클래스에 맞춘다.
	std::map<std::string, TerrainObjectInfo*> m_StaticObjectStorage; // 모델 이름은 반드시 클래스에 맞춘다.
	std::vector<XMFLOAT4X4> m_AltarTransformStorage; // Altar transform 위치 저장하는 곳


	// 인스턴싱을 통해 렌더합니다.
	void RenderObj(ID3D12GraphicsCommandList* pd3dCommandList, int index, bool isGBuffers);
	void RenderObjForShadow(ID3D12GraphicsCommandList* pd3dCommandList, int index, bool isGBuffers);
	void RenderObjBOBox(ID3D12GraphicsCommandList* pd3dCommandList, int index);

	void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	
public:
	 
	XMFLOAT4X4* GetWorldMatrix(int index, const std::string& name);
	XMFLOAT4X4 GetAltarTransform(int index, const std::string& name);

	// StaticObjectStorage 에서 가져옴 //////////////////////

	GameObject* m_EmptyObj{ nullptr };

public:
	void Render(ID3D12GraphicsCommandList *pd3dCommandList, bool isGBuffers);

	virtual void Init();
	virtual void PrintInfo();

private:
	static int gTreePieceCount;

	virtual void AddDataListOfNode(quadtree::QT_DRAWER_NODE& node, const quadtree::QT_DRAWER_ADDER& world) override;
	virtual void ProcessDataOfNode(quadtree::QT_DRAWER_NODE& node, GameObject& gameobj) override;

	// 컴포넌트가 아닌, 게임오브젝트 내에서 동적할당된 멤버변수를 해제한다.
	void ReleaseMembers();
	// 컴포넌트가 아닌, 게임오브젝트 내에서 동적할당된 업로드 힙을 해제한다.
	void ReleaseMemberUploadBuffers();

private:
	UINT m_widthTotal{ 0 };
	UINT m_lengthTotal{ 0 };

	const UINT m_lengthMin{ 256 + 1 }; // 나누어지는 픽셀 크기
	const UINT m_widthMin{ 256 + 1 }; // 나누어지는 픽셀 크기

	XMFLOAT3 m_xmf3Scale{ 0.f, 0.f, 0.f };
	XMFLOAT4 m_xmf4Color{ 1.f, 0.f, 0.f , 1.f};

	quadtree::QT_DRAWER_NODE* m_pRootNode{ nullptr };

	int m_ReafNodeCount = 0;
	quadtree::QT_DRAWER_NODE** m_pReafNodes{ nullptr };
	 
 
private:  
	// 터레인 초기 위치 정보
	void LoadTerrainObjectFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, const char* pstrFileName, const QtTerrainInstancingDrawer const* pTerrain);
	void LoadNameAndPositionFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, FILE* pInFile, const QtTerrainInstancingDrawer const* pTerrain);
	bool LoadTransform(char* name, const char* comp_name, const XMINT4&, const XMFLOAT4X4& tr);
	// 터레인 초기 위치 정보 로드
	 
	void RecursiveRenderTerrainObjects_BOBox(const quadtree::QT_DRAWER_NODE* node, ID3D12GraphicsCommandList *pd3dCommandList);
	 
	void RenderTerrainObjects(ID3D12GraphicsCommandList *pd3dCommandList, bool isGBuffers);
	void RecursiveRenderTerrainObjects(const quadtree::QT_DRAWER_NODE* node, ID3D12GraphicsCommandList *pd3dCommandList, bool isGBuffers);
	void RecursiveRenderTerrainObjectsForShadow(const quadtree::QT_DRAWER_NODE* node, ID3D12GraphicsCommandList *pd3dCommandList, bool isGBuffers);
	void RecursiveRender(const quadtree::QT_DRAWER_NODE* node, ID3D12GraphicsCommandList *pd3dCommandList, bool isGBuffers);
	void RecursiveInitReafNodes(quadtree::QT_DRAWER_NODE* node);
	void RecursiveReleaseUploadBuffers(quadtree::QT_DRAWER_NODE* node);
	void RecursiveReleaseObjects(quadtree::QT_DRAWER_NODE* node);
	void RecursiveCalculateIDs(quadtree::QT_DRAWER_NODE* node, const XMFLOAT3 position, int* pIDs) const;
	void CalculateIDs(const XMFLOAT3 position, XMINT4& pIDs) const;
	

	// 해당 함수를 재귀적으로 호출하며 터레인을 생성하는 함수입니다.
	void RecursiveCreateTerrain(quadtree::QT_DRAWER_NODE* node, ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, 
		int xStart, int zStart, int nBlockWidth, int nBlockLength,
		HeightMapImage *pContext = NULL);
	 
public:
	QtTerrainInstancingDrawer(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, 
		const XMFLOAT3& center, const XMFLOAT3& extents, float min_size,
		int nWidth, int nLength, XMFLOAT3 xmf3Scale = XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT4 xmf4Color = XMFLOAT4(1.0f, 1.0f, 0.0f, 0.0f), HeightMapImage *pContext = NULL);
	virtual ~QtTerrainInstancingDrawer();
	  
	quadtree::QT_DRAWER_NODE* const GetRootNode() const { return m_pRootNode; }
	// 해당 포지션에 속하는 리프노드의 아이디들을 리턴한다. 쿼드트리이므로 최대 4개가 존재한다.
	XMINT4 const GetIDs(const XMFLOAT3& position) const;

	void RenderObjAll(ID3D12GraphicsCommandList* pd3dCommandList, bool isGBuffers);

	void RenderTerrainForShadow(ID3D12GraphicsCommandList *pd3dCommandList, Terrain * pTerrain, ID3D12DescriptorHeap* pHeap);
	void RenderInstancingObjectsForShadow(ID3D12GraphicsCommandList *pd3dCommandList);
	void Render(ID3D12GraphicsCommandList *pd3dCommandList, Terrain* pTerrain, ID3D12DescriptorHeap* pHeap, bool isGBuffers);
	void Render(int index, ID3D12GraphicsCommandList *pd3dCommandList, bool isGBuffers);
	static int GetTerrainPieceCount() { return gTreePieceCount; }
	quadtree::QT_DRAWER_NODE* GetReafNode(int index) { return m_pReafNodes[index]; }
	 
	void AddWorldMatrix(const MyBOBox& collider, const std::string& model_name, const XMFLOAT4X4& world);
	 
	// delete 이전에 반드시 호출
	void ReleaseObjects();
	void ReleaseUploadBuffers();

private:

};