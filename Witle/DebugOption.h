#pragma once


/*
   Debug Mesh 가 보이는지 유무를 결정하는 static 변수가 존재하는 클래스
*/
class DebugOption
{
public:
	static bool GetisShowBoundingBox() { return isShowBoundingBox; };
	static bool GetisShowSphere() { return isShowSphere; };

	static void ChangeisShowBoundingBox() { isShowBoundingBox = !isShowBoundingBox; }
	static void ChangeisShowSphere() { isShowSphere = !isShowSphere; }
	 
private:
	DebugOption() = delete;
	~DebugOption() = delete;

	static bool isShowBoundingBox;
	static bool isShowSphere;
}; 
