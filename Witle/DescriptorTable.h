#pragma once
#include "ResourceBase.h"

/*
쉐이더에 전달하는 루트 디스크립터 힙 클래스 입니다.
*/ 
class DescriptorTable
	:public ResourceBase
{
	ID3D12DescriptorHeap			*m_pCbvSrvUavDescriptorHeap = NULL;

	int m_ConstantBufferViewsCount{ 0 };
	int m_ShaderResourceViewsCount{ 0 };
	int m_UnorderedAcessViewsCount{ 0 };

	D3D12_CPU_DESCRIPTOR_HANDLE		m_CbvCPUDescriptorStartHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE		m_CbvGPUDescriptorStartHandle;

	D3D12_CPU_DESCRIPTOR_HANDLE		m_SrvCPUDescriptorStartHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE		m_SrvGPUDescriptorStartHandle;
	
	D3D12_CPU_DESCRIPTOR_HANDLE		m_UavCPUDescriptorStartHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE		m_UavGPUDescriptorStartHandle;

public:
	DescriptorTable(UINT paraIndex, ID3D12GraphicsCommandList *pd3dCommandList, int nConstantBufferViews, int nShaderResourceViews, int nUnorderedAcessViews);
	virtual ~DescriptorTable() {};

	virtual void CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList) override;
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList, const SourcePtr& data) override;
	virtual void ReleaseShaderVariables() override;

	void CreateConstantBufferViews(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, int nConstantBufferViews, ID3D12Resource *pd3dConstantBuffers, UINT nStride);
	void CreateShaderResourceViews(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, int nConstantBufferViews, ID3D12Resource *pd3dShaderResources, UINT nStride);
	void CreateUnorderedAccessViews(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, int nConstantBufferViews, ID3D12Resource *pd3dUnorderedAccess, UINT nStride); 

	const ID3D12DescriptorHeap * const GetDescriptorHeap() const { return m_pCbvSrvUavDescriptorHeap; }

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandleForHeapStart() const { return(m_pCbvSrvUavDescriptorHeap->GetCPUDescriptorHandleForHeapStart()); }
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandleForHeapStart() const { return(m_pCbvSrvUavDescriptorHeap->GetGPUDescriptorHandleForHeapStart()); }

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUCbvDescriptorStartHandle() const { return(m_CbvCPUDescriptorStartHandle); }
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUCbvDescriptorStartHandle() const { return(m_CbvGPUDescriptorStartHandle); }

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUSrvDescriptorStartHandle() const { return(m_SrvCPUDescriptorStartHandle); }
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUSrvDescriptorStartHandle() const { return(m_SrvGPUDescriptorStartHandle); }

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUUAVDescriptorStartHandle() const { return(m_UavCPUDescriptorStartHandle); }
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUUAVDescriptorStartHandle() const { return(m_UavGPUDescriptorStartHandle); }
};
//
//UINT ncbElementBytes = ((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255); //256의 배수
//m_pd3dcbGameObject = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbElementBytes, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);
//
//m_pd3dcbGameObject->Map(0, NULL, (void **)&m_pcbMappedGameObject);
 