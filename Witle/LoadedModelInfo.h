#pragma once

class LoadObject; 
class CSkinnedMesh;
class CAnimationSets;


class CLoadedModelInfo
{
public:
	CLoadedModelInfo() { }
	~CLoadedModelInfo();

	LoadObject						*m_pModelRootObject = NULL;

	int 							m_nSkinnedMeshes = 0;
	CSkinnedMesh					**m_ppSkinnedMeshes = NULL; //[SkinnedMeshes], Skinned Mesh Cache

	CAnimationSets					**m_ppAnimationSets = NULL;

	int								*m_pnAnimatedBoneFrames = NULL; //[SkinnedMeshes]
	LoadObject						***m_pppAnimatedBoneFrameCaches = NULL; //[SkinnedMeshes][Bones]
};
