#include "D3DApp.h"


/*
_in_, _in_opt_는 SAL. 자세한 내용은 
https://learn.microsoft.com/ko-kr/previous-versions/visualstudio/visual-studio-2015/code-quality/understanding-sal?view=vs-2015&redirectedfrom=MSDN
*/
int WINAPI WinMain(	_In_ HINSTANCE HInstance, 
					_In_opt_ HINSTANCE HPrevInstance,
					_In_ PSTR CmdLine,
					_In_ int CmdShow)
{
	D3DApp App(HInstance, CmdShow);

	if (!App.Init())
	{
		return 1;
	}

	return App.Run();
}