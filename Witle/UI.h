#pragma once
#include "ComponentBase.h"

class MyRectangle; 
class Texture;

class UI
	: public ComponentBase
{


private:
	virtual void ReleaseObjects() = 0;
	virtual void ReleaseUploadBuffers() = 0;

public:
	UI(GameObject* pOwner);
	virtual ~UI();

	virtual void Update(float fElapsedTime) override {};

	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList) = 0;
};