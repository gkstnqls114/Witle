#pragma once

class ResourceBase;

// ��Ʈ �ñ״�ó, ��Ʈ �з����͸� ���� ������Ʈ �ؾ��� ���ҽ��� Ʋ
// 
class ParameterForm
{ 
#ifdef CHECK_ROOT_SIGNATURE
	ID3D12RootSignature* m_pRootSignature{ nullptr }; // ���� ����ϴ� ��Ʈ �ñ״�ó�� �´��� üũ�ϱ� ���� ���
#endif // CHECK_ROOT_SIGNATURE

	int m_VectorIndex{ 0 };
	// �ε��� �����ε� ã��, �ε����ε� ã�� ���� �� ���� �ڷᱸ�� ���
	std::map<std::string, int> m_resourceMap;
	std::vector<ResourceBase*> m_resourceVector;

private:

public:
	// ��Ʈ �ñ״�ó�� ũ�� ��ŭ ���ҽ��� ������.
	ParameterForm(int rootSignatureSize) :m_resourceVector(rootSignatureSize, nullptr) {};
	~ParameterForm();

	void ReleaseObjects();
	void SetRootSignature(ID3D12RootSignature* rootsignature)
	{
		assert(!(m_pRootSignature != nullptr)); // �ѹ��� ������ �� �ֽ��ϴ�.

		m_pRootSignature = rootsignature;
	}
	 
	ResourceBase* GetResource(int index) const { return m_resourceVector[index]; }
	ResourceBase* GetResource(const std::string& name) const
	{
		assert(m_resourceMap.find(name) != m_resourceMap.end());
		return m_resourceVector[(*m_resourceMap.find(name)).second];
	}

	// �ش� ��Ʈ �з����Ϳ� ���缭 Resource�� �����Ѵ�.
	// int parametersInde
	void InsertResource(int parametersIndex, const std::string& name, const D3D12_ROOT_PARAMETER& rootParameter);

	void UpdateShaderVariable(ID3D12GraphicsCommandList* commandList, ID3D12RootSignature* CurrentRootsignature, const std::string& name, const SourcePtr& resource);
	void UpdateShaderVariable(ID3D12GraphicsCommandList* commandList, ID3D12RootSignature* CurrentRootsignature, UINT index, const SourcePtr& resource);
};