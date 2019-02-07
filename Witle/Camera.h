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
	: public ResourceComponentBase
{
private:
	struct VS_CB_CAMERA_INFO
	{
		XMFLOAT4X4						m_xmf4x4View;
		XMFLOAT4X4						m_xmf4x4Projection;
		XMFLOAT3						m_xmf3Position;
	};
	ID3D12Resource					*m_pd3dcbCamera{ nullptr };
	VS_CB_CAMERA_INFO				*m_pcbMappedCamera{ nullptr };

protected:

	XMFLOAT3		m_At				{ 0.0f, 0.0f, 1.0f }; // Position + Offset = At
	XMFLOAT3		m_Offset			{ 0.0f, 0.0f, 1.0f }; // Offset = At - Position

	float			m_fPitch{ 0.0f };
	float			m_fRoll{ 0.0f };
	float			m_fYaw{ 0.0f };

	float			m_fTimeLag{ 0.0f };

	XMFLOAT4X4		m_xmf4x4View{ Matrix4x4::Identity() };
	XMFLOAT4X4		m_xmf4x4Projection{ Matrix4x4::Identity() };

	D3D12_VIEWPORT	m_d3dViewport;
	D3D12_RECT		m_d3dScissorRect;


protected:  
	//����ü(���� ��ǥ��)�� �����Ѵ�.
	void GenerateFrustum();
	virtual void MoveSmoothly(float fTimeElapsed, const XMFLOAT3& xmf3LookAt) {}; 

public:
	Camera(GameObject* pOwner);
	Camera(GameObject* pOwner, Camera *pCamera);
	virtual ~Camera();

	// ���� Ŭ������ �����ؾ��ϴ� ���� ���� �Լ�

	virtual void LastUpdate(float fTimeElapsed) = 0;

	// ���� Ŭ������ �����ؾ��ϴ� ���� ���� �Լ�
	
	void CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	void UpdateShaderVariables(ID3D12GraphicsCommandList * pd3dCommandList, int parameterIndex);
	void ReleaseShaderVariables();

	virtual void Teleport(const XMFLOAT3& pos); // right, up, look�� ������ ���·� position, at�� �̵��Ѵ�.
	virtual void Move(const XMFLOAT3& Shift);
	virtual void Rotate(float x = 0.0f, float y = 0.0f, float z = 0.0f) = 0;
	
	virtual void Update(float fTimeElapsed) override { };
	virtual void Update(float fTimeElapsed, const XMFLOAT3& xmf3LookAt) {};

	//ī�޶� ��ȯ ���(�����) �� �����Ѵ�.
	void GenerateViewMatrix();
	void GenerateViewMatrix(XMFLOAT3 xmf3Position, XMFLOAT3 xmf3LookAt, XMFLOAT3 xmf3Up);
	
	// GenerateViewMatrix�� ���� ���, �ݵ�� �������� Regenterate�� ȣ���ؾ��Ѵ�.
	void RegenerateViewMatrix();

	//���� ��ȯ ����� �����Ѵ�.
	void GenerateProjectionMatrix(float fNearPlaneDistance, float fFarPlaneDistance, float fAspectRatio, float fFOVAngle);
	

	///////////////////////////////////////////////////////////////////////// Set
	void SetViewport(int xTopLeft, int yTopLeft, int nWidth, int nHeight, float fMinZ = 0.0f, float fMaxZ = 1.0f);
	void SetViewport(const D3D12_VIEWPORT& viewprot) { m_d3dViewport = viewprot; };
	void SetScissorRect(LONG xLeft, LONG yTop, LONG xRight, LONG yBottom);
	void SetScissorRect(const D3D12_RECT& scissorRect) { m_d3dScissorRect = scissorRect; };
	void SetViewportsAndScissorRects(ID3D12GraphicsCommandList *pd3dCommandList);

	void SetOffset(XMFLOAT3 offset) { m_Offset = offset; } 
	void SetTimeLag(float fTimeLag) { m_fTimeLag = fTimeLag; }
	
	void SetAt(XMFLOAT3 at) { m_At = at; };
    ///////////////////////////////////////////////////////////////////////// Set

	///////////////////////////////////////////////////////////////////////// Get
	XMFLOAT3 GetAt()             const noexcept { return (m_At); }
	XMFLOAT3 GetOffset()         const noexcept { return (m_Offset); }
	 
	float GetPitch() const noexcept { return (m_fPitch); }
	float GetRoll()	 const noexcept { return (m_fRoll); }
	float GetYaw()	 const noexcept { return (m_fYaw); }

	float GetTimeLag() const noexcept { return (m_fTimeLag); }

	XMFLOAT4X4 GetViewMatrix()		 const noexcept { return (m_xmf4x4View); }
	XMFLOAT4X4 GetProjectionMatrix() const noexcept { return (m_xmf4x4Projection); }
	D3D12_VIEWPORT GetViewport()	 const noexcept { return (m_d3dViewport); }
	D3D12_RECT GetScissorRect()		 const noexcept { return (m_d3dScissorRect); }
	///////////////////////////////////////////////////////////////////////// Get

	Camera& operator= (const Camera&);

	void ShowData() const noexcept;
};
