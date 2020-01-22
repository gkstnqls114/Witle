#pragma once

#ifndef QUADTREE
#define QUADTREE

class Movement;

template<typename T>
class Quadtree
{ 
private:
	Quadtree(Quadtree const&) = delete;            // 복사 숨김
	Quadtree& operator=(Quadtree const&) = delete; // 할당 숨김
	  
protected:
	//T* m_RootNode{ nullptr }; // 루트 노드

	//int m_ReafNodeCount{ 0 };
	//T** m_pReafNodes{ nullptr }; // 리프 노드를 접근할 동적 배열로 컨테이너에 불과함

	//float m_minSize{ 0 }; // 제일 작은 width or height의 조건. 둘 중 하나가 minSize라면 더이상 노드를 만들지 않는다.
	 
private:
//	void SetminSize(float min_size)
//	{
//		// 이미 minSize가 설정된 경우
//		bool isAlreadySeted = m_minSize != 0;
//		assert(!(isAlreadySeted) && "minSize is already seted");
//		if (isAlreadySeted) return;
//
//		// 함수 인자가 0과 작거나 같은 경우
//		bool isLessEqualZero = min_size <= 0;
//		assert(!(isLessEqualZero) && "min_size is less than or equal 0");
//		if (isLessEqualZero) return;
//
//		m_minSize = min_size;
//	}
//
//	void CreateQuadTree()
//	{
//		// 제일 작은 쿼드트리 사이즈가 0보다 같거나 작습니다.
//		bool isLessEqualZero = m_minSize <= 0;
//		assert(!(isLessEqualZero) && "minSize is less than or equal 0");
//		if (isLessEqualZero) return;
//
//		// m_pReafNodes 의 개수를 계산하여 동적 배열 생성한다.
//		int leafnodeX = ceil((m_RootNode->BoBox->GetBOBox().Extents.x * 2.f) / int(m_minSize)); // x 축에서 나눠지는 수
//		int leafnodeZ = ceil((m_RootNode->BoBox->GetBOBox().Extents.z * 2.f) / int(m_minSize)); // z 축에서 나눠지는 수
//
//		// 큰 값을 골라 포인터를 담는 동적 배열 생성
//		m_ReafNodeCount = (leafnodeX > leafnodeZ) ? leafnodeX : leafnodeZ;
//		m_ReafNodeCount = m_ReafNodeCount * m_ReafNodeCount; // 사각형이므로 N^2
//
//		m_pReafNodes = new T * [m_ReafNodeCount];
//
//		// 그 후
//		int leafnodeIndex = 0;
//		CreateRecursiveQuadTree(m_RootNode, leafnodeIndex);
//	}
//
//	void CreateRecursiveQuadTree(T* node, int& leafnodeIndex)
//	{
//		// Extents 의 x, z 의 *2 중 하나라도 minSize보다 작거나 같으면 더 이상 노드를 만들지 않는다.
//		XMFLOAT3 nodeExents = pNode->BoBox->GetBOBox().Extents;
//		XMFLOAT3 nodeCenter = pNode->BoBox->GetBOBox().Center;
//		bool isLeafNode = ((nodeExents.x * 2) <= m_minSize) | ((nodeExents.z * 2) <= m_minSize);
//		if (isLeafNode)
//		{
//			assert(!(m_ReafNodeCount <= leafnodeIndex) && "index is greater than leaf node count");
//
//#if defined(DEBUG) | defined(_DEBUG)
//			pNode->reafIndex = leafnodeIndex;
//#endif   
//			m_pReafNodes[leafnodeIndex] = pNode;
//			leafnodeIndex += 1;
//		}
//		else
//		{
//			// y 값은 변화하지 않습니다.
//			XMFLOAT3 extents{ nodeExents.x / 2.f, nodeExents.y, nodeExents.z / 2.f };
//
//			// ▲ Z
//			// ┌───┬───┐
//			// │ 2 │ 3 │
//			// ├───┼───┤
//			// │ 0 │ 1 │
//			// └───┴───┘ ▶ x
//			// 위와 같은 형식과 순서로 생성합니다. y는 그대로입니다.
//			XMFLOAT3 center[4]
//			{
//				XMFLOAT3(nodeCenter.x - extents.x, nodeCenter.y, nodeCenter.z - extents.z), // 0
//				XMFLOAT3(nodeCenter.x + extents.x, nodeCenter.y, nodeCenter.z - extents.z), // 1
//				XMFLOAT3(nodeCenter.x - extents.x, nodeCenter.y, nodeCenter.z + extents.z), // 2
//				XMFLOAT3(nodeCenter.x + extents.x, nodeCenter.y, nodeCenter.z + extents.z)  // 3
//			};
//
//			for (int x = 0; x < 4; ++x)
//			{
//				pNode->children[x] = new T(center[x], extents);
//				CreateRecursiveQuadTree(pNode->children[x], leafnodeIndex);
//			}
//		}
//	}
//
//	void ReleaseQuadTree()
//	{
//		if (m_RootNode != nullptr)
//		{
//			ReleaseRecursiveQuadTree(m_RootNode);
//			m_RootNode = nullptr;
//		}
//
//		if (m_pReafNodes != nullptr)
//		{
//			delete[] m_pReafNodes;
//			m_pReafNodes = nullptr;
//		}
//	}
//
//	void ReleaseRecursiveQuadTree(T* node)
//	{
//		// 자식이 있는 경우 재귀함수를 이용하여 children 내부로 들어간다.
//		bool isHaveChildren = node->children[0] != nullptr;
//		if (isHaveChildren)
//		{
//			for (int x = 0; x < 4; ++x)
//			{
//				if (node->children[x])
//				{
//					ReleaseRecursiveQuadTree(node->children[x]);
//					node->children[x] = nullptr;
//				}
//			}
//		}
//
//		// 자식이 없거나 위 과정이 끝나면 해당 노드를 delete한다.
//		delete node;
//	}
//
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
//					int nRootChild = FileRead::ReadIntegerFromFile(pInFile); // 모든 오브젝트 개수
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
//							// AddCollider 함수를 통해 리프 노드에 충돌체를 추가한다.
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
//				// fbx sdk 에서 꺼내올때 x, z좌표에 -부호 붙여야함 ...
//				transform._41 = -(temp._41);
//				transform._42 = 0; // y좌표는 0으로 고정
//				if (!strcmp(name, Flower)) transform._42 = temp._42;
//				transform._43 = -(temp._43);
//
//				assert(!(temp._41 >= 40000));
//				assert(!(temp._43 >= 40000));
//
//
//				// 모델Stg에 존재하는 이름과 현재 이름이 일치하는지 확인한다.
//				for (const auto& modelname : ModelStorage::GetInstance()->m_NameList)
//				{
//					// Object001을 예외로 Cliff , 절벽을 의미한다.
//					if (!(!strcmp(name, modelname.c_str()) || !strcmp(name, "Object001")))
//					{
//						continue;
//					}
//
//					if (!strcmp(name, "Object001"))
//					{
//						strncpy(name, "Cliff", 64); // Object001을 Cliff로 취급하도록 이름을 바꾼다.
//					}
//
//
//					// 모든 모델은 충돌체를 지니고있지 않다. 한번 확인해준다.
//					MyBOBox* pBoBox = ModelStorage::GetInstance()->GetBOBox(modelname);
//					if (pBoBox == nullptr) continue;
//
//#ifdef _DEBUG
//					std::cout << name << " ... Quaddtree Add" << std::endl;
//#endif // DEBUG
//
//					// 모델 오브젝트에 대한 월드 행렬을 재계산한다. 필수과정!
//					LoadObject* TestObject = ModelStorage::GetInstance()->GetRootObject(name);
//					TestObject->SetTransform(transform);
//					TestObject->UpdateTransform(NULL);
//
//					// 충돌체를 월드행렬에 맞추어 계산한다. 
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
//		// 현재 프레임 위치에서 node와 부딪히는지 확인.
//		bool isCollided = Collision::isCollide(*node.BoBox, collider);
//		if (!isCollided) return;
//
//		bool isHaveChildren = node.children[0] != nullptr; // 자식이 있는 경우
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
//				// 다음 프레임에서 터레인 오브젝트와 부딪히는 지 확인한다.
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
//		// 해당 노드의 충돌체와 부딪히지않으면 넘어간다.
//		bool isCollided = Collision::isCollide(*(pNode->BoBox), BoBox);
//		if (!isCollided) return;
//
//		bool isHaveChildren = pNode->children[0] != nullptr;
//		if (isHaveChildren)
//		{
//			// 자식이 있는 경우 재귀함수를 이용하여 children 내부로 들어간다.
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
//			// 자식이 없으므로 리프노드라는 뜻이다.
//			// 충돌했으므로 노드 리스트에 추가한다.
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
		//if (m_RootNode)
		//{
		//	ReleaseQuadTree();
		//}
	}

	void Release()
	{

	}

	//void Init(const XMFLOAT3& center, const XMFLOAT3& extents, float min_size)
	//{ 
	//	SetminSize(min_size);

	//	if (m_RootNode == nullptr)
	//	{
	//		// 쿼드 트리의 부모 노드를 만듭니다.
	//		m_RootNode = new T(center, extents) // 여기 문제네..
	//	}

	//	// Quadtree를 구성한다.
	//	CreateQuadTree();

	//	// 나누어진 Quadtree를 통해 지형 오브젝트의 정보를 넣는다.
	//	CreateTerrainObj(TERRAIN_OBJS_INFO_PATH);
	//}
	// 
	//// 해당 충돌체와 충돌하는 treePiece에 충돌체를 추가합니다.
	//void AddCollider(const MyBOBox& BoBox, const XMFLOAT4X4& world)
	//{ 
	//	AddRecursiveCollider(m_RootNode, collider, world);
	//}

	//// bobox 와 충돌체크합니다.
	//void ProcessCollide(Movement& movement, const BoundingOrientedBox& nextFrameBoBox, const MyBOBox& BoBox)
	//{ 
	//	ProcessRecursiveCollide(*m_RootNode, movement, nextFrameBoBox, collider);
	//}

	//// 리프노드의 개수를 반환합니다.
	//int GetReafNodeCount() { return m_ReafNodeCount; }
	//const T& GetRoot() const { return *m_RootNode; }
	// 
	//virtual void PrintInfo() = 0;
}; 

#endif