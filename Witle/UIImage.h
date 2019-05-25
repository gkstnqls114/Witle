#pragma once
#include "UI.h"

class MyRectangle; 
class Texture;

class UIImage
	: public UI
{
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

private:
	virtual void ReleaseObjects() override;
	virtual void ReleaseUploadBuffers() override;

public:
	UIImage(GameObject* pOwner, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, RECT rect, const wchar_t * filepath);
	UIImage(GameObject* pOwner, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, POINT center, float width, float height, const wchar_t * filepath);
	virtual ~UIImage();

	virtual void Update(float fElapsedTime) override;
	 
	void Render(ID3D12GraphicsCommandList *pd3dCommandList);

	Texture* GetTexture(bool isON);
};