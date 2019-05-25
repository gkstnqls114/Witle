#pragma once
#include "GameObject.h"

class MyRectangle; 
class Texture;
class Shader;

class Button
	: public GameObject
{
	RECT m_rect;
	MyRectangle* m_pButtonON{ nullptr };
	MyRectangle* m_pButtonOFF{ nullptr };

	Shader* m_pButtonShader{ nullptr };

	bool m_isClick{ false };

private:
	virtual void ReleaseMembers() override;
	virtual void ReleaseMemberUploadBuffers() override;

public:
	Button(const std::string& entityID, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, RECT rect, const wchar_t * ON_filepath, const wchar_t * OFF_filepath);
	Button(const std::string& entityID, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, POINT center, float width, float height, const wchar_t * ON_filepath, const wchar_t * OFF_filepath);
	virtual ~Button();

	virtual void Update(float fElapsedTime) override;

	bool CheckClick(POINT clickcursor);
	bool CheckClickTRUE(POINT clickcursor);

	void IsNotClick() { m_isClick = false; };
	void IsClick() { m_isClick = true; };
	bool GetisClick() const { return m_isClick; }
	void Render(ID3D12GraphicsCommandList *pd3dCommandList);

	Texture* GetTexture(bool isON);
};