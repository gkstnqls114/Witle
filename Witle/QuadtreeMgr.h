#pragma once 
#include "QuadtreeNode.h"
#include "Singleton.h"

class Player;

/*
   QuadTree �˰����� ����� Map(= Terrain)���� �ִ� �浹ü�� ��ġ(Transform)�� �����ϴ� Ŭ����

   LeafNode(= TreePiece) : �� �̻� ���������� ���� Quadtree Node�� �ǹ���
*/
class QuadtreeMgr : public Singleton<QuadtreeMgr>
{  
private: 
	quadtree::NODE* m_RootNode{ nullptr }; // ��Ʈ ���

	int m_ReafNodeCount = 0;
	quadtree::NODE** m_pReafNodes{ nullptr }; // ���� ��带 ������ ���� �迭�� �����̳ʿ� �Ұ���

	float m_minSize{ 0 }; // ���� ���� width or height�� ����. �� �� �ϳ��� minSize��� ���̻� ��带 ������ �ʴ´�.


private:
	void SetminSize(float min_size);

	void CreateQuadTree();
	void CreateRecursiveQuadTree(quadtree::NODE* node, int& leafnodeIndex);
	   
	void ReleaseQuadTree();
	void ReleaseRecursiveQuadTree(quadtree::NODE* node);

	void CreateTerrainObj(const char* terrain_info_path);
	void CreateTerrainObj(FILE* pInFile);

	void ProcessRecursiveCollide(const quadtree::NODE& node, Player& player, const MyBOBox& BoBox, float fElapsedTime);

	void AddRecursiveCollider(quadtree::NODE* node, const MyBOBox& collider, const XMFLOAT4X4& world);
	
public:
	QuadtreeMgr();
	virtual ~QuadtreeMgr();

	void Init(const XMFLOAT3& center, const XMFLOAT3& extents, float min_size);
	void Init(XMFLOAT3&& center, XMFLOAT3&& extents, float min_size);

	virtual void Init(const SingletonInitializer* singletonMgr) override;
	
	void Update(float fElapsedTime) ;
	void LastUpdate(float fElapsedTime);

	// �ش� �浹ü�� �浹�ϴ� treePiece�� �浹ü�� �߰��մϴ�.
	void AddCollider(const MyBOBox& BoBox, const XMFLOAT4X4& world);
	 
	// bobox �� �浹üũ�մϴ�.
	void ProcessCollide(Player& player, const MyBOBox& BoBox, float fElapsedTime);

	// ��������� ������ ��ȯ�մϴ�.
	int GetReafNodeCount() { return m_ReafNodeCount; } 

	const quadtree::NODE& GetRoot() const { return *m_RootNode; }

	// QuadtreeMgr�� ������忡 ���� ������ Ȯ���մϴ�. ����׸���� ��쿡�� �ֿܼ� ����մϴ�.
	void PrintInfo();
private:

};