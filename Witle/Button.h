#pragma once
#include "GameObject.h"

class MyRectangle; 
class Texture;

class Button
	: public GameObject
{
	RECT m_rect;
	MyRectangle* m_pButtonON{ nullptr };
	MyRectangle* m_pButtonOFF{ nullptr };

	bool m_isClick{ false };

private:
	virtual void ReleaseMembers() override;
	virtual void ReleaseMemberUploadBuffers() override;

public:
	Button(const std::string& entityID, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, RECT rect, const wchar_t * ON_filepath, const wchar_t * OFF_filepath);
	Button(const std::string& entityID, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, POINT center, float width, float height, const wchar_t * ON_filepath, const wchar_t * OFF_filepath);
	virtual ~Button();

	void CheckClick(POINT clickcursor);
	virtual void Update(float fElapsedTime) override;
	void Render(ID3D12GraphicsCommandList *pd3dCommandList);

	Texture* GetTexture(bool isON);
};