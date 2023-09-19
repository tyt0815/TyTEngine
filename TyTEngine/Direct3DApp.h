#pragma once

#include <SDKDDKVer.h>
#include <Windows.h>
#include <d3dcommon.h>

#define MAX_LOADSTRING 100

struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;
struct ID3D11Texture2D;
struct D3D11_VIEWPORT;

class CGameTimer;

class CDirect3DApp
{
public:
	CDirect3DApp(HINSTANCE HInstance, int CmdShow);
	~CDirect3DApp();
	virtual bool Init();
	int Run();

	virtual LRESULT WndProc(HWND HWnd, UINT Msg, WPARAM WParam, LPARAM LParam);

protected:
	bool InitMainWindow();
	bool InitDirect3D();
	void OnResize();
	void CalculateFrameStats();
	float GetAspectRatio();
	virtual ATOM AppRegisterClass();
	virtual BOOL InitInstance();
	virtual void Process();
	virtual void UpdateScene(float DeltaTime) = 0; // = 0 은 순수 가상함수. 자식 클래스에서 구현
	virtual void DrawScene() = 0;			// 자식 클래스에서 구현
	virtual void OnMouseDown(WPARAM btnState, int x, int y) { }	// 자식 클래스에서 구현
	virtual void OnMouseUp(WPARAM btnState, int x, int y) { }	// 자식 클래스에서 구현
	virtual void OnMouseMove(WPARAM btnState, int x, int y) { }	// 자식 클래스에서 구현

	CGameTimer *mTimer;

	/*
	* Win32
	*/
	HINSTANCE mhInst;
	int mCmdShow;
	HWND mhMainWnd;
	WCHAR mAppTitle[MAX_LOADSTRING];
	WCHAR mWindowClass[MAX_LOADSTRING];
	int mClientWidth;
	int mClientHeight;
	bool mResizing;
	bool mMinimized;
	bool mMaximized;
	bool mAppPaused;

	/*
	* DirectX 11
	*/
	ID3D11Device* mD3DDevice;
	ID3D11DeviceContext* mD3DImmediateContext;
	D3D_DRIVER_TYPE mD3DDriverType;
	UINT mSampleCount;
	UINT m4xMsaaQuality;
	bool mEnable4xMsaa;
	IDXGISwapChain* mSwapChain;
	ID3D11RenderTargetView* mRenderTargetView;
	ID3D11DepthStencilView* mDepthStencilView;
	ID3D11Texture2D* mDepthStencilBuffer;
	D3D11_VIEWPORT* mScreenViewport;
};
