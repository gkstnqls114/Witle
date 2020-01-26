#pragma once 
#include "QuadtreeNode.h"
#include "Quadtree.h"

class Player;
class Movement;

/*
   QuadTree 알고리즘을 사용해 Map(= Terrain)위에 있는 충돌체의 위치(Transform)를 관리하는 클래스

   LeafNode(= TreePiece) : 더 이상 나누어질수 없는 Quadtree Node를 의미함
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
	  
	// 해당 충돌체와 충돌하는 treePiece에 충돌체를 추가합니다.
	void AddCollider(const MyBOBox& BoBox, const XMFLOAT4X4& world);
	 
	// 해당 충돌체에 해당하는 mybobox와 충돌체크를 확인하여 가속도를 설정합니다. 
	void ProcessCollide(const MyBOBox& collider, GameObject& gameobj);

	// QuadtreeMgr의 리프노드에 대한 정보를 확인합니다. 디버그모드인 경우에만 콘솔에 출력합니다.
	virtual void PrintInfo() override;
private:

};