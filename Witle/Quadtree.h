#pragma once

#ifndef QUADTREE
#define QUADTREE

class Movement;

template<typename T>
class Quadtree
{ 
private:
	Quadtree(Quadtree const&) = delete;            // ���� ����
	Quadtree& operator=(Quadtree const&) = delete; // �Ҵ� ����
	  
private:
	T* m_RootNode{ nullptr }; // ��Ʈ ���

	int m_ReafNodeCount{ 0 };
	T** m_pReafNodes{ nullptr }; // ���� ��带 ������ ���� �迭�� �����̳ʿ� �Ұ���

	float m_minSize{ 0 }; // ���� ���� width or height�� ����. �� �� �ϳ��� minSize��� ���̻� ��带 ������ �ʴ´�.
	 
private: 
	void CreateRecursiveQuadTree(T* pNode, int& leafnodeIndex)
	{
		// Extents �� x, z �� *2 �� �ϳ��� minSize���� �۰ų� ������ �� �̻� ��带 ������ �ʴ´�.
		XMFLOAT3 nodeExents = pNode->BoBox->GetBOBox().Extents;
		XMFLOAT3 nodeCenter = pNode->BoBox->GetBOBox().Center;
		bool isLeafNode = ((nodeExents.x * 2) <= m_minSize) | ((nodeExents.z * 2) <= m_minSize);
		if (isLeafNode)
		{
			assert(!(m_ReafNodeCount <= leafnodeIndex) && "index is greater than leaf node count");

#if defined(DEBUG) | defined(_DEBUG)
			pNode->reafIndex = leafnodeIndex;
#endif   
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
			XMFLOAT3 center[4]
			{
				XMFLOAT3(nodeCenter.x - extents.x, nodeCenter.y, nodeCenter.z - extents.z), // 0
				XMFLOAT3(nodeCenter.x + extents.x, nodeCenter.y, nodeCenter.z - extents.z), // 1
				XMFLOAT3(nodeCenter.x - extents.x, nodeCenter.y, nodeCenter.z + extents.z), // 2
				XMFLOAT3(nodeCenter.x + extents.x, nodeCenter.y, nodeCenter.z + extents.z)  // 3
			};

			for (int x = 0; x < 4; ++x)
			{
				pNode->children[x] = new T(center[x], extents);
				CreateRecursiveQuadTree(pNode->children[x], leafnodeIndex);
			}
		}
	}


	void ReleaseRecursiveQuadTree(T* node)
	{
		// �ڽ��� �ִ� ��� ����Լ��� �̿��Ͽ� children ���η� ����.
		bool isHaveChildren = node->children[0] != nullptr;
		if (isHaveChildren)
		{
			for (int x = 0; x < 4; ++x)
			{
				if (node->children[x])
				{
					ReleaseRecursiveQuadTree(node->children[x]);
					node->children[x] = nullptr;
				}
			}
		}

		// �ڽ��� ���ų� �� ������ ������ �ش� ��带 delete�Ѵ�.
		delete node;
	}

protected: 

	void SetminSize(float min_size)
	{ 
		// �̹� minSize�� ������ ���
		bool isAlreadySeted = m_minSize != 0;
		assert(!(isAlreadySeted) && "minSize is already seted");
		if (isAlreadySeted) return;

		// �Լ� ���ڰ� 0�� �۰ų� ���� ���
		bool isLessEqualZero = min_size <= 0;
		assert(!(isLessEqualZero) && "min_size is less than or equal 0");
		if (isLessEqualZero) return;

		m_minSize = min_size;
	}

	float GetminSize() { return m_minSize; }

	void CreateQuadTree()
	{
		// ���� ���� ����Ʈ�� ����� 0���� ���ų� �۽��ϴ�.
		bool isLessEqualZero = m_minSize <= 0;
		assert(!(isLessEqualZero) && "minSize is less than or equal 0");
		if (isLessEqualZero) return;

		// m_pReafNodes �� ������ ����Ͽ� ���� �迭 �����Ѵ�.
		int leafnodeX = ceil((m_RootNode->BoBox->GetBOBox().Extents.x * 2.f) / int(m_minSize)); // x �࿡�� �������� ��
		int leafnodeZ = ceil((m_RootNode->BoBox->GetBOBox().Extents.z * 2.f) / int(m_minSize)); // z �࿡�� �������� ��

		// ū ���� ��� �����͸� ��� ���� �迭 ����
		m_ReafNodeCount = (leafnodeX > leafnodeZ) ? leafnodeX : leafnodeZ;
		m_ReafNodeCount = m_ReafNodeCount * m_ReafNodeCount; // �簢���̹Ƿ� N^2

		m_pReafNodes = new T * [m_ReafNodeCount];

		// �� ��
		int leafnodeIndex = 0;
		CreateRecursiveQuadTree(m_RootNode, leafnodeIndex);
	}

	void ReleaseQuadTree()
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

//	void CreateTerrainObj(const char* terrain_info_path)
//	{
//		FILE* pInFile = NULL;
//		::fopen_s(&pInFile, terrain_info_path, "rb");
//		::rewind(pInFile);
//
//		char pstrToken[64] = { '\0' };
//
//		for (; ; )
//		{
//			if (FileRead::ReadStringFromFile(pInFile, pstrToken))
//			{
//				if (!strcmp(pstrToken, "<Hierarchy>:"))
//				{
//					int nRootChild = FileRead::ReadIntegerFromFile(pInFile); // ��� ������Ʈ ����
//
//					FileRead::ReadStringFromFile(pInFile, pstrToken); //<ObjectCount>: 
//					int nObjects = FileRead::ReadIntegerFromFile(pInFile);
//					for (int i = 0; i < nObjects; ++i)
//					{
//						FileRead::ReadStringFromFile(pInFile, pstrToken); // Object Name
//						FileRead::ReadIntegerFromFile(pInFile); // Object Count
//					}
//
//					for (int i = 0; i < nRootChild; ++i)
//					{
//						FileRead::ReadStringFromFile(pInFile, pstrToken);
//						if (!strcmp(pstrToken, "<Frame>:"))
//						{
//							// AddCollider �Լ��� ���� ���� ��忡 �浹ü�� �߰��Ѵ�.
//							CreateTerrainObj(pInFile);
//						}
//
//					}
//				}
//				else if (!strcmp(pstrToken, "</Hierarchy>"))
//				{
//					break;
//				}
//			}
//			else
//			{
//				break;
//			}
//		}
//
//#ifdef _WITH_DEBUG_FRAME_HIERARCHY
//		TCHAR pstrDebug[256] = { 0 };
//		_stprintf_s(pstrDebug, 256, _T("Frame Hierarchy\n"));
//		OutputDebugString(pstrDebug);
//
//		LoadObject::PrintFrameInfo(pLoadedModel->m_pModelRootObject, NULL);
//#endif
//
//		::fclose(pInFile);
//	}
//	void CreateTerrainObj(FILE* pInFile)
//	{
//		char pstrToken[64] = { '\0' };
//		UINT nReads = 0;
//
//		char name[64];
//
//		int nFrame = FileRead::ReadIntegerFromFile(pInFile);
//		FileRead::ReadStringFromFile(pInFile, name);
//
//		for (; ; )
//		{
//			FileRead::ReadStringFromFile(pInFile, pstrToken);
//			if (!strcmp(pstrToken, "<GlobalTransform>:"))
//			{
//				XMFLOAT4X4 temp;
//				nReads = (UINT)::fread(&temp, sizeof(XMFLOAT4X4), 1, pInFile);
//
//				FileRead::ReadStringFromFile(pInFile, pstrToken); // <GlobalRotation>:
//				XMFLOAT4 rotationXYZ;
//				nReads = (UINT)::fread(&rotationXYZ, sizeof(XMFLOAT4), 1, pInFile);
//				XMFLOAT4X4 rotate180Y = Matrix4x4::RotateMatrix(0.f, 0.f, 0.f);
//				XMFLOAT4X4 rotateInfo = Matrix4x4::RotateMatrix(0.f, rotationXYZ.z, 0.f);
//
//				// XMFLOAT4X4 transform = Matrix4x4::Multiply(rotate180Y, rotateInfo);
//				XMFLOAT4X4 transform = Matrix4x4::Identity();
//
//				// fbx sdk ���� �����ö� x, z��ǥ�� -��ȣ �ٿ����� ...
//				transform._41 = -(temp._41);
//				transform._42 = 0; // y��ǥ�� 0���� ����
//				if (!strcmp(name, Flower)) transform._42 = temp._42;
//				transform._43 = -(temp._43);
//
//				assert(!(temp._41 >= 40000));
//				assert(!(temp._43 >= 40000));
//
//
//				// ��Stg�� �����ϴ� �̸��� ���� �̸��� ��ġ�ϴ��� Ȯ���Ѵ�.
//				for (const auto& modelname : ModelStorage::GetInstance()->m_NameList)
//				{
//					// Object001�� ���ܷ� Cliff , ������ �ǹ��Ѵ�.
//					if (!(!strcmp(name, modelname.c_str()) || !strcmp(name, "Object001")))
//					{
//						continue;
//					}
//
//					if (!strcmp(name, "Object001"))
//					{
//						strncpy(name, "Cliff", 64); // Object001�� Cliff�� ����ϵ��� �̸��� �ٲ۴�.
//					}
//
//
//					// ��� ���� �浹ü�� ���ϰ����� �ʴ�. �ѹ� Ȯ�����ش�.
//					MyBOBox* pBoBox = ModelStorage::GetInstance()->GetBOBox(modelname);
//					if (pBoBox == nullptr) continue;
//
//#ifdef _DEBUG
//					std::cout << name << " ... Quaddtree Add" << std::endl;
//#endif // DEBUG
//
//					// �� ������Ʈ�� ���� ���� ����� �����Ѵ�. �ʼ�����!
//					LoadObject* TestObject = ModelStorage::GetInstance()->GetRootObject(name);
//					TestObject->SetTransform(transform);
//					TestObject->UpdateTransform(NULL);
//
//					// �浹ü�� ������Ŀ� ���߾� ����Ѵ�. 
//					MyBOBox mybobox = *pBoBox;
//					mybobox.Move(XMFLOAT3(TestObject->m_pChild->m_xmf4x4World._41, 0, TestObject->m_pChild->m_xmf4x4World._43));
//
//					AddCollider(mybobox, TestObject->m_pChild->m_xmf4x4World);
//
//					delete TestObject;
//					TestObject = nullptr;
//
//				}
//
//			}
//			else if (!strcmp(pstrToken, "<Children>:"))
//			{
//				int nChilds = FileRead::ReadIntegerFromFile(pInFile);
//				if (nChilds > 0)
//				{
//					for (int i = 0; i < nChilds; i++)
//					{
//						FileRead::ReadStringFromFile(pInFile, pstrToken);
//						if (!strcmp(pstrToken, "<Frame>:"))
//						{
//							CreateTerrainObj(pInFile);
//#ifdef _WITH_DEBUG_FRAME_HIERARCHY
//							TCHAR pstrDebug[256] = { 0 };
//							_stprintf_s(pstrDebug, 256, _T("(Frame: %p) (Parent: %p)\n"), pChild, pGameObject);
//							OutputDebugString(pstrDebug);
//#endif
//						}
//					}
//				}
//			}
//			else if (!strcmp(pstrToken, "</Frame>"))
//			{
//				break;
//			}
//		}
//	}
//
//	void ProcessRecursiveCollide(const T& node, Movement& movement, const BoundingOrientedBox& nextFrameBoBox, const MyBOBox& BoBox)
//	{
//		// ���� ������ ��ġ���� node�� �ε������� Ȯ��.
//		bool isCollided = Collision::isCollide(*node.BoBox, collider);
//		if (!isCollided) return;
//
//		bool isHaveChildren = node.children[0] != nullptr; // �ڽ��� �ִ� ���
//		if (isHaveChildren)
//		{
//			for (int x = 0; x < 4; ++x)
//			{
//				ProcessRecursiveCollide(*node.children[x], movement, nextFrameBoBox, collider);
//			}
//		}
//		else
//		{
//			float fElapsedTime = CGameTimer::GetInstance()->GetTimeElapsed();
//			for (const auto& tobj : node.terrainObjBoBoxs)
//			{
//				XMFLOAT3 slideVector{ 0.f, 0.f, 0.f };
//
//				// ���� �����ӿ��� �ͷ��� ������Ʈ�� �ε����� �� Ȯ���Ѵ�.
//				bool isSlide = Collision::ProcessCollision(
//					nextFrameBoBox,
//					tobj.BoBox,
//					movement.GetpOwner()->GetTransform().GetPosition(),
//					movement.GetVelocity(),
//					fElapsedTime,
//					true,
//					slideVector);
//
//				if (isSlide)
//				{
//					movement.SetVelocity(slideVector);
//				}
//			}
//		}
//	}
//
//	void AddRecursiveCollider(T* node, const MyBOBox& collider, const XMFLOAT4X4& world)
//	{
//		// �ش� ����� �浹ü�� �ε����������� �Ѿ��.
//		bool isCollided = Collision::isCollide(*(pNode->BoBox), BoBox);
//		if (!isCollided) return;
//
//		bool isHaveChildren = pNode->children[0] != nullptr;
//		if (isHaveChildren)
//		{
//			// �ڽ��� �ִ� ��� ����Լ��� �̿��Ͽ� children ���η� ����.
//			for (int x = 0; x < 4; ++x)
//			{
//				if (pNode->children[x])
//				{
//					AddRecursiveCollider(pNode->children[x], BoBox, world);
//				}
//			}
//		}
//		else
//		{
//			// �ڽ��� �����Ƿ� ��������� ���̴�.
//			// �浹�����Ƿ� ��� ����Ʈ�� �߰��Ѵ�.
//			pNode->terrainObjCount += 1;
//			pNode->terrainObjBoBoxs.emplace_back(BoBox, world);
//			return;
//		}
//	}

public: 
	Quadtree()
	{
	}

	virtual ~Quadtree()
	{
		if (m_RootNode)
		{
			ReleaseQuadTree();
		}
	}

	void Release()
	{

	}


	//void Init(const XMFLOAT3& center, const XMFLOAT3& extents, float min_size)
	//{ 
	//	SetminSize(min_size);

	//	if (m_RootNode == nullptr)
	//	{
	//		// ���� Ʈ���� �θ� ��带 ����ϴ�.
	//		m_RootNode = new T(center, extents) // ���� ������..
	//	}

	//	// Quadtree�� �����Ѵ�.
	//	CreateQuadTree();

	//	// �������� Quadtree�� ���� ���� ������Ʈ�� ������ �ִ´�.
	//	CreateTerrainObj(TERRAIN_OBJS_INFO_PATH);
	//}
	// 
	//// �ش� �浹ü�� �浹�ϴ� treePiece�� �浹ü�� �߰��մϴ�.
	//void AddCollider(const MyBOBox& BoBox, const XMFLOAT4X4& world)
	//{ 
	//	AddRecursiveCollider(m_RootNode, collider, world);
	//}

	//// bobox �� �浹üũ�մϴ�.
	//void ProcessCollide(Movement& movement, const BoundingOrientedBox& nextFrameBoBox, const MyBOBox& BoBox)
	//{ 
	//	ProcessRecursiveCollide(*m_RootNode, movement, nextFrameBoBox, collider);
	//}

	//// ��������� ������ ��ȯ�մϴ�.
	//int GetReafNodeCount() { return m_ReafNodeCount; }
	//const T& GetRoot() const { return *m_RootNode; }
	// 
	//virtual void PrintInfo() = 0;


public: 
	// ��������� ������ ��ȯ�մϴ�.
	int GetReafNodeCount()   { return m_ReafNodeCount; } 

	// Root �����͸� ��ȯ�մϴ�.
	T* const GetpRoot() const { return m_RootNode;     }
	void SetpRoot(T* root) { m_RootNode = root; }
	const T& GetReaf(int index) const 
	{ 
		assert(!(index < 0 || index >= m_minSize) && "range over index");
		return *(m_pReafNodes[index]);
	}

}; 

#endif


