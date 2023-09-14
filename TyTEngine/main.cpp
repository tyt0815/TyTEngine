#include "pch.h"
#include "Core.h"
#include <crtdbg.h>
#include <iostream>

#pragma comment(lib, "d3d11.lib")


int WINAPI WinMain(	_In_ HINSTANCE hInstance, 
					_In_opt_ HINSTANCE hPrevInstance,
					_In_ PSTR CmdLine,
					_In_ int CmdShow)
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	
	CCore TyTEngine(hInstance, CmdShow);

	if (!TyTEngine.Init())
	{
		OutputDebugString(L"엔진 초기화 실패");
		return 0;
	}

	return TyTEngine.Run();
}
