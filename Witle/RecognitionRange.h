#pragma once
#include "ComponentBase.h"

class MyRectangle;
class LineSphere;

// ���� �ν� ������ ���� Ŭ����
class RecognitionRange
	: ComponentBase
{ 
#ifdef SHOW_DEBUGMESH
	LineSphere* m_DebugMesh{ nullptr };
#endif // SHOW_DEBUGMESH

public:
	const float m_RecognitionTime{ 0.f }; // �÷��̾ �ν��� ������ �ɸ��� �ð�
	float m_TotalTime { 0.f }; 
	const float m_RecognitionRange{ 0.f }; // �÷��̾ �ν��ϴ� ����
	 
public:
	virtual void ReleaseObjects() override;
	virtual void ReleaseUploadBuffers() override;

public:
	RecognitionRange(GameObject* pOwner, float range, float time) 
		: m_RecognitionRange(range), m_RecognitionTime(time), ComponentBase(pOwner) {};
	virtual ~RecognitionRange();

	void CreateDebugMesh(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	
	virtual void Update(float) override;

	void Render(ID3D12GraphicsCommandList *pd3dCommandList); 
};