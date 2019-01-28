#include"stdafx.h"
#include "d3dUtil.h"
#include "DescriptorTable.h"
 
void DescriptorTable::CreateConstantBufferViews(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, int nConstantBufferViews, ID3D12Resource * pd3dConstantBuffers, UINT nStride)
{
	D3D12_GPU_VIRTUAL_ADDRESS d3dGpuVirtualAddress = pd3dConstantBuffers->GetGPUVirtualAddress();
	D3D12_CONSTANT_BUFFER_VIEW_DESC d3dCBVDesc;
	d3dCBVDesc.SizeInBytes = nStride;
	for (int j = 0; j < nConstantBufferViews; j++)
	{
		d3dCBVDesc.BufferLocation = d3dGpuVirtualAddress + (nStride * j);
		D3D12_CPU_DESCRIPTOR_HANDLE d3dCbvCPUDescriptorHandle;
		d3dCbvCPUDescriptorHandle.ptr = m_CbvCPUDescriptorStartHandle.ptr + (d3dUtil::gnCbvSrvDescriptorIncrementSize * j);
		pd3dDevice->CreateConstantBufferView(&d3dCBVDesc, d3dCbvCPUDescriptorHandle);
	}
}

void DescriptorTable::CreateShaderResourceViews(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, int nConstantBufferViews, ID3D12Resource * pd3dShaderResources, UINT nStride)
{
}

void DescriptorTable::CreateUnorderedAccessViews(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, int nConstantBufferViews, ID3D12Resource * pd3dUnorderedAccess, UINT nStride)
{
}
 
DescriptorTable::DescriptorTable(UINT paraIndex, ID3D12GraphicsCommandList * pd3dCommandList, int nConstantBufferViews, int nShaderResourceViews, int nUnorderedAcessViews)
{
}

void DescriptorTable::CreateShaderVariables(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
{
	D3D12_DESCRIPTOR_HEAP_DESC d3dDescriptorHeapDesc;
	d3dDescriptorHeapDesc.NumDescriptors = m_ConstantBufferViewsCount + m_ShaderResourceViewsCount + m_UnorderedAcessViewsCount; //CBVs + SRVs + UAVs 
	d3dDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	d3dDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	d3dDescriptorHeapDesc.NodeMask = 0;
	pd3dDevice->CreateDescriptorHeap(&d3dDescriptorHeapDesc, __uuidof(ID3D12DescriptorHeap), (void **)&m_pCbvSrvUavDescriptorHeap);

	m_CbvCPUDescriptorStartHandle = m_pCbvSrvUavDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	m_CbvGPUDescriptorStartHandle = m_pCbvSrvUavDescriptorHeap->GetGPUDescriptorHandleForHeapStart();
	m_SrvCPUDescriptorStartHandle.ptr = m_CbvCPUDescriptorStartHandle.ptr + (d3dUtil::gnCbvSrvDescriptorIncrementSize * m_ConstantBufferViewsCount);
	m_SrvGPUDescriptorStartHandle.ptr = m_CbvGPUDescriptorStartHandle.ptr + (d3dUtil::gnCbvSrvDescriptorIncrementSize * m_ConstantBufferViewsCount);
}

void DescriptorTable::UpdateShaderVariables(ID3D12GraphicsCommandList * pd3dCommandList, const SourcePtr & data)
{

}

void DescriptorTable::ReleaseShaderVariables()
{
}
