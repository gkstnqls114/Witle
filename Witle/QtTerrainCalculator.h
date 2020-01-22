#pragma once 
#include "QuadtreeNode.h"
#include "Quadtree.h"

class Player;
class Movement;

/*
   QuadTree 알고리즘을 사용해 Map(= Terrain)위에 있는 충돌체의 위치(Transform)를 관리하는 클래스

   LeafNode(= TreePiece) : 더 이상 나누어질수 없는 Quadtree Node를 의미함
*/
class QtTerrainCalculator : public Quadtree<QtTerrainCalculator>
{  
private: 
	quadtree::NODE* m_RootNode{ nullptr }; // 루트 노드

	int m_ReafNodeCount = 0;
	quadtree::NODE** m_pReafNodes{ nullptr }; // 리프 노드를 접근할 동적 배열로 컨테이너에 불과함
	 
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
	  
	// 해당 충돌체와 충돌하는 treePiece에 충돌체를 추가합니다.
	void AddCollider(const MyBOBox& BoBox, const XMFLOAT4X4& world);
	 
	// bobox 와 충돌체크합니다.
	void ProcessCollide(Movement& movement, const BoundingOrientedBox& nextFrameBoBox, const MyBOBox& BoBox);

	// 리프노드의 개수를 반환합니다.
	int GetReafNodeCount() { return m_ReafNodeCount; } 

	const quadtree::NODE& GetRoot() const { return *m_RootNode; }

	// QuadtreeMgr의 리프노드에 대한 정보를 확인합니다. 디버그모드인 경우에만 콘솔에 출력합니다.
	void PrintInfo();
private:

};