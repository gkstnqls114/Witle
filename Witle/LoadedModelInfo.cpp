#include "stdafx.h"
#include "d3dUtil.h"
#include "Object.h"
#include "LoadedModelInfo.h"
 

CLoadedModelInfo::~CLoadedModelInfo()
{
	if (m_ppAnimationSets) delete[] m_ppAnimationSets;
	if (m_pnAnimatedBoneFrames) delete[] m_pnAnimatedBoneFrames;
	if (m_ppSkinnedMeshes) delete[] m_ppSkinnedMeshes;

	if (m_pppAnimatedBoneFrameCaches)
	{
		for (int i = 0; i < m_nSkinnedMeshes; i++)
		{
			if (m_pppAnimatedBoneFrameCaches[i]) delete[] m_pppAnimatedBoneFrameCaches[i];
		}
	}
	if (m_pppAnimatedBoneFrameCaches) delete[] m_pppAnimatedBoneFrameCaches;
}
 