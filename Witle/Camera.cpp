#include "stdafx.h"
#include "d3dUtil.h" 
#include "GameObject.h"
#include "Camera.h"

Camera::Camera()
{
	m_ShaderVariables = new RootConstants<XMFLOAT4X4>(1, 2);
	m_FamillyID = "Camera";
}

Camera::Camera(Camera *pCamera)
{
	if (pCamera)
	{
		//카메라가 이미 있으면 기존 카메라의 정보를 새로운 카메라에 복사한다.
		*this = *pCamera;
	}

}

Camera::~Camera()
{
	if (m_ShaderVariables)
	{
		delete m_ShaderVariables;
		m_ShaderVariables = nullptr;
	}
}

void Camera::Teleport(const XMFLOAT3 & pos)
{
	m_Position = pos;  
	m_At = Vector3::Add(m_Position, m_Offset);
	m_xmf4x4View = Matrix4x4::LookAtLH(m_Position, m_At, m_Up);
}

void Camera::Move(const XMFLOAT3 & Shift)
{
	m_At.x += Shift.x;
	m_At.y += Shift.y;
	m_At.z += Shift.z;
}

void Camera::Rotate(float x, float y, float z)
{ 
	if ((x != 0.0f))
	{
		//플레이어의 로컬 x-축에 대한 x 각도의 회전 행렬을 계산한다.
		XMFLOAT3 xmf3Right = m_Right;
		XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&xmf3Right), XMConvertToRadians(x));
		//카메라의 로컬 x-축, y-축, z-축을 회전한다.
		m_Right = Vector3::TransformNormal(m_Right, xmmtxRotate);
		m_Up = Vector3::TransformNormal(m_Up, xmmtxRotate);
		m_Look = Vector3::TransformNormal(m_Look, xmmtxRotate);

		// Position에서 At으로 향하는 Offset 을 회전한다.
		//XMFLOAT3 PosToAt_Offset = Vector3::ScalarProduct(m_Offset, -1, false);
		//m_Offset = Vector3::TransformCoord(PosToAt_Offset, xmmtxRotate);
		//m_At = Vector3::Add(m_Position, m_Offset);

		// 다시 At 에서 Position으로 향하게 만들어준다.
		//m_Offset = Vector3::ScalarProduct(m_Offset, -1, false);
	}
	if ((y != 0.0f))
	{
		XMFLOAT3 xmf3Up = m_Up;
		XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&xmf3Up), XMConvertToRadians(y));
		m_Right = Vector3::TransformNormal(m_Right, xmmtxRotate);
		m_Up = Vector3::TransformNormal(m_Up, xmmtxRotate);
		m_Look = Vector3::TransformNormal(m_Look, xmmtxRotate);

		//// Position에서 At으로 향하는 Offset 을 회전한다.
		//XMFLOAT3 PosToAt_Offset = Vector3::ScalarProduct(m_Offset, -1, false);
		//m_Offset = Vector3::TransformCoord(PosToAt_Offset, xmmtxRotate);
		//m_At = Vector3::Add(m_Position, m_Offset);

		//// 다시 At 에서 Position으로 향하게 만들어준다.
		//m_Offset = Vector3::ScalarProduct(m_Offset, -1, false);
	}
	if (z != 0.0f)
	{
		XMFLOAT3 xmf3Look = m_Look;
		XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&xmf3Look), XMConvertToRadians(z));
		m_Right = Vector3::TransformNormal(m_Right, xmmtxRotate);
		m_Up = Vector3::TransformNormal(m_Up, xmmtxRotate);
		m_Look = Vector3::TransformNormal(m_Look, xmmtxRotate);

		//// Position에서 At으로 향하는 Offset 을 회전한다.
		//XMFLOAT3 PosToAt_Offset = Vector3::ScalarProduct(m_Offset, -1, false);
		//m_Offset = Vector3::TransformCoord(PosToAt_Offset, xmmtxRotate);
		//m_At = Vector3::Add(m_Position, m_Offset);

		//// 다시 At 에서 Position으로 향하게 만들어준다.
		//m_Offset = Vector3::ScalarProduct(m_Offset, -1, false);
	}

	/*회전으로 인해 플레이어의 로컬 x-축, y-축, z-축이 서로 직교하지 않을 수 있으므로 z-축(LookAt 벡터)을 기준으
	로 하여 서로 직교하고 단위벡터가 되도록 한다.*/
	m_Look = Vector3::Normalize(m_Look);
	m_Right = Vector3::CrossProduct(m_Up, m_Look, true);
	m_Up = Vector3::CrossProduct(m_Look, m_Right, true);
}

void Camera::RegenerateAt()
{
}
 
 
void Camera::LastUpdate()
{
	// 물리 계산
	// 벽과의 충돌 등...

	//Move, Rotate된 At과 카메라 좌표축(Right, Up, Look)을 기준으로 Position 재설정
	m_Position = Vector3::Subtract(m_At, m_Offset);

	// 카메라 변환 행렬을 재설정한다.
	RegenerateViewMatrix();

	// 카메라 변환 행렬이 바뀔 때마다 카메라 절두체를 다시 생성한다(절두체는 월드 좌표계로 생성한다).
	GenerateFrustum();
}
 
void Camera::GenerateFrustum()
{
	//원근 투영 변환 행렬에서 절두체를 생성한다(절두체는 카메라 좌표계로 표현된다).
	m_xmFrustum.CreateFromMatrix(m_xmFrustum, XMLoadFloat4x4(&m_xmf4x4Projection));
	//카메라 변환 행렬의 역행렬을 구한다.
	XMMATRIX xmmtxInversView = XMMatrixInverse(NULL, XMLoadFloat4x4(&m_xmf4x4View));
	//절두체를 카메라 변환 행렬의 역행렬로 변환한다(이제 절두체는 월드 좌표계로 표현된다).
	m_xmFrustum.Transform(m_xmFrustum, xmmtxInversView);


}


bool Camera::IsInFrustum(const BoundingOrientedBox & xmBoundingBox) const
{
	return (m_xmFrustum.Intersects(xmBoundingBox));
}

bool Camera::IsInFrustum(const BoundingBox & xmBoundingBox) const
{
	return (m_xmFrustum.Intersects(xmBoundingBox));
}

bool Camera::IsInBoudingBox(const BoundingOrientedBox & xmBoundingBox) const
{
	return m_xmBoundingBox.Intersects(xmBoundingBox);
}

bool Camera::IsInBoudingBox(const BoundingBox & xmBoundingBox) const
{
	return m_xmBoundingBox.Intersects(xmBoundingBox);
}

bool Camera::IsInSphere(const BoundingOrientedBox & xmBoundingBox) const
{
	return m_xmSphere.Intersects(xmBoundingBox);
}

bool Camera::IsInSphere(const BoundingBox & xmBoundingBox) const
{
	return m_xmSphere.Intersects(xmBoundingBox);
}

bool Camera::IsInSphere(const BoundingSphere & xmBoundingBox) const
{
	return m_xmSphere.Intersects(xmBoundingBox);
}


void Camera::SetViewport(int xTopLeft, int yTopLeft, int nWidth, int nHeight, float
	fMinZ, float fMaxZ)
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
	// m_xmf4x4View = Matrix4x4::LookAtLH(m_Position, Vector3::Add(m_Position, m_Look), m_Up);
	m_xmf4x4View = Matrix4x4::LookAtLH(m_Position, m_At, m_Up);

	m_Right = XMFLOAT3(m_xmf4x4View._11, m_xmf4x4View._21, m_xmf4x4View._31);
	m_Up = XMFLOAT3(m_xmf4x4View._12, m_xmf4x4View._22, m_xmf4x4View._32);
	m_Look = XMFLOAT3(m_xmf4x4View._13, m_xmf4x4View._23, m_xmf4x4View._33);


	//m_Look = Vector3::Normalize(m_Look);
	//m_Right = Vector3::CrossProduct(m_Up, m_Look, true);
	//m_Up = Vector3::CrossProduct(m_Look, m_Right, true);

	//m_xmf4x4View._11 = m_Right.x; m_xmf4x4View._12 = m_Up.x; m_xmf4x4View._13 = m_Look.x;
	//m_xmf4x4View._21 = m_Right.y; m_xmf4x4View._22 = m_Up.y; m_xmf4x4View._23 = m_Look.y;
	//m_xmf4x4View._31 = m_Right.z; m_xmf4x4View._32 = m_Up.z; m_xmf4x4View._33 = m_Look.z;

	//m_xmf4x4View._41 = -Vector3::DotProduct(m_Position, m_Right);
	//m_xmf4x4View._42 = -Vector3::DotProduct(m_Position, m_Up);
	//m_xmf4x4View._43 = -Vector3::DotProduct(m_Position, m_Look);
}

void Camera::GenerateProjectionMatrix(float fNearPlaneDistance, float fFarPlaneDistance, float fAspectRatio, float fFOVAngle)
{
	m_xmf4x4Projection = Matrix4x4::PerspectiveFovLH(XMConvertToRadians(fFOVAngle), fAspectRatio, fNearPlaneDistance, fFarPlaneDistance);
}

/*카메라 변환 행렬을 생성한다. 카메라의 위치 벡터, 카메라가 바라보는 지점, 카메라의 Up 벡터(로컬 y-축 벡터)를
파라메터로 사용하는 XMMatrixLookAtLH() 함수를 사용한다.*/
void Camera::GenerateViewMatrix()
{
	m_xmf4x4View = Matrix4x4::LookAtLH(m_Position, m_At, m_Up);
}

void Camera::GenerateViewMatrix(XMFLOAT3 xmf3Position, XMFLOAT3 xmf3LookAt, XMFLOAT3 xmf3Up)
{
	m_Position = xmf3Position;
	m_At = xmf3LookAt;
	m_Up = xmf3Up;

	GenerateViewMatrix();
}
//
//void Camera::CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
//{
//	// PPT 파이프라인 02 - 68p
//	HRESULT hResult = S_FALSE;
//
//	// 자원 생성
//	UINT HardwareSize = d3dUtil::CalcConstantBufferByteSize(sizeof(VS_CB_MYCAMERA_INFO)); // 256의 배수
//
//	// 힙 속성
//	D3D12_HEAP_PROPERTIES HeapProperties;
//	::ZeroMemory(&HeapProperties, sizeof(D3D12_HEAP_PROPERTIES));
//	HeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
//	HeapProperties.CreationNodeMask = 1;
//	HeapProperties.VisibleNodeMask = 1;
//
//	D3D12_RESOURCE_DESC ResourceDesc;
//	::ZeroMemory(&ResourceDesc, sizeof(D3D12_RESOURCE_DESC));
//	ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
//	ResourceDesc.Width = HardwareSize; // 256의 배수
//	ResourceDesc.Height = ResourceDesc.SampleDesc.Count = 1;
//	ResourceDesc.DepthOrArraySize = ResourceDesc.MipLevels = 1;
//	ResourceDesc.Format = DXGI_FORMAT_UNKNOWN;
//	ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR; // 행우선
//
//														  // 자원을 생성한다.
//	hResult = pd3dDevice->CreateCommittedResource(
//		&HeapProperties,
//		D3D12_HEAP_FLAG_NONE,
//		&ResourceDesc,
//		D3D12_RESOURCE_STATE_GENERIC_READ, // 업로드힙의 초기상태
//		nullptr,
//		IID_PPV_ARGS(&m_d3dCameraCBVUpload));
//
//	assert(hResult == S_OK);
//#if defined(_DEBUG)
//	OutputDebugString(L"...CreateCommittedResource\n");
//#endif
//
//	hResult = m_d3dCameraCBVUpload->Map(0, nullptr, (void**)&m_pMappedCameraInfo);
//	assert(hResult == S_OK);
//#if defined(_DEBUG)
//	OutputDebugString(L"...Constant Buffer Map\n");
//#endif
//
//	::memcpy(&m_pMappedCameraInfo->m_xmf4x4View, &m_xmf4x4View, sizeof(XMFLOAT4X4));
//	::memcpy(&m_pMappedCameraInfo->m_xmf4x4Projection, &m_xmf4x4Projection, sizeof(XMFLOAT4X4));
//
//	// 서술자 힙을 생성한다.
//	D3D12_DESCRIPTOR_HEAP_DESC d3dCBVHeap;
//	::ZeroMemory(&d3dCBVHeap, sizeof(D3D12_DESCRIPTOR_HEAP_DESC));
//	d3dCBVHeap.NumDescriptors = 1; // 현재 서술자는 한개만 연결한다.
//	d3dCBVHeap.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV; // 힙타입 CBV , SRV, UAV
//	d3dCBVHeap.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE; //쉐이더가 해당 자원을 볼 수 있다.
//	d3dCBVHeap.NodeMask = 0;
//	hResult = pd3dDevice->CreateDescriptorHeap(&d3dCBVHeap, IID_PPV_ARGS(&m_d3dCameraCBVDescriptorHeap));
//	assert(hResult == S_OK);
//#if defined(_DEBUG)
//	OutputDebugString(L"...Create CBV Descriptor Heap\n");
//#endif
//
//	////////////// 상수버퍼뷰 생성
//	// ppt 파이프라인 02 - 69p
//
//	// 상수버퍼뷰를 생성하는 것은 CPU(응용프로그램), 사용하는 것은 GPU(쉐이더)
//	D3D12_CPU_DESCRIPTOR_HANDLE d3dCPUdescriptorHandle =
//		m_d3dCameraCBVDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
//	D3D12_GPU_VIRTUAL_ADDRESS d3dGPUVirtualAddress =
//		m_d3dCameraCBVUpload->GetGPUVirtualAddress();
//
//	// 상수버퍼 뷰 구조체
//	D3D12_CONSTANT_BUFFER_VIEW_DESC CBVdesc;
//	::ZeroMemory(&CBVdesc, sizeof(D3D12_CONSTANT_BUFFER_VIEW_DESC));
//	CBVdesc.BufferLocation = d3dGPUVirtualAddress;
//	CBVdesc.SizeInBytes = HardwareSize;
//
//	pd3dDevice->CreateConstantBufferView(
//		&CBVdesc,
//		d3dCPUdescriptorHandle // 상수버퍼뷰를 포함하는 서술자 힙의 시작
//	);
//}

void Camera::UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList)
{
	XMFLOAT4X4 xmf4x4View;
	XMStoreFloat4x4(&xmf4x4View, XMMatrixTranspose(XMLoadFloat4x4(&m_xmf4x4View)));
	//::memcpy(&m_pMappedCameraInfo->m_xmf4x4View, &xmf4x4View, sizeof(XMFLOAT4X4));

	XMFLOAT4X4 xmf4x4Projection;
	XMStoreFloat4x4(&xmf4x4Projection, XMMatrixTranspose(XMLoadFloat4x4(&m_xmf4x4Projection)));
	//::memcpy(&m_pMappedCameraInfo->m_xmf4x4Projection, &xmf4x4Projection, sizeof(XMFLOAT4X4));

	////카메라 위치 복사
	//::memcpy(&m_pMappedCameraInfo->m_xmf3Position, &m_Position, sizeof(XMFLOAT3));

	//pd3dCommandList->SetGraphicsRootConstantBufferView(1, m_d3dCameraCBVUpload->GetGPUVirtualAddress());
	// pd3dCommandList->SetGraphicsRoot32BitConstants(1, 16, &xmf4x4View, 0);
	//pd3dCommandList->SetGraphicsRoot32BitConstants(1, 16, &xmf4x4Projection, 16);
	XMFLOAT4X4 CameraMatrixs[2] = { xmf4x4View , xmf4x4Projection };

	if (m_ShaderVariables)
	{
		static_cast<RootConstants<XMFLOAT4X4> *>(m_ShaderVariables)->UpdateShaderVariables(pd3dCommandList, CameraMatrixs);
	}
}

//void Camera::ReleaseShaderVariables()
//{
//
//}

void Camera::SetViewportsAndScissorRects(ID3D12GraphicsCommandList *pd3dCommandList)
{
	pd3dCommandList->RSSetViewports(1, &m_d3dViewport);
	pd3dCommandList->RSSetScissorRects(1, &m_d3dScissorRect);
}

void Camera::ShowData() const noexcept
{
#if _DEBUG
	std::cout << "Camera Data..." << std::endl;
	std::cout << "m_xmf3Position: "; Vector3::Show(m_Position);
	std::cout << "m_xmf3Right: "; Vector3::Show(m_Right);
	std::cout << "m_xmf3Up: "; Vector3::Show(m_Up);
	std::cout << "m_xmf3Look: "; Vector3::Show(m_Look);
	std::cout << "m_xmf3LookAtWorld: "; Vector3::Show(m_At);
	std::cout << "m_Offset: "; Vector3::Show(m_Offset);
	std::cout << "m_xmf4x4View" << std::endl;
	Matrix4x4::Show(m_xmf4x4View);
	std::cout << "Camera Data..." << std::endl;
#endif
}
