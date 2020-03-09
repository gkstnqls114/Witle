#pragma once


/*
   Debug Mesh �� ���̴��� ������ �����ϴ� static ������ �����ϴ� Ŭ����
*/
class DebugOption
{
public:
	static bool GetisShowBoundingBox() { return isShowBoundingBox; };
	static bool GetisShowSphere() { return GetisShowSphere; };

	static void ChangeisShowBoundingBox() { isShowBoundingBox = !isShowBoundingBox; }
	static void ChangeisShowSphere() { isShowSphere = !isShowSphere; }
	 
private:
	DebugOption() = delete;
	~DebugOption() = delete;

	static bool isShowBoundingBox;
	static bool isShowSphere;
}; 
