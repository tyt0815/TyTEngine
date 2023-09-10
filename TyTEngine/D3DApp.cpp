#include "D3DApp.h"

namespace 
{
    D3DApp* gD3DApp = nullptr;
}

LRESULT CALLBACK MainWndProc(HWND HWnd, UINT Msg, WPARAM WParam, LPARAM LParam)
{
    return gD3DApp->WndProc(HWnd, Msg, WParam, LParam);
}

D3DApp::D3DApp(HINSTANCE HInstance, int CmdShow):
    mAppTitle(L"TyTEngine"),
    mWindowClass(L"TyT"),
    mHInst(HInstance),
    mCmdShow(CmdShow)
{
    gD3DApp = this;
}

D3DApp::~D3DApp()
{
}

bool D3DApp::Init()
{
    if (!InitWindow())
    {
        return false;
    }

    

    return true;
}

bool D3DApp::InitWindow()
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

LRESULT D3DApp::WndProc(HWND HWnd, UINT Msg, WPARAM WParam, LPARAM LParam)
{
    switch (Msg)
    {
    case WM_COMMAND:
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(HWnd, &ps);
            // TODO: ���⿡ hdc�� ����ϴ� �׸��� �ڵ带 �߰��մϴ�...
            Rectangle(hdc, 10, 10, 110, 110);
            EndPaint(HWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(HWnd, Msg, WParam, LParam);
    }
    return 0;
}

bool D3DApp::Run()
{
    // ����Ű ���̺� ���� �ε�
    HACCEL hAccelTable = nullptr;

    MSG msg;

    // �⺻ �޽��� �����Դϴ�:
    /*
    * GetMessage
    * �޼����� ������ ���� ���. �� �޼����� ������ while���� ���� ����.
    * msg.message == WM_QUIT�� ��� false�� ��ȯ��. -> while�� ��������.
    */
    while (GetMessage(&msg, nullptr, 0, 0))     // msg�� ���� ����
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))     // ����Ű ���� Ȯ��. �ƴҰ�� if�� ����
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}

ATOM D3DApp::AppRegisterClass()
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = MainWndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = mHInst;
    wcex.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = mWindowClass;
    wcex.hIconSm = 0;

    return RegisterClassExW(&wcex); // �����찡 �����ϴ� �Լ�.
}



BOOL D3DApp::InitInstance()
{
    HWND hWnd = CreateWindowW(mWindowClass, mAppTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, mHInst, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, mCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}
