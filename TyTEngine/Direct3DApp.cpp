#include "pch.h"
#include "Direct3DApp.h"
#include <sstream>
#include <vector>


namespace
{
	CDirect3DApp* gD3DApp = nullptr;
}

LRESULT CALLBACK MainWndProc(HWND HWnd, UINT Msg, WPARAM WParam, LPARAM LParam)
{
	return gD3DApp->WndProc(HWnd, Msg, WParam, LParam);
}

CDirect3DApp::CDirect3DApp(HINSTANCE HInstance, int CmdShow) :
	mAppTitle(L"TyTEngine"),
	mWindowClass(L"TyT"),
	mhInst(HInstance),
	mCmdShow(CmdShow),
	mD3DDriverType(D3D_DRIVER_TYPE_HARDWARE),
	mClientWidth(400),
	mClientHeight(400),
	mSampleCount(4),
	mEnable4xMsaa(false),
	mhMainWnd(0),
	mD3DDevice(0),
	mD3DImmediateContext(0),
	m4xMsaaQuality(0),
	mSwapChain(0),
	mRenderTargetView(0),
	mDepthStencilView(0),
	mDepthStencilBuffer(0),
	mScreenViewport(new D3D11_VIEWPORT),
	mAppPaused(false),
	mMinimized(false),
	mMaximized(false),
	mResizing(false),
	mTimer(new CGameTimer)
{
	gD3DApp = this;
}

CDirect3DApp::~CDirect3DApp()
{
	ReleaseCOM(mRenderTargetView);
	ReleaseCOM(mDepthStencilView);
	ReleaseCOM(mSwapChain);
	ReleaseCOM(mDepthStencilBuffer);
	if (mD3DImmediateContext)
	{
		mD3DImmediateContext->ClearState();
	}
	ReleaseCOM(mD3DImmediateContext);
	ReleaseCOM(mD3DDevice);
	delete mScreenViewport;
	delete mTimer;
}

bool CDirect3DApp::Init()
{
	if (!InitMainWindow())
	{
		return false;
	}

	if (!InitDirect3D())
	{
		return false;
	}

	return true;
}

int CDirect3DApp::Run()
{
	MSG msg = { 0 };

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Process();
		}
	}

	return (int)msg.wParam;
}

LRESULT CDirect3DApp::WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg)
	{
		// 윈도우가 활성화 혹은 비활성화 될때
	case WM_ACTIVATE:
	{
		if (LOWORD(wParam) == WA_INACTIVE)
		{
			mAppPaused = true;
			mTimer->Stop();
		}
		else
		{
			mAppPaused = false;
			mTimer->Start();
		}
		return 0;
	}

	// 윈도우의 사이즈가 변경되었을때
	case WM_SIZE:
	{
		mClientWidth = LOWORD(lParam);
		mClientWidth = HIWORD(lParam);
		if (mD3DDevice)
		{
			if (wParam == SIZE_MINIMIZED)
			{
				mAppPaused = true;
				mMinimized = true;
				mMaximized = false;
			}
			else if (wParam == SIZE_MAXIMIZED)
			{
				mAppPaused = false;
				mMinimized = false;
				mMaximized = true;
				OnResize();
			}
			else if (wParam == SIZE_RESTORED)
			{
				if (mMinimized)
				{
					mAppPaused = false;
					mMinimized = false;
					OnResize();
				}
				else if (mMaximized)
				{
					mAppPaused = false;
					mMaximized = false;
					OnResize();
				}
				else if (mResizing)
				{
					// 유저가 임의로 Resizing을 할때, 아무 동작도 하지 않게 한다.
				}
				else // API가 SetWindowPos나 mSwapChain->SetFullscreenState 같은 메소드를 호출할때
				{
					OnResize();
				}
			}
		}
		return 0;
	}
	// 윈도우의 resize바(가장자리)를 끌기 시작할때
	case WM_ENTERSIZEMOVE:
	{
		mAppPaused = true;
		mResizing = true;
		mTimer->Stop();
		return 0;
	}
	// 윈도우의 resize바(가장자리)를 끌기를 끝낼때
	case WM_EXITSIZEMOVE:
	{
		mAppPaused = false;
		mResizing = true;
		mTimer->Start();
		OnResize();
		return 0;
	}
	// 윈도우 창 닫기
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	// 메뉴 바가 활성화 될때
	case WM_MENUCHAR:
	{
		// ALT+Enter로 전체화면을 키고 끌때, 소리가 안나게 해줌.
		return MAKELRESULT(0, MNC_CLOSE);
	}
	// 윈도우 최대/최소 크기 설정
	case WM_GETMINMAXINFO:
	{
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = 200;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = 200;
		return 0;
	}

	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
	{
		OnMouseDown(wParam, LOWORD(lParam), HIWORD(lParam));
		return 0;
	}
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
	{
		OnMouseUp(wParam, LOWORD(lParam), HIWORD(lParam));
		return 0;
	}
	case WM_MOUSEMOVE:
	{
		OnMouseMove(wParam, LOWORD(lParam), HIWORD(lParam));
		return 0;
	}
	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case 'W':
		{
			int a = 0;
			break;
		}
		default:
			break;
		}
		return 0;
	}

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
		//Rectangle(hdc, 10, 10, 110, 110);
		EndPaint(hWnd, &ps);
		return 0;
	}


	default:
		return DefWindowProc(hWnd, Msg, wParam, lParam);
	}
	return 0;
}

bool CDirect3DApp::InitMainWindow()
{
	AppRegisterClass();

	// 윈도우 생성.
	// 애플리케이션 초기화를 수행합니다:
	if (!InitInstance())
	{
		return FALSE;
	}

	return true;

}

bool CDirect3DApp::InitDirect3D()
{
	UINT CreateDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	CreateDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// 4.2.1 Create the Device and Context
	D3D_FEATURE_LEVEL FeatureLevel;
	HRESULT hr = D3D11CreateDevice(
		nullptr,                // 디스플레이를 선택. null시 주 모니터로 선택.
		mD3DDriverType,         // D3D_DRIVER_TYPE_HARDWARE를 일반적으로 사용. 다른 옵션은 문서 또는 교재 참고
		0,                      // 소프트 웨어 디바이스 사용 여부
		CreateDeviceFlags,      // 옵셔널 플래그
		nullptr,                // 피쳐 레벨 배열.
		0,                      // 배열 원소의 수
		D3D11_SDK_VERSION,      // SDK 버전 명시
		&mD3DDevice,            // 반환값: Device
		&FeatureLevel,          // 반환값: Feature Level
		&mD3DImmediateContext   // 반환값: Context. Deferred context는 따로 존재함.
	);

	if (FAILED(hr))
	{
		MessageBox(0, L"D3D11CreateDevice Failed.", 0, 0);
		return false;
	}
	if (FeatureLevel != D3D_FEATURE_LEVEL_11_0)
	{
		MessageBox(0, L"Dirext3D Feature Level 11 unsupported.", 0, 0);
		return false;
	}

	// 4.2.2 Check 4XMSAA Quality Support
	HR(mD3DDevice->CheckMultisampleQualityLevels(
		DXGI_FORMAT_R8G8B8A8_UNORM, mSampleCount, &m4xMsaaQuality));   // 차례대로 이미지 및 텍스처 포맷, 샘플의 수, 반환값
	assert(m4xMsaaQuality > 0);         // 항상 0보다 커야함.

	// 4.2.3 Describe the Swap Chain
	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = mClientWidth;
	sd.BufferDesc.Height = mClientHeight;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	if (mEnable4xMsaa)
	{
		sd.SampleDesc.Count = mSampleCount;
		sd.SampleDesc.Quality = m4xMsaaQuality - 1;
	}
	else
	{
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
	}

	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = mhMainWnd;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	// 4.2.4 Create the Swap Chain
	IDXGIDevice* dxgiDevice = 0;
	HR(mD3DDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice));
	IDXGIAdapter* dxgiAdapter = 0;
	HR(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter));
	IDXGIFactory* dxgiFactory = 0;
	HR(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory));
	HR(dxgiFactory->CreateSwapChain(mD3DDevice, &sd, &mSwapChain));
	HR(dxgiFactory->MakeWindowAssociation(mhMainWnd, DXGI_MWA_NO_WINDOW_CHANGES));

	// 4.7 Exercises
//	UINT NumAdapters = 0;
//	UINT NumSupportedAdapters = 0;
//	UINT NumOutputs = 0;
//	UINT NumModes = 0;
//	IDXGIAdapter* pAdapter = { 0 };
//	IDXGIOutput* Output = { 0 };
//	DXGI_MODE_DESC* ModeDesc = { 0 };
//	std::vector<IDXGIAdapter*> UsableAdapters;
//	while (dxgiFactory->EnumAdapters(NumSupportedAdapters, &pAdapter) != DXGI_ERROR_NOT_FOUND)
//	{
//		++NumAdapters;
//		LARGE_INTEGER umdVersion;
//		DXGI_ADAPTER_DESC NewAdapter, DefaultAdapter;
//		pAdapter->GetDesc(&NewAdapter);
//		dxgiAdapter->GetDesc(&DefaultAdapter);
//		// NewAdapter.Description, DefaultAdapter.Description 값을 확인해 보면 어떤 Adapter인지 비교가 가능. 실제로 pAdapter와 dxgiAdapter는 값이 다르지만(주소가 다르지만) 같은 장치를 가리키고 있음.
//		// Adpater(GPU)가 디바이스 인터페이스(DirectX11)을 지원하는지 확인.
//		if ((hr = pAdapter->CheckInterfaceSupport(__uuidof(dxgiDevice), &umdVersion)) != DXGI_ERROR_UNSUPPORTED)	// S_OK 혹은 DXGI_ERROR_UNSUPPORTED를 반환
//		{
//			++NumSupportedAdapters;
//		}
//		ReleaseCOM(pAdapter);
//	}
//	while (dxgiAdapter->EnumOutputs(NumOutputs, &Output) != DXGI_ERROR_NOT_FOUND)
//	{
//		++NumOutputs;
//		ReleaseCOM(Output);
//	}
//#if defined(DEBUG) | defined(_DEBUG)
//	_PrintDebugString(L"*** NUM ADAPTERS = %d\n", NumSupportedAdapters);
//	_PrintDebugString(L"*** D3D11 SUPPORTED FOR ADAPTER %d\n", NumSupportedAdapters);
//	_PrintDebugString(L"*** NUM OUTPUTS FOR DEFAULT ADAPTER %d\n", NumOutputs);
//	NumOutputs = 0;
//	while (dxgiAdapter->EnumOutputs(NumOutputs, &Output) != DXGI_ERROR_NOT_FOUND)
//	{
//		++NumOutputs;
//		NumModes = 0;
//		ModeDesc = { 0 };
//		Output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, 0, &NumModes, ModeDesc);
//		ModeDesc = new DXGI_MODE_DESC[NumModes];
//		Output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, 0, &NumModes, ModeDesc);
//		for (int i = 0; i < NumModes; ++i)
//		{
//			_PrintDebugString(L"***WIDTH = %d HEIGHT = %d REFRESH = %d/%d\n", ModeDesc[i].Width, ModeDesc[i].Height, ModeDesc[i].RefreshRate.Numerator, ModeDesc[i].RefreshRate.Denominator);
//		}
//		ReleaseCOM(Output);
//		delete[] ModeDesc;
//	}
//#endif
	// 4.7 Exercises end


	ReleaseCOM(dxgiDevice);
	ReleaseCOM(dxgiAdapter);
	ReleaseCOM(dxgiFactory);

	OnResize();

	return true;
}

void CDirect3DApp::OnResize()
{
	assert(mD3DImmediateContext);
	assert(mD3DDevice);
	assert(mSwapChain);

	// Release the old views
	ReleaseCOM(mRenderTargetView);
	ReleaseCOM(mDepthStencilView);
	ReleaseCOM(mDepthStencilBuffer);

	// Create the Render Target View
	HR(mSwapChain->ResizeBuffers(1, mClientWidth, mClientHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0));
	ID3D11Texture2D* backBuffer;
	HR(mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)));
	HR(mD3DDevice->CreateRenderTargetView(backBuffer, 0, &mRenderTargetView));
	ReleaseCOM(backBuffer);

	// Create the Depth/Stencil Buffer and View
	D3D11_TEXTURE2D_DESC DepthStencilDesc;
	DepthStencilDesc.Width = mClientWidth;
	DepthStencilDesc.Height = mClientHeight;
	DepthStencilDesc.MipLevels = 1;                             // Depth/Stencil buffer에선 1사용
	DepthStencilDesc.ArraySize = 1;                             // Depth/Stencil buffer에선 1사용
	DepthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// must match swap chain MSAA Values
	if (mEnable4xMsaa)
	{
		DepthStencilDesc.SampleDesc.Count = mSampleCount;
		DepthStencilDesc.SampleDesc.Quality = m4xMsaaQuality - 1;
	}
	else
	{
		DepthStencilDesc.SampleDesc.Count = 1;
		DepthStencilDesc.SampleDesc.Quality = 0;
	}

	DepthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	DepthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	DepthStencilDesc.CPUAccessFlags = 0;
	DepthStencilDesc.MiscFlags = 0;

	HR(mD3DDevice->CreateTexture2D(&DepthStencilDesc, 0, &mDepthStencilBuffer));
	HR(mD3DDevice->CreateDepthStencilView(mDepthStencilBuffer, 0, &mDepthStencilView));

	// Bind the Views to the Output Merger Stage
	mD3DImmediateContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);

	// Set the Viewport
	mScreenViewport->TopLeftX = 0;
	mScreenViewport->TopLeftY = 0;
	mScreenViewport->Width = static_cast<float>(mClientWidth);
	mScreenViewport->Height = static_cast<float>(mClientHeight);
	/*mScreenViewport->TopLeftX = 100;
	mScreenViewport->TopLeftY = 100;
	mScreenViewport->Width = 100.f;
	mScreenViewport->Height = 100.f;*/
	mScreenViewport->MinDepth = 0.f;
	mScreenViewport->MaxDepth = 1.f;

	mD3DImmediateContext->RSSetViewports(1, mScreenViewport);
}

void CDirect3DApp::CalculateFrameStats()
{
	// FPS 계산
	static int FrameCnt = 0;
	static float TimeElapsed = 0.f;
	++FrameCnt;
	if ((mTimer->TotalTime() - TimeElapsed) >= 1.f)
	{
		float FPS = (float)FrameCnt;
		float MSPF = 1000.f / FPS;

		std::wostringstream Outs;
		Outs.precision(6);
		Outs << mAppTitle << L"  "
			<< L"FPS: " << FPS << L"  "
			<< L"Frame Time: " << MSPF << L" (ms)";
		SetWindowText(mhMainWnd, Outs.str().c_str());

		FrameCnt = 0;
		TimeElapsed += 1.f;
	}
}

ATOM CDirect3DApp::AppRegisterClass()
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = MainWndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = mhInst;
	wcex.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = mWindowClass;
	wcex.hIconSm = 0;

	return RegisterClassExW(&wcex); // 윈도우가 제공하는 함수.
}

BOOL CDirect3DApp::InitInstance()
{
	RECT R = { 0, 0, mClientWidth, mClientHeight };
	AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
	int Width = R.right - R.left;
	int Height = R.bottom - R.top;

	mhMainWnd = CreateWindowW(mWindowClass, mAppTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, Width, Height, nullptr, nullptr, mhInst, nullptr);

	if (!mhMainWnd)
	{
		return FALSE;
	}

	ShowWindow(mhMainWnd, mCmdShow);
	UpdateWindow(mhMainWnd);

	return TRUE;
}

void CDirect3DApp::Process()
{
	mTimer->Tick();

	if (!mAppPaused)
	{
		CalculateFrameStats();
		UpdateScene(mTimer->DeltaTime());
		DrawScene();
	}
	else
	{
		Sleep(100);
	}
}
