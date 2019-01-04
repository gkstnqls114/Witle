#pragma once
#include "Renderer.h"
class MeshRenderer :
	public Renderer
{
	bool m_bIndexBuffer{ false };

public:
	MeshRenderer();
	virtual ~MeshRenderer();

	void SetbIndexBuffer(bool indexbuffer) { m_bIndexBuffer = indexbuffer; };
	virtual void Update();
	virtual void Render();

};

