#include "stdafx.h"
#include "d3dUtil.h"
#include "RootConstantBuffer.h"

RootConstantBuffer::RootConstantBuffer(UINT elementByte)
{
	m_ElementBytes = elementByte;
}

RootConstantBuffer::~RootConstantBuffer()
{
}

void RootConstantBuffer::CreateShaderVariables(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
{
	UINT ncbElementBytes = ((sizeof(m_ElementBytes) + 255) & ~255); //256ÀÇ ¹è¼ö
	m_pResource = d3dUtil::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbElementBytes, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);

	m_pResource->Map(0, NULL, (void **)&m_MappedResource);
}

void RootConstantBuffer::UpdateShaderVariables(ID3D12GraphicsCommandList * pd3dCommandList, const SourcePtr & data)
{
	//XMFLOAT4X4 xmf4x4View;
	//XMStoreFloat4x4(&xmf4x4View, XMMatrixTranspose(XMLoadFloat4x4(&m_xmf4x4View)));
	//::memcpy(&m_MappedResource->m_xmf4x4View, &xmf4x4View, sizeof(XMFLOAT4X4));

	//XMFLOAT4X4 xmf4x4Projection;
	//XMStoreFloat4x4(&xmf4x4Projection, XMMatrixTranspose(XMLoadFloat4x4(&m_xmf4x4Projection)));
	//::memcpy(&m_MappedResource->m_xmf4x4Projection, &xmf4x4Projection, sizeof(XMFLOAT4X4));

	//::memcpy(&m_pcbMappedCamera->m_xmf3Position, &m_xmf3Position, sizeof(XMFLOAT3));

	//D3D12_GPU_VIRTUAL_ADDRESS d3dGpuVirtualAddress = m_pResource->GetGPUVirtualAddress();
	//pd3dCommandList->SetGraphicsRootConstantBufferView(m_, d3dGpuVirtualAddress);

}
 
void RootConstantBuffer::ReleaseShaderVariables()
{
	if (m_pResource)
	{
		m_pResource->Unmap(0, NULL);
		m_pResource->Release();
	}
}
