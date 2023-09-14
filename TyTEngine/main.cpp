#include "D3DApp.h"
#include "InitDirect3DApp.h"
#include <crtdbg.h>
#include <iostream>

#pragma comment(lib, "d3d11.lib")

enum class EChapter
{
	ECInitDirect3D
};
EChapter Chapter = EChapter::ECInitDirect3D;

int InitDirect3D(HINSTANCE HInstance, int CmdShow);

int WINAPI WinMain(	_In_ HINSTANCE HInstance, 
					_In_opt_ HINSTANCE HPrevInstance,
					_In_ PSTR CmdLine,
					_In_ int CmdShow)
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	switch (Chapter)
	{
	case EChapter::ECInitDirect3D:
		InitDirect3D(HInstance, CmdShow);
		break;
	default:
		break;
	}
	return 0;
}

int InitDirect3D(HINSTANCE HInstance, int CmdShow)
{
	InitDirect3DApp TheApp(HInstance, CmdShow);

	if (!TheApp.Init())
		return 0;
	return  TheApp.Run();
}
