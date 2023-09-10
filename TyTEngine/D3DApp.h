#pragma once
#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN     // 거의 사용되지 않는 windows.h를 제거함.
#include<windows.h>

#define MAX_LOADSTRING 100

class D3DApp
{
public:
	D3DApp(HINSTANCE HInstance, int CmdShow);
	~D3DApp();
	virtual bool Init();
	virtual LRESULT WndProc(HWND HWnd, UINT Msg, WPARAM WParam, LPARAM LParam);
	virtual bool Run();

protected:
	bool InitWindow();
	virtual ATOM AppRegisterClass();
	virtual BOOL InitInstance();

	HINSTANCE mHInst;
	int mCmdShow;
	WCHAR mAppTitle[MAX_LOADSTRING];
	WCHAR mWindowClass[MAX_LOADSTRING];
};