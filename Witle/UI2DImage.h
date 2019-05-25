#pragma once
#include "UI2D.h"

class MyDescriptorHeap;
class Texture;

// MyRectangle�� ������ ������ ����
class UI2DImage
	: public UI2D
{
public:
	virtual void Render(ID3D12GraphicsCommandList * pd3dCommandList, const Shader* shader) override {};

private:
	struct UIVertex
	{
		XMFLOAT3 position;
		XMFLOAT2 uv;

		UIVertex() {}
		UIVertex(XMFLOAT3 p, XMFLOAT4 c, XMFLOAT2 u) : position(p), uv(u) {}
	};

	// �ؽ��İ��� ����
	MyDescriptorHeap* m_pHeap{ nullptr };
	Texture* m_pTexture{ nullptr };

private:
	virtual void ReleaseObjects() override;
	virtual void ReleaseUploadBuffers() override;

public:
	UI2DImage(GameObject* pOwner, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, RECT rect, const wchar_t * filepath);
	UI2DImage(GameObject* pOwner, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, POINT center, float width, float height, const wchar_t * filepath);
	virtual ~UI2DImage();

	virtual void Update(float fElapsedTime) override;
	 
	// Render������ ���̴��� ���̴����� ������ �ؾ���
	void Render(ID3D12GraphicsCommandList *pd3dCommandList);
};