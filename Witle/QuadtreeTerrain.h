#pragma once
#include "GameObject.h"
 
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
	float centerX; // �޽��� �߽� ��ġ
	float centerZ; // �޽��� �߽� ��ġ
	float meshWidth; //�Ž��� �ִ� ����
};

struct QUAD_TREE_NODE
{
	BoundingBox boundingBox; // �ش� �ͷ��ο� ���ϴ°� Ȯ���� ���ִ� �ٿ���ڽ�
	bool isRendering{ false }; // ������ �� ���ΰ�, �� ���ΰ�. 
	int id{ -1 }; // �ͷ��� ���̵� �ѹ�
	Mesh* terrainMesh{ nullptr }; // ������ �ͷ��� �޽�
	QUAD_TREE_NODE* children[QUAD]{ nullptr,  nullptr , nullptr , nullptr };
};

class QuadtreeTerrain
	: public GameObject
{
public:
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, bool isGBuffers) override;

private:
	static int gTreePieceCount;
	
	// ������Ʈ�� �ƴ�, ���ӿ�����Ʈ ������ �����Ҵ�� ��������� �����Ѵ�.
	virtual void ReleaseMembers() override;
	// ������Ʈ�� �ƴ�, ���ӿ�����Ʈ ������ �����Ҵ�� ���ε� ���� �����Ѵ�.
	virtual void ReleaseMemberUploadBuffers() override;

private:
	UINT m_widthTotal{ 0 };
	UINT m_lengthTotal{ 0 };

	const UINT m_lengthMin{ 256 + 1 }; // ���������� �ȼ� ũ��
	const UINT m_widthMin{ 256 + 1 }; // ���������� �ȼ� ũ��

	XMFLOAT3 m_xmf3Scale{ 0.f, 0.f, 0.f };
	XMFLOAT4 m_xmf4Color{ 1.f, 0.f, 0.f , 1.f};

	QUAD_TREE_NODE* m_pRootNode{ nullptr };

	int m_ReafNodeCount = 0;
	QUAD_TREE_NODE** m_pReafNodes{ nullptr };

public:
	MyDescriptorHeap* m_AllDescriptorHeap{ nullptr }; // �׸��ڴ� �����̰�, �ؽ��ĵ� ����.

	MyDescriptorHeap* m_pShadowHeap{ nullptr };
 
private: 


	void RecursiveRenderTerrainObjects_BOBox(const QUAD_TREE_NODE* node, ID3D12GraphicsCommandList *pd3dCommandList);
	 
	void RenderTerrainObjects(ID3D12GraphicsCommandList *pd3dCommandList, bool isGBuffers);
	void RecursiveRenderTerrainObjects(const QUAD_TREE_NODE* node, ID3D12GraphicsCommandList *pd3dCommandList, bool isGBuffers);
	void RecursiveRenderTerrainObjectsForShadow(const QUAD_TREE_NODE* node, ID3D12GraphicsCommandList *pd3dCommandList, bool isGBuffers);
	void RecursiveRender(const QUAD_TREE_NODE* node, ID3D12GraphicsCommandList *pd3dCommandList, bool isGBuffers);
	void RecursiveInitReafNodes(QUAD_TREE_NODE* node);
	void RecursiveReleaseUploadBuffers(QUAD_TREE_NODE* node);
	void RecursiveReleaseObjects(QUAD_TREE_NODE* node);
	void RecursiveCalculateIDs(QUAD_TREE_NODE* node, const XMFLOAT3 position, int* pIDs) const;
	void CalculateIDs(const XMFLOAT3 position, XMINT4& pIDs) const;
	void CalculateIndex(const XMFLOAT3 position, int* pIDs) const;

	// �ش� �Լ��� ��������� ȣ���ϸ� �ͷ����� �����ϴ� �Լ��Դϴ�.
	void RecursiveCreateTerrain(QUAD_TREE_NODE* node, ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, 
		int xStart, int zStart, int nBlockWidth, int nBlockLength,
		HeightMapImage *pContext = NULL);

	UINT CalculateVertex(UINT widht, UINT length);
	UINT CalculateTriangles(UINT widthPixel, UINT lengthPixel);

public:
	QuadtreeTerrain(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, int nWidth, int nLength, XMFLOAT3 xmf3Scale = XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT4 xmf4Color = XMFLOAT4(1.0f, 1.0f, 0.0f, 0.0f), HeightMapImage *pContext = NULL);
	virtual ~QuadtreeTerrain();
	 
	virtual void Update(float fElapsedTime) override;
	void LastUpdate(float fElapsedTime);

	QUAD_TREE_NODE* const GetRootNode() const { return m_pRootNode; }
	// �ش� �����ǿ� ���ϴ� ��������� ���̵���� �����Ѵ�. ����Ʈ���̹Ƿ� �ִ� 4���� �����Ѵ�.
	XMINT4 const GetIDs(const XMFLOAT3& position) const;
	int * const GetIndex(const XMFLOAT3& position) const;
	
	void RenderTerrainForShadow(ID3D12GraphicsCommandList *pd3dCommandList, Terrain * pTerrain, ID3D12DescriptorHeap* pHeap);
	void RenderInstancingObjectsForShadow(ID3D12GraphicsCommandList *pd3dCommandList);
	void Render(ID3D12GraphicsCommandList *pd3dCommandList, Terrain* pTerrain, ID3D12DescriptorHeap* pHeap, bool isGBuffers);
	void Render(int index, ID3D12GraphicsCommandList *pd3dCommandList, bool isGBuffers);
	static int GetTerrainPieceCount() { return gTreePieceCount; }
	QUAD_TREE_NODE* GetReafNode(int index) { return m_pReafNodes[index]; }
	QUAD_TREE_NODE* GetReafNodeByID(int id);
private:

};