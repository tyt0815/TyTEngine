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

#include "DirectXUtil.h"
#include "MathHelper.h"
#include "CommonUtility.h"
#include "GeometryGenerator.h"

using namespace std;