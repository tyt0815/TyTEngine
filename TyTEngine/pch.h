#pragma once

#define WIN32_LEAN_AND_MEAN     // ���� ������ �ʴ� windows.h�� ������.
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

struct SVertex
{
    XMFLOAT3 Position;
    XMFLOAT3 Normal;
    XMFLOAT2 UV;
};

struct SVSPerObjectConstantBuffer
{
    XMMATRIX World;
    XMMATRIX WorldInverseTranspose;
    XMMATRIX WorldViewProj;
    XMMATRIX TexTransform;
};

struct SPSPerFrameConstantBuffer
{
    SDirectionalLight DirLit;
    SPointLight PointLit;
    SSpotLight SpotLit;
    XMFLOAT3 EyePosW;
};

struct SPSPerObjectConstantBuffer
{
    SMaterial Mat;
};