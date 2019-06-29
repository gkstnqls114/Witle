#pragma once
#include "UI3D.h"

class MyDescriptorHeap;
class Texture;

// MyRectangle�� ������ ������ ����
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

	// �ؽ��İ��� ����
	MyDescriptorHeap* m_pHeap{ nullptr };
	Texture* m_pTexture{ nullptr };


public:
	UI3DImage(GameObject* pOwner, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, RECT rect, const wchar_t * filepath);
	UI3DImage(GameObject* pOwner, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, POINT center, float width, float height, const wchar_t * filepath);
	virtual ~UI3DImage();

	virtual void Update(float fElapsedTime) override;
	 
	// Render������ ���̴��� ���̴����� ������ �ؾ���
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, const XMFLOAT4X4& world) override;
};