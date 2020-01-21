#pragma once
#include "QuadtreeNode.h"
#include "GameObject.h"
 
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

/*
    ����Ʈ���� �ν��Ͻ��� �̿��Ͽ� ������ ���� ������Ʈ�� �������� �ϴ� Ŭ����
*/
class QtTerrainInstancingDrawer
	: public GameObject
{
	// StaticObjectStorage ���� ������ //////////////////////
	int TerrainPieceCount = 0; // �ͷ��� ���� ���� ������ � �ִ°�?
	int TerrainObjectAllCount = 0; //��� �ͷ��� ������Ʈ�� ��� �ִ°�?

	struct TerrainObjectInfo
	{
		ID3D12Resource* m_pd3dcbGameObjects{ nullptr };         // �ν��Ͻ��� ���� ���Ǵ� ����
		VS_SRV_INSTANCEINFO* m_pcbMappedGameObjects{ nullptr }; // �ν��Ͻ��� ���� ���Ǵ� ����
		int         TerrainObjectCount{ 0 };					// �ͷ��� ���� ���� ��ġ�Ǵ� ������Ʈ�� ����
		std::vector<XMFLOAT4X4> TransformList;                  // �ͷ��� ���� ���� ��ġ�Ǵ� ������Ʈ�� ���� ���
	};

	struct RenderInfo // �׸��� �׸��� ���� �ʿ��� ��/�ؽ�������
	{
		LoadObject* pLoadObject{ nullptr };
		Texture* pTexture{ nullptr };
	};

	std::map<std::string, RenderInfo> m_StaticObjectModelsStorage; // �� �̸��� �ݵ�� Ŭ������ �����.
	std::map<std::string, TerrainObjectInfo*> m_StaticObjectStorage; // �� �̸��� �ݵ�� Ŭ������ �����.
	std::vector<XMFLOAT4X4> m_AltarTransformStorage; // Altar transform ��ġ �����ϴ� ��
	// StaticObjectStorage ���� ������ //////////////////////

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

	quadtree::QUAD_TREE_NODE* m_pRootNode{ nullptr };

	int m_ReafNodeCount = 0;
	quadtree::QUAD_TREE_NODE** m_pReafNodes{ nullptr };
	 
 
private:  
	// �ͷ��� �ʱ� ��ġ ����
	void LoadTerrainObjectFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, const char* pstrFileName, const QtTerrainInstancingDrawer const* pTerrain);
	void LoadNameAndPositionFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, FILE* pInFile, const QtTerrainInstancingDrawer const* pTerrain);
	bool LoadTransform(char* name, const char* comp_name, const XMINT4&, const XMFLOAT4X4& tr);
	// �ͷ��� �ʱ� ��ġ ���� �ε�
	 
	void RecursiveRenderTerrainObjects_BOBox(const quadtree::QUAD_TREE_NODE* node, ID3D12GraphicsCommandList *pd3dCommandList);
	 
	void RenderTerrainObjects(ID3D12GraphicsCommandList *pd3dCommandList, bool isGBuffers);
	void RecursiveRenderTerrainObjects(const quadtree::QUAD_TREE_NODE* node, ID3D12GraphicsCommandList *pd3dCommandList, bool isGBuffers);
	void RecursiveRenderTerrainObjectsForShadow(const quadtree::QUAD_TREE_NODE* node, ID3D12GraphicsCommandList *pd3dCommandList, bool isGBuffers);
	void RecursiveRender(const quadtree::QUAD_TREE_NODE* node, ID3D12GraphicsCommandList *pd3dCommandList, bool isGBuffers);
	void RecursiveInitReafNodes(quadtree::QUAD_TREE_NODE* node);
	void RecursiveReleaseUploadBuffers(quadtree::QUAD_TREE_NODE* node);
	void RecursiveReleaseObjects(quadtree::QUAD_TREE_NODE* node);
	void RecursiveCalculateIDs(quadtree::QUAD_TREE_NODE* node, const XMFLOAT3 position, int* pIDs) const;
	void CalculateIDs(const XMFLOAT3 position, XMINT4& pIDs) const;
	void CalculateIndex(const XMFLOAT3 position, int* pIDs) const;

	// �ش� �Լ��� ��������� ȣ���ϸ� �ͷ����� �����ϴ� �Լ��Դϴ�.
	void RecursiveCreateTerrain(quadtree::QUAD_TREE_NODE* node, ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, 
		int xStart, int zStart, int nBlockWidth, int nBlockLength,
		HeightMapImage *pContext = NULL);
	 
public:
	QtTerrainInstancingDrawer(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, int nWidth, int nLength, XMFLOAT3 xmf3Scale = XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT4 xmf4Color = XMFLOAT4(1.0f, 1.0f, 0.0f, 0.0f), HeightMapImage *pContext = NULL);
	virtual ~QtTerrainInstancingDrawer();
	 
	virtual void Update(float fElapsedTime) override;
	void LastUpdate(float fElapsedTime);

	quadtree::QUAD_TREE_NODE* const GetRootNode() const { return m_pRootNode; }
	// �ش� �����ǿ� ���ϴ� ��������� ���̵���� �����Ѵ�. ����Ʈ���̹Ƿ� �ִ� 4���� �����Ѵ�.
	XMINT4 const GetIDs(const XMFLOAT3& position) const;
	int * const GetIndex(const XMFLOAT3& position) const;
	
	void RenderTerrainForShadow(ID3D12GraphicsCommandList *pd3dCommandList, Terrain * pTerrain, ID3D12DescriptorHeap* pHeap);
	void RenderInstancingObjectsForShadow(ID3D12GraphicsCommandList *pd3dCommandList);
	void Render(ID3D12GraphicsCommandList *pd3dCommandList, Terrain* pTerrain, ID3D12DescriptorHeap* pHeap, bool isGBuffers);
	void Render(int index, ID3D12GraphicsCommandList *pd3dCommandList, bool isGBuffers);
	static int GetTerrainPieceCount() { return gTreePieceCount; }
	quadtree::QUAD_TREE_NODE* GetReafNode(int index) { return m_pReafNodes[index]; }
	quadtree::QUAD_TREE_NODE* GetReafNodeByID(int id);
private:

};