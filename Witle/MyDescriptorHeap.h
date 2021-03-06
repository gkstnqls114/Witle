#pragma once

class Texture;

class MyDescriptorHeap
{
	UINT							m_Count{ 0 }; // 모든 자원 뷰 개수
	ID3D12DescriptorHeap			*m_pd3dCbvSrvUavDescriptorHeap{ nullptr };

	D3D12_CPU_DESCRIPTOR_HANDLE		m_CbvCPUDescriptorStartHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE		m_CbvGPUDescriptorStartHandle;

	D3D12_CPU_DESCRIPTOR_HANDLE		m_SrvCPUDescriptorStartHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE		m_SrvGPUDescriptorStartHandle;

	D3D12_CPU_DESCRIPTOR_HANDLE		m_d3dUAVCPUDescriptorStartHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE		m_d3dUAVGPUDescriptorStartHandle;

public:
	MyDescriptorHeap();
	~MyDescriptorHeap();

	void ReleaseObjects();
	 
	// ▼ 서술자 힙에 대한 함수들 
	void CreateCbvSrvUavDescriptorHeaps(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, int nConstantBufferViews, int nShaderResourceViews, int nUnorderedAcessViews, ENUM_SCENE SceneType);
	
	// CBV 를 설정한다.
	void CreateConstantBufferViews(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, int nConstantBufferViews, ID3D12Resource *pd3dConstantBuffers, UINT nStride);

	// SRV 를 설정한다.
	void CreateShaderResourceViews(ID3D12Device *pd3dDevice, Texture *pTexture, UINT nRootParameterStartIndex, bool bAutoIncrement, UINT Start = 0);
	void CreateShaderResourceViews(ID3D12Device * pd3dDevice, int nShaderResourceBufferViews, ID3D12Resource *pd3dShaderResourceBuffers, int nTypes, UINT count);
	void CreateShaderResourceViews(ID3D12Device * pd3dDevice, ID3D12Resource *pd3dShaderResourceBuffers, int nTypes, UINT index, DXGI_FORMAT format);

	// UAV 를 설정한다.
	void CreateUnorderedAccessViews(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, Texture *pTexture, UINT nRootParameterStartIndex, bool bAutoIncrement);
	void CreateUnorderedAccessViews(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12Resource *pd3dShaderResourceBuffers, int nTypes, UINT count, DXGI_FORMAT format) ;

	void UpdateShaderVariable(ID3D12GraphicsCommandList *pd3dCommandList) const 
	{
		pd3dCommandList->SetDescriptorHeaps(1, &m_pd3dCbvSrvUavDescriptorHeap);
	}
	 
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandleForHeapStart() const { return(m_pd3dCbvSrvUavDescriptorHeap->GetCPUDescriptorHandleForHeapStart()); }
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandleForHeapStart() const { return(m_pd3dCbvSrvUavDescriptorHeap->GetGPUDescriptorHandleForHeapStart()); }

	//D3D12_CPU_DESCRIPTOR_HANDLE GetCPUCbvDescriptorStartHandle() const { return(m_CbvCPUDescriptorStartHandle); }
	//D3D12_GPU_DESCRIPTOR_HANDLE GetGPUCbvDescriptorStartHandle() const { return(m_CbvGPUDescriptorStartHandle); }

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUSrvDescriptorStartHandle() const { return(m_SrvCPUDescriptorStartHandle); }
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUSrvDescriptorStartHandle() const { return(m_SrvGPUDescriptorStartHandle); }

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUSrvDescriptorHandle(int index) const;
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUSrvDescriptorHandle(int index) const;
	
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUUAVDescriptorStartHandle() const { return(m_d3dUAVCPUDescriptorStartHandle); }
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUUAVDescriptorStartHandle() const { return(m_d3dUAVGPUDescriptorStartHandle); }

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUUAVDescriptorHandle(int index) const;
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUUAVDescriptorHandle(int index) const;

	ID3D12DescriptorHeap* GetpDescriptorHeap() const { return m_pd3dCbvSrvUavDescriptorHeap; };
	UINT GetNumDescriptors() const { return m_pd3dCbvSrvUavDescriptorHeap->GetDesc().NumDescriptors;}
};

