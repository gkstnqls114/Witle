#pragma once
#include "Shape.h"

class MyRectangle; 
class Texture;

class UI2D
	: public Shape
{


private:
	virtual void ReleaseObjects() = 0;
	virtual void ReleaseUploadBuffers() = 0;

public:
	UI2D(GameObject* pOwner);
	virtual ~UI2D();

	virtual void Update(float fElapsedTime) override {};

	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList) = 0;
};