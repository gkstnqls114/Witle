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
    ����Ʈ���� �ν��Ͻ��� �̿��Ͽ� ������ ���� ������Ʈ�� �������� �ϴ� Ŭ����
*/
class QtTerrainInstancingDrawer
	: public Quadtree<quadtree::QT_DRAWER_NODE, quadtree::QT_DRAWER_ADDER>
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


	// �ν��Ͻ��� ���� �����մϴ�.
	void RenderObjAll(ID3D12GraphicsCommandList* pd3dCommandList, bool isGBuffers);
	void RenderObj(ID3D12GraphicsCommandList* pd3dCommandList, int index, bool isGBuffers);
	void RenderObjForShadow(ID3D12GraphicsCommandList* pd3dCommandList, int index, bool isGBuffers);
	void RenderObjBOBox(ID3D12GraphicsCommandList* pd3dCommandList, int index);

	void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	
public:
	 
	XMFLOAT4X4* GetWorldMatrix(int index, const std::string& name);
	XMFLOAT4X4 GetAltarTransform(int index, const std::string& name);

	// StaticObjectStorage ���� ������ //////////////////////

	GameObject* m_EmptyObj{ nullptr };

public:
	void Render(ID3D12GraphicsCommandList *pd3dCommandList, bool isGBuffers);

	virtual void Init();
	virtual void PrintInfo();

private:
	static int gTreePieceCount;

	virtual void AddDataListOfNode(quadtree::QT_DRAWER_NODE& node, const quadtree::QT_DRAWER_ADDER& world) override;
	virtual void ProcessDataOfNode(quadtree::QT_DRAWER_NODE& node, GameObject& gameobj) override;

	// ������Ʈ�� �ƴ�, ���ӿ�����Ʈ ������ �����Ҵ�� ��������� �����Ѵ�.
	void ReleaseMembers();
	// ������Ʈ�� �ƴ�, ���ӿ�����Ʈ ������ �����Ҵ�� ���ε� ���� �����Ѵ�.
	void ReleaseMemberUploadBuffers();

private:
	UINT m_widthTotal{ 0 };
	UINT m_lengthTotal{ 0 };

	const UINT m_lengthMin{ 256 + 1 }; // ���������� �ȼ� ũ��
	const UINT m_widthMin{ 256 + 1 }; // ���������� �ȼ� ũ��

	XMFLOAT3 m_xmf3Scale{ 0.f, 0.f, 0.f };
	XMFLOAT4 m_xmf4Color{ 1.f, 0.f, 0.f , 1.f};

	quadtree::QT_DRAWER_NODE* m_pRootNode{ nullptr };

	int m_ReafNodeCount = 0;
	quadtree::QT_DRAWER_NODE** m_pReafNodes{ nullptr };
	 
 
private:  
	// �ͷ��� �ʱ� ��ġ ����
	void LoadTerrainObjectFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, const char* pstrFileName, const QtTerrainInstancingDrawer const* pTerrain);
	void LoadNameAndPositionFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, FILE* pInFile, const QtTerrainInstancingDrawer const* pTerrain);
	bool LoadTransform(char* name, const char* comp_name, const XMINT4&, const XMFLOAT4X4& tr);
	// �ͷ��� �ʱ� ��ġ ���� �ε�
	 
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
	

	// �ش� �Լ��� ��������� ȣ���ϸ� �ͷ����� �����ϴ� �Լ��Դϴ�.
	void RecursiveCreateTerrain(quadtree::QT_DRAWER_NODE* node, ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, 
		int xStart, int zStart, int nBlockWidth, int nBlockLength,
		HeightMapImage *pContext = NULL);
	 
public:
	QtTerrainInstancingDrawer(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, 
		const XMFLOAT3& center, const XMFLOAT3& extents, float min_size,
		int nWidth, int nLength, XMFLOAT3 xmf3Scale = XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT4 xmf4Color = XMFLOAT4(1.0f, 1.0f, 0.0f, 0.0f), HeightMapImage *pContext = NULL);
	virtual ~QtTerrainInstancingDrawer();
	  
	quadtree::QT_DRAWER_NODE* const GetRootNode() const { return m_pRootNode; }
	// �ش� �����ǿ� ���ϴ� ��������� ���̵���� �����Ѵ�. ����Ʈ���̹Ƿ� �ִ� 4���� �����Ѵ�.
	XMINT4 const GetIDs(const XMFLOAT3& position) const;
	 
	void RenderTerrainForShadow(ID3D12GraphicsCommandList *pd3dCommandList, Terrain * pTerrain, ID3D12DescriptorHeap* pHeap);
	void RenderInstancingObjectsForShadow(ID3D12GraphicsCommandList *pd3dCommandList);
	void Render(ID3D12GraphicsCommandList *pd3dCommandList, Terrain* pTerrain, ID3D12DescriptorHeap* pHeap, bool isGBuffers);
	void Render(int index, ID3D12GraphicsCommandList *pd3dCommandList, bool isGBuffers);
	static int GetTerrainPieceCount() { return gTreePieceCount; }
	quadtree::QT_DRAWER_NODE* GetReafNode(int index) { return m_pReafNodes[index]; }
	 
	void AddWorldMatrix(const MyBOBox& collider, const std::string& model_name, const XMFLOAT4X4& world);
	 
	// delete ������ �ݵ�� ȣ��
	void ReleaseObjects();
	void ReleaseUploadBuffers();

private:

};