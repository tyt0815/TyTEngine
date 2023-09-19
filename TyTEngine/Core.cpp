#include "pch.h"
#include "Core.h"

struct Vertex
{
	XMFLOAT3 Pos;
	XMFLOAT4 Color;
};

CCore::CCore(HINSTANCE hInstance, int CmdShow):
	CDirect3DApp(hInstance, CmdShow),
	mBoxVB(0),
	mBoxIB(0),
	mNumBoxIndex(0),
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
	XMStoreFloat4x4(&mWorldMat, I);
	XMStoreFloat4x4(&mViewMat, I);
	XMStoreFloat4x4(&mProjMat, I);
}

CCore::~CCore()
{
	ReleaseCOM(mInputLayout);
	ReleaseCOM(mBoxVB);
	ReleaseCOM(mBoxIB);
	ReleaseCOM(mVSBlob);
	ReleaseCOM(mPSBlob);
	ReleaseCOM(mVertexShader);
	ReleaseCOM(mPixelShader);
}

bool CCore::Init()
{
	if (!CDirect3DApp::Init())
	{
		return false;
	}

	BuildGeometryBuffers();
	BuildShader();
	BuildVertexLayout();

	return true;
}

void CCore::OnResize()
{
	CDirect3DApp::OnResize();

	// 창이 리사이즈되면 projection matrix를 재계산
	XMMATRIX ResizedProjMat = XMMatrixPerspectiveFovLH(
		0.25f * UMathHelper::PI,	// FOV = 45
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
	assert(mD3DImmediateContext);
	assert(mSwapChain);

	mD3DImmediateContext->ClearRenderTargetView(mRenderTargetView, reinterpret_cast<const float*>(&Colors::Cyan));
	mD3DImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D10_CLEAR_STENCIL, 1.0f, 0);

	mD3DImmediateContext->IASetInputLayout(mInputLayout);
	mD3DImmediateContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	UINT Stride = sizeof(Vertex);
	UINT Offset = 0;
	mD3DImmediateContext->IASetVertexBuffers(0, 1, &mBoxVB, &Stride, &Offset);
	mD3DImmediateContext->IASetIndexBuffer(mBoxIB, DXGI_FORMAT_R32_UINT, 0);

	XMMATRIX WorldMat = XMLoadFloat4x4(&mWorldMat);
	XMMATRIX ViewMat = XMLoadFloat4x4(&mViewMat);
	XMMATRIX ProjMat = XMLoadFloat4x4(&mProjMat);
	XMMATRIX WVPMat = WorldMat * ViewMat * ProjMat;

	mD3DImmediateContext->VSSetShader(mVertexShader, nullptr, 0);
	mD3DImmediateContext->PSSetShader(mPixelShader, nullptr, 0);

	//mD3DImmediateContext->Draw(8, 0);
	mD3DImmediateContext->DrawIndexed(mNumBoxIndex, 0, 0);

	HR(mSwapChain->Present(0, 0));
}

void CCore::BuildGeometryBuffers()
{
	/*Vertex Vertices[] = {
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4((const float*)(&Colors::White))	},
		{ XMFLOAT3(-1.0f, +1.0f, -1.0f), XMFLOAT4((const float*)(&Colors::Black))   },
		{ XMFLOAT3(+1.0f, +1.0f, -1.0f), XMFLOAT4((const float*)(&Colors::Red))   },
		{ XMFLOAT3(+1.0f, -1.0f, -1.0f), XMFLOAT4((const float*)(&Colors::Green))  },
		{ XMFLOAT3(-1.0f, -1.0f, +1.0f), XMFLOAT4((const float*)(&Colors::Blue)) },
		{ XMFLOAT3(-1.0f, +1.0f, +1.0f), XMFLOAT4((const float*)(&Colors::Yellow)) },
		{ XMFLOAT3(+1.0f, +1.0f, +1.0f), XMFLOAT4((const float*)(&Colors::Cyan)) },
		{ XMFLOAT3(+1.0f, -1.0f, +1.0f), XMFLOAT4((const float*)(&Colors::Magenta)) }
	};*/
	Vertex Vertices[] = {
		{ XMFLOAT3(-.5f, -.5f, -.5f), XMFLOAT4((const float*)(&Colors::White))	},
		{ XMFLOAT3(-.5f, +.5f, -.5f), XMFLOAT4((const float*)(&Colors::Black))   },
		{ XMFLOAT3(+.5f, +.5f, -.5f), XMFLOAT4((const float*)(&Colors::Red))   },
		{ XMFLOAT3(+.5f, -.5f, -.5f), XMFLOAT4((const float*)(&Colors::Green))  },
		{ XMFLOAT3(-.5f, -.5f, +.5f), XMFLOAT4((const float*)(&Colors::Blue)) },
		{ XMFLOAT3(-.5f, +.5f, +.5f), XMFLOAT4((const float*)(&Colors::Yellow)) },
		{ XMFLOAT3(+.5f, +.5f, +.5f), XMFLOAT4((const float*)(&Colors::Cyan)) },
		{ XMFLOAT3(+.5f, -.5f, +.5f), XMFLOAT4((const float*)(&Colors::Magenta)) }
	};
	
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertices);
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = Vertices;
	HR(mD3DDevice->CreateBuffer(&vbd, &vinitData, &mBoxVB));

	UINT Indices[] = {
		// front face
		0, 1, 2,
		0, 2, 3,
		// back face
		4, 6, 5,
		4, 7, 6,
		// left face
		4, 5, 1,
		4, 1, 0,
		// right face
		3, 2, 6,
		3, 6, 7,
		// top face
		1, 5, 6,
		1, 6, 2,
		// bottom face
		4, 0, 3,
		4, 3, 7
	};

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(Indices);
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = Indices;
	mNumBoxIndex = 12;
	HR(mD3DDevice->CreateBuffer(&ibd, &iinitData, &mBoxIB));
}

void CCore::BuildShader()
{
	HR(D3DCompileFromFile(L"VertexShader.hlsl", nullptr, nullptr, "vs_main", "vs_5_0", 0, 0, &mVSBlob, nullptr));
	HR(mD3DDevice->CreateVertexShader(mVSBlob->GetBufferPointer(), mVSBlob->GetBufferSize(), nullptr, &mVertexShader));

	HR(D3DCompileFromFile(L"PixelShader.hlsl", nullptr, nullptr, "ps_main", "ps_5_0", 0, 0, &mPSBlob, nullptr));
	HR(mD3DDevice->CreatePixelShader(mPSBlob->GetBufferPointer(), mPSBlob->GetBufferSize(), nullptr, &mPixelShader));
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
	HR(mD3DDevice->CreateInputLayout(VertexDesc, ARRAYSIZE(VertexDesc), mVSBlob->GetBufferPointer(), mVSBlob->GetBufferSize(), &mInputLayout));
}
