#pragma once


class MyBOBox;
class CAnimationController;
class CMesh;
class CMaterial;
class CTexture;
class CSkinnedMesh;
class CLoadedModelInfo;

class LoadObject
{
protected:

	MyBOBox* m_MyBOBox{ nullptr };

private:
	int								m_nReferences = 0;

public:
	void AddRef();
	void Release();

public:
	LoadObject();
	LoadObject(int nMaterials);
	virtual ~LoadObject();

public:
	char							m_pstrFrameName[64];

	CMesh							*m_pMesh = NULL;

	int								m_nMaterials = 0;
	CMaterial						**m_ppMaterials = NULL;

	XMFLOAT4X4						m_xmf4x4ToParent;
	XMFLOAT4X4						m_xmf4x4World;

	LoadObject 					*m_pParent = NULL;
	LoadObject 					*m_pChild = NULL;
	LoadObject 					*m_pSibling = NULL;

	void SetMesh(CMesh *pMesh);  
	void SetWireFrameShader();
	void SetSkinnedAnimationWireFrameShader();
	void SetMaterial(int nMaterial, CMaterial *pMaterial);

	void SetChild(LoadObject *pChild, bool bReferenceUpdate = false);

	virtual void BuildMaterials(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList) { }

	virtual void OnPrepareAnimate() { }
	virtual void Animate(float fTimeElapsed);

	virtual void OnPrepareRender() { }
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList);

	virtual void CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void ReleaseShaderVariables();

	virtual void UpdateShaderVariable(ID3D12GraphicsCommandList *pd3dCommandList, XMFLOAT4X4 *pxmf4x4World);
	virtual void UpdateShaderVariable(ID3D12GraphicsCommandList *pd3dCommandList, CMaterial *pMaterial);

	virtual void ReleaseUploadBuffers();

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetLook();
	XMFLOAT3 GetUp();
	XMFLOAT3 GetRight();

	void SetPosition(float x, float y, float z);
	void SetPosition(XMFLOAT3 xmf3Position);
	void SetScale(float x, float y, float z);

	void MoveStrafe(float fDistance = 1.0f);
	void MoveUp(float fDistance = 1.0f);
	void MoveForward(float fDistance = 1.0f);

	void Rotate(float fPitch = 10.0f, float fYaw = 10.0f, float fRoll = 10.0f);
	void Rotate(XMFLOAT3 *pxmf3Axis, float fAngle);
	void Rotate(XMFLOAT4 *pxmf4Quaternion);

	LoadObject *GetParent() { return(m_pParent); }
	void UpdateTransform(XMFLOAT4X4 *pxmf4x4Parent = NULL);
	LoadObject *FindFrame(char *pstrFrameName);

	UINT GetMeshType();

public:
	CAnimationController 			*m_pSkinnedAnimationController = NULL;

	CSkinnedMesh *FindSkinnedMesh(char *pstrSkinnedMeshName);
	void FindAndSetSkinnedMesh(CSkinnedMesh **ppSkinnedMeshes, int *pnSkinnedMesh);

	void SetTrackAnimationSet(int nAnimationTrack, int nAnimationSet);
	void SetTrackAnimationPosition(int nAnimationTrack, float fPosition);

	static void LoadAnimationFromFile(FILE *pInFile, CLoadedModelInfo *pLoadedModel);
	static LoadObject *LoadFrameHierarchyFromFile(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, LoadObject *pParent, FILE *pInFile, int *pnSkinnedMeshes);

	static CLoadedModelInfo *LoadGeometryAndAnimationFromFile(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, const char *pstrFileName);

	static void PrintFrameInfo(LoadObject *pGameObject, LoadObject *pParent);
	static void CopyWorldMatrix(LoadObject* copy, LoadObject* copyed);

	// 내가 추가
	MyBOBox* GetBOBox() { return m_MyBOBox; }
};

//
//class CHeightMapTerrain : public LoadObject
//{
//public:
//	CHeightMapTerrain(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, LPCTSTR pFileName, int nWidth, int nLength, XMFLOAT3 xmf3Scale, XMFLOAT4 xmf4Color);
//	virtual ~CHeightMapTerrain();
//
//private:
//	CHeightMapImage				*m_pHeightMapImage;
//
//	int							m_nWidth;
//	int							m_nLength;
//
//	XMFLOAT3					m_xmf3Scale;
//
//public:
//	float GetHeight(float x, float z, bool bReverseQuad = false) { return(m_pHeightMapImage->GetHeight(x, z, bReverseQuad) * m_xmf3Scale.y); } //World
//	XMFLOAT3 GetNormal(float x, float z) { return(m_pHeightMapImage->GetHeightMapNormal(int(x / m_xmf3Scale.x), int(z / m_xmf3Scale.z))); }
//
//	int GetHeightMapWidth() { return(m_pHeightMapImage->GetHeightMapWidth()); }
//	int GetHeightMapLength() { return(m_pHeightMapImage->GetHeightMapLength()); }
//
//	XMFLOAT3 GetScale() { return(m_xmf3Scale); }
//	float GetWidth() { return(m_nWidth * m_xmf3Scale.x); }
//	float GetLength() { return(m_nLength * m_xmf3Scale.z); }
//};

class CSkyBox : public LoadObject
{
public:
	CSkyBox(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature);
	virtual ~CSkyBox();

	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList);
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
class CAngrybotObject : public LoadObject
{
public:
	CAngrybotObject(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, CLoadedModelInfo *pModel, int nAnimationTracks);
	virtual ~CAngrybotObject();
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
class CElvenWitchObject : public LoadObject
{
public:
	CElvenWitchObject(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, CLoadedModelInfo *pModel, int nAnimationTracks);
	virtual ~CElvenWitchObject();
};
