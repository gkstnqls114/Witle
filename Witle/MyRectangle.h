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
	Texture* m_pTexture{ nullptr };

public:
	virtual void Update(float fTimeElapsed) override {};
	virtual void ReleaseObjects() override;
	virtual void ReleaseUploadBuffers() override;

public: 
	MyRectangle(GameObject* pOwner, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, RECT rect, const wchar_t * filepath);
	MyRectangle(GameObject* pOwner, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, POINT center, float width, float height, const wchar_t * filepath);
	virtual ~MyRectangle();

	Texture* GetTexture() { return m_pTexture; }

	virtual void Render(ID3D12GraphicsCommandList * pd3dCommandList, const Shader* shader) override;
	void Render(ID3D12GraphicsCommandList * pd3dCommandList, XMFLOAT2 pos, float time); //picking Point

	// 쉐이더 설정과 쉐이더 변수는 이 함수 이전에서 설정한다.
	void Render(ID3D12GraphicsCommandList * pd3dCommandList);
};