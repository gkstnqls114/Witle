#pragma once  
class Transform;
class ComponentBase;



class GameObject
{
private:
	std::map< std::string, ComponentBase*> m_Components;
	std::string m_EntityID;


protected:
	Transform* m_Transform{ nullptr }; //월드변환을 위한 좌표계
	ComponentBase* GetComponent(const std::string& id) const;

public:
	GameObject(const std::string& entityID);
	virtual ~GameObject();

	// GameObject::Update의 모든 구현은 파생된 게임 오브젝트마다 다르다.
	virtual void Update(float fElapsedTime) {};

	bool InsertComponent(const std::string& ComponenetID, ComponentBase* pComponentBase);
	
	template <typename T>
	T* GetComponent(const std::string& id = "") const
	{
		assert(id != "");
		return static_cast<T *>(GetComponent(id));
	}
	template <>
	Transform* GetComponent<Transform>(const std::string& id) const
	{
		return m_Transform;
	}

	Transform* GetTransform() const { return m_Transform; }
	
};
 