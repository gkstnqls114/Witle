#pragma once
#include "MyCollider.h"

class LineCube;

class MyBBox
	: public MyCollider
{
#ifdef _DEBUG
	static bool RENDER_BBOX;
#endif // _DEBUG

public:
	virtual void ReleaseObjects() override;
	virtual void ReleaseUploadBuffers() override;

private:
	// �ش� Degree ���� �´� ȸ�� ����� ���մϴ�.
	XMFLOAT4X4 SetRotate(float x, float y, float z);

protected:
	XMFLOAT4X4 m_world;
	XMFLOAT3 m_Pivot; // ȸ���߽�
#ifdef _DEBUG
	LineCube* m_pLineCube{ nullptr };
#endif // _DEBUG

	BoundingBox m_BOBox;
	XMFLOAT4 m_BoBoxPlane[4]; // 0: +X, 1: -X, 2: +Z, 3: -Z

public:
	static void CHANGEMODE();

	MyBBox(GameObject* pOwner, ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, XMFLOAT3 center, XMFLOAT3 extents, XMFLOAT4 quaternion = XMFLOAT4(0.f, 0.f, 0.f, 1.f));
	MyBBox(const MyBBox& other);
	MyBBox(XMFLOAT3 center, XMFLOAT3 extents);
	virtual ~MyBBox();

	void Render(ID3D12GraphicsCommandList* pd3dCommandList);
	void RenderInstancing(ID3D12GraphicsCommandList* pd3dCommandList, int InstancingCount);
	 
	void Move(const XMFLOAT3& xmf3Shift);
	void Move(float x, float y, float z);
	void SetPosition(const XMFLOAT3& pos);

	BoundingBox GetBOBox() const { return m_BOBox; }

	// 0: +X ��, 1: -X��, 2: +Z��, 3: -Z��
	const XMFLOAT4 GetPlane(int index) const
	{
		assert(!(index < 0 && index >= 4));
		return m_BoBoxPlane[index];
	}

	XMFLOAT3 GetPosOnPlane(int index) const
	{
		assert(!(index < 0 && index >= 4));
		switch (index)
		{
		case 0:
			return Vector3::Add(m_BOBox.Center, XMFLOAT3(m_BOBox.Extents.x, 0.f, 0.f));
			break;

		case 1:
			return Vector3::Add(m_BOBox.Center, XMFLOAT3(-m_BOBox.Extents.x, 0.f, 0.f));
			break;

		case 2:
			return Vector3::Add(m_BOBox.Center, XMFLOAT3(0.f, 0.f, m_BOBox.Extents.z));
			break;

		case 3:
			return Vector3::Add(m_BOBox.Center, XMFLOAT3(0.f, 0.f, -m_BOBox.Extents.z));
			break;
		}
	}

	const XMFLOAT3 GetPlaneNormal(int index) const
	{
		assert(!(index < 0 && index >= 4));
		return XMFLOAT3{ m_BoBoxPlane[index].x, m_BoBoxPlane[index].y, m_BoBoxPlane[index].z };
	}

	// pos�� index�� �ش��ϸ� ��ȿ� ������ Ȯ���Ѵ�.
	// ���� ȸ���� �����ʴ´ٴ� �����Ͽ� �����ϹǷ� ȸ��������� ���� ����
	bool IsIn(int index, const XMFLOAT3& pos);

	// LineCube�� �׸��� �ʴ´�.
	MyBBox operator=(const MyBBox& other)
	{
#ifdef USING_DEBUGMESH
		m_pLineCube = nullptr;
#endif // USING_DEBUGMESH

		m_BOBox = other.m_BOBox;
		for (int i = 0; i < 4; ++i)
		{
			m_BoBoxPlane[i] = other.m_BoBoxPlane[i];
		}
		return *this;
	}
};