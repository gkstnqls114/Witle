#pragma once
#include "32bitRootConstants.h"
#include "ComponentBase.h"

struct VS_CB_MYCAMERA_INFO
{
	XMFLOAT4X4 m_xmf4x4View;		// ī�޶� ��ȯ ���
	XMFLOAT4X4 m_xmf4x4Projection;	// ī�޶� ���� ��ȯ ���
	XMFLOAT3 m_xmf3Position; // ī�޶� ��ġ
};

class GameObject;

// ���� �⺻���� ī�޶��Դϴ�.
// ī�޶� �ڽ��� Position�� �������� �̵��ϰ� ȸ���մϴ�.
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
	DWORD			m_nLastMode{ 0x00 }; // ������ ī�޶� ��� 
#endif

	float			m_fTimeLag{ 0.0f };

	XMFLOAT4X4		m_xmf4x4View{ Matrix4x4::Identity() };
	XMFLOAT4X4		m_xmf4x4Projection{ Matrix4x4::Identity() };

	D3D12_VIEWPORT	m_d3dViewport{ 0, 0, FRAME_BUFFER_WIDTH , FRAME_BUFFER_HEIGHT, 0.0f, 1.0f };
	D3D12_RECT		m_d3dScissorRect{ 0, 0, FRAME_BUFFER_WIDTH , FRAME_BUFFER_HEIGHT };

	//// ���̴�
	//VS_CB_MYCAMERA_INFO*				m_pMappedCameraInfo{ nullptr };
	//ComPtr<ID3D12Resource>			m_d3dCameraCBVUpload{ nullptr };
	//ComPtr<ID3D12DescriptorHeap>	m_d3dCameraCBVDescriptorHeap{ nullptr };
	ResourceBase	*m_ShaderVariables{ nullptr };

	//����ü(���� ��ǥ��)
	BoundingFrustum m_xmFrustum;
	BoundingBox		m_xmBoundingBox;
	BoundingSphere	m_xmSphere;

protected:
	//����ü(���� ��ǥ��)�� �����Ѵ�.
	void GenerateFrustum();
	virtual void MoveSmoothly(float fTimeElapsed, const XMFLOAT3& xmf3LookAt) {}; 

public:
	Camera();
	Camera(Camera *pCamera);
	virtual ~Camera();

	virtual void Teleport(const XMFLOAT3& pos); // right, up, look�� ������ ���·� position, at�� �̵��Ѵ�.
	virtual void Move(const XMFLOAT3& Shift);
	virtual void Rotate(float x = 0.0f, float y = 0.0f, float z = 0.0f);
	virtual void RegenerateAt();

	virtual void Update() override {};
	virtual void Update(float fTimeElapsed, const XMFLOAT3& xmf3LookAt) {};
	void LastUpdate();
	////ī�޶��� ������ ���̴� ���α׷����� �����ϱ� ���� ��� ���۸� �����ϰ� �����Ѵ�.
	//virtual void CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList);
	//virtual void ReleaseShaderVariables();

	//�ٿ�� �ڽ�(OOBB, ���� ��ǥ��)�� ����ü�� ���ԵǴ� ���� �˻��Ѵ�.
	bool IsInFrustum(const BoundingOrientedBox& xmBoundingBox) const;
	bool IsInFrustum(const BoundingBox& xmBoundingBox) const;

	bool IsInBoudingBox(const BoundingOrientedBox& xmBoundingBox) const;
	bool IsInBoudingBox(const BoundingBox& xmBoundingBox) const;

	bool IsInSphere(const BoundingOrientedBox& xmBoundingBox) const;
	bool IsInSphere(const BoundingBox& xmBoundingBox) const;
	bool IsInSphere(const BoundingSphere& xmBoundingBox) const;

	//ī�޶� ��ȯ ���(�����) �� �����Ѵ�.
	void GenerateViewMatrix();
	void GenerateViewMatrix(XMFLOAT3 xmf3Position, XMFLOAT3 xmf3LookAt, XMFLOAT3 xmf3Up);
	//���� ��ȯ ����� �����Ѵ�.
	void GenerateProjectionMatrix(float fNearPlaneDistance, float fFarPlaneDistance, float fAspectRatio, float fFOVAngle);
	// GenerateViewMatrix�� ���� ���, �ݵ�� �������� Regenterate�� ȣ���ؾ��Ѵ�.
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
