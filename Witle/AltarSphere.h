#pragma once
#include "GameObject.h"

class Camera;
class CLoadedModelInfo;
class LoadObject;
class MyBOBox; 
class Texture;
class MyDescriptorHeap;

class LineMesh;
class UI3DImage;

class RecognitionRange;
class MyRectangle;

enum AltarSphere_STATE
{
	FLOW,
	IDLE
};

class AltarSphere :
	public GameObject
{
public:
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, bool isGBuffers) override;

private:
	static bool RENDER_DEBUG;
    const float add_guage{ 10.f };
protected: 
	RecognitionRange*   m_RecognitionRange{ nullptr }; 
	MyBOBox*		   m_pMyBOBox{ nullptr };

	Texture*			m_pDeactiveTexture{ nullptr };
	Texture*			m_pActiveTexture{ nullptr };

	CLoadedModelInfo*  m_AltarSphereModel{ nullptr };
	LoadObject*		   m_pLoadObject{ nullptr };

	PlayerStatus* m_ActiveGuageBar{ nullptr }; // 임시로 Player Status 사용

	bool m_isActive{ false };
	bool m_isEnguaged{ false };
	bool m_isFinishFlow{ false };
	
	float m_guage{0.f};

private:
	int m_CurrAnimation{ 0 }; // 현재 사용하는 애니메이션
	int m_PrevAnimation{ 0 }; // 현재 사용하는 애니메이션

protected:
	virtual void ReleaseMembers() override;
	virtual void ReleaseMemberUploadBuffers() override;
	 
private:
	void SetTrackAnimationSet();

public:
	static void CHANGEMODE()
	{
		RENDER_DEBUG = !RENDER_DEBUG;
	}

	AltarSphere(const std::string& entityID, const XMFLOAT3& SpwanPoint, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature);
	virtual ~AltarSphere();
	 
	virtual void Animate(float fElapsedTime);

	void Update(float);
	void Move(ULONG nDirection, float fDistance, bool bVelocity = false);
	void Move(const XMFLOAT3& xmf3Shift);
	void MoveVelocity(const XMFLOAT3& xmf3Shift);
	void Move(float fxOffset = 0.0f, float fyOffset = 0.0f, float fzOffset = 0.0f);
	void Rotate(float x, float y, float z);

	void AddGuage(float );

	//// Get ///////////////////  
	RecognitionRange* GetRecognitionRange() const { return m_RecognitionRange; };
	AXIS GetCoorAxis() const { return m_Transform.GetCoorAxis(); }
	MyBOBox* GetBOBox() const { return m_pMyBOBox; } 
	LoadObject* GetpLoadObject() const { return m_pLoadObject; }
	bool GetisActive() const { return m_isActive; }
	//// Get /////////////////// 

	//// Set /////////////////// 
	void SetAnimationState(int state);
	void SetisActive(bool b) { m_isActive = b; }
	void SetisEnguaged(bool b) { m_isEnguaged = b; }
	//// Set /////////////////// 

};