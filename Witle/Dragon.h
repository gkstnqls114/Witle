#pragma once
#include "Monster.h"
class Dragon
	: public Monster
{
private:
	LoadObject* m_BOBoxFrame;

protected:
	virtual void ReleaseMembers() override;
	virtual void ReleaseMemberUploadBuffers() override;
	virtual int GetAnimationHitID() override { return 0; }; // ���� �ӽ÷� 0 ��ȯ�մϴ�.

public:
	Dragon(const std::string& entityID, const XMFLOAT3& SpawnPoint, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature);
	virtual ~Dragon();

	virtual void Update(float) override;

	// ���� ������ �׼�(���ӵ������ϴ�Ŭ����)�� �����մϴ�.
	virtual void UpdateState(float fElapsedTime) override;

	virtual void Animate(float fElapsedTime) override;

};