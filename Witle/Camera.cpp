#include "stdafx.h"
#include "d3dUtil.h"
#include "Transform.h"
#include "MyFrustum.h"
#include "GameScreen.h"
#include "GameObject.h"
#include "Light.h"
#include "Camera.h"

void Camera::ReleaseObjects()
{ 
	if (m_pFrustum)
	{
		delete m_pFrustum;
		m_pFrustum = nullptr;
	}
	ReleaseShaderVariables();
}

void Camera::ReleaseUploadBuffers()
{

}

Camera::Camera(GameObject* pOwner, const XMFLOAT3& AtOffset)
	: ComponentBase(pOwner)
{
	m_d3dViewport = D3D12_VIEWPORT{ 0.0f, 0.0f, static_cast<FLOAT>(GameScreen::GetWidth()) , static_cast<FLOAT>(GameScreen::GetHeight()), 0.0f, 1.0f };
	m_AtOffset = AtOffset;
	m_DebugAtOffset = AtOffset;
	m_d3dScissorRect = D3D12_RECT{ 0, 0, static_cast<LONG>(GameScreen::GetWidth()) ,static_cast<LONG>(GameScreen::GetHeight()) };
	m_FamilyID.InitCamera();
	m_pFrustum = new MyFrustum(pOwner);
}

Camera::Camera(GameObject* pOwner, Camera *pCamera)
	: ComponentBase(pOwner)
{
	if (pCamera)
	{
		//카메라가 이미 있으면 기존 카메라의 정보를 새로운 카메라에 복사한다.
		*this = *pCamera;
	}

}

Camera::~Camera()
{

}

void Camera::CreateShaderVariables(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
{
	UINT ncbElementBytes = ((sizeof(VS_CB_CAMERA_INFO) + 255) & ~255); //256의 배수
	m_pd3dcbCamera = d3dUtil::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbElementBytes, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);

	m_pd3dcbCamera->Map(0, NULL, (void **)&m_pcbMappedCamera);
}

void Camera::UpdateShaderVariables(ID3D12GraphicsCommandList * pd3dCommandList, int parameterIndex)
{
	XMFLOAT4X4 xmf4x4View;
	XMStoreFloat4x4(&xmf4x4View, XMMatrixTranspose(XMLoadFloat4x4(&m_xmf4x4View)));
	::memcpy(&m_pcbMappedCamera->m_xmf4x4View, &xmf4x4View, sizeof(XMFLOAT4X4));

	XMFLOAT4X4 xmf4x4Projection;
	XMStoreFloat4x4(&xmf4x4Projection, XMMatrixTranspose(XMLoadFloat4x4(&m_xmf4x4Projection)));
	::memcpy(&m_pcbMappedCamera->m_xmf4x4Projection, &xmf4x4Projection, sizeof(XMFLOAT4X4));

	::memcpy(&m_pcbMappedCamera->m_xmf3Position, &m_pOwner->GetTransform().GetPosition(), sizeof(XMFLOAT3));

	D3D12_GPU_VIRTUAL_ADDRESS d3dGpuVirtualAddress = m_pd3dcbCamera->GetGPUVirtualAddress();
	pd3dCommandList->SetGraphicsRootConstantBufferView(ROOTPARAMETER_CAMERA, d3dGpuVirtualAddress);
}

void Camera::ReleaseShaderVariables()
{
	if (m_pd3dcbCamera)
	{
		m_pd3dcbCamera->Unmap(0, NULL);
		m_pd3dcbCamera->Release(); 
	}
}

void Camera::MoveAtOffset(const XMFLOAT3 & Shift)
{ 
	Vector3::Add(m_DebugAtOffset, Shift);

	XMFLOAT3 right = m_pOwner->GetTransform().GetRight();
	XMFLOAT3 up = m_pOwner->GetTransform().GetUp();
	XMFLOAT3 look = m_pOwner->GetTransform().GetLook();

	XMFLOAT3 atoffsetRight = Vector3::ScalarProduct(right, Shift.x, false);
	XMFLOAT3 atoffsetUp = Vector3::ScalarProduct(up, Shift.y, false);
	XMFLOAT3 atoffsetLook = Vector3::ScalarProduct(look, Shift.z, false);

	m_AtOffset = Vector3::Add(m_AtOffset, atoffsetRight); // (0, 0, 0)을 기준으로..  
	m_AtOffset = Vector3::Add(m_AtOffset, atoffsetUp); // (0, 0, 0)을 기준으로..  
	m_AtOffset = Vector3::Add(m_AtOffset, atoffsetLook); // (0, 0, 0)을 기준으로..  
}

void Camera::ZoomIn(float value)
{
	if (value >= 1) return;
	m_Offset = Vector3::ScalarProduct(m_Offset, value, false);
}

void Camera::ZoomOut(float value)
{
	if (value <= 1) return;

	m_Offset = Vector3::ScalarProduct(m_Offset, value, false);
}
 
void Camera::Teleport(const XMFLOAT3 & pos)
{
	m_pOwner->GetTransform().SetPosition(pos);
	m_At = Vector3::Add(m_pOwner->GetTransform().GetPosition(), m_Offset);
	m_xmf4x4View = Matrix4x4::LookAtLH(m_pOwner->GetTransform().GetPosition(), m_At, m_pOwner->GetTransform().GetUp());
}

void Camera::Move(const XMFLOAT3 & Shift)
{
	m_At.x += Shift.x;
	m_At.y += Shift.y;
	m_At.z += Shift.z;
}

void Camera::SetViewport(int xTopLeft, int yTopLeft, int nWidth, int nHeight, float fMinZ, float fMaxZ)
{
	m_d3dViewport.TopLeftX = float(xTopLeft);
	m_d3dViewport.TopLeftY = float(yTopLeft);
	m_d3dViewport.Width = float(nWidth);
	m_d3dViewport.Height = float(nHeight);
	m_d3dViewport.MinDepth = fMinZ;
	m_d3dViewport.MaxDepth = fMaxZ;
}

void Camera::SetScissorRect(LONG xLeft, LONG yTop, LONG xRight, LONG yBottom)
{
	m_d3dScissorRect.left = xLeft;
	m_d3dScissorRect.top = yTop;
	m_d3dScissorRect.right = xRight;
	m_d3dScissorRect.bottom = yBottom;
}

void Camera::RegenerateViewMatrix()
{ 
	m_xmf4x4View = Matrix4x4::LookAtLH(m_pOwner->GetTransform().GetPosition(), m_At, m_pOwner->GetTransform().GetUp());

	m_pOwner->GetTransform().SetRight(XMFLOAT3(m_xmf4x4View._11, m_xmf4x4View._21, m_xmf4x4View._31)); 
	m_pOwner->GetTransform().SetUp(XMFLOAT3(m_xmf4x4View._12, m_xmf4x4View._22, m_xmf4x4View._32));
	m_pOwner->GetTransform().SetLook(XMFLOAT3(m_xmf4x4View._13, m_xmf4x4View._23, m_xmf4x4View._33));

	m_pFrustum->GenerateFrustum(m_xmf4x4Projection, m_xmf4x4View);
}

void Camera::GenerateProjectionMatrix(float fNearPlaneDistance, float fFarPlaneDistance, float fAspectRatio, float fFOVAngle)
{
	m_xmf4x4Projection = Matrix4x4::PerspectiveFovLH(XMConvertToRadians(fFOVAngle), fAspectRatio, fNearPlaneDistance, fFarPlaneDistance);
}

/*카메라 변환 행렬을 생성한다. 카메라의 위치 벡터, 카메라가 바라보는 지점, 카메라의 Up 벡터(로컬 y-축 벡터)를
파라메터로 사용하는 XMMatrixLookAtLH() 함수를 사용한다.*/
void Camera::GenerateViewMatrix()
{
	m_xmf4x4View = Matrix4x4::LookAtLH(m_pOwner->GetTransform().GetPosition(), m_At, m_pOwner->GetTransform().GetUp());
}

void Camera::GenerateViewMatrix(XMFLOAT3 xmf3Position, XMFLOAT3 xmf3LookAt, XMFLOAT3 xmf3Up)
{
	m_pOwner->GetTransform().SetPosition(xmf3Position);
	m_At = xmf3LookAt;
	m_pOwner->GetTransform().SetUp(xmf3Up) ;

	GenerateViewMatrix();
}
 
// 빛이 위를 바라보지 않는 가정 하에 진행합니다.
XMFLOAT4X4 Camera::GenerateLightViewMatrix(const LIGHT* light) const
{
	XMFLOAT3 lookat = Vector3::Add(light->Position, light->Direction);
	// lookat 방향과 평행하지 않은 정규벡터, 그냥 up벡터 (0, 1, 0)와 lookat normal의 외적으로 갑니다.
	// 빛이 위를 바라보지 않는다는 가정...
	XMFLOAT3 randomnormal = Vector3::CrossProduct(XMFLOAT3(0, 1, 0), Vector3::Normalize(lookat));

	return Matrix4x4::LookAtLH(
		light->Position,
		lookat,
		randomnormal
	);
}

void Camera::UpdateLightShaderVariables(ID3D12GraphicsCommandList * pd3dCommandList, const LIGHT* light) const
{
	XMFLOAT4X4 xmf4x4LightView;
	xmf4x4LightView = GenerateLightViewMatrix(light);
	XMStoreFloat4x4(&xmf4x4LightView, XMMatrixTranspose(XMLoadFloat4x4(&m_xmf4x4View)));
	::memcpy(&m_pcbMappedCamera->m_xmf4x4View, &xmf4x4LightView, sizeof(XMFLOAT4X4));

	XMFLOAT4X4 xmf4x4Projection;
	XMStoreFloat4x4(&xmf4x4Projection, XMMatrixTranspose(XMLoadFloat4x4(&m_xmf4x4Projection)));
	::memcpy(&m_pcbMappedCamera->m_xmf4x4Projection, &xmf4x4Projection, sizeof(XMFLOAT4X4));

	::memcpy(&m_pcbMappedCamera->m_xmf3Position, &light->Position, sizeof(XMFLOAT3));

	D3D12_GPU_VIRTUAL_ADDRESS d3dGpuVirtualAddress = m_pd3dcbCamera->GetGPUVirtualAddress();
	pd3dCommandList->SetGraphicsRootConstantBufferView(ROOTPARAMETER_CAMERA, d3dGpuVirtualAddress);
}

void Camera::SetViewportsAndScissorRects(ID3D12GraphicsCommandList *pd3dCommandList)
{
	pd3dCommandList->RSSetViewports(1, &m_d3dViewport);
	pd3dCommandList->RSSetScissorRects(1, &m_d3dScissorRect);
}

Camera & Camera::operator=(const Camera & camera)
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	m_AtOffset = camera.m_AtOffset; 
	m_At = camera.m_At;
	m_Offset = camera.m_Offset;

	m_fPitch = camera.m_fPitch;
	m_fRoll = camera.m_fRoll;
	m_fYaw = camera.m_fYaw;

	m_fTimeLag = camera.m_fTimeLag;

	m_xmf4x4View = camera.m_xmf4x4View;
	m_xmf4x4Projection = camera.m_xmf4x4Projection;

	m_d3dViewport = camera.m_d3dViewport;
	m_d3dScissorRect = camera.m_d3dScissorRect;

	return *this;
}

void Camera::ShowData() const noexcept
{
#if _DEBUG
	std::cout << "Camera Data..." << std::endl;
	std::cout << "m_xmf3Position: "; Vector3::Show(m_pOwner->GetTransform().GetPosition());
	std::cout << "m_xmf3Right: "; Vector3::Show(m_pOwner->GetTransform().GetRight());
	std::cout << "m_xmf3Up: "; Vector3::Show(m_pOwner->GetTransform().GetUp());
	std::cout << "m_xmf3Look: "; Vector3::Show(m_pOwner->GetTransform().GetLook());
	std::cout << "m_xmf3LookAtWorld: "; Vector3::Show(m_At);
	std::cout << "m_Offset: "; Vector3::Show(m_Offset);
	std::cout << "m_xmf4x4View" << std::endl;
	Matrix4x4::Show(m_xmf4x4View);
	std::cout << "Camera Data..." << std::endl;
#endif
}
