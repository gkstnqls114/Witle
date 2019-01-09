#pragma once
#include "ComponentBase.h"

struct VS_CB_MYCAMERA_INFO
{
	XMFLOAT4X4 m_xmf4x4View;		// 카메라 변환 행렬
	XMFLOAT4X4 m_xmf4x4Projection;	// 카메라 투영 변환 행렬
	XMFLOAT3 m_xmf3Position; // 카메라 위치
};

class GameObject;

// 가장 기본적인 카메라입니다.
// 카메라 자신의 Position을 기준으로 이동하고 회전합니다.
class Camera
	: public ComponentBase
{
protected:
	XMFLOAT3		m_Position			{ 0.0f, 0.0f, 0.0f }; // Position = At - Offset
	XMFLOAT3		m_Right				{ 1.0f, 0.0f, 0.0f };
	XMFLOAT3		m_Up				{ 0.0f, 1.0f, 0.0f };
	XMFLOAT3		m_Look				{ 0.0f, 0.0f, 1.0f };

	XMFLOAT3		m_At				{ 0.0f, 0.0f, 1.0f }; // Position + Offset = At
	XMFLOAT3		m_Offset			{ 0.0f, 0.0f, 1.0f }; // Offset = At - Position

	float			m_fPitch{ 0.0f };
	float			m_fRoll{ 0.0f };
	float			m_fYaw{ 0.0f };

	DWORD			m_nMode{ 0x00 };
#if _DEBUG
	DWORD			m_nLastMode{ 0x00 }; // 이전의 카메라 모드 
#endif

	float			m_fTimeLag{ 0.0f };

	XMFLOAT4X4		m_xmf4x4View{ Matrix4x4::Identity() };
	XMFLOAT4X4		m_xmf4x4Projection{ Matrix4x4::Identity() };

	D3D12_VIEWPORT	m_d3dViewport{ 0, 0, FRAME_BUFFER_WIDTH , FRAME_BUFFER_HEIGHT, 0.0f, 1.0f };
	D3D12_RECT		m_d3dScissorRect{ 0, 0, FRAME_BUFFER_WIDTH , FRAME_BUFFER_HEIGHT };

	// 쉐이더
	VS_CB_MYCAMERA_INFO*				m_pMappedCameraInfo{ nullptr };
	ComPtr<ID3D12Resource>			m_d3dCameraCBVUpload{ nullptr };
	ComPtr<ID3D12DescriptorHeap>	m_d3dCameraCBVDescriptorHeap{ nullptr };

	//절두체(월드 좌표계)
	BoundingFrustum m_xmFrustum;
	BoundingBox		m_xmBoundingBox;
	BoundingSphere	m_xmSphere;

protected:
	//절두체(월드 좌표계)를 생성한다.
	void GenerateFrustum();
	
	void MoveAroundCamera(const XMFLOAT3& Shift);

	void RotateOnCamera(float x, float y, float z);

public:
	Camera();
	Camera(Camera *pCamera);
	virtual ~Camera();

	virtual void Move(const XMFLOAT3& Shift);
	virtual void Rotate(float x = 0.0f, float y = 0.0f, float z = 0.0f);
	virtual void RegenerateAt();

	virtual void Update() override;
	virtual void ZoomIn(float val);
	virtual void ZoomOut(float val);

	//카메라의 정보를 셰이더 프로그램에게 전달하기 위한 상수 버퍼를 생성하고 갱신한다.
	virtual void CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void ReleaseShaderVariables();

	//바운딩 박스(OOBB, 월드 좌표계)가 절두체에 포함되는 가를 검사한다.
	bool IsInFrustum(const BoundingOrientedBox& xmBoundingBox) const;
	bool IsInFrustum(const BoundingBox& xmBoundingBox) const;

	bool IsInBoudingBox(const BoundingOrientedBox& xmBoundingBox) const;
	bool IsInBoudingBox(const BoundingBox& xmBoundingBox) const;

	bool IsInSphere(const BoundingOrientedBox& xmBoundingBox) const;
	bool IsInSphere(const BoundingBox& xmBoundingBox) const;
	bool IsInSphere(const BoundingSphere& xmBoundingBox) const;

	//카메라 변환 행렬(뷰행렬) 을 생성한다.
	void GenerateViewMatrix();
	void GenerateViewMatrix(XMFLOAT3 xmf3Position, XMFLOAT3 xmf3LookAt, XMFLOAT3 xmf3Up);
	//투영 변환 행렬을 생성한다.
	void GenerateProjectionMatrix(float fNearPlaneDistance, float fFarPlaneDistance, float fAspectRatio, float fFOVAngle);
	// GenerateViewMatrix를 했을 경우, 반드시 마지막에 Regenterate를 호출해야한다.
	void RegenerateViewMatrix();


	///////////////////////////////////////////////////////////////////////// Set
	void SetViewport(int xTopLeft, int yTopLeft, int nWidth, int nHeight, float fMinZ = 0.0f, float fMaxZ = 1.0f);
	void SetScissorRect(LONG xLeft, LONG yTop, LONG xRight, LONG yBottom);
	virtual void SetViewportsAndScissorRects(ID3D12GraphicsCommandList *pd3dCommandList);

	void SetOffset(XMFLOAT3 offset) { m_Offset = offset; }
	void SetMode(DWORD nMode) { m_nMode = nMode; }
	void SetTimeLag(float fTimeLag) { m_fTimeLag = fTimeLag; }
	void SetLookAtPosition(XMFLOAT3 xmf3LookAtWorld) { m_At = xmf3LookAtWorld; }

	void SetAt(XMFLOAT3 at) { m_At = at; };
	void SetPosition(XMFLOAT3 xmf3Position) { m_Position = xmf3Position; };
	///////////////////////////////////////////////////////////////////////// Set

	///////////////////////////////////////////////////////////////////////// Get
	XMFLOAT3 GetAt()             const noexcept { return (m_At); }
	XMFLOAT3 GetRightVector()	 const noexcept { return (m_Right); }
	XMFLOAT3 GetUpVector()		 const noexcept { return (m_Up); }
	XMFLOAT3 GetLookVector()	 const noexcept { return (m_Look); }
	XMFLOAT3 GetPosition()		 const noexcept { return (m_Position); }
	XMFLOAT3 GetOffset()         const noexcept { return (m_Offset); }

	DWORD GetMode()  const noexcept { return (m_nMode); }

	float GetPitch() const noexcept { return (m_fPitch); }
	float GetRoll()	 const noexcept { return (m_fRoll); }
	float GetYaw()	 const noexcept { return (m_fYaw); }

	float GetTimeLag() const noexcept { return (m_fTimeLag); }

	XMFLOAT4X4 GetViewMatrix()		 const noexcept { return (m_xmf4x4View); }
	XMFLOAT4X4 GetProjectionMatrix() const noexcept { return (m_xmf4x4Projection); }
	D3D12_VIEWPORT GetViewport()	 const noexcept { return (m_d3dViewport); }
	D3D12_RECT GetScissorRect()		 const noexcept { return (m_d3dScissorRect); }
	///////////////////////////////////////////////////////////////////////// Get

	void ShowData() const noexcept;
};
