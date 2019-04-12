#pragma once
#include "Shape.h"

class Texture;

class MyRectangle
	: public Shape
{
	struct RectVertex
	{ 
		XMFLOAT3 position;
		XMFLOAT4 color;
		XMFLOAT2 uv;

		RectVertex() {}
		RectVertex(XMFLOAT3 p, XMFLOAT4 c, XMFLOAT2 u) : position(p), color(c), uv(u) {}
	};
	 
	// 텍스쳐관련 정보
	MyDescriptorHeap* m_pHeap{ nullptr };
	Texture* m_pTexture{ nullptr };

public: 
	MyRectangle(GameObject* pOwner, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, RECT rect, const wchar_t * filepath);
	MyRectangle(GameObject* pOwner, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, float centerX, float centerY, float width, float height);
	virtual ~MyRectangle();

	Texture* GetTexture() { return m_pTexture; }

	virtual void Render(ID3D12GraphicsCommandList * pd3dCommandList) override;
};