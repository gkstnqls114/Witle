#pragma once  
class Transform;
class ComponentBase;

class GameObject
{

protected:
	Transform* m_Transform{ nullptr }; //월드변환을 위한 좌표계
	ComponentBase* GetComponent(const std::string& id) const;

public:
	GameObject(const std::string& entityID);
	virtual ~GameObject();

	virtual void Update();
	
	bool InsertComponent(const std::string& ComponenetID, ComponentBase* pComponentBase);
	
	template <typename T>
	T* GetComponent(const std::string& id) const
	{
		return static_cast<T *>(GetComponent(id));
	}
	template <>
	Transform* GetComponent<Transform>(const std::string& id) const { return m_Transform; }
	
private:
	std::map< std::string, ComponentBase*> m_Components;
	std::string m_EntityID;
};
 