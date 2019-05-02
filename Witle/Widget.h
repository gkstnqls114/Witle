#pragma once
#include "GameObject.h"

class MyRectangle;
class Texture;

class Widget
	: public GameObject
{
	XMFLOAT2 m_PickingPoint;
	RECT m_rect;
	MyRectangle* m_AimPoint{ nullptr };
	 
private:
	virtual void ReleaseMembers() override;
	virtual void ReleaseMemberUploadBuffers() override;

public:
	Widget(const std::string& entityID, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, RECT rect, const wchar_t * filepath);
	Widget(const std::string& entityID, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, POINT center, float width, float height, const wchar_t * filepath);
	virtual ~Widget();

	virtual void Update(float fElapsedTime) override;
	  
	void Render(ID3D12GraphicsCommandList *pd3dCommandList);

	void MovePickingPoint(const XMFLOAT2& shift);
	XMFLOAT2 GetPickingPoint() const { return m_PickingPoint; }

	Texture* GetTexture();
};