#include "stdafx.h"
#include "Object.h"
#include "LoadedModelInfo.h"

CLoadedModelInfo::CLoadedModelInfo()
{
}

CLoadedModelInfo::~CLoadedModelInfo()
{
	if (m_ppAnimationSets) delete[] m_ppAnimationSets;
	if (m_pnAnimatedBoneFrames) delete[] m_pnAnimatedBoneFrames;
	if (m_ppSkinnedMeshes) delete[] m_ppSkinnedMeshes;

	for (int i = 0; i < m_nSkinnedMeshes; i++)
	{
		if (m_pppAnimatedBoneFrameCaches[i]) delete[] m_pppAnimatedBoneFrameCaches[i];
	}
	if (m_pppAnimatedBoneFrameCaches) delete[] m_pppAnimatedBoneFrameCaches;
}

int CLoadedModelInfo::GetCountAnimatedBoneFrames(int n) const
{
	assert(!(n >= m_nSkinnedMeshes || n < 0));
	return m_pnAnimatedBoneFrames[n];
}

CSkinnedMesh * const CLoadedModelInfo::GetSkinnedMesh(int n) const
{
	assert(!(n >= m_nSkinnedMeshes || n < 0));
	return m_ppSkinnedMeshes[n];
}

CAnimationSets * const CLoadedModelInfo::GetAnimationSet(int n) const
{
	assert(!(n >= m_nSkinnedMeshes || n < 0));
	return m_ppAnimationSets[n];
}

LoadObject * const CLoadedModelInfo::GetAnimatedBoneFrameCaches(int meshCount, int boneCount)
{
	assert(!(meshCount >= m_nSkinnedMeshes || meshCount < 0));
	return m_pppAnimatedBoneFrameCaches[meshCount][boneCount];
}

void CLoadedModelInfo::Create()
{
	assert(!(m_nSkinnedMeshes == 0));
	m_pnAnimatedBoneFrames = new int[m_nSkinnedMeshes];
	m_ppAnimationSets = new CAnimationSets*[m_nSkinnedMeshes];
	m_ppSkinnedMeshes = new CSkinnedMesh*[m_nSkinnedMeshes];
	m_pppAnimatedBoneFrameCaches = new LoadObject**[m_nSkinnedMeshes];
}

void CLoadedModelInfo::CreateAnimationSets(int nAnimationSet, int i, float fLength, int nFramesPerSecond, int nKeyFrames, char* pstrToken)
{
	m_ppAnimationSets[i]->m_ppAnimationSets[nAnimationSet] = 
		new CAnimationSet(fLength, nFramesPerSecond, nKeyFrames, m_pnAnimatedBoneFrames[i], pstrToken);
}

void CLoadedModelInfo::CreateFrameAndAnimationSets(FILE* pInFile, char* pstrToken, int nAnimationSets, int i)
{
	m_ppAnimationSets[i] = new CAnimationSets(nAnimationSets);

	int nSkin = ::ReadIntegerFromFile(pInFile); //i

	::ReadStringFromFile(pInFile, pstrToken); //Skinned Mesh Name
	m_ppSkinnedMeshes[i] = m_pModelRootObject->FindSkinnedMesh(pstrToken);
	m_ppSkinnedMeshes[i]->PrepareSkinning(m_pModelRootObject);

	m_pnAnimatedBoneFrames[i] = ::ReadIntegerFromFile(pInFile);
	m_pppAnimatedBoneFrameCaches[i] = new LoadObject*[m_pnAnimatedBoneFrames[i]];

	for (int j = 0; j < m_pnAnimatedBoneFrames[i]; j++)
	{
		::ReadStringFromFile(pInFile, pstrToken);
		m_pppAnimatedBoneFrameCaches[i][j] = m_pModelRootObject->FindFrame(pstrToken);

#ifdef _WITH_DEBUG_SKINNING_BONE
		TCHAR pstrDebug[256] = { 0 };
		TCHAR pwstrAnimationBoneName[64] = { 0 };
		TCHAR pwstrBoneCacheName[64] = { 0 };
		size_t nConverted = 0;
		mbstowcs_s(&nConverted, pwstrAnimationBoneName, 64, pstrToken, _TRUNCATE);
		mbstowcs_s(&nConverted, pwstrBoneCacheName, 64, pLoadedModel->m_pppAnimatedBoneFrameCaches[i][j]->m_pstrFrameName, _TRUNCATE);
		_stprintf_s(pstrDebug, 256, _T("AnimationBoneFrame:: Cache(%s) AnimationBone(%s)\n"), pwstrBoneCacheName, pwstrAnimationBoneName);
		OutputDebugString(pstrDebug);
#endif
	}
}

void CLoadedModelInfo::CreateKeyFrame(FILE * pInFile, int i, UINT* nReads, int nAnimationSet)
{
	int nKey = ::ReadIntegerFromFile(pInFile); //i
	float fKeyTime = ::ReadFloatFromFile(pInFile);
	for (int j = 0; j < m_nSkinnedMeshes; j++)
	{
		int nSkin = ::ReadIntegerFromFile(pInFile); //j
		CAnimationSet *pAnimationSet = m_ppAnimationSets[j]->m_ppAnimationSets[nAnimationSet];
		pAnimationSet->m_pfKeyFrameTimes[i] = fKeyTime;
		*nReads = (UINT)::fread(pAnimationSet->m_ppxmf4x4KeyFrameTransforms[i], sizeof(XMFLOAT4X4), m_pnAnimatedBoneFrames[j], pInFile);
	}
}
