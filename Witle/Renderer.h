#pragma once
#include "ComponentBase.h"
class Renderer :
	public ComponentBase
{
public:
	Renderer();
	virtual ~Renderer();

	virtual const ComponenetID& GetComponentID() const { return m_ComponenetID; };
	virtual const ComponenetID& GetFamillyID() const { return m_FamillyID; };

	virtual void Update();
	void Render();

private:
	ComponenetID m_ComponenetID{""};
	ComponenetID m_FamillyID{"Renderer"};
};

