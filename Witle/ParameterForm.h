#pragma once

class ResourceBase;

// ��Ʈ �ñ״�ó, ��Ʈ �з����͸� ���� ������Ʈ �ؾ��� ���ҽ��� Ʋ
// 
class ParameterForm
{ 
	class Form
	{
	public:
		virtual void UpdateShaderVariable(ID3D12GraphicsCommandList* commandList, UINT index, const SourcePtr& resource) = 0;
		virtual void ReleaseShaderVariables() = 0;
	};

	class FormCBV : public Form
	{
	public:
		virtual void UpdateShaderVariable(ID3D12GraphicsCommandList* commandList, UINT index, const SourcePtr& resource) override;
		virtual void ReleaseShaderVariables();
	};

	class FormConstant : public Form
	{
		UINT m_4ByteSize{ 0 };
	public:
		FormConstant(int num4byte) : m_4ByteSize(num4byte) {};
		virtual void UpdateShaderVariable(ID3D12GraphicsCommandList* commandList, UINT index, const SourcePtr& resource) override;
		virtual void ReleaseShaderVariables() {};
	};

#ifdef CHECK_ROOT_SIGNATURE
	ID3D12RootSignature* m_pRootSignature{ nullptr }; // ���� ����ϴ� ��Ʈ �ñ״�ó�� �´��� üũ�ϱ� ���� ���
#endif // CHECK_ROOT_SIGNATURE

	// �ε��� �����ε� ã��, �ε����ε� ã�� ���� �� ���� �ڷᱸ�� ���
	std::map<std::string, int> m_resourceMap;
	std::vector<Form*> m_resourceVector;

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
	 
	Form* GetResource(int index) const { return m_resourceVector[index]; }
	Form* GetResource(const std::string& name, int& index) const
	{
		assert(m_resourceMap.find(name) != m_resourceMap.end());
		index = (*m_resourceMap.find(name)).second;
		return m_resourceVector[index];
	}
	int GetIndex(const std::string& name) const
	{ 
		assert(m_resourceMap.find(name) != m_resourceMap.end());
		return (*m_resourceMap.find(name)).second;
	}

	// �ش� ��Ʈ �з����Ϳ� ���缭 Resource�� �����Ѵ�.
	// int parametersInde
	void InsertResource(int parametersIndex, const std::string& name, const D3D12_ROOT_PARAMETER& rootParameter);

	void UpdateShaderVariable(ID3D12GraphicsCommandList* commandList, ID3D12RootSignature* CurrentRootsignature, const std::string& name, const SourcePtr& resource);
	void UpdateShaderVariable(ID3D12GraphicsCommandList* commandList, ID3D12RootSignature* CurrentRootsignature, UINT index, const SourcePtr& resource);
};