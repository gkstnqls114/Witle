#include "stdafx.h"
#include "d3dUtil.h" 
#include "MyBOBox.h" 
#include "QuadtreeMgr.h"
 
 
void QuadtreeMgr::SetminSize(float min_size)
{
	// �̹� minSize�� ������ ���
	assert(!(min != 0) && "minSize is already seted");

	// �Լ� �����ڰ� 0�� �۰ų� ���� ���
	assert(!(min_size <= 0) && "min_size is less than or equal 0");

	m_minSize = min_size;
}

void QuadtreeMgr::CreateQuadTree()
{
	// ���� ���� ����Ʈ�� ����� 0���� ���ų� �۽��ϴ�.
	assert(!(m_minSize <= 0) && "minSize is less than or equal 0");

	// m_pReafNodes �� ������ ����Ͽ� ���� �迭 �����Ѵ�.
	int leafnodeX = ceil(m_pRootNode->BoBox->GetBOBox().Extents.x / int(m_minSize));
	int leafnodeZ = ceil(m_pRootNode->BoBox->GetBOBox().Extents.z / int(m_minSize));

	// ū ���� ��� �����͸� ��� ���� �迭 ����
	m_ReafNodeCount = (leafnodeX > leafnodeZ) ? leafnodeX : leafnodeZ;
	m_pReafNodes = new NODE*[m_ReafNodeCount];

	// �� ��
	int leafnodeIndex = 0;
	CreateRecursiveQuadTree(m_pRootNode, leafnodeIndex);
}

void QuadtreeMgr::CreateRecursiveQuadTree(NODE* pNode, int& leafnodeIndex)
{
	// Extents �� x, z �� *2 �� �ϳ��� minSize���� �۰ų� ������ �� �̻� ��带 ������ �ʴ´�.
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
		// y ���� ��ȭ���� �ʽ��ϴ�.
		XMFLOAT3 extents{ nodeExents.x / 2.f, nodeExents.y, nodeExents.z / 2.f };

		// �� Z
		// ������������������
		// �� 2 �� 3 ��
		// ������������������
		// �� 0 �� 1 ��
		// ������������������ �� x
		// ���� ���� ���İ� ������ �����մϴ�. y�� �״���Դϴ�.
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
		// ���� Ʈ���� �θ� ��带 ����ϴ�.
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
