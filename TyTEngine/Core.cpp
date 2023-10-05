#include "pch.h"
#include "Core.h"
#include "ObjectManager.h"
#include "Object.h"

CCore::CCore():
	CDirect3DApp(),
	mLastMousePos({0, 0}),
	mPSPerFrameConstantBuffer(0),
	mVSPerObjectConstantBuffer(0),
	mVSBlob(0),
	mPSBlob(0),
	mVertexShader(0),
	mPixelShader(0),
	mInputLayout(0),
	mRadius(5.f),
	mTheta(1.5f * UMathHelper::Pi),
	mPhi(0.25f * UMathHelper::Pi)
{
	mCameraPos = {};
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
	ReleaseCOM(mPSPerFrameConstantBuffer);
	ReleaseCOM(mVSPerObjectConstantBuffer);
}

bool CCore::Init(HINSTANCE hInstance, int CmdShow)
{
	if (!CDirect3DApp::Init(hInstance, CmdShow))
	{
		return false;
	}
	BuildShader();
	BuildVertexLayout();
	BuildConstantBuffer();
	return true;
}

void CCore::OnResize()
{
	CDirect3DApp::OnResize();
	// 창이 리사이즈되면 projection matrix를 재계산
	XMMATRIX ResizedProjMat = XMMatrixPerspectiveFovLH(
		0.5f * UMathHelper::Pi,	// FOV = 90
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
	XMStoreFloat3(&mCameraPos, Pos);
	XMVECTOR Target = XMVectorZero();
	XMVECTOR Up = XMVectorSet(0.f, 1.f, 0.f, 0.f);

	XMMATRIX ViewMat = XMMatrixLookAtLH(Pos, Target, Up);
	XMStoreFloat4x4(&mViewMat, ViewMat);
}

void CCore::DrawScene()
{
	assert(md3dDeviceContext);
	assert(mSwapChain);

	md3dDeviceContext->ClearRenderTargetView(mRenderTargetView, reinterpret_cast<const float*>(&Colors::Black));
	md3dDeviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D10_CLEAR_STENCIL, 1.0f, 0);
	md3dDeviceContext->IASetInputLayout(mInputLayout);
	md3dDeviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	UINT Stride = sizeof(Vertex);
	UINT Offset = 0;
	md3dDeviceContext->VSSetShader(mVertexShader, nullptr, 0);
	md3dDeviceContext->PSSetShader(mPixelShader, nullptr, 0);

	PSPerFrameConstantBufferUpdate();

	vector<unique_ptr<OObject>>& Objects = CObjectManager::GetInstance()->mObjects;
	for (int i = 0; i < Objects.size(); ++i)
	{
		VSPerObjectConstantBufferUpdate(Objects[i]);
		PSPerObjectConstantBufferUpdate(Objects[i]);

		md3dDeviceContext->IASetVertexBuffers(0, 1, &(Objects[i]->mVertexBuffer), &Stride, &Offset);
		md3dDeviceContext->IASetIndexBuffer(Objects[i]->mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		md3dDeviceContext->PSSetShaderResources(0, 1, &Objects[i]->mTextureView);
		md3dDeviceContext->DrawIndexed(Objects[i]->mNumIndex, 0, 0);
	}

	HR(mSwapChain->Present(0, 0));
}

void CCore::VSPerObjectConstantBufferUpdate(unique_ptr<OObject>& Object)
{
	D3D11_MAPPED_SUBRESOURCE MappedSubresource;
	VSPerObjectConstantBuffer* ConstantBuffer;
	md3dDeviceContext->Map(mVSPerObjectConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedSubresource);
	ConstantBuffer = (VSPerObjectConstantBuffer*)MappedSubresource.pData;
	XMMATRIX WorldMat = Object->GetWorldMatrix();
	XMMATRIX ViewMat = XMLoadFloat4x4(&mViewMat);
	XMMATRIX ProjMat = XMLoadFloat4x4(&mProjMat);
	ConstantBuffer->World = XMMatrixTranspose(WorldMat);
	ConstantBuffer->WorldInverseTranspose = XMMatrixTranspose(UMathHelper::InverseTranspose(WorldMat));
	ConstantBuffer->WorldViewProj = XMMatrixTranspose(WorldMat * ViewMat * ProjMat);
	md3dDeviceContext->Unmap(mVSPerObjectConstantBuffer, 0);
	md3dDeviceContext->VSSetConstantBuffers(1, 1, &mVSPerObjectConstantBuffer);
}

void CCore::PSPerFrameConstantBufferUpdate()
{
	D3D11_MAPPED_SUBRESOURCE MappedSubresource;
	PSPerFrameConstantBuffer* ConstantBuffer;
	md3dDeviceContext->Map(mPSPerFrameConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedSubresource);
	ConstantBuffer = (PSPerFrameConstantBuffer*)MappedSubresource.pData;
	ConstantBuffer->DirLit = CObjectManager::GetInstance()->mDirLight;
	ConstantBuffer->PointLit = CObjectManager::GetInstance()->mPointLight;
	ConstantBuffer->SpotLit = CObjectManager::GetInstance()->mSpotLight;
	ConstantBuffer->EyePosW = mCameraPos;
	md3dDeviceContext->Unmap(mPSPerFrameConstantBuffer, 0);
	md3dDeviceContext->PSSetConstantBuffers(0, 1, &mPSPerFrameConstantBuffer);
}

void CCore::PSPerObjectConstantBufferUpdate(unique_ptr<OObject>& Object)
{
	D3D11_MAPPED_SUBRESOURCE MappedSubresource;
	PSPerObjectConstantBuffer* ConstantBuffer;
	md3dDeviceContext->Map(mPSPerObjectConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedSubresource);
	ConstantBuffer = (PSPerObjectConstantBuffer*)MappedSubresource.pData;
	ConstantBuffer->Mat = Object->mMaterial;
	md3dDeviceContext->Unmap(mPSPerObjectConstantBuffer, 0);
	md3dDeviceContext->PSSetConstantBuffers(1, 1, &mPSPerObjectConstantBuffer);
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
		mPhi = UMathHelper::Clamp<float>(mPhi, 0.1f, UMathHelper::Pi - 0.1f);
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

void CCore::BuildConstantBuffer()
{
	D3D11_BUFFER_DESC VSPerObjectConstantBufferDesc = {};
	VSPerObjectConstantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	VSPerObjectConstantBufferDesc.ByteWidth = sizeof(VSPerObjectConstantBuffer) + 0xf & 0xfffffff0;
	VSPerObjectConstantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	VSPerObjectConstantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	HR(md3dDevice->CreateBuffer(&VSPerObjectConstantBufferDesc, nullptr, &mVSPerObjectConstantBuffer));

	D3D11_BUFFER_DESC PSPerFrameConstantBufferDesc = {};
	PSPerFrameConstantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	PSPerFrameConstantBufferDesc.ByteWidth = sizeof(PSPerFrameConstantBuffer) + 0xf & 0xfffffff0;
	PSPerFrameConstantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	PSPerFrameConstantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	HR(md3dDevice->CreateBuffer(&PSPerFrameConstantBufferDesc, nullptr, &mPSPerFrameConstantBuffer));

	D3D11_BUFFER_DESC PSPerObjectConstantBufferDesc = {};
	PSPerObjectConstantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	PSPerObjectConstantBufferDesc.ByteWidth = sizeof(PSPerObjectConstantBuffer) + 0xf & 0xfffffff0;
	PSPerObjectConstantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	PSPerObjectConstantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	HR(md3dDevice->CreateBuffer(&PSPerObjectConstantBufferDesc, nullptr, &mPSPerObjectConstantBuffer));

	D3D11_SAMPLER_DESC SamplerDesc = {};
	SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	SamplerDesc.MaxAnisotropy = 4;
	ID3D11SamplerState* SamplerState = {};
	md3dDevice->CreateSamplerState(&SamplerDesc, &SamplerState);
	md3dDeviceContext->PSSetSamplers(0, 1, &SamplerState);
}

void CCore::BuildShader()
{
	HR(D3DCompileFromFile(L"VertexShader.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VSMain", "vs_5_0", 0, 0, &mVSBlob, nullptr));
	HR(md3dDevice->CreateVertexShader(mVSBlob->GetBufferPointer(), mVSBlob->GetBufferSize(), nullptr, &mVertexShader));
	HR(D3DCompileFromFile(L"PixelShader.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PSMain", "ps_5_0", 0, 0, &mPSBlob, nullptr));
	HR(md3dDevice->CreatePixelShader(mPSBlob->GetBufferPointer(), mPSBlob->GetBufferSize(), nullptr, &mPixelShader));
}

void CCore::BuildVertexLayout()
{
	D3D11_INPUT_ELEMENT_DESC VertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
		D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24,
		D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	HR(md3dDevice->CreateInputLayout(VertexDesc, ARRAYSIZE(VertexDesc), mVSBlob->GetBufferPointer(), mVSBlob->GetBufferSize(), &mInputLayout));
}

