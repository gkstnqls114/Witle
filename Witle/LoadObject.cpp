#include "stdafx.h"
#include "Object.h"
#include "Shader.h"
#include "d3dUtil.h"
#include "CMesh.h"
#include "LoadedModelInfo.h"
#include "ShaderManager.h"
#include "LoadObject.h"

LoadObject::LoadObject()
{
	m_xmf4x4ToParent = Matrix4x4::Identity();
	m_xmf4x4World = Matrix4x4::Identity();
}

LoadObject::LoadObject(int nMaterials) : LoadObject()
{
	m_nMaterials = nMaterials;
	if (m_nMaterials > 0)
	{
		m_ppMaterials = new CMaterial*[m_nMaterials];
		for (int i = 0; i < m_nMaterials; i++) m_ppMaterials[i] = NULL;
	}
}

LoadObject::~LoadObject()
{
	if (m_pMesh) m_pMesh->Release();

	if (m_nMaterials > 0)
	{
		for (int i = 0; i < m_nMaterials; i++)
		{
			if (m_ppMaterials[i]) m_ppMaterials[i]->Release();
		}
	}
	if (m_ppMaterials) delete[] m_ppMaterials;

	if (m_pSkinnedAnimationController) delete m_pSkinnedAnimationController;
}

void LoadObject::AddRef()
{
	m_nReferences++;

	if (m_pSibling) m_pSibling->AddRef();
	if (m_pChild) m_pChild->AddRef();
}

void LoadObject::Release()
{
	if (m_pChild) m_pChild->Release();
	if (m_pSibling) m_pSibling->Release();

	if (--m_nReferences <= 0) delete this;
}

void LoadObject::SetChild(LoadObject *pChild, bool bReferenceUpdate)
{
	if (pChild)
	{
		pChild->m_pParent = this;
		if (bReferenceUpdate) pChild->AddRef();
	}
	if (m_pChild)
	{
		if (pChild) pChild->m_pSibling = m_pChild->m_pSibling;
		m_pChild->m_pSibling = pChild;
	}
	else
	{
		m_pChild = pChild;
	}
}

void LoadObject::SetMesh(CMesh *pMesh)
{
	if (m_pMesh) m_pMesh->Release();
	m_pMesh = pMesh;
	if (m_pMesh) m_pMesh->AddRef();
}
 
void LoadObject::SetWireFrameShader()
{
	m_nMaterials = 1;
	m_ppMaterials = new CMaterial*[m_nMaterials];
	m_ppMaterials[0] = NULL;
	CMaterial *pMaterial = new CMaterial(0);
	// pMaterial->SetWireFrameShader();
	SetMaterial(0, pMaterial);
}

void LoadObject::SetSkinnedAnimationWireFrameShader()
{
	m_nMaterials = 1;
	m_ppMaterials = new CMaterial*[m_nMaterials];
	m_ppMaterials[0] = NULL;
	CMaterial *pMaterial = new CMaterial(0);
	// pMaterial->SetSkinnedAnimationWireFrameShader();
	SetMaterial(0, pMaterial);
}

void LoadObject::SetMaterial(int nMaterial, CMaterial *pMaterial)
{
	if (m_ppMaterials[nMaterial]) m_ppMaterials[nMaterial]->Release();
	m_ppMaterials[nMaterial] = pMaterial;
	if (m_ppMaterials[nMaterial]) m_ppMaterials[nMaterial]->AddRef();
}

CSkinnedMesh *LoadObject::FindSkinnedMesh(char *pstrSkinnedMeshName)
{
	CSkinnedMesh *pSkinnedMesh = NULL;
	if (m_pMesh && (m_pMesh->GetType() & VERTEXT_BONE_INDEX_WEIGHT))
	{
		pSkinnedMesh = (CSkinnedMesh *)m_pMesh;
		if (!strcmp(pSkinnedMesh->m_pstrMeshName, pstrSkinnedMeshName)) return(pSkinnedMesh);
	}

	if (m_pSibling) if (pSkinnedMesh = m_pSibling->FindSkinnedMesh(pstrSkinnedMeshName)) return(pSkinnedMesh);
	if (m_pChild) if (pSkinnedMesh = m_pChild->FindSkinnedMesh(pstrSkinnedMeshName)) return(pSkinnedMesh);

	return(NULL);
}

void LoadObject::FindAndSetSkinnedMesh(CSkinnedMesh **ppSkinnedMeshes, int *pnSkinnedMesh)
{
	if (m_pMesh && (m_pMesh->GetType() & VERTEXT_BONE_INDEX_WEIGHT)) ppSkinnedMeshes[(*pnSkinnedMesh)++] = (CSkinnedMesh *)m_pMesh;

	if (m_pSibling) m_pSibling->FindAndSetSkinnedMesh(ppSkinnedMeshes, pnSkinnedMesh);
	if (m_pChild) m_pChild->FindAndSetSkinnedMesh(ppSkinnedMeshes, pnSkinnedMesh);
}

LoadObject *LoadObject::FindFrame(char *pstrFrameName)
{
	LoadObject *pFrameObject = NULL;
	if (!strcmp(m_pstrFrameName, pstrFrameName)) return(this);

	if (m_pSibling) if (pFrameObject = m_pSibling->FindFrame(pstrFrameName)) return(pFrameObject);
	if (m_pChild) if (pFrameObject = m_pChild->FindFrame(pstrFrameName)) return(pFrameObject);

	return(NULL);
}

UINT LoadObject::GetMeshType()
{
	return((m_pMesh) ? m_pMesh->GetType() : 0x00);
}

void LoadObject::UpdateTransform(XMFLOAT4X4 *pxmf4x4Parent)
{
	m_xmf4x4World = (pxmf4x4Parent) ? Matrix4x4::Multiply(m_xmf4x4ToParent, *pxmf4x4Parent) : m_xmf4x4ToParent;

	if (m_pSibling) m_pSibling->UpdateTransform(pxmf4x4Parent);
	if (m_pChild) m_pChild->UpdateTransform(&m_xmf4x4World);
}

void LoadObject::SetTrackAnimationSet(int nAnimationTrack, int nAnimationSet)
{
	if (m_pSkinnedAnimationController) m_pSkinnedAnimationController->SetTrackAnimationSet(nAnimationTrack, nAnimationSet);
}

void LoadObject::SetTrackAnimationPosition(int nAnimationTrack, float fPosition)
{
	if (m_pSkinnedAnimationController) m_pSkinnedAnimationController->SetTrackPosition(nAnimationTrack, fPosition);
}

void LoadObject::Animate(float fTimeElapsed)
{
	OnPrepareRender();

	if (m_pSkinnedAnimationController) m_pSkinnedAnimationController->AdvanceTime(fTimeElapsed, this);

	if (m_pSibling) m_pSibling->Animate(fTimeElapsed);
	if (m_pChild) m_pChild->Animate(fTimeElapsed);
}

void LoadObject::Render(ID3D12GraphicsCommandList *pd3dCommandList)
{
	pd3dCommandList->SetPipelineState(ShaderManager::GetInstance()->GetShader("LoadFBX")->GetPSO());

	if (m_pSkinnedAnimationController) m_pSkinnedAnimationController->UpdateShaderVariables(pd3dCommandList);

	if (m_pMesh)
	{
		UpdateShaderVariable(pd3dCommandList, &m_xmf4x4World);
		m_pMesh->Render(pd3dCommandList, 0);

		if (m_nMaterials > 0)
		{
			for (int i = 0; i < m_nMaterials; i++)
			{
				// TEST
				//if (m_ppMaterials[i])
				//{
				//	if (m_ppMaterials[i]->m_pShader) m_ppMaterials[i]->m_pShader->Render(pd3dCommandList, pCamera);
				//	m_ppMaterials[i]->UpdateShaderVariable(pd3dCommandList);
				//}

				//m_pMesh->Render(pd3dCommandList, i);
			}
		}
	}

	if (m_pSibling) m_pSibling->Render(pd3dCommandList);
	if (m_pChild) m_pChild->Render(pd3dCommandList);
}

void LoadObject::CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
{
}

void LoadObject::UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList)
{
}

void LoadObject::UpdateShaderVariable(ID3D12GraphicsCommandList *pd3dCommandList, XMFLOAT4X4 *pxmf4x4World)
{
	XMFLOAT4X4 xmf4x4World;
	XMStoreFloat4x4(&xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(pxmf4x4World)));
	pd3dCommandList->SetGraphicsRoot32BitConstants(ROOTPARAMETER_WORLD, 16, &xmf4x4World, 0);
}

void LoadObject::UpdateShaderVariable(ID3D12GraphicsCommandList *pd3dCommandList, CMaterial *pMaterial)
{
}

void LoadObject::ReleaseShaderVariables()
{
}

void LoadObject::ReleaseUploadBuffers()
{
	if (m_pMesh) m_pMesh->ReleaseUploadBuffers();

	for (int i = 0; i < m_nMaterials; i++)
	{
		if (m_ppMaterials[i]) m_ppMaterials[i]->ReleaseUploadBuffers();
	}

	if (m_pSibling) m_pSibling->ReleaseUploadBuffers();
	if (m_pChild) m_pChild->ReleaseUploadBuffers();
}

void LoadObject::SetPosition(float x, float y, float z)
{
	m_xmf4x4ToParent._41 = x;
	m_xmf4x4ToParent._42 = y;
	m_xmf4x4ToParent._43 = z;

	UpdateTransform(NULL);
}

void LoadObject::SetPosition(XMFLOAT3 xmf3Position)
{
	SetPosition(xmf3Position.x, xmf3Position.y, xmf3Position.z);
}

void LoadObject::SetScale(float x, float y, float z)
{
	XMMATRIX mtxScale = XMMatrixScaling(x, y, z);
	m_xmf4x4ToParent = Matrix4x4::Multiply(mtxScale, m_xmf4x4ToParent);

	UpdateTransform(NULL);
}

XMFLOAT3 LoadObject::GetPosition()
{
	return(XMFLOAT3(m_xmf4x4World._41, m_xmf4x4World._42, m_xmf4x4World._43));
}

XMFLOAT3 LoadObject::GetLook()
{
	return(Vector3::Normalize(XMFLOAT3(m_xmf4x4World._31, m_xmf4x4World._32, m_xmf4x4World._33)));
}

XMFLOAT3 LoadObject::GetUp()
{
	return(Vector3::Normalize(XMFLOAT3(m_xmf4x4World._21, m_xmf4x4World._22, m_xmf4x4World._23)));
}

XMFLOAT3 LoadObject::GetRight()
{
	return(Vector3::Normalize(XMFLOAT3(m_xmf4x4World._11, m_xmf4x4World._12, m_xmf4x4World._13)));
}

void LoadObject::MoveStrafe(float fDistance)
{
	XMFLOAT3 xmf3Position = GetPosition();
	XMFLOAT3 xmf3Right = GetRight();
	xmf3Position = Vector3::Add(xmf3Position, xmf3Right, fDistance);
	LoadObject::SetPosition(xmf3Position);
}

void LoadObject::MoveUp(float fDistance)
{
	XMFLOAT3 xmf3Position = GetPosition();
	XMFLOAT3 xmf3Up = GetUp();
	xmf3Position = Vector3::Add(xmf3Position, xmf3Up, fDistance);
	LoadObject::SetPosition(xmf3Position);
}

void LoadObject::MoveForward(float fDistance)
{
	XMFLOAT3 xmf3Position = GetPosition();
	XMFLOAT3 xmf3Look = GetLook();
	xmf3Position = Vector3::Add(xmf3Position, xmf3Look, fDistance);
	LoadObject::SetPosition(xmf3Position);
}

void LoadObject::Rotate(float fPitch, float fYaw, float fRoll)
{
	XMMATRIX mtxRotate = XMMatrixRotationRollPitchYaw(XMConvertToRadians(fPitch), XMConvertToRadians(fYaw), XMConvertToRadians(fRoll));
	m_xmf4x4ToParent = Matrix4x4::Multiply(mtxRotate, m_xmf4x4ToParent);

	UpdateTransform(NULL);
}

void LoadObject::Rotate(XMFLOAT3 *pxmf3Axis, float fAngle)
{
	XMMATRIX mtxRotate = XMMatrixRotationAxis(XMLoadFloat3(pxmf3Axis), XMConvertToRadians(fAngle));
	m_xmf4x4ToParent = Matrix4x4::Multiply(mtxRotate, m_xmf4x4ToParent);

	UpdateTransform(NULL);
}

void LoadObject::Rotate(XMFLOAT4 *pxmf4Quaternion)
{
	XMMATRIX mtxRotate = XMMatrixRotationQuaternion(XMLoadFloat4(pxmf4Quaternion));
	m_xmf4x4ToParent = Matrix4x4::Multiply(mtxRotate, m_xmf4x4ToParent);

	UpdateTransform(NULL);
}

//#define _WITH_DEBUG_FRAME_HIERARCHY

//int ReadIntegerFromFile(FILE *pInFile)
//{
//	int nValue = 0;
//	UINT nReads = (UINT)::fread(&nValue, sizeof(int), 1, pInFile); 
//	return(nValue);
//}
//
//float ReadFloatFromFile(FILE *pInFile)
//{
//	float fValue = 0;
//	UINT nReads = (UINT)::fread(&fValue, sizeof(float), 1, pInFile); 
//	return(fValue);
//}
//
//int ReadStringFromFile(FILE *pInFile, char *pstrToken)
//{
//	int nStrLength = 0;
//	UINT nReads = 0;
//	nReads = (UINT)::fread(&nStrLength, sizeof(int), 1, pInFile);
//	nReads = (UINT)::fread(pstrToken, sizeof(char), nStrLength, pInFile); 
//	pstrToken[nStrLength] = '\0';
//
//	return(nStrLength);
//}

LoadObject *LoadObject::LoadFrameHierarchyFromFile(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, LoadObject *pParent, FILE *pInFile, int *pnSkinnedMeshes)
{
	char pstrToken[64] = { '\0' };
	UINT nReads = 0;

	int nFrame = d3dUtil::ReadIntegerFromFile(pInFile);

	LoadObject *pGameObject = new LoadObject();
	d3dUtil::ReadStringFromFile(pInFile, pGameObject->m_pstrFrameName);

	for (; ; )
	{
		d3dUtil::ReadStringFromFile(pInFile, pstrToken);
		if (!strcmp(pstrToken, "<Transform>:"))
		{
			nReads = (UINT)::fread(&pGameObject->m_xmf4x4ToParent, sizeof(XMFLOAT4X4), 1, pInFile);
		}
		else if (!strcmp(pstrToken, "<Mesh>:"))
		{
			CMesh *pMesh = new CMesh(pd3dDevice, pd3dCommandList);
			pMesh->LoadMeshFromFile(pd3dDevice, pd3dCommandList, pInFile);
			pGameObject->SetMesh(pMesh);

			/**/pGameObject->SetWireFrameShader();
		}
		else if (!strcmp(pstrToken, "<SkinDeformations>:"))
		{
			if (pnSkinnedMeshes) (*pnSkinnedMeshes)++;

			CSkinnedMesh *pSkinnedMesh = new CSkinnedMesh(pd3dDevice, pd3dCommandList);
			pSkinnedMesh->LoadSkinDeformationsFromFile(pd3dDevice, pd3dCommandList, pInFile);
			pSkinnedMesh->CreateShaderVariables(pd3dDevice, pd3dCommandList);

			d3dUtil::ReadStringFromFile(pInFile, pstrToken); //<Mesh>:
			if (!strcmp(pstrToken, "<Mesh>:")) pSkinnedMesh->LoadMeshFromFile(pd3dDevice, pd3dCommandList, pInFile);

			pGameObject->SetMesh(pSkinnedMesh);

			/**/pGameObject->SetSkinnedAnimationWireFrameShader();
		}
		else if (!strcmp(pstrToken, "<Children>:"))
		{
			int nChilds = d3dUtil::ReadIntegerFromFile(pInFile);
			if (nChilds > 0)
			{
				for (int i = 0; i < nChilds; i++)
				{
					d3dUtil::ReadStringFromFile(pInFile, pstrToken);
					if (!strcmp(pstrToken, "<Frame>:"))
					{
						LoadObject *pChild = LoadObject::LoadFrameHierarchyFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, pGameObject, pInFile, pnSkinnedMeshes);
						if (pChild) pGameObject->SetChild(pChild);
#ifdef _WITH_DEBUG_FRAME_HIERARCHY
						TCHAR pstrDebug[256] = { 0 };
						_stprintf_s(pstrDebug, 256, _T("(Frame: %p) (Parent: %p)\n"), pChild, pGameObject);
						OutputDebugString(pstrDebug);
#endif
					}
				}
			}
		}
		else if (!strcmp(pstrToken, "</Frame>"))
		{
			break;
		}
	}
	return(pGameObject);
}

void LoadObject::PrintFrameInfo(LoadObject *pGameObject, LoadObject *pParent)
{
	TCHAR pstrDebug[256] = { 0 };

	_stprintf_s(pstrDebug, 256, _T("(Frame: %p) (Parent: %p)\n"), pGameObject, pParent);
	OutputDebugString(pstrDebug);

	if (pGameObject->m_pSibling) LoadObject::PrintFrameInfo(pGameObject->m_pSibling, pParent);
	if (pGameObject->m_pChild) LoadObject::PrintFrameInfo(pGameObject->m_pChild, pGameObject);
}

void LoadObject::LoadAnimationFromFile(FILE *pInFile, CLoadedModelInfo *pLoadedModel)
{
	char pstrToken[64] = { '\0' };
	UINT nReads = 0;

	int nAnimationSets = 0;

	for (; ; )
	{
		d3dUtil::ReadStringFromFile(pInFile, pstrToken);
		if (!strcmp(pstrToken, "<AnimationSets>:"))
		{
			nAnimationSets = d3dUtil::ReadIntegerFromFile(pInFile);
			pLoadedModel->m_pAnimationSets = new CAnimationSets(nAnimationSets);
		}
		else if (!strcmp(pstrToken, "<AnimationSet>:"))
		{
			int nAnimationSet = d3dUtil::ReadIntegerFromFile(pInFile);

			d3dUtil::ReadStringFromFile(pInFile, pstrToken); //Animation Set Name

			float fStartTime = d3dUtil::ReadFloatFromFile(pInFile);
			float fEndTime = d3dUtil::ReadFloatFromFile(pInFile);

			pLoadedModel->m_pAnimationSets->m_ppAnimationSets[nAnimationSet] = new CAnimationSet(fStartTime, fEndTime, pstrToken);
			CAnimationSet *pAnimationSet = pLoadedModel->m_pAnimationSets->m_ppAnimationSets[nAnimationSet];

			d3dUtil::ReadStringFromFile(pInFile, pstrToken);
			if (!strcmp(pstrToken, "<AnimationLayers>:"))
			{
				pAnimationSet->m_nAnimationLayers = d3dUtil::ReadIntegerFromFile(pInFile);
				pAnimationSet->m_pAnimationLayers = new CAnimationLayer[pAnimationSet->m_nAnimationLayers];

				for (int i = 0; i < pAnimationSet->m_nAnimationLayers; i++)
				{
					d3dUtil::ReadStringFromFile(pInFile, pstrToken);
					if (!strcmp(pstrToken, "<AnimationLayer>:"))
					{
						int nAnimationLayer = d3dUtil::ReadIntegerFromFile(pInFile);
						CAnimationLayer *pAnimationLayer = &pAnimationSet->m_pAnimationLayers[nAnimationLayer];

						pAnimationLayer->m_nAnimatedBoneFrames = d3dUtil::ReadIntegerFromFile(pInFile);

						pAnimationLayer->m_ppAnimatedBoneFrameCaches = new LoadObject *[pAnimationLayer->m_nAnimatedBoneFrames];
						pAnimationLayer->m_ppAnimationCurves = new CAnimationCurve *[pAnimationLayer->m_nAnimatedBoneFrames][9];

						pAnimationLayer->m_fWeight = d3dUtil::ReadFloatFromFile(pInFile);

						for (int j = 0; j < pAnimationLayer->m_nAnimatedBoneFrames; j++)
						{
							d3dUtil::ReadStringFromFile(pInFile, pstrToken);
							if (!strcmp(pstrToken, "<AnimationCurve>:"))
							{
								int nCurveNode = d3dUtil::ReadIntegerFromFile(pInFile); //j

								for (int k = 0; k < 9; k++) pAnimationLayer->m_ppAnimationCurves[j][k] = NULL;

								d3dUtil::ReadStringFromFile(pInFile, pstrToken);
								pAnimationLayer->m_ppAnimatedBoneFrameCaches[j] = pLoadedModel->m_pModelRootObject->FindFrame(pstrToken);

								for (; ; )
								{
									d3dUtil::ReadStringFromFile(pInFile, pstrToken);
									if (!strcmp(pstrToken, "<TX>:")) pAnimationLayer->LoadAnimationKeyValues(j, 0, pInFile);
									else if (!strcmp(pstrToken, "<TY>:")) pAnimationLayer->LoadAnimationKeyValues(j, 1, pInFile);
									else if (!strcmp(pstrToken, "<TZ>:")) pAnimationLayer->LoadAnimationKeyValues(j, 2, pInFile);
									else if (!strcmp(pstrToken, "<RX>:")) pAnimationLayer->LoadAnimationKeyValues(j, 3, pInFile);
									else if (!strcmp(pstrToken, "<RY>:")) pAnimationLayer->LoadAnimationKeyValues(j, 4, pInFile);
									else if (!strcmp(pstrToken, "<RZ>:")) pAnimationLayer->LoadAnimationKeyValues(j, 5, pInFile);
									else if (!strcmp(pstrToken, "<SX>:")) pAnimationLayer->LoadAnimationKeyValues(j, 6, pInFile);
									else if (!strcmp(pstrToken, "<SY>:")) pAnimationLayer->LoadAnimationKeyValues(j, 7, pInFile);
									else if (!strcmp(pstrToken, "<SZ>:")) pAnimationLayer->LoadAnimationKeyValues(j, 8, pInFile);
									else if (!strcmp(pstrToken, "</AnimationCurve>"))
									{
										break;
									}
								}
							}
						}
						d3dUtil::ReadStringFromFile(pInFile, pstrToken); //</AnimationLayer>
					}
				}
				d3dUtil::ReadStringFromFile(pInFile, pstrToken); //</AnimationLayers>
			}
			d3dUtil::ReadStringFromFile(pInFile, pstrToken); //</AnimationSet>
		}
		else if (!strcmp(pstrToken, "</AnimationSets>"))
		{
			break;
		}
	}
}

CLoadedModelInfo *LoadObject::LoadGeometryAndAnimationFromFile(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, const char *pstrFileName)
{
	FILE *pInFile = NULL;
	::fopen_s(&pInFile, pstrFileName, "rb");
	::rewind(pInFile);

	CLoadedModelInfo *pLoadedModel = new CLoadedModelInfo();
	pLoadedModel->m_pModelRootObject = new LoadObject();
	strcpy_s(pLoadedModel->m_pModelRootObject->m_pstrFrameName, "RootNode");

	char pstrToken[64] = { '\0' };

	for (; ; )
	{
		if (d3dUtil::ReadStringFromFile(pInFile, pstrToken))
		{
			if (!strcmp(pstrToken, "<Hierarchy>"))
			{
				for (; ; )
				{
					d3dUtil::ReadStringFromFile(pInFile, pstrToken);
					if (!strcmp(pstrToken, "<Frame>:"))
					{
						LoadObject *pChild = LoadObject::LoadFrameHierarchyFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, NULL, pInFile, &pLoadedModel->m_nSkinnedMeshes);
						if (pChild) pLoadedModel->m_pModelRootObject->SetChild(pChild);
					}
					else if (!strcmp(pstrToken, "</Hierarchy>"))
					{
						break;
					}
				}
			}
			else if (!strcmp(pstrToken, "<Animation>"))
			{
				LoadObject::LoadAnimationFromFile(pInFile, pLoadedModel);
				pLoadedModel->PrepareSkinning();
			}
			else if (!strcmp(pstrToken, "</Animation>"))
			{
				break;
			}
		}
		else
		{
			break;
		}
	}

#ifdef _WITH_DEBUG_FRAME_HIERARCHY
	TCHAR pstrDebug[256] = { 0 };
	_stprintf_s(pstrDebug, 256, _T("Frame Hierarchy\n"));
	OutputDebugString(pstrDebug);

	LoadObject::PrintFrameInfo(pLoadedModel->m_pModelRootObject, NULL);
#endif

	::fclose(pInFile);

	return(pLoadedModel);
}

 
/////////////////
void LoadObject::CopyWorldMatrix(LoadObject* copy, LoadObject* copyed)
{
	copy->m_pMesh = copyed->m_pMesh;
	copy->m_xmf4x4ToParent = copyed->m_xmf4x4ToParent;
	copy->m_xmf4x4World = copyed->m_xmf4x4World;

	if (copyed->m_pParent) copy->m_pSibling = copyed->m_pParent;
	if (copyed->m_pSibling)
	{
		copy->m_pSibling = new LoadObject;
		CopyWorldMatrix(copy->m_pSibling, copyed->m_pSibling);
	}
	if (copyed->m_pChild)
	{
		copy->m_pChild = new LoadObject;
		CopyWorldMatrix(copy->m_pChild, copyed->m_pChild);
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// 
//CHeightMapTerrain::CHeightMapTerrain(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, LPCTSTR pFileName, int nWidth, int nLength, XMFLOAT3 xmf3Scale, XMFLOAT4 xmf4Color) : CGameObject(1)
//{
//	m_nWidth = nWidth;
//	m_nLength = nLength;
//
//	m_xmf3Scale = xmf3Scale;
//
//	m_pHeightMapImage = new CHeightMapImage(pFileName, nWidth, nLength, xmf3Scale);
//
//	CHeightMapGridMesh *pMesh = new CHeightMapGridMesh(pd3dDevice, pd3dCommandList, 0, 0, nWidth, nLength, xmf3Scale, xmf4Color, m_pHeightMapImage);
//	SetMesh(pMesh);
//
//	CreateShaderVariables(pd3dDevice, pd3dCommandList);
//
//	CTexture *pTerrainBaseTexture = new CTexture(1, RESOURCE_TEXTURE2D, 0);
//	pTerrainBaseTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Terrain/Base_Texture.dds", 0);
//
//	CTexture *pTerrainDetailTexture = new CTexture(1, RESOURCE_TEXTURE2D, 0);
//	pTerrainDetailTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Terrain/Detail_Texture_7.dds", 0);
//
//	CTerrainShader *pTerrainShader = new CTerrainShader();
//	pTerrainShader->CreateShader(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);
//	pTerrainShader->CreateShaderVariables(pd3dDevice, pd3dCommandList);
//
//	CScene::CreateShaderResourceViews(pd3dDevice, pTerrainBaseTexture, 13, false);
//	CScene::CreateShaderResourceViews(pd3dDevice, pTerrainDetailTexture, 14, false);
//
//	CMaterial *pTerrainMaterial = new CMaterial(2);
//	pTerrainMaterial->SetTexture(pTerrainBaseTexture, 0);
//	pTerrainMaterial->SetTexture(pTerrainDetailTexture, 1);
//	pTerrainMaterial->SetShader(pTerrainShader);
//
//	SetMaterial(0, pTerrainMaterial);
//}
//
//CHeightMapTerrain::~CHeightMapTerrain(void)
//{
//	if (m_pHeightMapImage) delete m_pHeightMapImage;
//}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
CSkyBox::CSkyBox(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature) : LoadObject(1)
{
	CSkyBoxMesh *pSkyBoxMesh = new CSkyBoxMesh(pd3dDevice, pd3dCommandList, 20.0f, 20.0f, 2.0f);
	SetMesh(pSkyBoxMesh);

	CreateShaderVariables(pd3dDevice, pd3dCommandList);

	CTexture *pSkyBoxTexture = new CTexture(1, RESOURCE_TEXTURE_CUBE, 0);
	pSkyBoxTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"SkyBox/SkyBox_0.dds", 0);

	// CSkyBoxShader *pSkyBoxShader = new CSkyBoxShader();
	// pSkyBoxShader->CreateShader(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);
	// pSkyBoxShader->CreateShaderVariables(pd3dDevice, pd3dCommandList);

	// CScene::CreateShaderResourceViews(pd3dDevice, pSkyBoxTexture, 10, false);

	CMaterial *pSkyBoxMaterial = new CMaterial(1);
	pSkyBoxMaterial->SetTexture(pSkyBoxTexture);
	// pSkyBoxMaterial->SetShader(pSkyBoxShader);

	SetMaterial(0, pSkyBoxMaterial);
}

CSkyBox::~CSkyBox()
{
}

void CSkyBox::Render(ID3D12GraphicsCommandList *pd3dCommandList)
{
	//XMFLOAT3 xmf3CameraPos = pCamera->GetPosition();
	//SetPosition(xmf3CameraPos.x, xmf3CameraPos.y, xmf3CameraPos.z);

	LoadObject::Render(pd3dCommandList);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
CAngrybotObject::CAngrybotObject(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, CLoadedModelInfo *pModel, int nAnimationTracks)
{
	CLoadedModelInfo *pAngrybotModel = pModel;
	if (!pAngrybotModel) pAngrybotModel = LoadObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/Player.bin");

	SetChild(pAngrybotModel->m_pModelRootObject, true);
	m_pSkinnedAnimationController = new CAnimationController(pd3dDevice, pd3dCommandList, nAnimationTracks, pAngrybotModel);

	Rotate(-90.0f, 0.0f, 0.0f);
	SetScale(0.2f, 0.2f, 0.2f);
}

CAngrybotObject::~CAngrybotObject()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
CElvenWitchObject::CElvenWitchObject(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, CLoadedModelInfo *pModel, int nAnimationTracks)
{
	CLoadedModelInfo *pElvenWitchModel = pModel;
	if (!pElvenWitchModel) pElvenWitchModel = LoadObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/Elven_Witch.bin");

	SetChild(pElvenWitchModel->m_pModelRootObject, true);
	m_pSkinnedAnimationController = new CAnimationController(pd3dDevice, pd3dCommandList, nAnimationTracks, pElvenWitchModel);

	Rotate(-90.0f, 0.0f, 0.0f);
	SetScale(0.0025f, 0.0025f, 0.0025f);
}

CElvenWitchObject::~CElvenWitchObject()
{
}


