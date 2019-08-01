#pragma once

class Texture; 
class MyDescriptorHeap;

class TextureStg
{
	static TextureStg* m_Instance;
	 
	// name과 texture
	std::map<std::string, Texture*> m_TextureStorage;
	std::list<std::string> m_expection;
	MyDescriptorHeap* m_Heap{ nullptr }; // 터레인 오브젝트들의 텍스쳐를 위한 설명자 힙

	bool m_isCreate{ false };

private:
	TextureStg();
	~TextureStg();

	void CreateTexture(const std::string& name, ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, int heapindex);
public:
	static TextureStg* GetInstance()
	{
		if (!m_Instance)
		{
			m_Instance = new TextureStg();
		}
		return m_Instance;
	}
	static void ReleaseInstance()
	{
		delete m_Instance;
		m_Instance = nullptr;
	}


	void ReleaseUploadBuffers();
	void ReleaseObjects();

	void CreateTextures(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList);
	Texture* const GetTexture(const std::string& name);
	const MyDescriptorHeap * GetHeap() const { return m_Heap; }
	void SetHeap(ID3D12GraphicsCommandList * pd3dCommandList);
};