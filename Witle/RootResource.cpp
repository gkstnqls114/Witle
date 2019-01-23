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

void RootResource::InsertResource(int parametersIndex, const D3D12_ROOT_PARAMETER & rootParameter)
{
	assert(parametersIndex <= m_resourceVector.size()); // ���� �ε����� ������ ũ�⺸�� ũ�ٸ� ���â�� ����.
	
	ResourceBase* value = nullptr;
	switch (rootParameter.ParameterType)
	{
	case D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS: // ��Ʈ ���
		value = new RootConstants(parametersIndex, 1);
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


	m_resourceMap.insert(std::pair<std::string, int>("", parametersIndex));
	m_resourceVector[parametersIndex] = value;
}
