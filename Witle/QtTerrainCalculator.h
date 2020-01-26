#pragma once 
#include "QuadtreeNode.h"
#include "Quadtree.h"

class Player;
class Movement;

/*
   QuadTree �˰����� ����� Map(= Terrain)���� �ִ� �浹ü�� ��ġ(Transform)�� �����ϴ� Ŭ����

   LeafNode(= TreePiece) : �� �̻� ���������� ���� Quadtree Node�� �ǹ���
*/
class QtTerrainCalculator : public Quadtree<quadtree::BASE_NODE>
{   
private:  
	void CreateTerrainObj(const char* terrain_info_path);
	void CreateTerrainObj(FILE* pInFile);

	void ProcessRecursiveCollide(const quadtree::BASE_NODE& node, Movement& movement, const BoundingOrientedBox& nextFrameBoBox, const MyBOBox& BoBox);

	void AddRecursiveCollider(quadtree::BASE_NODE* node, const MyBOBox& collider, const XMFLOAT4X4& world);
	
public:
	QtTerrainCalculator();
	virtual ~QtTerrainCalculator();

	void Init(const XMFLOAT3& center, const XMFLOAT3& extents, float min_size); 
	  
	// �ش� �浹ü�� �浹�ϴ� treePiece�� �浹ü�� �߰��մϴ�.
	void AddCollider(const MyBOBox& BoBox, const XMFLOAT4X4& world);
	 
	// bobox �� �浹üũ�մϴ�.
	void ProcessCollide(Movement& movement, const BoundingOrientedBox& nextFrameBoBox, const MyBOBox& BoBox);

	// QuadtreeMgr�� ������忡 ���� ������ Ȯ���մϴ�. ����׸���� ��쿡�� �ֿܼ� ����մϴ�.
	void PrintInfo();
private:

};