#pragma once
#include "Shape.h"

class MyRectangle; 
class Texture;

class UI2D
	: public Shape
{ 
public:
	UI2D(GameObject* pOwner);
	virtual ~UI2D();

	virtual void Update(float fElapsedTime) override {};

	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList) = 0;
};