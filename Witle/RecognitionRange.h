#pragma once
#include "ComponentBase.h"

class MyRectangle;
class LineSphere;

// 몬스터 인식 범위에 대한 클래스
class RecognitionRange
	: ComponentBase
{ 
	LineSphere* m_DebugMesh;

public:
	const float m_RecognitionRange{ 0.f }; // 플레이어를 인식하는 범위
	 
public:
	virtual void ReleaseObjects() override;
	virtual void ReleaseUploadBuffers() override;

public:
	RecognitionRange(GameObject* pOwner, float range) : m_RecognitionRange(range), ComponentBase(pOwner) {};
	virtual ~RecognitionRange();

	void CreateDebugMesh(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	
	virtual void Update(float) override;

	void Render(ID3D12GraphicsCommandList *pd3dCommandList);
	void RenderDebug(ID3D12GraphicsCommandList *pd3dCommandList);
};