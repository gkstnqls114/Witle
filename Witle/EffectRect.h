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

	virtual void Render(ID3D12GraphicsCommandList * pd3dCommandList) override {};
	// pos는 스크린 좌표계(왼쪽위가 0,0)를 기준으로 한 위치
	virtual void Render(ID3D12GraphicsCommandList * pd3dCommandList, const XMFLOAT2& pos) {};

	void Render(ID3D12GraphicsCommandList * pd3dCommandList, float fElapsedTime);
};