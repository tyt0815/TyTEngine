#include "pch.h"
#include "MathHelper.h"

const float UMathHelper::Infinity = FLT_MAX;
const float UMathHelper::Pi = 3.1415926535f;

UMathHelper::UMathHelper()
{
}	

UMathHelper::~UMathHelper()
{
}

UINT UMathHelper::Min(UINT A, UINT B)
{
    return A > B ? B : A;
}

float UMathHelper::AngleFromXY(float A, float B)
{
	float theta = 0.0f;

	// Quadrant I or IV
	if (A >= 0.0f)
	{
		// If x = 0, then atanf(y/x) = +pi/2 if y > 0
		//                atanf(y/x) = -pi/2 if y < 0
		theta = atanf(B / A); // in [-pi/2, +pi/2]

		if (theta < 0.0f)
			theta += 2.0f * Pi; // in [0, 2*pi).
	}

	// Quadrant II or III
	else
		theta = atanf(B / A) + Pi; // in [0, 2*pi).

	return theta;
}

XMMATRIX UMathHelper::InverseTranspose(CXMMATRIX Matrix)
{
	XMMATRIX Result = Matrix;
	Result.r[3] = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	XMVECTOR Det = XMMatrixDeterminant(Result);
	return XMMatrixTranspose(XMMatrixInverse(&Det, Result));
}
