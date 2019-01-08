#include "stdafx.h"
#include "d3dUtil.h" 
#include "GameObject.h"
#include "Camera.h"

Camera::Camera()
{
}

Camera::Camera(Camera *pCamera)
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

void Camera::Move(const XMFLOAT3 & Shift)
{
	if (m_pLookingObject) {
		MoveAroundLookingobject(Shift);
	}
	else {
		MoveAroundCamera(Shift);
	}
}

void Camera::Rotate(float x, float y, float z)
{
	if (m_pLookingObject) {
		RotateOnLookingobject(x, y, z);
	}
	else {
		RotateOnCamera(x, y, z);
	}
}

void Camera::RegenerateAt()
{
}

void Camera::Update(float fTimeElapsed)
{
	// ���� ���
	// ������ �浹 ��...

	//Move, Rotate�� At�� ī�޶� ��ǥ��(Right, Up, Look)�� �������� Position �缳��
	m_Position = Vector3::Subtract(m_At, m_Offset);

	// ī�޶� ��ȯ ����� �缳���Ѵ�.
	RegenerateViewMatrix();

	// ī�޶� ��ȯ ����� �ٲ� ������ ī�޶� ����ü�� �ٽ� �����Ѵ�(����ü�� ���� ��ǥ��� �����Ѵ�).
	GenerateFrustum();
}

void Camera::ZoomIn(float val)
{
	m_Offset = Vector3::ScalarProduct(m_Offset, Vector3::Length(m_Offset) * val);
}

void Camera::ZoomOut(float val)
{
	m_Offset = Vector3::ScalarProduct(m_Offset, Vector3::Length(m_Offset) * val);
 
}

void Camera::GenerateFrustum()
{
	//���� ���� ��ȯ ��Ŀ��� ����ü�� �����Ѵ�(����ü�� ī�޶� ��ǥ��� ǥ���ȴ�).
	m_xmFrustum.CreateFromMatrix(m_xmFrustum, XMLoadFloat4x4(&m_xmf4x4Projection));
	//ī�޶� ��ȯ ����� ������� ���Ѵ�.
	XMMATRIX xmmtxInversView = XMMatrixInverse(NULL, XMLoadFloat4x4(&m_xmf4x4View));
	//����ü�� ī�޶� ��ȯ ����� ����ķ� ��ȯ�Ѵ�(���� ����ü�� ���� ��ǥ��� ǥ���ȴ�).
	m_xmFrustum.Transform(m_xmFrustum, xmmtxInversView);


}

void Camera::MoveAroundLookingobject(const XMFLOAT3& Shift)
{
	m_At.x += Shift.x;
	m_At.y += Shift.y;
	m_At.z += Shift.z;
}

void Camera::MoveAroundCamera(const XMFLOAT3& Shift)
{
	m_At.x += Shift.x;
	m_At.y += Shift.y;
	m_At.z += Shift.z;
}

void Camera::RotateOnCamera(float x, float y, float z)
{
	if ((x != 0.0f))
	{
		//�÷��̾��� ���� x-�࿡ ���� x ������ ȸ�� ����� ����Ѵ�.
		XMFLOAT3 xmf3Right = m_Right;
		XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&xmf3Right), XMConvertToRadians(x));
		//ī�޶��� ���� x-��, y-��, z-���� ȸ���Ѵ�.
		m_Right = Vector3::TransformNormal(m_Right, xmmtxRotate);
		m_Up = Vector3::TransformNormal(m_Up, xmmtxRotate);
		m_Look = Vector3::TransformNormal(m_Look, xmmtxRotate);

		// Position���� At���� ���ϴ� Offset �� ȸ���Ѵ�.
		XMFLOAT3 PosToAt_Offset = Vector3::ScalarProduct(m_Offset, -1, false);
		m_Offset = Vector3::TransformCoord(PosToAt_Offset, xmmtxRotate);
		m_At = Vector3::Add(m_Position, m_Offset);

		// �ٽ� At ���� Position���� ���ϰ� ������ش�.
		m_Offset = Vector3::ScalarProduct(m_Offset, -1, false);
	}
	if ((y != 0.0f))
	{
		XMFLOAT3 xmf3Up = m_Up;
		XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&xmf3Up), XMConvertToRadians(y));
		m_Right = Vector3::TransformNormal(m_Right, xmmtxRotate);
		m_Up = Vector3::TransformNormal(m_Up, xmmtxRotate);
		m_Look = Vector3::TransformNormal(m_Look, xmmtxRotate);

		// Position���� At���� ���ϴ� Offset �� ȸ���Ѵ�.
		XMFLOAT3 PosToAt_Offset = Vector3::ScalarProduct(m_Offset, -1, false);
		m_Offset = Vector3::TransformCoord(PosToAt_Offset, xmmtxRotate);
		m_At = Vector3::Add(m_Position, m_Offset);

		// �ٽ� At ���� Position���� ���ϰ� ������ش�.
		m_Offset = Vector3::ScalarProduct(m_Offset, -1, false);
	}
	if (z != 0.0f)
	{
		XMFLOAT3 xmf3Look = m_Look;
		XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&xmf3Look), XMConvertToRadians(z));
		m_Right = Vector3::TransformNormal(m_Right, xmmtxRotate);
		m_Up = Vector3::TransformNormal(m_Up, xmmtxRotate);
		m_Look = Vector3::TransformNormal(m_Look, xmmtxRotate);

		// Position���� At���� ���ϴ� Offset �� ȸ���Ѵ�.
		XMFLOAT3 PosToAt_Offset = Vector3::ScalarProduct(m_Offset, -1, false);
		m_Offset = Vector3::TransformCoord(PosToAt_Offset, xmmtxRotate);
		m_At = Vector3::Add(m_Position, m_Offset);

		// �ٽ� At ���� Position���� ���ϰ� ������ش�.
		m_Offset = Vector3::ScalarProduct(m_Offset, -1, false);
	}

	/*ȸ������ ���� �÷��̾��� ���� x-��, y-��, z-���� ���� �������� ���� �� �����Ƿ� z-��(LookAt ����)�� ������
	�� �Ͽ� ���� �����ϰ� �������Ͱ� �ǵ��� �Ѵ�.*/
	m_Look = Vector3::Normalize(m_Look);
	m_Right = Vector3::CrossProduct(m_Up, m_Look, true);
	m_Up = Vector3::CrossProduct(m_Look, m_Right, true);
}

void Camera::RotateOnLookingobject(float x, float y, float z)
{
	//DWORD nCameraMode = m_pCamera->GetMode();
	////1��Ī ī�޶� �Ǵ� 3��Ī ī�޶��� ��� �÷��̾��� ȸ���� �ణ�� ������ ������.
	//if ((nCameraMode == FIRST_PERSON_CAMERA) || (nCameraMode == THIRD_PERSON_CAMERA))
	//{
	//	/*���� x-���� �߽����� ȸ���ϴ� ���� ���� �յڷ� ���̴� ���ۿ� �ش��Ѵ�. �׷��Ƿ� x-���� �߽����� ȸ���ϴ�
	//	������ -89.0~+89.0�� ���̷� �����Ѵ�. x�� ������ m_fPitch���� ���� ȸ���ϴ� �����̹Ƿ� x��ŭ ȸ���� ����
	//	Pitch�� +89�� ���� ũ�ų� -89�� ���� ������ m_fPitch�� +89�� �Ǵ� -89���� �ǵ��� ȸ������(x)�� �����Ѵ�.*/
	//	if (x != 0.0f)
	//	{
	//		m_fPitch += x;
	//		if (m_fPitch > +89.0f) { x -= (m_fPitch - 89.0f); m_fPitch = +89.0f; }
	//		if (m_fPitch < -89.0f) { x -= (m_fPitch + 89.0f); m_fPitch = -89.0f; }
	//	}
	//	if (y != 0.0f)
	//	{
	//		//���� y-���� �߽����� ȸ���ϴ� ���� ������ ������ ���̹Ƿ� ȸ�� ������ ������ ����.
	//		m_fYaw += y;
	//		if (m_fYaw > 360.0f) m_fYaw -= 360.0f;
	//		if (m_fYaw < 0.0f) m_fYaw += 360.0f;
	//	}
	//	if (z != 0.0f)
	//	{
	//		/*���� z-���� �߽����� ȸ���ϴ� ���� ������ �¿�� ����̴� ���̹Ƿ� ȸ�� ������ -20.0~+20.0�� ���̷� ���ѵ�
	//		��. z�� ������ m_fRoll���� ���� ȸ���ϴ� �����̹Ƿ� z��ŭ ȸ���� ���� m_fRoll�� +20�� ���� ũ�ų� -20������
	//		������ m_fRoll�� +20�� �Ǵ� -20���� �ǵ��� ȸ������(z)�� �����Ѵ�.*/
	//		m_fRoll += z;
	//		if (m_fRoll > +20.0f) { z -= (m_fRoll - 20.0f); m_fRoll = +20.0f; }
	//		if (m_fRoll < -20.0f) { z -= (m_fRoll + 20.0f); m_fRoll = -20.0f; }
	//	}
	//	//ī�޶� x, y, z ��ŭ ȸ���Ѵ�. �÷��̾ ȸ���ϸ� ī�޶� ȸ���ϰ� �ȴ�.
	//	m_pCamera->Rotate(x, y, z);

	//	/*�÷��̾ ȸ���Ѵ�. 1��Ī ī�޶� �Ǵ� 3��Ī ī�޶󿡼� �÷��̾��� ȸ���� ���� y-�࿡���� �Ͼ��. �÷��̾�
	//	�� ���� y-��(Up ����)�� �������� ���� z-��(Look ����)�� ���� x-��(Right ����)�� ȸ����Ų��. �⺻������ Up ��
	//	�͸� �������� ȸ���ϴ� ���� �÷��̾ �ȹٷ� ���ִ� ���� �����Ѵٴ� �ǹ��̴�.*/
	//	if (y != 0.0f)
	//	{
	//		XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_Up),
	//			XMConvertToRadians(y));
	//		m_Look = Vector3::TransformNormal(m_Look, xmmtxRotate);
	//		m_Right = Vector3::TransformNormal(m_Right, xmmtxRotate);
	//	}
	//}
	//	}

	//if ((x != 0.0f))
	//{
	//	//�÷��̾��� ���� x-�࿡ ���� x ������ ȸ�� ����� ����Ѵ�.
	//	XMFLOAT3 xmf3Right = m_pLookingObject->GetRight();
	//	XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&xmf3Right), XMConvertToRadians(x));
	//	//ī�޶��� ���� x-��, y-��, z-���� ȸ���Ѵ�.
	//	m_Right = Vector3::TransformNormal(m_Right, xmmtxRotate);
	//	m_Up = Vector3::TransformNormal(m_Up, xmmtxRotate);
	//	m_Look = Vector3::TransformNormal(m_Look, xmmtxRotate);

	//	// At Pos�� ���������� ȸ��
	//	XMFLOAT3 LookObject_To_At = Vector3::Subtract(m_At, m_pLookingObject->GetPosition());
	//	LookObject_To_At = Vector3::TransformCoord(LookObject_To_At, xmmtxRotate);
	//	m_At = Vector3::Add(m_pLookingObject->GetPosition(), LookObject_To_At);

	//	// Offset �� ���������� ȸ��
	//	m_Offset = Vector3::TransformCoord(m_Offset, xmmtxRotate);
	//}
	//if ((y != 0.0f))
	//{
	//	XMFLOAT3 xmf3Up = m_pLookingObject->GetUp();
	//	XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&xmf3Up), XMConvertToRadians(y));
	//	m_Right = Vector3::TransformNormal(m_Right, xmmtxRotate);
	//	m_Up = Vector3::TransformNormal(m_Up, xmmtxRotate);
	//	m_Look = Vector3::TransformNormal(m_Look, xmmtxRotate);

	//	// At Pos�� ���������� ȸ��
	//	XMFLOAT3 LookObject_To_At = Vector3::Subtract(m_At, m_pLookingObject->GetPosition());
	//	LookObject_To_At = Vector3::TransformCoord(LookObject_To_At, xmmtxRotate);
	//	m_At = Vector3::Add(m_pLookingObject->GetPosition(), LookObject_To_At);

	//	// Offset �� ���������� ȸ��
	//	m_Offset = Vector3::TransformCoord(m_Offset, xmmtxRotate);
	//}
	//if (z != 0.0f)
	//{
	//	XMFLOAT3 xmf3Look = m_pLookingObject->GetLook();
	//	XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&xmf3Look), XMConvertToRadians(z));
	//	m_Right = Vector3::TransformNormal(m_Right, xmmtxRotate);
	//	m_Up = Vector3::TransformNormal(m_Up, xmmtxRotate);
	//	m_Look = Vector3::TransformNormal(m_Look, xmmtxRotate);

	//	// At Pos�� ���������� ȸ��
	//	XMFLOAT3 LookObject_To_At = Vector3::Subtract(m_At, m_pLookingObject->GetPosition());
	//	LookObject_To_At = Vector3::TransformCoord(LookObject_To_At, xmmtxRotate);
	//	m_At = Vector3::Add(m_pLookingObject->GetPosition(), LookObject_To_At);

	//	// Offset �� ���������� ȸ��
	//	m_Offset = Vector3::TransformCoord(m_Offset, xmmtxRotate);
	//}

	///*ȸ������ ���� �÷��̾��� ���� x-��, y-��, z-���� ���� �������� ���� �� �����Ƿ� z-��(LookAt ����)�� ������
	//�� �Ͽ� ���� �����ϰ� �������Ͱ� �ǵ��� �Ѵ�.*/
	//m_Look = Vector3::Normalize(m_Look);
	//m_Right = Vector3::CrossProduct(m_Up, m_Look, true);
	//m_Up = Vector3::CrossProduct(m_Look, m_Right, true);
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
	m_xmf4x4View = Matrix4x4::LookAtLH(m_Position, m_At, m_Up);

	m_Right = XMFLOAT3(m_xmf4x4View._11, m_xmf4x4View._21, m_xmf4x4View._31);
	m_Up = XMFLOAT3(m_xmf4x4View._12, m_xmf4x4View._22, m_xmf4x4View._32);
	m_Look = XMFLOAT3(m_xmf4x4View._13, m_xmf4x4View._23, m_xmf4x4View._33);

}

void Camera::GenerateProjectionMatrix(float fNearPlaneDistance, float fFarPlaneDistance, float fAspectRatio, float fFOVAngle)
{
	m_xmf4x4Projection = Matrix4x4::PerspectiveFovLH(XMConvertToRadians(fFOVAngle), fAspectRatio, fNearPlaneDistance, fFarPlaneDistance);
}

/*ī�޶� ��ȯ ����� �����Ѵ�. ī�޶��� ��ġ ����, ī�޶� �ٶ󺸴� ����, ī�޶��� Up ����(���� y-�� ����)��
�Ķ���ͷ� ����ϴ� XMMatrixLookAtLH() �Լ��� ����Ѵ�.*/
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

void Camera::CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
{
	// PPT ���������� 02 - 68p
	HRESULT hResult = S_FALSE;

	// �ڿ� ����
	UINT HardwareSize = d3dUtil::CalcConstantBufferByteSize(sizeof(VS_CB_MYCAMERA_INFO)); // 256�� ���

	// �� �Ӽ�
	D3D12_HEAP_PROPERTIES HeapProperties;
	::ZeroMemory(&HeapProperties, sizeof(D3D12_HEAP_PROPERTIES));
	HeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
	HeapProperties.CreationNodeMask = 1;
	HeapProperties.VisibleNodeMask = 1;

	D3D12_RESOURCE_DESC ResourceDesc;
	::ZeroMemory(&ResourceDesc, sizeof(D3D12_RESOURCE_DESC));
	ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	ResourceDesc.Width = HardwareSize; // 256�� ���
	ResourceDesc.Height = ResourceDesc.SampleDesc.Count = 1;
	ResourceDesc.DepthOrArraySize = ResourceDesc.MipLevels = 1;
	ResourceDesc.Format = DXGI_FORMAT_UNKNOWN;
	ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR; // ��켱

														  // �ڿ��� �����Ѵ�.
	hResult = pd3dDevice->CreateCommittedResource(
		&HeapProperties,
		D3D12_HEAP_FLAG_NONE,
		&ResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, // ���ε����� �ʱ����
		nullptr,
		IID_PPV_ARGS(&m_d3dCameraCBVUpload));

	assert(hResult == S_OK);
#if defined(_DEBUG)
	OutputDebugString(L"...CreateCommittedResource\n");
#endif

	hResult = m_d3dCameraCBVUpload->Map(0, nullptr, (void**)&m_pMappedCameraInfo);
	assert(hResult == S_OK);
#if defined(_DEBUG)
	OutputDebugString(L"...Constant Buffer Map\n");
#endif

	::memcpy(&m_pMappedCameraInfo->m_xmf4x4View, &m_xmf4x4View, sizeof(XMFLOAT4X4));
	::memcpy(&m_pMappedCameraInfo->m_xmf4x4Projection, &m_xmf4x4Projection, sizeof(XMFLOAT4X4));

	// ������ ���� �����Ѵ�.
	D3D12_DESCRIPTOR_HEAP_DESC d3dCBVHeap;
	::ZeroMemory(&d3dCBVHeap, sizeof(D3D12_DESCRIPTOR_HEAP_DESC));
	d3dCBVHeap.NumDescriptors = 1; // ���� �����ڴ� �Ѱ��� �����Ѵ�.
	d3dCBVHeap.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV; // ��Ÿ�� CBV , SRV, UAV
	d3dCBVHeap.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE; //���̴��� �ش� �ڿ��� �� �� �ִ�.
	d3dCBVHeap.NodeMask = 0;
	hResult = pd3dDevice->CreateDescriptorHeap(&d3dCBVHeap, IID_PPV_ARGS(&m_d3dCameraCBVDescriptorHeap));
	assert(hResult == S_OK);
#if defined(_DEBUG)
	OutputDebugString(L"...Create CBV Descriptor Heap\n");
#endif

	////////////// ������ۺ� ����
	// ppt ���������� 02 - 69p

	// ������ۺ並 �����ϴ� ���� CPU(�������α׷�), ����ϴ� ���� GPU(���̴�)
	D3D12_CPU_DESCRIPTOR_HANDLE d3dCPUdescriptorHandle =
		m_d3dCameraCBVDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	D3D12_GPU_VIRTUAL_ADDRESS d3dGPUVirtualAddress =
		m_d3dCameraCBVUpload->GetGPUVirtualAddress();

	// ������� �� ����ü
	D3D12_CONSTANT_BUFFER_VIEW_DESC CBVdesc;
	::ZeroMemory(&CBVdesc, sizeof(D3D12_CONSTANT_BUFFER_VIEW_DESC));
	CBVdesc.BufferLocation = d3dGPUVirtualAddress;
	CBVdesc.SizeInBytes = HardwareSize;

	pd3dDevice->CreateConstantBufferView(
		&CBVdesc,
		d3dCPUdescriptorHandle // ������ۺ並 �����ϴ� ������ ���� ����
	);
}

void Camera::UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList)
{
	XMFLOAT4X4 xmf4x4View;
	XMStoreFloat4x4(&xmf4x4View, XMMatrixTranspose(XMLoadFloat4x4(&m_xmf4x4View)));
	::memcpy(&m_pMappedCameraInfo->m_xmf4x4View, &xmf4x4View, sizeof(XMFLOAT4X4));

	XMFLOAT4X4 xmf4x4Projection;
	XMStoreFloat4x4(&xmf4x4Projection, XMMatrixTranspose(XMLoadFloat4x4(&m_xmf4x4Projection)));
	::memcpy(&m_pMappedCameraInfo->m_xmf4x4Projection, &xmf4x4Projection, sizeof(XMFLOAT4X4));

	//ī�޶� ��ġ ����
	::memcpy(&m_pMappedCameraInfo->m_xmf3Position, &m_Position, sizeof(XMFLOAT3));

	pd3dCommandList->SetGraphicsRootConstantBufferView(1, m_d3dCameraCBVUpload->GetGPUVirtualAddress());
}

void Camera::ReleaseShaderVariables()
{

}

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
