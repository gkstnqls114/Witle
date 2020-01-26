#pragma once 
#include "QuadtreeNode.h"
#include "Quadtree.h"

class Player;
class Movement;

/*
   QuadTree �˰����� ����� Map(= Terrain)���� �ִ� �浹ü�� ��ġ(Transform)�� �����ϴ� Ŭ����

   LeafNode(= TreePiece) : �� �̻� ���������� ���� Quadtree Node�� �ǹ���
*/
class QtTerrainCalculator : public Quadtree<quadtree::COLLISION_NODE, quadtree::COLLIDER>
{   
private:  
	void CreateTerrainObj(const char* terrain_info_path);
	void CreateTerrainObj(FILE* pInFile);
	 
	virtual void AddDataListOfNode(quadtree::COLLISION_NODE& node, const quadtree::COLLIDER& collider) override;
	virtual void ProcessDataOfNode(quadtree::COLLISION_NODE& node, GameObject& gameobj) override;

public:
	QtTerrainCalculator(const XMFLOAT3& center, const XMFLOAT3& extents, float min_size);
	virtual ~QtTerrainCalculator();

	virtual void Init() override;
	  
	// �ش� �浹ü�� �浹�ϴ� treePiece�� �浹ü�� �߰��մϴ�.
	void AddCollider(const MyBOBox& BoBox, const XMFLOAT4X4& world);
	 
	// �ش� �浹ü�� �ش��ϴ� mybobox�� �浹üũ�� Ȯ���Ͽ� ���ӵ��� �����մϴ�. 
	void ProcessCollide(const MyBOBox& collider, GameObject& gameobj);

	// QuadtreeMgr�� ������忡 ���� ������ Ȯ���մϴ�. ����׸���� ��쿡�� �ֿܼ� ����մϴ�.
	virtual void PrintInfo() override;
private:

};