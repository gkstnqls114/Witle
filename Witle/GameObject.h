#pragma once 
#include <typeinfo.h>
class ComponentBase;

class GameObject
{
protected:
	ComponentBase* GetComponent(const std::string& id) const;

public:
	GameObject(std::string entityID) : m_EntityID(entityID) {};
	virtual ~GameObject();

	void Update() {};
	
	bool InsertComponent(const std::string& ComponenetID, ComponentBase* pComponentBase);
	
	template <typename T>
	T* GetComponent(const std::string& id) const
	{
		return static_cast<T *>(GetComponent(id));
	}

	
private:
	std::map< std::string, ComponentBase*> m_Components;
	std::string m_EntityID;
};
 