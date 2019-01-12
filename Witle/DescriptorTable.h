#pragma once
#include "ResourceBase.h"

/*
���̴��� �����ϴ� ��Ʈ ��ũ���� �� Ŭ���� �Դϴ�.
*/
template <typename T>
class DescriptorTable :
	public ResourceBase
{
	UINT m_parameterIndex{ 0 };
	UINT m_ResourceCount;
	UINT *DestOffsets{ nullptr };

public:
	DescriptorTable(UINT paraIndex, UINT resourceCount, UINT StartOffset = 0) {};
	virtual ~DescriptorTable() {};

	virtual void CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList) {};
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList, const T *resources) {};
	virtual void ReleaseShaderVariables() override {};

};
//
//UINT ncbElementBytes = ((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255); //256�� ���
//m_pd3dcbGameObject = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbElementBytes, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);
//
//m_pd3dcbGameObject->Map(0, NULL, (void **)&m_pcbMappedGameObject);