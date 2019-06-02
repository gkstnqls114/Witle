#pragma once
#include "Shape.h"

class MyDescriptorHeap;
class Texture;


// 
class BroomEffectRect
	: public Shape
{
	struct RectVertex
	{
		XMFLOAT3 position;
		XMFLOAT2 uv;
		XMFLOAT2 time; /* Life time , End time */
		XMFLOAT3 velocity;
		
		RectVertex() {}
		RectVertex(XMFLOAT3 p, XMFLOAT2 u, XMFLOAT2 t, XMFLOAT3 v) 
			: position(p), uv(u), time(t), velocity(v) {}
	};

	// 텍스쳐관련 정보
	MyDescriptorHeap* m_pHeap{ nullptr };
	Texture* m_pTexture{ nullptr };

public:
	BroomEffectRect(GameObject * pOwner, ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList);
	virtual ~BroomEffectRect();

	virtual void Render(ID3D12GraphicsCommandList * pd3dCommandList, const Shader* shader) override;

	void Render(ID3D12GraphicsCommandList * pd3dCommandList, float fElapsedTime);
};