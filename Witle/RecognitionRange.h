#pragma once
#include "ComponentBase.h"

class MyRectangle;
class LineSphere;

// ���� �ν� ������ ���� Ŭ����
class RecognitionRange
	: ComponentBase
{
#ifdef USING_DEBUGMESH
	LineSphere* m_DebugMesh;
#endif // DEBUG

	float m_RecognitionRange{ 0.f }; // �÷��̾ �ν��ϴ� ����

public:
	int m_HP{ 1000 };

public:
	virtual void ReleaseObjects() override;
	virtual void ReleaseUploadBuffers() override;

public:
	RecognitionRange(GameObject* pOwner);
	virtual ~RecognitionRange();

	void CreateDebugMesh(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	
	virtual void Update(float) override;

	void Render(ID3D12GraphicsCommandList *pd3dCommandList);
	void RenderDebug(ID3D12GraphicsCommandList *pd3dCommandList);
};