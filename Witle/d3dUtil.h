#pragma once

namespace FileRead
{
	static int ReadStringFromFile(FILE *pInFile, char *pstrToken)
	{
		int nStrLength = 0;
		UINT nReads = 0;
		nReads = (UINT)::fread(&nStrLength, sizeof(int), 1, pInFile);
		nReads = (UINT)::fread(pstrToken, sizeof(char), nStrLength, pInFile);
		pstrToken[nStrLength] = '\0';
		return(nStrLength);
	}

	static float ReadFloatFromFile(FILE *pInFile)
	{
		float fValue = 0;
		UINT nReads = (UINT)::fread(&fValue, sizeof(float), 1, pInFile);
		return(fValue);
	}

	static int ReadIntegerFromFile(FILE *pInFile)
	{
		int nValue = 0;
		UINT nReads = (UINT)::fread(&nValue, sizeof(int), 1, pInFile);
		return(nValue);
	}
}
// 유용한 함수들이 모여있는 클래스
class d3dUtil
{
public:
	d3dUtil();
	~d3dUtil();

	static D3D12_DESCRIPTOR_RANGE CreateDescriptorRangeSRV(UINT NumDescriptors, UINT BaseShaderRegister, UINT RegisterSpace = 0);
	static D3D12_ROOT_PARAMETER CreateRootParameterConstants(UINT Num32BitValues, UINT ShaderRegister, UINT RegisterSpace = 0, D3D12_SHADER_VISIBILITY ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL);
	static D3D12_ROOT_PARAMETER CreateRootParameterCBV(UINT ShaderRegister, UINT RegisterSpace = 0, D3D12_SHADER_VISIBILITY ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL);
	static D3D12_ROOT_PARAMETER CreateRootParameterSRV(UINT ShaderRegister, UINT RegisterSpace = 0, D3D12_SHADER_VISIBILITY ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL);
	static D3D12_ROOT_PARAMETER CreateRootParameterTable(UINT NumDescriptorRanges, D3D12_DESCRIPTOR_RANGE* pDescriptorRanges, D3D12_SHADER_VISIBILITY ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL);

	static ID3D12Resource* CreateBufferResource(ID3D12Device *pd3dDevice,
		ID3D12GraphicsCommandList *pd3dCommandList, void *pData, UINT nBytes, D3D12_HEAP_TYPE
		d3dHeapType = D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATES d3dResourceStates =
		D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, ID3D12Resource **ppd3dUploadBuffer =
		NULL);

	static Microsoft::WRL::ComPtr<ID3D12Resource> d3dUtil::CreateDefaultBuffer(
		ID3D12Device* device,
		ID3D12GraphicsCommandList* cmdList,
		const void* initData,
		UINT64 byteSize,
		Microsoft::WRL::ComPtr<ID3D12Resource>& uploadBuffer); 

	static ID3D12Resource *CreateTextureResourceFromDDSFile(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, const wchar_t *pszFileName, ID3D12Resource **ppd3dUploadBuffer, D3D12_RESOURCE_STATES d3dResourceStates);
	static ID3D12Resource *CreateTextureResourceFromWICFile(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, const wchar_t *pszFileName, ID3D12Resource **ppd3dUploadBuffer, D3D12_RESOURCE_STATES d3dResourceStates);

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
	static UINT gnDsvDescriptorIncrementSize;

	static int ReadStringFromFile(FILE *pInFile, char *pstrToken)
	{
		int nStrLength = 0;
		UINT nReads = 0;
		nReads = (UINT)::fread(&nStrLength, sizeof(int), 1, pInFile);
		nReads = (UINT)::fread(pstrToken, sizeof(char), nStrLength, pInFile);
		pstrToken[nStrLength] = '\0'; 
		return(nStrLength);
	}

	static float ReadFloatFromFile(FILE *pInFile)
	{
		float fValue = 0;
		UINT nReads = (UINT)::fread(&fValue, sizeof(float), 1, pInFile); 
		return(fValue);
	}

	static int ReadIntegerFromFile(FILE *pInFile)
	{
		int nValue = 0;
		UINT nReads = (UINT)::fread(&nValue, sizeof(int), 1, pInFile); 
		return(nValue);
	}
};


inline void CreateConstantBuffer(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12Resource* pResource, unsigned long long byteSize, void** ppData)
{
	UINT ncbBytes = ((sizeof(byteSize) + 255) & ~255); //256의 배수
	pResource = d3dUtil::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbBytes, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);

	HRESULT hResult = pResource->Map(0, NULL, ppData);
	assert(!(hResult != S_OK));
}
