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
		// �����찡 Ȱ��ȭ Ȥ�� ��Ȱ��ȭ �ɶ�
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

	// �������� ����� ����Ǿ�����
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
					// ������ ���Ƿ� Resizing�� �Ҷ�, �ƹ� ���۵� ���� �ʰ� �Ѵ�.
				}
				else // API�� SetWindowPos�� mSwapChain->SetFullscreenState ���� �޼ҵ带 ȣ���Ҷ�
				{
					OnResize();
				}
			}
		}
		return 0;
	}
	// �������� resize��(�����ڸ�)�� ���� �����Ҷ�
	case WM_ENTERSIZEMOVE:
	{
		mAppPaused = true;
		mResizing = true;
		mTimer->Stop();
		return 0;
	}
	// �������� resize��(�����ڸ�)�� ���⸦ ������
	case WM_EXITSIZEMOVE:
	{
		mAppPaused = false;
		mResizing = true;
		mTimer->Start();
		OnResize();
		return 0;
	}
	// ������ â �ݱ�
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	// �޴� �ٰ� Ȱ��ȭ �ɶ�
	case WM_MENUCHAR:
	{
		// ALT+Enter�� ��üȭ���� Ű�� ����, �Ҹ��� �ȳ��� ����.
		return MAKELRESULT(0, MNC_CLOSE);
	}
	// ������ �ִ�/�ּ� ũ�� ����
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
		// TODO: ���⿡ hdc�� ����ϴ� �׸��� �ڵ带 �߰��մϴ�...
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

	// ������ ����.
	// ���ø����̼� �ʱ�ȭ�� �����մϴ�:
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
		nullptr,                // ���÷��̸� ����. null�� �� ����ͷ� ����.
		mD3DDriverType,         // D3D_DRIVER_TYPE_HARDWARE�� �Ϲ������� ���. �ٸ� �ɼ��� ���� �Ǵ� ���� ����
		0,                      // ����Ʈ ���� ����̽� ��� ����
		CreateDeviceFlags,      // �ɼų� �÷���
		nullptr,                // ���� ���� �迭.
		0,                      // �迭 ������ ��
		D3D11_SDK_VERSION,      // SDK ���� ���
		&mD3DDevice,            // ��ȯ��: Device
		&FeatureLevel,          // ��ȯ��: Feature Level
		&mD3DImmediateContext   // ��ȯ��: Context. Deferred context�� ���� ������.
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
		DXGI_FORMAT_R8G8B8A8_UNORM, mSampleCount, &m4xMsaaQuality));   // ���ʴ�� �̹��� �� �ؽ�ó ����, ������ ��, ��ȯ��
	assert(m4xMsaaQuality > 0);         // �׻� 0���� Ŀ����.

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
//		// NewAdapter.Description, DefaultAdapter.Description ���� Ȯ���� ���� � Adapter���� �񱳰� ����. ������ pAdapter�� dxgiAdapter�� ���� �ٸ�����(�ּҰ� �ٸ�����) ���� ��ġ�� ����Ű�� ����.
//		// Adpater(GPU)�� ����̽� �������̽�(DirectX11)�� �����ϴ��� Ȯ��.
//		if ((hr = pAdapter->CheckInterfaceSupport(__uuidof(dxgiDevice), &umdVersion)) != DXGI_ERROR_UNSUPPORTED)	// S_OK Ȥ�� DXGI_ERROR_UNSUPPORTED�� ��ȯ
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
	DepthStencilDesc.MipLevels = 1;                             // Depth/Stencil buffer���� 1���
	DepthStencilDesc.ArraySize = 1;                             // Depth/Stencil buffer���� 1���
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
	// FPS ���
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

	return RegisterClassExW(&wcex); // �����찡 �����ϴ� �Լ�.
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
