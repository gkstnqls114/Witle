#pragma once
#include "ComponentBase.h"
class Renderer :
	public ComponentBase
{
public:
	Renderer();
	virtual ~Renderer();

	virtual const std::string& GetComponentID() const { return m_ComponenetID; };
	virtual std::string& GetComponentID() { return m_ComponenetID; }; 
	virtual const std::string& GetFamillyID() const { return m_FamillyID; };
	virtual std::string& GetFamillyID() { return m_FamillyID; };

	virtual void Update();
	virtual void Render();

private:
	std::string m_ComponenetID{""};
	std::string m_FamillyID{"Renderer"};
};

