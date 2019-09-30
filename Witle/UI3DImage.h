#pragma once
#include "UI3D.h"

class MyDescriptorHeap;
class Texture;

// MyRectangle과 유사한 동작을 수행
class UI3DImage
	: public UI3D
{
public:
	virtual void Render(ID3D12GraphicsCommandList * pd3dCommandList, const Shader* shader) override {};
	 
private:
	struct UIVertex
	{
		XMFLOAT3 position;
		XMFLOAT2 uv;

		UIVertex() {}
		UIVertex(XMFLOAT3 p, XMFLOAT2 u) : position(p), uv(u) {}
	};

	// 텍스쳐관련 정보 
	Texture* m_pTexture{ nullptr };


public:
	UI3DImage(GameObject* pOwner, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, RECT rect, const std::string& texture_name);
	UI3DImage(GameObject* pOwner, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, POINT center, float width, float height, const std::string& texture_name);
	virtual ~UI3DImage();

	virtual void Update(float fElapsedTime) override;
	 
	// Render이전에 쉐이더와 쉐이더변수 설정을 해야함
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, const XMFLOAT4X4& world) override;
};