#pragma once 

class ComponentBase;

class GameObject
{
public:
	GameObject(std::string entityID) : m_EntityID(entityID) {};
	virtual ~GameObject();

	void Update() {};
	
	bool InsertComponent(const std::string& ComponenetID, ComponentBase* pComponentBase);
	ComponentBase* GetComponent(const std::string& id);
	const ComponentBase* GetComponent(const  std::string& id) const;

private:
	std::map< std::string, ComponentBase*> m_Components;
	std::string m_EntityID;
};

