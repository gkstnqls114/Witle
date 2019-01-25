#include "stdafx.h"
#include "32bitRootConstants.h"
#include "RootResource.h"

RootResource::~RootResource()
{
}

void RootResource::Release()
{
	for (int x = 0; x < m_resourceVector.size(); ++x)
	{
		// ������
		// ����Ʈ
		// �������� ����
	}

	for (auto& p : m_resourceMap)
	{
		// �� ������ ����
	} 
}

void RootResource::InsertResource(int parametersIndex, const std::string& name, const D3D12_ROOT_PARAMETER & rootParameter)
{
	assert(!(parametersIndex >= m_resourceVector.size())); // ���� �ε����� ������ ũ�⺸�� ũ�ٸ� ������ �߻��Ѵ�.
	
	ResourceBase* value = nullptr;
	switch (rootParameter.ParameterType)
	{
	case D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS: // ��Ʈ ���
		value = new RootConstants(parametersIndex, rootParameter.Constants.Num32BitValues);
		break;

	case D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE: // ������ ���̺�
		break;

	case D3D12_ROOT_PARAMETER_TYPE_CBV: // ��� ���� ��
		break;

	case D3D12_ROOT_PARAMETER_TYPE_SRV:
		break;

	case D3D12_ROOT_PARAMETER_TYPE_UAV:
		break;

	default:
		break;
	}

	m_resourceMap.insert(std::pair<std::string, int>(name, m_VectorIndex));
	m_resourceVector[m_VectorIndex] = value;
	m_VectorIndex += 1;
}

void RootResource::UpdateShaderVariables(ID3D12CommandList * commandList, ID3D12RootSignature * CurrentRootsignature, const SourcePtr & resource)
{
#ifdef CHECK_ROOT_SIGNATURE
	assert(CurrentRootsignature != m_pRootSignature);
#endif // CHECK_ROOT_SIGNATURE

	//���� Ÿ�� T�� ����� update�ϴ� ���ҽ��� ũ�⺸�� Ŭ ��� ����Ѵ�.

	// ������Ʈ�Ѵ�.
}
