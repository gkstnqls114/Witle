#pragma once
#include "ResourceBase.h"

/*
쉐이더에 전달하는 루트 상수 클래스 입니다.
*/ 
class RootConstants :
	public ResourceBase
{ 

public: 
	RootConstants(UINT paraIndex, UINT elementSize);
	virtual ~RootConstants();

	virtual void CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList) override {};
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList, void* data) override ;
	virtual void ReleaseShaderVariables() override {};

};
 
