#include "pch.h"
#include "Core.h"
#include "ObjectManager.h"
#include "Object.h"

CCore::CCore():
	CDirect3DApp(),
	mLastMousePos({0, 0}),
	mWVPBuffer(0),
	mVSBlob(0),
	mPSBlob(0),
	mVertexShader(0),
	mPixelShader(0),
	mInputLayout(0),
	mRadius(5.f),
	mTheta(1.5f * UMathHelper::PI),
	mPhi(0.25f * UMathHelper::PI)
{
	XMMATRIX I = XMMatrixIdentity();
	XMStoreFloat4x4(&mViewMat, I);
	XMStoreFloat4x4(&mProjMat, I);
}

CCore::~CCore()
{
	ReleaseCOM(mInputLayout);
	ReleaseCOM(mVSBlob);
	ReleaseCOM(mPSBlob);
	ReleaseCOM(mVertexShader);
	ReleaseCOM(mPixelShader);
	ReleaseCOM(mWVPBuffer);
}

bool CCore::Init(HINSTANCE hInstance, int CmdShow)
{
	if (!CDirect3DApp::Init(hInstance, CmdShow))
	{
		return false;
	}
	BuildShader();
	BuildVertexLayout();
	BuildMat();
	return true;
}

void CCore::OnResize()
{
	CDirect3DApp::OnResize();
	// 창이 리사이즈되면 projection matrix를 재계산
	XMMATRIX ResizedProjMat = XMMatrixPerspectiveFovLH(
		0.5f * UMathHelper::PI,	// FOV = 90
		GetAspectRatio(),
		1.f,
		1000.f
	);
	XMStoreFloat4x4(&mProjMat, ResizedProjMat);
}

void CCore::UpdateScene(float DeltaTime)
{
	float x = mRadius * sinf(mPhi) * cosf(mTheta);
	float z = mRadius * sinf(mPhi) * sinf(mTheta);
	float y = mRadius * cosf(mPhi);

	XMVECTOR Pos = XMVectorSet(x, y, z, 1.f);
	XMVECTOR Target = XMVectorZero();
	XMVECTOR Up = XMVectorSet(0.f, 1.f, 0.f, 0.f);

	XMMATRIX ViewMat = XMMatrixLookAtLH(Pos, Target, Up);
	XMStoreFloat4x4(&mViewMat, ViewMat);
}

void CCore::DrawScene()
{
	assert(md3dDeviceContext);
	assert(mSwapChain);

	md3dDeviceContext->ClearRenderTargetView(mRenderTargetView, reinterpret_cast<const float*>(&Colors::White));
	md3dDeviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D10_CLEAR_STENCIL, 1.0f, 0);

	// Draw Objects
	md3dDeviceContext->IASetInputLayout(mInputLayout);
	md3dDeviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	UINT Stride = sizeof(Vertex);
	UINT Offset = 0;
	md3dDeviceContext->VSSetShader(mVertexShader, nullptr, 0);
	md3dDeviceContext->PSSetShader(mPixelShader, nullptr, 0);
	vector<unique_ptr<OObject>>* Objects = &(CObjectManager::GetInstance()->mObjects);
	vector<unique_ptr<OObject>>::iterator ObjectIter = (*Objects).begin();
	for (; ObjectIter != (*Objects).end(); ++ObjectIter)
	{
		D3D11_MAPPED_SUBRESOURCE mappedSubresource;
		md3dDeviceContext->Map(mWVPBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
		XMMATRIX WorldMat = (*ObjectIter)->GetWorldMatrix();
		XMMATRIX ViewMat = XMLoadFloat4x4(&mViewMat);
		XMMATRIX ProjMat = XMLoadFloat4x4(&mProjMat);
		XMMATRIX* WVPMat = (XMMATRIX*)mappedSubresource.pData;
		*WVPMat = XMMatrixTranspose(WorldMat * ViewMat * ProjMat);
		md3dDeviceContext->Unmap(mWVPBuffer, 0);
		md3dDeviceContext->VSSetConstantBuffers(0, 1, &mWVPBuffer);

		md3dDeviceContext->IASetVertexBuffers(0, 1, &((*ObjectIter)->mVertexBuffer), &Stride, &Offset);
		md3dDeviceContext->IASetIndexBuffer((*ObjectIter)->mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		md3dDeviceContext->DrawIndexed((*ObjectIter)->mNumIndex, 0, 0);
	}

	HR(mSwapChain->Present(0, 0));
}

void CCore::OnMouseDown(WPARAM BtnState, int x, int y)
{
	mLastMousePos.x = x;
	mLastMousePos.y = y;
	SetCapture(mhMainWnd);
}

void CCore::OnMouseUp(WPARAM BtnState, int x, int y)
{
	ReleaseCapture();
}

void CCore::OnMouseMove(WPARAM BtnState, int x, int y)
{
	if ((BtnState & MK_LBUTTON) != 0)
	{
		float dx = XMConvertToRadians(0.25f * static_cast<float>(x - mLastMousePos.x));
		float dy = XMConvertToRadians(0.25f * static_cast<float>(y - mLastMousePos.y));
		mTheta -= dx;
		mPhi -= dy;
		mPhi = UMathHelper::Clamp<float>(mPhi, 0.1f, UMathHelper::PI - 0.1f);
	}
	else if((BtnState & MK_RBUTTON) != 0)
	{
		float dx = 0.005f * static_cast<float>(x - mLastMousePos.x);
		float dy = 0.005f * static_cast<float>(y - mLastMousePos.y);
		mRadius += dx - dy;
		mRadius = UMathHelper::Clamp<float>(mRadius, 3.0f, 15.0f);
	}
	mLastMousePos.x = x;
	mLastMousePos.y = y;
}

HRESULT CCore::CreateD3D11Buffer(D3D11_BUFFER_DESC* BufferDesc, D3D11_SUBRESOURCE_DATA* InitData, ID3D11Buffer** Buffer)
{
	return md3dDevice->CreateBuffer(BufferDesc, InitData, Buffer);
}

void CCore::BuildMat()
{
	D3D11_BUFFER_DESC mbd = {};
	mbd.Usage = D3D11_USAGE_DYNAMIC;
	mbd.ByteWidth = sizeof(XMMATRIX) + 0xf & 0xfffffff0;
	mbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	mbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	HR(md3dDevice->CreateBuffer(&mbd, nullptr, &mWVPBuffer));
}

void CCore::BuildShader()
{
	HR(D3DCompileFromFile(L"VertexShader.hlsl", nullptr, nullptr, "VSMain", "vs_5_0", 0, 0, &mVSBlob, nullptr));
	HR(md3dDevice->CreateVertexShader(mVSBlob->GetBufferPointer(), mVSBlob->GetBufferSize(), nullptr, &mVertexShader));
	HR(D3DCompileFromFile(L"PixelShader.hlsl", nullptr, nullptr, "PSMain", "ps_5_0", 0, 0, &mPSBlob, nullptr));
	HR(md3dDevice->CreatePixelShader(mPSBlob->GetBufferPointer(), mPSBlob->GetBufferSize(), nullptr, &mPixelShader));
}

void CCore::BuildVertexLayout()
{
	D3D11_INPUT_ELEMENT_DESC VertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
		D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	HR(md3dDevice->CreateInputLayout(VertexDesc, ARRAYSIZE(VertexDesc), mVSBlob->GetBufferPointer(), mVSBlob->GetBufferSize(), &mInputLayout));
}
