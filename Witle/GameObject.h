#pragma once  
#include "Transform.h"
class ComponentBase;

class IGameObject
{
protected:
	// 컴포넌트가 아닌, 게임오브젝트 내에서 동적할당된 멤버변수를 해제한다.
	virtual void ReleaseMembers() = 0;

public: 
	// GameObject::Update의 모든 구현은 파생된 게임 오브젝트마다 다르다.
	virtual void Update(float fElapsedTime) = 0; 
};

class GameObject
	: public IGameObject
{
private:
	std::map< std::string, ComponentBase*> m_Components;
	std::string m_EntityID;

protected:
	Transform m_Transform; //월드변환을 위한 좌표계
	ComponentBase* GetComponent(const std::string& id) const;

private:
	void ReleaseComponents();

public:
	GameObject(const std::string& entityID);
	virtual ~GameObject();

	virtual void Update(float fElapsedTime) override {};
	void ReleaseObjects()
	{
		ReleaseComponents();
		ReleaseMembers();
	};

	bool InsertComponent(const std::string& ComponenetID, ComponentBase* pComponentBase);
	
	template <typename T>
	T* GetComponent(const std::string& id = "") const
	{
		assert(id != "");
		return static_cast<T *>(GetComponent(id));
	}

	const Transform& GetTransform() const { return m_Transform; }
	Transform& GetTransform() { return m_Transform; }
};
 