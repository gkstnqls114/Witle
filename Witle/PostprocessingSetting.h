#pragma once

class PostprocessingSetting
{
private:
	static const bool DefferedRendering ;
	static bool isBloom ;
	static bool isToneCurve;

private:
	PostprocessingSetting() {};
	~PostprocessingSetting() {};

public:
	static void ActiveBloom();
	static void DeactiveBloom();

	static void ActiveTonecurve();
	static void DeactiveTonecurve();

	static bool IsBloom() { return isBloom; }
	static bool IsToneCurve() { return isToneCurve; }
	static bool IsDefferedRendering() { return DefferedRendering; }
};