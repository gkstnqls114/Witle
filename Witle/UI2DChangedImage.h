#pragma once
#include "UI2DImage.h"

class UI2DChangedImage
	: public UI2DImage
{
	bool isState{ false };

	// isState가 false인 경우와 true 인 경우 사용하는 텍스쳐
	Texture* m_pFalseTexture{ nullptr };
	Texture* m_pTrueTexture{ nullptr };

public:
	UI2DChangedImage(GameObject* pOwner, ENUM_SCENE Scenetype, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, RECT rect, const std::string& FalseFilepath, const std::string& TrueFilepath);
	UI2DChangedImage(GameObject* pOwner, ENUM_SCENE Scenetype, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, POINT center, float width, float height, const std::string& FalseFilepath, const std::string& TrueFilepath);

	void Init();

	void ChangeFalse();
	void ChangeTrue();
};