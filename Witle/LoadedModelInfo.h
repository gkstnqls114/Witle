#pragma once

class LoadObject;
class CSkinnedMesh;
class CAnimationSets;

class CLoadedModelInfo
{
public:
	CLoadedModelInfo();
	~CLoadedModelInfo();

private:
	LoadObject						*m_pModelRootObject{ nullptr };

	int 							m_nSkinnedMeshes{ 0 };
	CSkinnedMesh					**m_ppSkinnedMeshes{ nullptr }; //[SkinnedMeshes], Skinned Mesh Cache

	CAnimationSets					**m_ppAnimationSets{ nullptr };

	int								*m_pnAnimatedBoneFrames{ nullptr }; //[SkinnedMeshes]
	LoadObject						***m_pppAnimatedBoneFrameCaches{ nullptr }; //[SkinnedMeshes][Bones]

public:
	////////////////////////////// Get
	LoadObject* GetRootObject() const { return m_pModelRootObject; }
	int GetCountSkinnedMesh() const { return m_nSkinnedMeshes; }
	int* GetpCountSkinnedMesh() { return &m_nSkinnedMeshes; }
	int GetCountAnimatedBoneFrames(int n) const;
	CSkinnedMesh* const GetSkinnedMesh(int n) const;
	CAnimationSets* const GetAnimationSet(int n) const;
	LoadObject* const GetAnimatedBoneFrameCaches(int meshCount, int boneCount);
	////////////////////////////// Get


	////////////////////////////// Set
	void SetRootObject(LoadObject* object) { m_pModelRootObject = object; }
	void SetCountSkinnedMesh(int n) { m_nSkinnedMeshes = n; }
	////////////////////////////// Set

	void Create();
	void CreateAnimationSets(int nAnimationSets, int i, float fLength, int nFramesPerSecond, int nKeyFrames, char* pstrToken);
	void CreateFrameAndAnimationSets(FILE* pInFile, char* pstrToken, int nAnimationSets,int i);
	void CreateKeyFrame(FILE * pInFile, int i, UINT* nReads, int nAnimationSet);
};