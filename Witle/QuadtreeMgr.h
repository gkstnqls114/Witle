#pragma once
#include "Singleton.h"

const int MAX_TRIANGLES = 10000;
const int QUAD = 4;

class TerrainMesh;
class HeightMapImage;
class MyFrustum;
class Mesh;
class Terrain;
class MyDescriptorHeap;

struct INFO
{
	float centerX; // 메쉬의 중심 위치
	float centerZ; // 메쉬의 중심 위치
	float meshWidth; //매쉬의 최대 직경
};

struct QUAD_TREE_NODE
{
	BoundingBox boundingBox; // 해당 터레인에 속하는가 확인을 해주는 바운딩박스
	bool isRendering{ false }; // 렌더링 할 것인가, 말 것인가. 
	int id{ -1 }; // 터레인 아이디 넘버
	Mesh* terrainMesh{ nullptr }; // 렌더할 터레인 메쉬
	QUAD_TREE_NODE* children[QUAD]{ nullptr,  nullptr , nullptr , nullptr };
};

/*
   QuadTree 알고리즘을 사용해 Map(= Terrain)위에 있는 충돌체의 위치(Transform)를 관리하는 클래스
*/
class QuadtreeMgr : Singleton<QuadtreeMgr>
{ 
private:
	static int gTreePieceCount; 

private:
	UINT m_widthTotal{ 0 };
	UINT m_lengthTotal{ 0 };

	const UINT m_lengthMin{ 256 + 1 }; // 나누어지는 픽셀 크기
	const UINT m_widthMin{ 256 + 1 }; // 나누어지는 픽셀 크기

	XMFLOAT3 m_xmf3Scale{ 0.f, 0.f, 0.f };
	XMFLOAT4 m_xmf4Color{ 1.f, 0.f, 0.f , 1.f };

	QUAD_TREE_NODE* m_pRootNode{ nullptr };

	int m_ReafNodeCount = 0;
	QUAD_TREE_NODE** m_pReafNodes{ nullptr };


private:
	 
	void RecursiveInitReafNodes(QUAD_TREE_NODE* node);
	void RecursiveReleaseUploadBuffers(QUAD_TREE_NODE* node);
	void RecursiveReleaseObjects(QUAD_TREE_NODE* node);
	void RecursiveCalculateIDs(QUAD_TREE_NODE* node, const XMFLOAT3 position, int* pIDs) const;
	void CalculateIDs(const XMFLOAT3 position, XMINT4& pIDs) const;
	void CalculateIndex(const XMFLOAT3 position, int* pIDs) const;

	// 해당 함수를 재귀적으로 호출하며 터레인을 생성하는 함수입니다.
	void RecursiveCreateTerrain(QUAD_TREE_NODE* node, ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList,
		int xStart, int zStart, int nBlockWidth, int nBlockLength,
		HeightMapImage* pContext = NULL);

	UINT CalculateVertex(UINT widht, UINT length);
	UINT CalculateTriangles(UINT widthPixel, UINT lengthPixel);

public:
	QuadtreeMgr(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, int nWidth, int nLength, XMFLOAT3 xmf3Scale = XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT4 xmf4Color = XMFLOAT4(1.0f, 1.0f, 0.0f, 0.0f), HeightMapImage* pContext = NULL);
	virtual ~QuadtreeMgr();

	void Update(float fElapsedTime) ;
	void LastUpdate(float fElapsedTime);

	QUAD_TREE_NODE* const GetRootNode() const { return m_pRootNode; }
	// 해당 포지션에 속하는 리프노드의 아이디들을 리턴한다. 쿼드트리이므로 최대 4개가 존재한다.
	XMINT4 const GetIDs(const XMFLOAT3& position) const;
	int* const GetIndex(const XMFLOAT3& position) const;

	void RenderTerrainForShadow(ID3D12GraphicsCommandList* pd3dCommandList, Terrain* pTerrain, ID3D12DescriptorHeap* pHeap);
	void RenderInstancingObjectsForShadow(ID3D12GraphicsCommandList* pd3dCommandList);
	void Render(ID3D12GraphicsCommandList* pd3dCommandList, Terrain* pTerrain, ID3D12DescriptorHeap* pHeap, bool isGBuffers);
	void Render(int index, ID3D12GraphicsCommandList* pd3dCommandList, bool isGBuffers);
	static int GetTerrainPieceCount() { return gTreePieceCount; }
	QUAD_TREE_NODE* GetReafNode(int index) { return m_pReafNodes[index]; }
	QUAD_TREE_NODE* GetReafNodeByID(int id);
private:

};