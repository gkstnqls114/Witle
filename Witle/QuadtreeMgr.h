#pragma once 
#include "Singleton.h"

const int MAX_TRIANGLES = 10000;
const int QUAD = 4;

class MyBOBox;
class MyBBox;
class Model;
 
/*
   QuadTree 알고리즘을 사용해 Map(= Terrain)위에 있는 충돌체의 위치(Transform)를 관리하는 클래스

   LeafNode(= TreePiece) : 더 이상 나누어질수 없는 Quadtree Node를 의미함
*/
class QuadtreeMgr : public Singleton<QuadtreeMgr>
{  
private: 
	struct TerrainObj // Terrain 위에 존재하는 지형 오브젝트
	{
		const Model* const pModel{ nullptr }; // ModelStg에서 갖고올 모델 포인터
		const MyBOBox* const BoBox{ nullptr };      // ModelStg에서 갖고올 모델의 충돌박스
		XMFLOAT4X4 world;				// 월드행렬
	};
	 
	struct NODE
	{
		MyBOBox* BoBox{ nullptr };  // 해당 노드(맵)의 충돌체 
		NODE* children[QUAD]{ nullptr,  nullptr , nullptr , nullptr }; // 자식 노드들
		
		int terrainObjCount = -1; // 만약 leafnode일 경우 해당 노드에 존재하는 지형 오브젝트의 충돌체 개수
		
		// 만약 leafnode일 경우 해당 노드에 존재하는 지형 오브젝트의 충돌체들 (포인터)
		std::list<TerrainObj> terrainObjBoBoxs;

	private: 
		NODE(NODE const&) = delete;            // 복사 숨김
		NODE& operator=(NODE const&) = delete; // 할당 숨김

	public: 
		NODE(XMFLOAT3&& center, XMFLOAT3&& extents); 
		NODE(const XMFLOAT3& center, const XMFLOAT3& extents);
		~NODE();
	}; 

	NODE* m_RootNode{ nullptr }; // 루트 노드

	int m_ReafNodeCount = 0;
	NODE** m_pReafNodes{ nullptr }; // 리프 노드를 접근할 동적 배열로 컨테이너에 불과함

	float m_minSize{ 0 }; // 제일 작은 width or height의 조건. 둘 중 하나가 minSize라면 더이상 노드를 만들지 않는다.


private:
	void SetminSize(float min_size);

	void CreateQuadTree();
	void CreateRecursiveQuadTree(NODE* node, int& leafnodeIndex);
	   
	void ReleaseQuadTree();
	void ReleaseRecursiveQuadTree(NODE* node);

	void AddRecursiveCollider(NODE* node, const Model* const pModel, const MyBOBox* const collider, const XMFLOAT4X4& world);
	
public:
	QuadtreeMgr();
	virtual ~QuadtreeMgr();

	void Init(const XMFLOAT3& center, const XMFLOAT3& extents, float min_size);
	void Init(XMFLOAT3&& center, XMFLOAT3&& extents, float min_size);

	virtual void Init(const SingletonInitializer* singletonMgr) override;
	
	void Update(float fElapsedTime) ;
	void LastUpdate(float fElapsedTime);

	// 해당 충돌체와 충돌하는 treePiece에 충돌체를 추가합니다.
	void AddCollider(const Model* const pModel, const MyBOBox* const collider, const XMFLOAT4X4& world);
	void AddCollider(const Model* const pModel, const MyBOBox* const collider, XMFLOAT4X4&& world);

	// 리프노드의 개수를 반환합니다.
	int GetReafNodeCount() { return m_ReafNodeCount; } 

	// QuadtreeMgr의 리프노드에 대한 정보를 확인합니다. 디버그모드인 경우에만 콘솔에 출력합니다.
	void PrintInfo();
private:

};