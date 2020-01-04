#include "stdafx.h"
#include "d3dUtil.h" 
#include "MyBOBox.h" 
#include "QuadtreeMgr.h"
 
 
void QuadtreeMgr::SetminSize(float min_size)
{
	// 이미 minSize가 설정된 경우
	assert(!(min != 0) && "minSize is already seted");

	// 함수 ㅇ인자가 0과 작거나 같은 경우
	assert(!(min_size <= 0) && "min_size is less than or equal 0");

	m_minSize = min_size;
}

void QuadtreeMgr::CreateQuadTree()
{
	// 제일 작은 쿼드트리 사이즈가 0보다 같거나 작습니다.
	assert(!(m_minSize <= 0) && "minSize is less than or equal 0");

	// m_pReafNodes 의 개수를 계산하여 동적 배열 생성한다.
	int leafnodeX = ceil(m_pRootNode->BoBox->GetBOBox().Extents.x / int(m_minSize));
	int leafnodeZ = ceil(m_pRootNode->BoBox->GetBOBox().Extents.z / int(m_minSize));

	// 큰 값을 골라 포인터를 담는 동적 배열 생성
	m_ReafNodeCount = (leafnodeX > leafnodeZ) ? leafnodeX : leafnodeZ;
	m_pReafNodes = new NODE*[m_ReafNodeCount];

	// 그 후
	int leafnodeIndex = 0;
	CreateRecursiveQuadTree(m_pRootNode, leafnodeIndex);
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
	ReleaseRecursiveQuadTree(m_pRootNode);
}

void QuadtreeMgr::ReleaseRecursiveQuadTree(NODE* pNode)
{
	bool isHaveChildren = pNode->children[0] != nullptr;
	 
	if (isHaveChildren)
	{
		for (int x = 0; x < QUAD; ++x)
		{
			if (pNode->children[x])
			{
				ReleaseRecursiveQuadTree(pNode->children[x]);
			}
		} 
	}
	else
	{
		delete pNode;
		pNode = nullptr;
	}
}
    
  
QuadtreeMgr::QuadtreeMgr()  
{
}

QuadtreeMgr::~QuadtreeMgr()
{
	if (m_pRootNode)
	{
		ReleaseQuadTree();
	}
}

void QuadtreeMgr::Init(const XMFLOAT3& center, const XMFLOAT3& extents, float min_size)
{  
	SetminSize(min_size);

	if (m_pRootNode == nullptr)
	{
		// 쿼드 트리의 부모 노드를 만듭니다.
		m_pRootNode = new NODE(center, extents);
	}

	CreateQuadTree();
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
