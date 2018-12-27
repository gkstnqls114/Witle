#pragma once


// 유용한 함수들이 모여있는 클래스
class d3dUtil
{
public:
	d3dUtil();
	~d3dUtil();

	static ComPtr<ID3D12Resource> CreateBufferResource(ID3D12Device *pd3dDevice,
		ID3D12GraphicsCommandList *pd3dCommandList, void *pData, UINT nBytes, D3D12_HEAP_TYPE
		d3dHeapType = D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATES d3dResourceStates =
		D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, ID3D12Resource **ppd3dUploadBuffer =
		NULL);
	

	static ID3D12Resource *CreateTextureResourceFromFile(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, const wchar_t *pszFileName, ID3D12Resource **ppd3dUploadBuffer, D3D12_RESOURCE_STATES d3dResourceStates);

	static ID3D12Resource * CreateTexture2DResource(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, UINT nWidth, UINT nHeight, DXGI_FORMAT dxgiFormat, D3D12_RESOURCE_FLAGS d3dResourceFlags, D3D12_RESOURCE_STATES d3dResourceStates, D3D12_CLEAR_VALUE *pd3dClearValue);

	static D3D12_SHADER_RESOURCE_VIEW_DESC GetShaderResourceViewDesc(D3D12_RESOURCE_DESC d3dResourceDesc, UINT nTextureType);
	static D3D12_UNORDERED_ACCESS_VIEW_DESC GetUnorderedAccessViewDesc(D3D12_RESOURCE_DESC d3dResourceDesc, UINT nTextureType);

	static void SynchronizeResourceTransition(ID3D12GraphicsCommandList *pd3dCommandList, ID3D12Resource *pd3dResource, D3D12_RESOURCE_STATES d3dStateBefore, D3D12_RESOURCE_STATES d3dStateAfter)
	{
		D3D12_RESOURCE_BARRIER d3dResourceBarrier;
		d3dResourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		d3dResourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		d3dResourceBarrier.Transition.pResource = pd3dResource;
		d3dResourceBarrier.Transition.StateBefore = d3dStateBefore;
		d3dResourceBarrier.Transition.StateAfter = d3dStateAfter;
		d3dResourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		pd3dCommandList->ResourceBarrier(1, &d3dResourceBarrier);
	}
	
	static UINT CalcConstantBufferByteSize(UINT byteSize);

	static UINT gnCbvSrvDescriptorIncrementSize;
};



