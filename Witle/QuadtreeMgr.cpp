#include "stdafx.h"
#include "d3dUtil.h" 
#include "MyBOBox.h" 
#include "Collision.h"
#include "QuadtreeMgr.h"
 
 
void QuadtreeMgr::SetminSize(float min_size)
{
	// 이미 minSize가 설정된 경우
	bool isAlreadySeted = m_minSize != 0;
	assert(!(isAlreadySeted) && "minSize is already seted");
	if (isAlreadySeted) return;

	// 함수 인자가 0과 작거나 같은 경우
	bool isLessEqualZero = min_size <= 0;
	assert(!(isLessEqualZero) && "min_size is less than or equal 0");
	if (isLessEqualZero) return;

	m_minSize = min_size;
}

void QuadtreeMgr::CreateQuadTree()
{
	// 제일 작은 쿼드트리 사이즈가 0보다 같거나 작습니다.
	bool isLessEqualZero = m_minSize <= 0;
	assert(!(isLessEqualZero) && "minSize is less than or equal 0");
	if (isLessEqualZero) return;

	// m_pReafNodes 의 개수를 계산하여 동적 배열 생성한다.
	int leafnodeX = ceil((m_RootNode->BoBox->GetBOBox().Extents.x * 2.f) / int(m_minSize)); // x 축에서 나눠지는 수
	int leafnodeZ = ceil((m_RootNode->BoBox->GetBOBox().Extents.z * 2.f) / int(m_minSize)); // z 축에서 나눠지는 수

	// 큰 값을 골라 포인터를 담는 동적 배열 생성
	m_ReafNodeCount = (leafnodeX > leafnodeZ) ? leafnodeX : leafnodeZ;
	m_ReafNodeCount = m_ReafNodeCount * m_ReafNodeCount; // 사각형이므로 N^2

	m_pReafNodes = new NODE*[m_ReafNodeCount];

	// 그 후
	int leafnodeIndex = 0;
	CreateRecursiveQuadTree(m_RootNode, leafnodeIndex);
}

void QuadtreeMgr::CreateRecursiveQuadTree(NODE* pNode, int& leafnodeIndex)
{
	// Extents 의 x, z 의 *2 중 하나라도 minSize보다 작거나 같으면 더 이상 노드를 만들지 않는다.
	XMFLOAT3 nodeExents = pNode->BoBox->GetBOBox().Extents;
	XMFLOAT3 nodeCenter = pNode->BoBox->GetBOBox().Center;
	bool isLeafNode = ((nodeExents.x * 2) <= m_minSize) | ((nodeExents.z * 2) <= m_minSize);
	if (isLeafNode)
	{
		assert(!(m_ReafNodeCount <= leafnodeIndex) && "index is greater than leaf node count");
		m_pReafNodes[leafnodeIndex] = pNode;
		leafnodeIndex += 1;
	}
	else
	{
		// y 값은 변화하지 않습니다.
		XMFLOAT3 extents{ nodeExents.x / 2.f, nodeExents.y, nodeExents.z / 2.f };

		// ▲ Z
		// ┌───┬───┐
		// │ 2 │ 3 │
		// ├───┼───┤
		// │ 0 │ 1 │
		// └───┴───┘ ▶ x
		// 위와 같은 형식과 순서로 생성합니다. y는 그대로입니다.
		XMFLOAT3 center[QUAD]
		{
			XMFLOAT3(nodeCenter.x - extents.x, nodeCenter.y, nodeCenter.z - extents.z), // 0
			XMFLOAT3(nodeCenter.x + extents.x, nodeCenter.y, nodeCenter.z - extents.z), // 1
			XMFLOAT3(nodeCenter.x - extents.x, nodeCenter.y, nodeCenter.z + extents.z), // 2
			XMFLOAT3(nodeCenter.x + extents.x, nodeCenter.y, nodeCenter.z + extents.z)  // 3
		};
		 
		for (int x = 0; x < QUAD; ++x)
		{
			pNode->children[x] = new NODE(center[x], extents);
			CreateRecursiveQuadTree(pNode->children[x], leafnodeIndex);
		} 
	}
}

void QuadtreeMgr::ReleaseQuadTree()
{
	if (m_RootNode != nullptr)
	{ 
		ReleaseRecursiveQuadTree(m_RootNode);
		m_RootNode = nullptr;
	}

	if (m_pReafNodes != nullptr)
	{
		delete[] m_pReafNodes;
		m_pReafNodes = nullptr;
	}
}

void QuadtreeMgr::ReleaseRecursiveQuadTree(NODE* pNode)
{
	// 자식이 있는 경우 재귀함수를 이용하여 children 내부로 들어간다.
	bool isHaveChildren = pNode->children[0] != nullptr;
	if (isHaveChildren)
	{
		for (int x = 0; x < QUAD; ++x)
		{
			if (pNode->children[x])
			{
				ReleaseRecursiveQuadTree(pNode->children[x]); 
				pNode->children[x] = nullptr;
			}
		}
	}

	// 자식이 없거나 위 과정이 끝나면 해당 노드를 delete한다.
	delete pNode;   
}

void QuadtreeMgr::AddRecursiveCollider(NODE* pNode, const MyBOBox& collider)
{
	// 해당 노드의 충돌체와 부딪히지않으면 넘어간다.
	bool isCollided = Collision::isCollide(*(pNode->BoBox), collider);
	if (!isCollided) return;

	bool isHaveChildren = pNode->children[0] != nullptr;
	if (isHaveChildren)
	{
		// 자식이 있는 경우 재귀함수를 이용하여 children 내부로 들어간다.
		for (int x = 0; x < QUAD; ++x)
		{
			if (pNode->children[x])
			{
				AddRecursiveCollider(pNode->children[x], collider); 
			}
		}
	}
	else
	{
		// 자식이 없으므로 리프노드라는 뜻이다.
		// 충돌했으므로 노드 리스트에 추가한다.
		pNode->terrainObjCount += 1;
		pNode->terrainObjBoBoxs.push_back(&collider);
	} 
}
    
  
QuadtreeMgr::QuadtreeMgr()  
{
}

QuadtreeMgr::~QuadtreeMgr()
{
	if (m_RootNode)
	{
		ReleaseQuadTree();
	}
}

void QuadtreeMgr::Init(const XMFLOAT3& center, const XMFLOAT3& extents, float min_size)
{  
	SetminSize(min_size);

	if (m_RootNode == nullptr)
	{
		// 쿼드 트리의 부모 노드를 만듭니다.
		m_RootNode = new NODE(center, extents);
	}

	CreateQuadTree();
}

void QuadtreeMgr::Init(XMFLOAT3&& center, XMFLOAT3&& extents, float min_size)
{
	Init(center, extents, min_size);
}

void QuadtreeMgr::Init(const SingletonInitializer* singletonMgr)
{
}
 
void QuadtreeMgr::Update(float fElapsedTime)
{
}

void QuadtreeMgr::LastUpdate(float fElapsedTime)
{ 
}

void QuadtreeMgr::AddCollider(const MyBOBox& collider)
{
	AddRecursiveCollider(m_RootNode, collider);
}

void QuadtreeMgr::PrintInfo()
{
#ifdef _DEBUG
	std::cout << "Quadtree Leaf Node Info ... " << std::endl;
	int n = 0;
	for (int x = 0; x < m_ReafNodeCount; ++x)
	{
		n += 1;

		auto bobox = m_pReafNodes[x]->BoBox->GetBOBox();
		std::cout << "Index ... " << x <<std::endl;
		std::cout << "Center ... (" << bobox.Center.x << " , "<< bobox.Center.y << " , "<< bobox.Center.z << ")" << std::endl;
		std::cout << "Size ... (" << bobox.Extents.x * 2 << " , " << bobox.Extents.y * 2 << " , " << bobox.Extents.z * 2<< ")" << std::endl;

		if (n % 4 == 0) std::cout << std::endl;
	}
#endif // _DEBUG

}
 
 
QuadtreeMgr::NODE::NODE(XMFLOAT3&& center, XMFLOAT3&& extents)
{
	NODE(center, extents);
}
 

QuadtreeMgr::NODE::NODE(const XMFLOAT3& center, const XMFLOAT3& extents)
{
	BoBox = new MyBOBox(center, extents); 
}

QuadtreeMgr::NODE::~NODE()
{
	if (BoBox)
	{
		delete BoBox;
		BoBox = nullptr;
	}

	for (int x = 0; x < QUAD; ++x)
	{
		if (children[x])
		{
			delete children[x];
			children[x] = nullptr;
		}
	}
}
