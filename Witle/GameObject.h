#pragma once  
#include "Transform.h"
class ComponentBase;
class ResourceBase;

class IGameObject
{
protected:
	// 컴포넌트가 아닌, 게임오브젝트 내에서 동적할당된 멤버변수를 해제한다.
	virtual void ReleaseMembers() = 0;
	// 컴포넌트가 아닌, 게임오브젝트 내에서 동적할당된 업로드 힙을 해제한다.
	virtual void ReleaseMemberUploadBuffers() = 0;

public: 
	// GameObject::Update의 모든 구현은 파생된 게임 오브젝트마다 다르다.
	virtual void Update(float fElapsedTime) = 0; 
};

class GameObject
	: public IGameObject
{
protected:
	virtual void ReleaseMembers() = 0;
	virtual void ReleaseMemberUploadBuffers() = 0;

public:
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, bool isGBuffers) {};

private:
	std::map< std::string, ComponentBase*> m_Components;
#ifdef _DEBUG
	std::string m_EntityID;  
#else // _DEBUG 

#endif
protected:
	Transform m_Transform; //월드변환을 위한 좌표계
	ComponentBase* GetComponent(const std::string& id) const;
	 
private:
	void ReleaseComponents();
	void ReleaseComponentUploadBuffers();
	 

public:
	GameObject(const std::string& entityID);
	virtual ~GameObject();

	virtual void Update(float fElapsedTime) override {};

	// delete 이전에 반드시 호출
	void ReleaseObjects();
	void ReleaseUploadBuffers();

	bool InsertComponent(const std::string& ComponenetID, ComponentBase* pComponentBase);

	template <typename T>
	T* GetComponent(const std::string& id = "") const
	{
		assert(id != "");
		return static_cast<T *>(GetComponent(id));
	}

	const Transform& GetTransform() const { return m_Transform; }
	Transform& GetTransform() { return m_Transform; }
	const std::string& GetName() const 
	{ 
#ifdef _DEBUG
		return m_EntityID;
#else // _DEBUG 
		return std::string();
#endif
	}
};

class EmptyGameObject
	: public GameObject
{
public:
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, bool isGBuffers) override {};

protected:
	virtual void ReleaseMembers() {};
	virtual void ReleaseMemberUploadBuffers() {};

public:
	EmptyGameObject(const std::string& entityID) : GameObject(entityID) {};
	virtual ~EmptyGameObject() {};
};