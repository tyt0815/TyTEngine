#include "pch.h"
#include "Core.h"

CCore::CCore(HINSTANCE hInstance, int CmdShow):
	CDirect3DApp(hInstance, CmdShow)
{
}

CCore::~CCore()
{
}

bool CCore::Init()
{
	if (!CDirect3DApp::Init())
	{
		return false;
	}
	return true;
}

void CCore::OnResize()
{
	CDirect3DApp::OnResize();
}

void CCore::UpdateScene(float DeltaTime)
{
}

void CCore::DrawScene()
{
	assert(mD3DImmediateContext);
	assert(mSwapChain);

	mD3DImmediateContext->ClearRenderTargetView(mRenderTargetView, reinterpret_cast<const float*>(&Colors::Blue));
	mD3DImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D10_CLEAR_STENCIL, 1.0f, 0);

	HR(mSwapChain->Present(0, 0));
}
