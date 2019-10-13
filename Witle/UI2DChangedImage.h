#pragma once
#include "UI2DImage.h"

class UI2DChangedImage
	: public UI2DImage
{
	bool isState{ false };

	// isState�� false�� ���� true �� ��� ����ϴ� �ؽ���
	Texture* m_pFalseTexture{ nullptr };
	Texture* m_pTrueTexture{ nullptr };

public:
	UI2DChangedImage(GameObject* pOwner, ENUM_SCENE Scenetype, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, RECT rect, const std::string& FalseFilepath, const std::string& TrueFilepath);
	UI2DChangedImage(GameObject* pOwner, ENUM_SCENE Scenetype, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, POINT center, float width, float height, const std::string& FalseFilepath, const std::string& TrueFilepath);

	void Init();

	void ChangeFalse();
	void ChangeTrue();
};