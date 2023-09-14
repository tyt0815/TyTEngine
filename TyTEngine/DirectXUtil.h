#pragma once

using namespace DirectX;

#include "GameTimer.h"

#if defined(DEBUG) || defined(_DEBUG)
    #ifndef HR
        #define HR(x)                                            \
            {                                                        \
                HRESULT hr = (x);                                    \
                if(FAILED(hr))                                       \
                {                                                    \
                    LPWSTR output;                                   \
                    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |       \
                        FORMAT_MESSAGE_IGNORE_INSERTS      |         \
                        FORMAT_MESSAGE_ALLOCATE_BUFFER,              \
                        NULL,                                        \
                        hr,                                          \
                        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),   \
                        (LPTSTR) &output,                            \
                        0,                                           \
                        NULL);                                       \
                    MessageBox(NULL, output, L"Error", MB_OK);       \
                }                                                    \
            }
    #endif
#else
    #ifndef HR
        #define HR(x) (x)
    #endif
#endif

#define ReleaseCOM(x) { if(x){x->Release(); x= 0; }}

#define RGBA(r,g,b,a) const FLOAT{r,g,b,a}
namespace Colors
{
    XMGLOBALCONST XMVECTORF32 White = { 1.0f, 1.0f, 1.0f, 1.0f };
    XMGLOBALCONST XMVECTORF32 Black = { 0.0f, 0.0f, 0.0f, 1.0f };
    XMGLOBALCONST XMVECTORF32 Red = { 1.0f, 0.0f, 0.0f, 1.0f };
    XMGLOBALCONST XMVECTORF32 Green = { 0.0f, 1.0f, 0.0f, 1.0f };
    XMGLOBALCONST XMVECTORF32 Blue = { 0.0f, 0.0f, 1.0f, 1.0f };
    XMGLOBALCONST XMVECTORF32 Yellow = { 1.0f, 1.0f, 0.0f, 1.0f };
    XMGLOBALCONST XMVECTORF32 Cyan = { 0.0f, 1.0f, 1.0f, 1.0f };
    XMGLOBALCONST XMVECTORF32 Magenta = { 1.0f, 0.0f, 1.0f, 1.0f };

    XMGLOBALCONST XMVECTORF32 Silver = { 0.75f, 0.75f, 0.75f, 1.0f };
    XMGLOBALCONST XMVECTORF32 LightSteelBlue = { 0.69f, 0.77f, 0.87f, 1.0f };
}

#if defined(DEBUG) | defined(_DEBUG)
static void _PrintDebugString(const WCHAR* format, ...) {
    WCHAR buf[1024];
    va_list vaList;
    va_start(vaList, format);
    _vsnwprintf_s(buf, sizeof(buf), format, vaList);
    va_end(vaList);
    OutputDebugStringW(buf);
}
static void _PrintDebugString(const char* format, ...) {
    char buf[1024];
    va_list vaList;
    va_start(vaList, format);
    _vsnprintf_s(buf, sizeof(buf), format, vaList);
    va_end(vaList);
    OutputDebugStringA(buf);
}
#endif
