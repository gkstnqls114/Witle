#include "stdafx.h"
#include "d3dUtil.h"
#include "Texture.h"
#include "MyDescriptorHeap.h"


MyDescriptorHeap::MyDescriptorHeap()
{
}


MyDescriptorHeap::~MyDescriptorHeap()
{

}

void MyDescriptorHeap::ReleaseObjects()
{
	if (m_pd3dCbvSrvUavDescriptorHeap)
	{
		m_pd3dCbvSrvUavDescriptorHeap->Release();
		m_pd3dCbvSrvUavDescriptorHeap = nullptr;
	}
}


void MyDescriptorHeap::CreateCbvSrvUavDescriptorHeaps(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, int nConstantBufferViews, int nShaderResourceViews, int nUnorderedAcessViews)
{
	D3D12_DESCRIPTOR_HEAP_DESC d3dDescriptorHeapDesc;
	d3dDescriptorHeapDesc.NumDescriptors = nConstantBufferViews + nShaderResourceViews + nUnorderedAcessViews; //CBVs + SRVs + UAVs 
	d3dDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	d3dDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	d3dDescriptorHeapDesc.NodeMask = 0;
	pd3dDevice->CreateDescriptorHeap(&d3dDescriptorHeapDesc, __uuidof(ID3D12DescriptorHeap), (void **)&m_pd3dCbvSrvUavDescriptorHeap);

	m_CbvCPUDescriptorStartHandle = m_pd3dCbvSrvUavDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	m_CbvGPUDescriptorStartHandle = m_pd3dCbvSrvUavDescriptorHeap->GetGPUDescriptorHandleForHeapStart();

	m_SrvCPUDescriptorStartHandle.ptr = m_CbvCPUDescriptorStartHandle.ptr + (d3dUtil::gnCbvSrvDescriptorIncrementSize * nConstantBufferViews);
	m_SrvGPUDescriptorStartHandle.ptr = m_CbvGPUDescriptorStartHandle.ptr + (d3dUtil::gnCbvSrvDescriptorIncrementSize * nConstantBufferViews);

	m_d3dUAVCPUDescriptorStartHandle.ptr = m_SrvCPUDescriptorStartHandle.ptr + (d3dUtil::gnCbvSrvDescriptorIncrementSize * nShaderResourceViews);
	m_d3dUAVGPUDescriptorStartHandle.ptr = m_SrvGPUDescriptorStartHandle.ptr + (d3dUtil::gnCbvSrvDescriptorIncrementSize * nShaderResourceViews);
}

void MyDescriptorHeap::CreateConstantBufferViews(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, int nConstantBufferViews, ID3D12Resource * pd3dConstantBuffers, UINT nStride)
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

void MyDescriptorHeap::CreateShaderResourceViews(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, Texture * pTexture, UINT nRootParameterStartIndex, bool bAutoIncrement, UINT Start)
{
	D3D12_CPU_DESCRIPTOR_HANDLE d3dSrvCPUDescriptorHandle = m_SrvCPUDescriptorStartHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE d3dSrvGPUDescriptorHandle = m_SrvGPUDescriptorStartHandle;
	
	d3dSrvCPUDescriptorHandle.ptr = d3dSrvCPUDescriptorHandle.ptr + (Start * d3dUtil::gnCbvSrvDescriptorIncrementSize);
	d3dSrvGPUDescriptorHandle.ptr = d3dSrvGPUDescriptorHandle.ptr + (Start * d3dUtil::gnCbvSrvDescriptorIncrementSize);

	int nTextures = pTexture->GetTextures();
	int nTextureType = pTexture->GetTextureType();
	for (int i = 0; i < nTextures; i++)
	{
		ID3D12Resource *pShaderResource = pTexture->GetTexture(i);
		D3D12_RESOURCE_DESC d3dResourceDesc = pShaderResource->GetDesc();
		D3D12_SHADER_RESOURCE_VIEW_DESC d3dShaderResourceViewDesc = d3dUtil::GetShaderResourceViewDesc(d3dResourceDesc, nTextureType);
		pd3dDevice->CreateShaderResourceView(pShaderResource, &d3dShaderResourceViewDesc, d3dSrvCPUDescriptorHandle);
		
		// 만약 bAutoIncrement가 true라면 nRootParameterStartIndex는 + i, false라면 nRootParameterStartIndex 그대로...
		pTexture->SetRootArgument(i, (bAutoIncrement) ? (nRootParameterStartIndex + i) : nRootParameterStartIndex, d3dSrvGPUDescriptorHandle);
		
		d3dSrvCPUDescriptorHandle.ptr += d3dUtil::gnCbvSrvDescriptorIncrementSize;
		d3dSrvGPUDescriptorHandle.ptr += d3dUtil::gnCbvSrvDescriptorIncrementSize;
	}
}

void MyDescriptorHeap::CreateShaderResourceViews(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, int nShaderResourceBufferViews, ID3D12Resource *pd3dShaderResourceBuffers, int nTypes, UINT count)
{
	D3D12_CPU_DESCRIPTOR_HANDLE d3dSrvCPUDescriptorHandle = m_SrvCPUDescriptorStartHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE d3dSrvGPUDescriptorHandle = m_SrvGPUDescriptorStartHandle;

	d3dSrvCPUDescriptorHandle.ptr = d3dSrvCPUDescriptorHandle.ptr + (count * d3dUtil::gnCbvSrvDescriptorIncrementSize);
	d3dSrvGPUDescriptorHandle.ptr = d3dSrvGPUDescriptorHandle.ptr + (count * d3dUtil::gnCbvSrvDescriptorIncrementSize);

	int nTextureType = nTypes;

	ID3D12Resource *pShaderResource = pd3dShaderResourceBuffers;
	D3D12_RESOURCE_DESC d3dResourceDesc = pShaderResource->GetDesc();
	D3D12_SHADER_RESOURCE_VIEW_DESC d3dShaderResourceViewDesc = d3dUtil::GetShaderResourceViewDesc(d3dResourceDesc, nTextureType);
	pd3dDevice->CreateShaderResourceView(pShaderResource, &d3dShaderResourceViewDesc, d3dSrvCPUDescriptorHandle);
}

void MyDescriptorHeap::CreateShaderResourceViews(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ID3D12Resource * pd3dShaderResourceBuffers, int nTypes, UINT count, DXGI_FORMAT format)
{
	D3D12_CPU_DESCRIPTOR_HANDLE d3dSrvCPUDescriptorHandle = m_SrvCPUDescriptorStartHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE d3dSrvGPUDescriptorHandle = m_SrvGPUDescriptorStartHandle;

	d3dSrvCPUDescriptorHandle.ptr = d3dSrvCPUDescriptorHandle.ptr + (count * d3dUtil::gnCbvSrvDescriptorIncrementSize);
	d3dSrvGPUDescriptorHandle.ptr = d3dSrvGPUDescriptorHandle.ptr + (count * d3dUtil::gnCbvSrvDescriptorIncrementSize);

	int nTextureType = nTypes;

	ID3D12Resource *pShaderResource = pd3dShaderResourceBuffers;
	D3D12_RESOURCE_DESC d3dResourceDesc = pShaderResource->GetDesc();
	d3dResourceDesc.Format = format;
	D3D12_SHADER_RESOURCE_VIEW_DESC d3dShaderResourceViewDesc = d3dUtil::GetShaderResourceViewDesc(d3dResourceDesc, nTextureType);
	pd3dDevice->CreateShaderResourceView(pShaderResource, &d3dShaderResourceViewDesc, d3dSrvCPUDescriptorHandle);
}

void MyDescriptorHeap::CreateUnorderedAccessViews(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, Texture * pTexture, UINT nRootParameterStartIndex, bool bAutoIncrement)
{
	D3D12_CPU_DESCRIPTOR_HANDLE UAVCPUDescriptorHandle = m_d3dUAVCPUDescriptorStartHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE UAVGPUDescriptorHandle = m_d3dUAVGPUDescriptorStartHandle;
	int nTextures = pTexture->GetTextures();
	int nTextureType = pTexture->GetTextureType();
	for (int i = 0; i < nTextures; ++i)
	{
		ID3D12Resource *pShaderResource = pTexture->GetTexture(i);
		D3D12_RESOURCE_DESC d3dResourceDesc = pShaderResource->GetDesc();
		 
		D3D12_UNORDERED_ACCESS_VIEW_DESC d3dShaderResourceViewDesc = d3dUtil::GetUnorderedAccessViewDesc(d3dResourceDesc, nTextureType);
		pd3dDevice->CreateUnorderedAccessView(pShaderResource, nullptr, &d3dShaderResourceViewDesc, UAVCPUDescriptorHandle);

		// 만약 bAutoIncrement가 true라면 nRootParameterStartIndex는 + i, false라면 nRootParameterStartIndex 그대로...
		// pTexture->SetRootArgument(i, (bAutoIncrement) ? (nRootParameterStartIndex + i) : nRootParameterStartIndex, UAVGPUDescriptorHandle);
		UAVGPUDescriptorHandle.ptr += d3dUtil::gnCbvSrvDescriptorIncrementSize;
		UAVCPUDescriptorHandle.ptr += d3dUtil::gnCbvSrvDescriptorIncrementSize;
	}
}