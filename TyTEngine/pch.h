#pragma once

#define WIN32_LEAN_AND_MEAN     // 거의 사용되지 않는 windows.h를 제거함.
#include <windows.h>
#include <cassert>
#include <strsafe.h>
#include <memory>
#include <vector>
#include <d3d11.h>
#include <d3dcommon.h>
#include <d3dcompiler.h>
#include <dxgi.h>
#include <DirectXMath.h>

#include <DirectXTex/DirectXTex.h>

#include "DirectXUtil.h"
#include "MathHelper.h"
#include "CommonUtility.h"
#include "GeometryGenerator.h"
#include "LightHelper.h"

using namespace std;

struct Vertex
{
    XMFLOAT3 Pos;
    XMFLOAT3 Normal;
    XMFLOAT2 UV;
};

struct VSPerObjectConstantBuffer
{
    XMMATRIX World;
    XMMATRIX WorldInverseTranspose;
    XMMATRIX WorldViewProj;
};

struct PSPerFrameConstantBuffer
{
    DirectionalLight DirLit;
    PointLight PointLit;
    SpotLight SpotLit;
    XMFLOAT3 EyePosW;
};

struct PSPerObjectConstantBuffer
{
    Material Mat;
};