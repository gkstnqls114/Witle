#include "stdafx.h"
#include "d3dUtil.h"
#include "MyDescriptorHeap.h"


MyDescriptorHeap::MyDescriptorHeap()
{
}


MyDescriptorHeap::~MyDescriptorHeap()
{
	if (m_pd3dCbvSrvUavDescriptorHeap) {
		delete m_pd3dCbvSrvUavDescriptorHeap;
		m_pd3dCbvSrvUavDescriptorHeap = nullptr;
	}
}


void MyDescriptorHeap::CreateCbvAndSrvDescriptorHeaps(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, int nConstantBufferViews, int nShaderResourceViews, int nUnorderedAcessViews)
{
	D3D12_DESCRIPTOR_HEAP_DESC d3dDescriptorHeapDesc;
	d3dDescriptorHeapDesc.NumDescriptors = nConstantBufferViews + nShaderResourceViews + nUnorderedAcessViews; //CBVs + SRVs + UAVs 
	d3dDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	d3dDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	d3dDescriptorHeapDesc.NodeMask = 0;
	pd3dDevice->CreateDescriptorHeap(&d3dDescriptorHeapDesc, __uuidof(ID3D12DescriptorHeap), (void **)&m_pd3dCbvSrvUavDescriptorHeap);

	m_d3dCbvCPUDescriptorStartHandle = m_pd3dCbvSrvUavDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	m_d3dCbvGPUDescriptorStartHandle = m_pd3dCbvSrvUavDescriptorHeap->GetGPUDescriptorHandleForHeapStart();

	m_d3dSrvCPUDescriptorStartHandle.ptr = m_d3dCbvCPUDescriptorStartHandle.ptr + (d3dUtil::gnCbvSrvDescriptorIncrementSize * nConstantBufferViews);
	m_d3dSrvGPUDescriptorStartHandle.ptr = m_d3dCbvGPUDescriptorStartHandle.ptr + (d3dUtil::gnCbvSrvDescriptorIncrementSize * nConstantBufferViews);

	m_d3dUAVCPUDescriptorStartHandle.ptr = m_d3dSrvCPUDescriptorStartHandle.ptr + (d3dUtil::gnCbvSrvDescriptorIncrementSize * nShaderResourceViews);
	m_d3dUAVGPUDescriptorStartHandle.ptr = m_d3dSrvGPUDescriptorStartHandle.ptr + (d3dUtil::gnCbvSrvDescriptorIncrementSize * nShaderResourceViews);
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
		d3dCbvCPUDescriptorHandle.ptr = m_d3dCbvCPUDescriptorStartHandle.ptr + (d3dUtil::gnCbvSrvDescriptorIncrementSize * j);
		pd3dDevice->CreateConstantBufferView(&d3dCBVDesc, d3dCbvCPUDescriptorHandle);
	}
}

void MyDescriptorHeap::CreateShaderResourceViews(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, CTexture * pTexture, UINT nRootParameterStartIndex, bool bAutoIncrement, UINT Start)
{
	D3D12_CPU_DESCRIPTOR_HANDLE d3dSrvCPUDescriptorHandle = m_d3dSrvCPUDescriptorStartHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE d3dSrvGPUDescriptorHandle = m_d3dSrvGPUDescriptorStartHandle;
	
	d3dSrvCPUDescriptorHandle.ptr = d3dSrvCPUDescriptorHandle.ptr + (Start * d3dUtil::gnCbvSrvDescriptorIncrementSize);
	d3dSrvGPUDescriptorHandle.ptr = d3dSrvGPUDescriptorHandle.ptr + (Start * d3dUtil::gnCbvSrvDescriptorIncrementSize);

	//int nTextures = pTexture->GetTextures();
	//int nTextureType = pTexture->GetTextureType();
	//for (int i = 0; i < nTextures; i++)
	//{
	//	ID3D12Resource *pShaderResource = pTexture->GetTexture(i);
	//	D3D12_RESOURCE_DESC d3dResourceDesc = pShaderResource->GetDesc();
	//	D3D12_SHADER_RESOURCE_VIEW_DESC d3dShaderResourceViewDesc = d3dUtil::GetShaderResourceViewDesc(d3dResourceDesc, nTextureType);
	//	pd3dDevice->CreateShaderResourceView(pShaderResource, &d3dShaderResourceViewDesc, d3dSrvCPUDescriptorHandle);
	//	
	//	d3dSrvCPUDescriptorHandle.ptr += d3dUtil::gnCbvSrvDescriptorIncrementSize;
	//	
	//	// ���� bAutoIncrement�� true��� nRootParameterStartIndex�� + i, false��� nRootParameterStartIndex �״��...
	//	pTexture->SetRootArgument(i, (bAutoIncrement) ? (nRootParameterStartIndex + i) : nRootParameterStartIndex, d3dSrvGPUDescriptorHandle);
	//	
	//	d3dSrvGPUDescriptorHandle.ptr += d3dUtil::gnCbvSrvDescriptorIncrementSize;
	//}
}

void MyDescriptorHeap::CreateUnorderedAccessViews(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, CTexture * pTexture, UINT nRootParameterStartIndex, bool bAutoIncrement)
{
	D3D12_CPU_DESCRIPTOR_HANDLE UAVCPUDescriptorHandle = m_d3dUAVCPUDescriptorStartHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE UAVGPUDescriptorHandle = m_d3dUAVGPUDescriptorStartHandle;
	//int nTextures = pTexture->GetTextures();
	//int nTextureType = pTexture->GetTextureType();
	//for (int i = 0; i < nTextures; ++i)
	//{
	//	ID3D12Resource *pShaderResource = pTexture->GetTexture(i);
	//	D3D12_RESOURCE_DESC d3dResourceDesc = pShaderResource->GetDesc();
	//	 
	//	D3D12_UNORDERED_ACCESS_VIEW_DESC d3dShaderResourceViewDesc = d3dUtil::GetUnorderedAccessViewDesc(d3dResourceDesc, nTextureType);
	//	pd3dDevice->CreateUnorderedAccessView(pShaderResource, nullptr, &d3dShaderResourceViewDesc, UAVCPUDescriptorHandle);

	//	// ���� bAutoIncrement�� true��� nRootParameterStartIndex�� + i, false��� nRootParameterStartIndex �״��...
	//	// pTexture->SetRootArgument(i, (bAutoIncrement) ? (nRootParameterStartIndex + i) : nRootParameterStartIndex, UAVGPUDescriptorHandle);
	//	UAVGPUDescriptorHandle.ptr += d3dUtil::gnCbvSrvDescriptorIncrementSize;
	//	UAVCPUDescriptorHandle.ptr += d3dUtil::gnCbvSrvDescriptorIncrementSize;
	//}
}