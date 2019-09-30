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
	 
private:
	struct UIVertex
	{
		XMFLOAT3 position;
		XMFLOAT2 uv;

		UIVertex() {}
		UIVertex(XMFLOAT3 p, XMFLOAT2 u) : position(p), uv(u) {}
	};

	// �ؽ��İ��� ���� 
	Texture* m_pTexture{ nullptr };


public:
	UI3DImage(GameObject* pOwner, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, RECT rect, const std::string& texture_name);
	UI3DImage(GameObject* pOwner, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, POINT center, float width, float height, const std::string& texture_name);
	virtual ~UI3DImage();

	virtual void Update(float fElapsedTime) override;
	 
	// Render������ ���̴��� ���̴����� ������ �ؾ���
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, const XMFLOAT4X4& world) override;
};