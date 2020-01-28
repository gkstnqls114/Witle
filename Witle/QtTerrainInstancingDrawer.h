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
	std::vector<XMFLOAT4X4> m_AltarTransformStorage; // Altar transform ��ġ �����ϴ� ��
	 
	// �ν��Ͻ��� ���� �����մϴ�.
	void RenderObj(ID3D12GraphicsCommandList* pd3dCommandList, const quadtree::QT_DRAWER_NODE& node, bool isGBuffers);
	void RenderObjForShadow(ID3D12GraphicsCommandList* pd3dCommandList, const quadtree::QT_DRAWER_NODE& node, bool isGBuffers);
	void RenderObjBOBox(ID3D12GraphicsCommandList* pd3dCommandList, const quadtree::QT_DRAWER_NODE& node);

	void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	
public:
	  
	XMFLOAT4X4 GetAltarTransform(int index, const std::string& name);

	// StaticObjectStorage ���� ������ //////////////////////

	GameObject* m_EmptyObj{ nullptr };

public:
	void Render(ID3D12GraphicsCommandList *pd3dCommandList, bool isGBuffers);

	virtual void Init();
	virtual void PrintInfo();

private: 

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
	const UINT m_widthMin{ 256+ 1 }; // ���������� �ȼ� ũ��

	XMFLOAT3 m_xmf3Scale{ 0.f, 0.f, 0.f };
	XMFLOAT4 m_xmf4Color{ 1.f, 0.f, 0.f , 1.f};

	quadtree::QT_DRAWER_NODE* m_pRootNode{ nullptr };
	 
private:  
	// �ͷ��� �ʱ� ��ġ ����
	void LoadTerrainObjectFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, const char* pstrFileName, const QtTerrainInstancingDrawer const* pTerrain);
	void LoadNameAndPositionFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, FILE* pInFile, const QtTerrainInstancingDrawer const* pTerrain);
	bool LoadTransform(char* name, const char* comp_name, const XMFLOAT4X4& tr);
	// �ͷ��� �ʱ� ��ġ ���� �ε�
	 
	void RecursiveRenderTerrainObjects_BOBox(const quadtree::QT_DRAWER_NODE* node, ID3D12GraphicsCommandList *pd3dCommandList);
	 
	void RenderTerrainObjects(ID3D12GraphicsCommandList *pd3dCommandList, bool isGBuffers);
	void RecursiveRenderTerrainObjects(const quadtree::QT_DRAWER_NODE* node, ID3D12GraphicsCommandList *pd3dCommandList, bool isGBuffers);
	void RecursiveRenderTerrainObjectsForShadow(const quadtree::QT_DRAWER_NODE* node, ID3D12GraphicsCommandList *pd3dCommandList, bool isGBuffers);
	void RecursiveRender(const quadtree::QT_DRAWER_NODE* node, ID3D12GraphicsCommandList *pd3dCommandList, bool isGBuffers);
	void RecursiveReleaseUploadBuffers(quadtree::QT_DRAWER_NODE* node);
	void RecursiveReleaseObjects(quadtree::QT_DRAWER_NODE* node);
	  
	// �ش� �Լ��� ��������� ȣ���ϸ� �ͷ����� �����ϴ� �Լ��Դϴ�.
	void RecursiveCreateTerrain(quadtree::QT_DRAWER_NODE* node, ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, 
		int xStart, int zStart, int nBlockWidth, int nBlockLength,
		HeightMapImage *pContext = NULL);
	 
	void CreateReafNode(quadtree::QT_DRAWER_NODE* node, ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList,
		 HeightMapImage* pContext = NULL);

public:
	QtTerrainInstancingDrawer(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, 
		const XMFLOAT3& center, const XMFLOAT3& extents, float min_size,
		int nWidth, int nLength, XMFLOAT3 xmf3Scale = XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT4 xmf4Color = XMFLOAT4(1.0f, 1.0f, 0.0f, 0.0f), HeightMapImage *pContext = NULL);
	virtual ~QtTerrainInstancingDrawer();
	  
	quadtree::QT_DRAWER_NODE* const GetRootNode() const { return m_pRootNode; }
	
	void RenderObjAll(ID3D12GraphicsCommandList* pd3dCommandList, Terrain* pTerrain, bool isGBuffers);

	void RenderTerrainForShadow(ID3D12GraphicsCommandList *pd3dCommandList, Terrain * pTerrain, ID3D12DescriptorHeap* pHeap);
	void RenderInstancingObjectsForShadow(ID3D12GraphicsCommandList *pd3dCommandList);
	void Render(ID3D12GraphicsCommandList *pd3dCommandList, Terrain* pTerrain, ID3D12DescriptorHeap* pHeap, bool isGBuffers);
	
	void AddWorldMatrix(const MyBOBox& collider, const std::string& model_name, const XMFLOAT4X4& world);
	 
	// delete ������ �ݵ�� ȣ��
	void ReleaseObjects();
	void ReleaseUploadBuffers();

private:

};