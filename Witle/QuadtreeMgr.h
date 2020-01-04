#pragma once
#include "Singleton.h"

const int MAX_TRIANGLES = 10000;
const int QUAD = 4;

class MyBOBox;
class MyBBox;

struct INFO
{
	float centerX; // �޽��� �߽� ��ġ
	float centerZ; // �޽��� �߽� ��ġ
	float meshWidth; //�Ž��� �ִ� ����
};


/*
   QuadTree �˰����� ����� Map(= Terrain)���� �ִ� �浹ü�� ��ġ(Transform)�� �����ϴ� Ŭ����

   LeafNode(= TreePiece) : �� �̻� ���������� ���� Quadtree Node�� �ǹ���
*/
class QuadtreeMgr : public Singleton<QuadtreeMgr>
{  
private: 
	struct NODE
	{
		MyBOBox* BoBox{ nullptr };  // �ش� ����� �浹ü 
		NODE* children[QUAD]{ nullptr,  nullptr , nullptr , nullptr }; // �ڽ� ����
		
		int terrainObjCount = -1; // ���� leafnode�� ��� �ش� ��忡 �����ϴ� ���� ������Ʈ�� �浹ü ����
		
		// ���� leafnode�� ��� �ش� ��忡 �����ϴ� ���� ������Ʈ�� �浹ü�� (������)
		std::list<MyBOBox*> terrainObjBoBoxs; 

	private: 
		NODE(NODE const&) = delete;            // ���� ����
		NODE& operator=(NODE const&) = delete; // �Ҵ� ����

	public: 
		NODE(XMFLOAT3&& center, XMFLOAT3&& extents); 
		NODE(const XMFLOAT3& center, const XMFLOAT3& extents);
		~NODE();
	}; 

	NODE* m_pRootNode{ nullptr }; // ��Ʈ ���

	int m_ReafNodeCount = 0;
	NODE** m_pReafNodes{ nullptr }; // ���� ��带 ������ ���� �迭�� �����̳ʿ� �Ұ���

	float m_minSize{ 0 }; // ���� ���� width or height�� ����. �� �� �ϳ��� minSize��� ���̻� ��带 ������ �ʴ´�.


private:
	void SetminSize(float min_size);

	void CreateQuadTree();
	void CreateRecursiveQuadTree(NODE* node, int& leafnodeIndex);
	   
	void ReleaseQuadTree();
	void ReleaseRecursiveQuadTree(NODE* node);
	  
public:
	QuadtreeMgr();
	virtual ~QuadtreeMgr();

	void Init(const XMFLOAT3& center, const XMFLOAT3& extents, float min_size);
	void Init(XMFLOAT3&& center, XMFLOAT3&& extents, float min_size);

	virtual void Init(const SingletonInitializer* singletonMgr) override;

	void Update(float fElapsedTime) ;
	void LastUpdate(float fElapsedTime);
	  
	// ��������� ������ ��ȯ�մϴ�.
	int GetReafNodeCount() { return m_ReafNodeCount; } 

	// QuadtreeMgr�� ������忡 ���� ������ Ȯ���մϴ�. ����׸���� ��쿡�� �ֿܼ� ����մϴ�.
	void PrintInfo();
private:

};