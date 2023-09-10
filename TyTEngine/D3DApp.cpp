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

    // 윈도우 생성.
    // 애플리케이션 초기화를 수행합니다:
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
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
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
    // 단축키 테이블 정보 로딩
    HACCEL hAccelTable = nullptr;

    MSG msg;

    // 기본 메시지 루프입니다:
    /*
    * GetMessage
    * 메세지를 받을때 까지 대기. 즉 메세지가 없으면 while문이 돌지 않음.
    * msg.message == WM_QUIT일 경우 false를 반환함. -> while문 빠져나옴.
    */
    while (GetMessage(&msg, nullptr, 0, 0))     // msg에 값을 복사
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))     // 단축키 인지 확인. 아닐경우 if문 진입
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

    return RegisterClassExW(&wcex); // 윈도우가 제공하는 함수.
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
