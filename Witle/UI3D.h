#pragma once
#include "Shape.h"

class MyRectangle; 
class Texture;

class UI3D
	: public Shape
{


private:
	virtual void ReleaseObjects() = 0;
	virtual void ReleaseUploadBuffers() = 0;

public:
	UI3D(GameObject* pOwner);
	virtual ~UI3D();

	virtual void Update(float fElapsedTime) override {};

	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, const XMFLOAT4X4& world) = 0;
};