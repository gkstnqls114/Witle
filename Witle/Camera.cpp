#include "stdafx.h"
#include "d3dUtil.h"
#include "Transform.h"
#include "MyFrustum.h"
#include "GameScreen.h"
#include "GameObject.h"
#include "Light.h"
#include "Player.h"
#include "PlayerManager.h"
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
		//ī�޶� �̹� ������ ���� ī�޶��� ������ ���ο� ī�޶� �����Ѵ�.
		*this = *pCamera;
	}

}

Camera::~Camera()
{

}

void Camera::CreateShaderVariables(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
{
	UINT ncbElementBytes = ((sizeof(VS_CB_CAMERA_INFO) + 255) & ~255); //256�� ���
	m_pd3dcbCamera = d3dUtil::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbElementBytes, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);

	m_pd3dcbCamera->Map(0, NULL, (void **)&m_pcbMappedCamera);

	ncbElementBytes = ((sizeof(VS_CB_LIGHT_INFO) + 255) & ~255); //256�� ���
	m_pd3dcbLight = d3dUtil::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbElementBytes, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);

	m_pd3dcbLight->Map(0, NULL, (void **)&m_pcbMappedLight);
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

	m_AtOffset = Vector3::Add(m_AtOffset, atoffsetRight); // (0, 0, 0)�� ��������..  
	m_AtOffset = Vector3::Add(m_AtOffset, atoffsetUp); // (0, 0, 0)�� ��������..  
	m_AtOffset = Vector3::Add(m_AtOffset, atoffsetLook); // (0, 0, 0)�� ��������..  
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
	m_fNearPlaneDistance = fNearPlaneDistance;
	m_fFarPlaneDistance = fFarPlaneDistance;
	m_xmf4x4Projection = Matrix4x4::PerspectiveFovLH(XMConvertToRadians(fFOVAngle), fAspectRatio, fNearPlaneDistance, fFarPlaneDistance);
}

/*ī�޶� ��ȯ ����� �����Ѵ�. ī�޶��� ��ġ ����, ī�޶� �ٶ󺸴� ����, ī�޶��� Up ����(���� y-�� ����)��
�Ķ���ͷ� ����ϴ� XMMatrixLookAtLH() �Լ��� ����Ѵ�.*/
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
 
// ���� ���� �ٶ��� �ʴ� ���� �Ͽ� �����մϴ�.
XMFLOAT4X4 Camera::GenerateLightViewMatrix(const LIGHT* light) const
{
	XMFLOAT3 lookat = Vector3::Add(light->Position, light->Direction);
	// lookat ����� �������� ���� ���Ժ���, �׳� up���� (0, 1, 0)�� lookat normal�� �������� ���ϴ�.
	// ���� ���� �ٶ��� �ʴ´ٴ� ����...
	XMFLOAT3 rand_right = Vector3::CrossProduct(XMFLOAT3(0, 1, 0), Vector3::Normalize(lookat));
	
	// �ٽ� up���� ����...
	XMFLOAT3 up = Vector3::CrossProduct(lookat, rand_right);

	return Matrix4x4::LookAtLH(
		light->Position,
		lookat,
		up
	);
}

XMFLOAT4X4 Camera::GenerateLightProjectionMatrix(const LIGHT * light) const
{
	return 	Matrix4x4::OrthographicLH(GameScreen::GetClientWidth(), GameScreen::GetClientHeight(), m_fNearPlaneDistance, m_fFarPlaneDistance);;
}

void Camera::UpdateLightShaderVariables(ID3D12GraphicsCommandList * pd3dCommandList, const LIGHT* light)
{     
	//auto playerpos = PlayerManager::GetMainPlayer()->GetTransform().GetPosition();
	//auto playerlook = PlayerManager::GetMainPlayer()->GetTransform().GetLook();
	//auto boundcenter = Vector3::Add(playerpos, Vector3::ScalarProduct(playerlook, m_fFarPlaneDistance / 2.f, false));
	auto boundcenter = XMFLOAT3(15000, 0, 15000);
	mSceneBounds = BoundingSphere { boundcenter, 17000 };
	
	// Only the first "main" light casts a shadow.
	XMVECTOR lightDir = XMLoadFloat3(&light->Direction);
	XMVECTOR lightPos = -2.0f* mSceneBounds.Radius*lightDir;
	XMVECTOR targetPos = XMLoadFloat3(&mSceneBounds.Center);
	XMVECTOR lightUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMMATRIX lightView = XMMatrixLookAtLH(lightPos, targetPos, lightUp);

	// Transform bounding sphere to light space.
	XMFLOAT3 sphereCenterLS;
	XMStoreFloat3(&sphereCenterLS, XMVector3TransformCoord(targetPos, lightView));

	// Ortho frustum in light space encloses scene.
	float l = sphereCenterLS.x - mSceneBounds.Radius;
	float b = sphereCenterLS.y - mSceneBounds.Radius;
	float n = sphereCenterLS.z - mSceneBounds.Radius;
	float r = sphereCenterLS.x + mSceneBounds.Radius;
	float t = sphereCenterLS.y + mSceneBounds.Radius;
	float f = sphereCenterLS.z + mSceneBounds.Radius;
/*
	mLightNearZ = 0.1;
	mLightFarZ = 30000;*/
	XMMATRIX lightProj = XMMatrixOrthographicOffCenterLH(l, r, b, t, n, f);

	// Transform NDC space [-1,+1]^2 to texture space [0,1]^2
	XMMATRIX T(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f);

	XMMATRIX S = lightView * lightProj*T;

	XMFLOAT4X4 xmf4x4LightView;
	xmf4x4LightView = GenerateLightViewMatrix(light);
	XMStoreFloat4x4(&xmf4x4LightView, XMMatrixTranspose(lightView));
	::memcpy(&m_pcbMappedLight->m_xmf4x4LightView, &xmf4x4LightView, sizeof(XMFLOAT4X4));

	XMFLOAT4X4 xmf4x4Projection;
	xmf4x4Projection = GenerateLightProjectionMatrix(light);
	XMStoreFloat4x4(&xmf4x4Projection, XMMatrixTranspose(lightProj));
	::memcpy(&m_pcbMappedLight->m_xmf4x4LightProjection, &xmf4x4Projection, sizeof(XMFLOAT4X4));
	
	XMFLOAT4X4 xmf4x4LightTransform; 
	XMStoreFloat4x4(&xmf4x4LightTransform, XMMatrixTranspose(S));
	::memcpy(&m_pcbMappedLight->m_xmf4x4LightTransform, &xmf4x4LightTransform, sizeof(XMFLOAT4X4));


	D3D12_GPU_VIRTUAL_ADDRESS d3dGpuVirtualAddress = m_pd3dcbLight->GetGPUVirtualAddress();
	pd3dCommandList->SetGraphicsRootConstantBufferView(ROOTPARAMETER_LIGHTFORSHADOW, d3dGpuVirtualAddress);
}

void Camera::SetViewportsAndScissorRects(ID3D12GraphicsCommandList *pd3dCommandList)
{
	pd3dCommandList->RSSetViewports(1, &m_d3dViewport);
	pd3dCommandList->RSSetScissorRects(1, &m_d3dScissorRect);
}

Camera & Camera::operator=(const Camera & camera)
{
	// TODO: ���⿡ ��ȯ ������ �����մϴ�.
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
