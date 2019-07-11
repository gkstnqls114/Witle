#pragma once
#include "GameObject.h"

class Camera;
class CLoadedModelInfo;
class LoadObject;
class MyBOBox;
class Texture;
class MyDescriptorHeap;

class LineMesh;
class UI3DImage;

class RecognitionRange;
class MyRectangle;

enum SkillEffect_STATE
{
	FLOW,
	IDLE
};

class SkillEffect :
	public GameObject
{
public:
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, bool isGBuffers) override;
	 
protected: 

private: 

protected:
	virtual void ReleaseMembers() override {};
	virtual void ReleaseMemberUploadBuffers() override {};
	 
public: 
	SkillEffect(const std::string& entityID);
	virtual ~SkillEffect();

	void Update(float);
	void Move(ULONG nDirection, float fDistance, bool bVelocity = false);
	void Move(const XMFLOAT3& xmf3Shift);
	void MoveVelocity(const XMFLOAT3& xmf3Shift);
	void Move(float fxOffset = 0.0f, float fyOffset = 0.0f, float fzOffset = 0.0f);
	void Rotate(float x, float y, float z);
};