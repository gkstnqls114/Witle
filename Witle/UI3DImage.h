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
	virtual void ReleaseObjects() override;
	virtual void ReleaseUploadBuffers() override;

private:
	struct UIVertex
	{
		XMFLOAT3 position;
		XMFLOAT2 uv;

		UIVertex() {}
		UIVertex(XMFLOAT3 p, XMFLOAT4 c, XMFLOAT2 u) : position(p), uv(u) {}
	};

	// 텍스쳐관련 정보
	MyDescriptorHeap* m_pHeap{ nullptr };
	Texture* m_pTexture{ nullptr };


public:
	UI3DImage(GameObject* pOwner, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, RECT rect, const wchar_t * filepath);
	UI3DImage(GameObject* pOwner, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, POINT center, float width, float height, const wchar_t * filepath);
	virtual ~UI3DImage();

	virtual void Update(float fElapsedTime) override;
	 
	// Render이전에 쉐이더와 쉐이더변수 설정을 해야함
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, const XMFLOAT4X4& world) override;
};