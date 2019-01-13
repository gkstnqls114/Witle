#pragma once
#include "ResourceBase.h"

/*
쉐이더에 전달하는 루트 상수 클래스 입니다.
*/
template <typename T>
class RootConstants :
	public ResourceBase
{
	UINT m_parameterIndex{ 0 };
	UINT m_ResourceCount; // 루트상수로 넘겨야할 리소스 개수
	UINT *DestOffsets{ nullptr };

public:
	RootConstants(UINT paraIndex, UINT resourceCount, UINT StartOffset = 0);
	virtual ~RootConstants();

	virtual void CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList) {};
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList, const T *resources) ;
	virtual void ReleaseShaderVariables() {};

};

template<typename T>
inline RootConstants<T>::RootConstants(UINT paraIndex, UINT resourceCount, UINT StartOffset)
{
	m_parameterIndex = paraIndex;
	m_ResourceCount = resourceCount;

	DestOffsets = new UINT[m_ResourceCount];
	DestOffsets[0] = StartOffset;
	for (int x = 1; x < m_ResourceCount; ++x)
	{
		DestOffsets[x] = StartOffset + (sizeof(T) / 4);
		std::cout << sizeof(T);
	}
}

template<typename T>
inline RootConstants<T>::~RootConstants()
{
	if (DestOffsets)
	{
		delete[] DestOffsets;
		DestOffsets = nullptr;
	}
}

template<typename T>
inline void RootConstants<T>::UpdateShaderVariables(ID3D12GraphicsCommandList * pd3dCommandList, const T *resources)
{
	for (int x = 0; x < m_ResourceCount; ++x)
	{
		pd3dCommandList->SetGraphicsRoot32BitConstants(m_parameterIndex, sizeof(T) / 4, &(resources[x]), DestOffsets[x]);
	}
}
