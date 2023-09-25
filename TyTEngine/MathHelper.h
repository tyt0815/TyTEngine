#pragma once

class UMathHelper
{
public:
	UMathHelper();
	~UMathHelper();

	static const float Infinity;
	static const float Pi;
	template<typename T>
	static T Clamp(const T& X, const T& Low, const T& High);
	static UINT Min(UINT A, UINT B);
	static float AngleFromXY(float A, float B);
};

template<typename T>
inline T UMathHelper::Clamp(const T& X, const T& Low, const T& High)
{
	return X < Low ? Low : (X > High ? High : X);
}
