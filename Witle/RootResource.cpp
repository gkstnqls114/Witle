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
		// 릴리즈
		// 딜리트
		// 널포인터 설정
	}

	for (auto& p : m_resourceMap)
	{
		// 널 포인터 설정
	} 
}

void RootResource::InsertResource(int parametersIndex, const D3D12_ROOT_PARAMETER & rootParameter)
{
	assert(parametersIndex <= m_resourceVector.size()); // 만약 인덱스가 벡터의 크기보다 크다면 경고창을 띄운다.
	
	ResourceBase* value = nullptr;
	switch (rootParameter.ParameterType)
	{
	case D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS: // 루트 상수
		value = new RootConstants(parametersIndex, 1);
		break;

	case D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE: // 서술자 테이블
		break;

	case D3D12_ROOT_PARAMETER_TYPE_CBV: // 상수 버퍼 뷰
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
