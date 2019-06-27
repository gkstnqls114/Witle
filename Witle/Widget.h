#pragma once
#include "GameObject.h"

class MyRectangle;
class Texture;

class Widget
	: public GameObject
{
public:
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, bool isGBuffers) override = 0;

private:
	RECT m_rect;

protected:
	MyRectangle* m_AimPoint{ nullptr };
	 
private:
	virtual void ReleaseMembers() override;
	virtual void ReleaseMemberUploadBuffers() override;

public:
	Widget(const std::string& entityID, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, RECT rect, const wchar_t * filepath);
	Widget(const std::string& entityID, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, POINT center, float width, float height, const wchar_t * filepath);
	virtual ~Widget();

	virtual void Update(float fElapsedTime) override;
	   
	Texture* GetTexture();
};

class AimPoint
	: public Widget
{ 
public:
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, bool isGBuffers) override;

private:
	XMFLOAT2 m_PickingPoint;
	
public:
	AimPoint(const std::string& entityID, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, POINT center, float width, float height, const wchar_t * filepath);
	virtual ~AimPoint();

	XMFLOAT2 GetPickingPoint() const { return m_PickingPoint; }
	void MovePickingPoint(const XMFLOAT2& shift);
	 
};