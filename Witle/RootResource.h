#pragma once

class ResourceBase;

// ��Ʈ �ñ״�ó, ��Ʈ �з����͸� ���� ������Ʈ �ϴ� ���ҽ� ���ø�
class RootResource
{
	// �ε��� �����ε� ã��, �ε����ε� ã�� ���� �� ���� �ڷᱸ�� ���
	std::map<std::string, int> m_resourceMap;
	std::vector<ResourceBase*> m_resourceVector;

public:
	// ��Ʈ �ñ״�ó�� ũ�� ��ŭ ���ҽ��� ������.
	RootResource(int rootSignatureSize) : m_resourceVector(rootSignatureSize) {};
	~RootResource();

	void Release();

	// �ش� ��Ʈ �з����Ϳ� ���缭 Resource�� �����Ѵ�.
	void InsertResource(int parametersIndex, const D3D12_ROOT_PARAMETER& rootParameter);

	template<typename T>
	void UpdateShaderVariables(ID3D12CommandList* commandList, const T& resource)
	{
		//���� Ÿ�� T�� ����� update�ϴ� ���ҽ��� ũ�⺸�� Ŭ ��� ����Ѵ�.
		
		// ������Ʈ�Ѵ�.
	};
};