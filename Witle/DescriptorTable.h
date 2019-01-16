#pragma once
#include "ResourceBase.h"

/*
쉐이더에 전달하는 루트 디스크립터 힙 클래스 입니다.
*/ 
class DescriptorTable
	:public ResourceBase
{
	UINT m_parameterIndex{ 0 };
	UINT m_ResourceCount;

	ID3D12DescriptorHeap			*m_pCbvSrvUavDescriptorHeap = NULL;

	D3D12_CPU_DESCRIPTOR_HANDLE		m_CbvCPUDescriptorStartHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE		m_CbvGPUDescriptorStartHandle;
	D3D12_CPU_DESCRIPTOR_HANDLE		m_SrvCPUDescriptorStartHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE		m_SrvGPUDescriptorStartHandle;
	D3D12_CPU_DESCRIPTOR_HANDLE		m_UavCPUDescriptorStartHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE		m_UavGPUDescriptorStartHandle;

public:
	DescriptorTable(UINT paraIndex, UINT resourceCount, UINT StartOffset = 0) {};
	virtual ~DescriptorTable() {};

	virtual void CreateCbvSrvDescriptorHeaps(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, int nConstantBufferViews, int nShaderResourceViews, int nUnorderedAcessViews);
	virtual void CreateConstantBufferViews(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, int nConstantBufferViews, ID3D12Resource *pd3dConstantBuffers, UINT nStride);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void ReleaseShaderVariables() ;

};
//
//UINT ncbElementBytes = ((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255); //256의 배수
//m_pd3dcbGameObject = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbElementBytes, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);
//
//m_pd3dcbGameObject->Map(0, NULL, (void **)&m_pcbMappedGameObject);
 