#pragma once 
#include "QuadtreeNode.h"
#include "Quadtree.h"

class Player;
class Movement;

/*
   QuadTree �˰����� ����� Map(= Terrain)���� �ִ� �浹ü�� ��ġ(Transform)�� �����ϴ� Ŭ����

   LeafNode(= TreePiece) : �� �̻� ���������� ���� Quadtree Node�� �ǹ���
*/
class QtTerrainCalculator : public Quadtree<QtTerrainCalculator>
{  
private: 
	quadtree::NODE* m_RootNode{ nullptr }; // ��Ʈ ���

	int m_ReafNodeCount = 0;
	quadtree::NODE** m_pReafNodes{ nullptr }; // ���� ��带 ������ ���� �迭�� �����̳ʿ� �Ұ���
	 
private: 

	void CreateQuadTree();
	void CreateRecursiveQuadTree(quadtree::NODE* node, int& leafnodeIndex);
	   
	void ReleaseQuadTree();
	void ReleaseRecursiveQuadTree(quadtree::NODE* node);

	void CreateTerrainObj(const char* terrain_info_path);
	void CreateTerrainObj(FILE* pInFile);

	void ProcessRecursiveCollide(const quadtree::NODE& node, Movement& movement, const BoundingOrientedBox& nextFrameBoBox, const MyBOBox& BoBox);

	void AddRecursiveCollider(quadtree::NODE* node, const MyBOBox& collider, const XMFLOAT4X4& world);
	
public:
	QtTerrainCalculator();
	virtual ~QtTerrainCalculator();

	void Init(const XMFLOAT3& center, const XMFLOAT3& extents, float min_size); 
	  
	// �ش� �浹ü�� �浹�ϴ� treePiece�� �浹ü�� �߰��մϴ�.
	void AddCollider(const MyBOBox& BoBox, const XMFLOAT4X4& world);
	 
	// bobox �� �浹üũ�մϴ�.
	void ProcessCollide(Movement& movement, const BoundingOrientedBox& nextFrameBoBox, const MyBOBox& BoBox);

	// ��������� ������ ��ȯ�մϴ�.
	int GetReafNodeCount() { return m_ReafNodeCount; } 

	const quadtree::NODE& GetRoot() const { return *m_RootNode; }

	// QuadtreeMgr�� ������忡 ���� ������ Ȯ���մϴ�. ����׸���� ��쿡�� �ֿܼ� ����մϴ�.
	void PrintInfo();
private:

};