#include "pch.h"
#include "Engine.h"
#include "ObjectManager.h"

TEngine::TEngine() :
	TDirect3DApp(),
	mInputLayout(nullptr),
	mVSPerObjectConstantBuffer(nullptr),
	mPSPerFrameConstantBuffer(nullptr),
	mPSPerObjectConstantBuffer(nullptr),
	mPSLitConstantBuffer(nullptr),
	mVSBlob(nullptr),
	mVertexShader(nullptr),
	mPSBlob(nullptr),
	mPixelShader(nullptr),
	mRSNoCull(nullptr),
	mSamplerState(nullptr),
	mTransparentBS(nullptr),
	mCullClockwiseRS(nullptr),
	mDrawReflectionDSS(nullptr),
	mMarkMirrorDSS(nullptr),
	mNoDoubleBlendDSS(nullptr),
	mNoRenderTargetWritesBS(nullptr),
	mShadowPixelShader(nullptr),
	mLastMousePos({ 0, 0 }),
	mRadius(10.f),
	mTheta(1.5f * UMathHelper::Pi),
	mPhi(0.25f * UMathHelper::Pi),
	mShadow(false),
	mMirror(false)
{
	mCameraPos = {};
	XMMATRIX I = XMMatrixIdentity();
	XMStoreFloat4x4(&mViewMat, I);
	XMStoreFloat4x4(&mProjMat, I);
}

TEngine::~TEngine()
{
	ReleaseCOM(mInputLayout);
	ReleaseCOM(mVSBlob);
	ReleaseCOM(mPSBlob);
	ReleaseCOM(mVertexShader);
	ReleaseCOM(mPixelShader);
	ReleaseCOM(mPSPerFrameConstantBuffer);
	ReleaseCOM(mVSPerObjectConstantBuffer);
	ReleaseCOM(mRSNoCull);
	ReleaseCOM(mTransparentBS);
}

bool TEngine::Init(HINSTANCE hInstance, int CmdShow)
{
	if (!TDirect3DApp::Init(hInstance, CmdShow))
	{
		return false;
	}
	BuildShader();
	BuildVertexLayout();
	BuildConstantBuffer();
	InitRenderState();
	CreateActors();
	CreateLits();
	ObjectsBeginPlay();
	return true;
}

void TEngine::ObjectsBeginPlay()
{
	ObjectBeginePlay(TObjectManager::GetInstance()->mOpaqueActors);
	ObjectBeginePlay(TObjectManager::GetInstance()->mTranslucentActors);
	ObjectBeginePlay(TObjectManager::GetInstance()->mMirrorActors);
}

void TEngine::ObjectBeginePlay(vector<unique_ptr<TObject>>& Object)
{
	vector<unique_ptr<TObject>>& OpaqueActors = Object;
	for (int i = 0; i < OpaqueActors.size(); ++i)
	{
		OpaqueActors[i]->BeginPlay();
	}
}

void TEngine::OnResize()
{
	TDirect3DApp::OnResize();
	// 창이 리사이즈되면 projection matrix를 재계산
	XMMATRIX ResizedProjMat = XMMatrixPerspectiveFovLH(
		0.5f * UMathHelper::Pi,	// FOV = 90
		GetAspectRatio(),
		1.f,
		1000.f
	);
	XMStoreFloat4x4(&mProjMat, ResizedProjMat);
}

void TEngine::UpdateScene(float DeltaTime)
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

	UpdateObject(TObjectManager::GetInstance()->mOpaqueActors);
	UpdateObject(TObjectManager::GetInstance()->mTranslucentActors);
	UpdateObject(TObjectManager::GetInstance()->mMirrorActors);
}

void TEngine::UpdateObject(vector<unique_ptr<TObject>>& Object)
{
	vector<unique_ptr<TObject>>& OpaqueObjects = Object;
	for (int i = 0; i < OpaqueObjects.size(); ++i)
	{
		OpaqueObjects[i]->Update(mTimer->DeltaTime());
	}
}

void TEngine::DrawScene()
{
	assert(md3dDeviceContext);
	assert(mSwapChain);

	md3dDeviceContext->ClearRenderTargetView(mRenderTargetView, reinterpret_cast<const float*>(&Colors::Cyan));
	md3dDeviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	md3dDeviceContext->IASetInputLayout(mInputLayout);
	md3dDeviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	md3dDeviceContext->VSSetShader(mVertexShader, nullptr, 0);
	md3dDeviceContext->PSSetShader(mPixelShader, nullptr, 0);

	PSPerFrameConstantBufferUpdate();
	PSLitConstantBufferUpdate();

	float blendFactors[] = { 0.0f, 0.0f, 0.0f, 0.0f };

	// Draw Opaque Actors
	vector<unique_ptr<TObject>>& OpaqueActors = TObjectManager::GetInstance()->mOpaqueActors;
	DrawActors(OpaqueActors);

	// Draw Translucent Actors
	md3dDeviceContext->RSSetState(mRSNoCull);
	md3dDeviceContext->OMSetBlendState(mTransparentBS, blendFactors, 0xffffffff);
	vector<unique_ptr<TObject>>& TranslucentActors = TObjectManager::GetInstance()->mTranslucentActors;
	DrawActors(TranslucentActors);
	SetDefaultState();

	// Draw Mirror
	md3dDeviceContext->OMSetBlendState(mNoRenderTargetWritesBS, blendFactors, 0xffffffff);
	md3dDeviceContext->OMSetDepthStencilState(mMarkMirrorDSS, 1);
	vector<unique_ptr<TObject>>& MirrorActors = TObjectManager::GetInstance()->mMirrorActors;
	DrawActors(MirrorActors);
	SetDefaultState();
	md3dDeviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	// Draw Reflected Object
	md3dDeviceContext->RSSetState(mCullClockwiseRS);
	md3dDeviceContext->OMSetDepthStencilState(mDrawReflectionDSS, 1);
	XMVECTOR MirrorPlane = XMVectorSet(1.f, 0.f, 0.f, 0.f);		//zy평면
	XMMATRIX R = XMMatrixReflect(MirrorPlane);
	DrawActors(OpaqueActors, R);
	md3dDeviceContext->OMSetBlendState(mTransparentBS, blendFactors, 0xffffffff);
	DrawActors(TranslucentActors, R);
	SetDefaultState();
	// Draw the mirror to the back buffer
	md3dDeviceContext->OMSetBlendState(mTransparentBS, blendFactors, 0xffffffff);
	DrawActors(MirrorActors);
	SetDefaultState();
	// Draw Shadow
	if (mShadow)
	{
		md3dDeviceContext->PSSetShader(mShadowPixelShader, nullptr, 0);
		md3dDeviceContext->OMSetBlendState(mTransparentBS, blendFactors, 0xffffffff);
		md3dDeviceContext->OMSetDepthStencilState(mNoDoubleBlendDSS, 0);
		XMVECTOR ShadowPlane = XMVectorSet(0.f, 1.f, 0.f, 0.f);
		XMVECTOR ToMainLight = -XMLoadFloat3(&(TObjectManager::GetInstance()->mDirLits[0]->Direction));
		XMMATRIX S = XMMatrixShadow(ShadowPlane, ToMainLight);
		XMMATRIX ShadowOffsetY = XMMatrixTranslation(0.f, 0.001f, 0.f);
		DrawActors(OpaqueActors, S * ShadowOffsetY);
		SetDefaultState();
	}

	HR(mSwapChain->Present(0, 0));
}

void TEngine::SetDefaultState()
{
	md3dDeviceContext->RSSetState(nullptr);
	md3dDeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);
	md3dDeviceContext->OMSetDepthStencilState(nullptr, 0);
}

void TEngine::DrawActors(std::vector<std::unique_ptr<TObject>>& Actors, XMMATRIX AdditionalMat)
{
	UINT Stride = sizeof(SVertex);
	UINT Offset = 0;
	size_t Size = Actors.size();
	for (int i = 0; i < Size; ++i)
	{
		VSPerObjectConstantBufferUpdate(Actors[i], AdditionalMat);
		PSPerObjectConstantBufferUpdate(Actors[i]);

		md3dDeviceContext->IASetVertexBuffers(0, 1, &(Actors[i]->mVertexBuffer), &Stride, &Offset);
		md3dDeviceContext->IASetIndexBuffer(Actors[i]->mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		md3dDeviceContext->PSSetShaderResources(0, 1, &Actors[i]->mTextureView);
		md3dDeviceContext->DrawIndexed(Actors[i]->mNumIndex, 0, 0);
	}
}

void TEngine::VSPerObjectConstantBufferUpdate(unique_ptr<TObject>& Object, XMMATRIX AdditionalMat)
{
	D3D11_MAPPED_SUBRESOURCE MappedSubresource;
	SVSPerObjectConstantBuffer* ConstantBuffer;
	md3dDeviceContext->Map(mVSPerObjectConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedSubresource);
	ConstantBuffer = (SVSPerObjectConstantBuffer*)MappedSubresource.pData;
	XMMATRIX WorldMat = Object->GetWorldMatrix() * AdditionalMat;
	XMMATRIX ViewMat = XMLoadFloat4x4(&mViewMat);
	XMMATRIX ProjMat = XMLoadFloat4x4(&mProjMat);
	ConstantBuffer->World = XMMatrixTranspose(WorldMat);
	ConstantBuffer->WorldInverseTranspose = XMMatrixTranspose(UMathHelper::InverseTranspose(WorldMat));
	ConstantBuffer->WorldViewProj = XMMatrixTranspose(WorldMat * ViewMat * ProjMat);
	ConstantBuffer->TexTransform = XMMatrixTranspose(Object->GetTexTransform());
	md3dDeviceContext->Unmap(mVSPerObjectConstantBuffer, 0);
	md3dDeviceContext->VSSetConstantBuffers(1, 1, &mVSPerObjectConstantBuffer);
}

void TEngine::PSPerFrameConstantBufferUpdate()
{
	TObjectManager* ObjectManager = TObjectManager::GetInstance();
	D3D11_MAPPED_SUBRESOURCE MappedSubresource;
	SPSPerFrameConstantBuffer* ConstantBuffer;
	md3dDeviceContext->Map(mPSPerFrameConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedSubresource);
	ConstantBuffer = (SPSPerFrameConstantBuffer*)MappedSubresource.pData;
	ConstantBuffer->NumLit = XMUINT4(
		static_cast<UINT>(ObjectManager->mDirLits.size()),
		static_cast<UINT>(ObjectManager->mPointLits.size()),
		static_cast<UINT>(ObjectManager->mSpotLits.size()),
		0
	);
	ConstantBuffer->EyePosW = mCameraPos;
	ConstantBuffer->FogColor = { 0.75f, 0.75f, 0.75f, 1.0f };
	ConstantBuffer->FogStart = 1.f;
	ConstantBuffer->FogRange = 100.f;
	md3dDeviceContext->Unmap(mPSPerFrameConstantBuffer, 0);
	md3dDeviceContext->PSSetConstantBuffers(0, 1, &mPSPerFrameConstantBuffer);
}

void TEngine::PSPerObjectConstantBufferUpdate(unique_ptr<TObject>& Object)
{
	D3D11_MAPPED_SUBRESOURCE MappedSubresource;
	SPSPerObjectConstantBuffer* ConstantBuffer;
	md3dDeviceContext->Map(mPSPerObjectConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedSubresource);
	ConstantBuffer = (SPSPerObjectConstantBuffer*)MappedSubresource.pData;
	ConstantBuffer->Mat = Object->mMaterial;
	md3dDeviceContext->Unmap(mPSPerObjectConstantBuffer, 0);
	md3dDeviceContext->PSSetConstantBuffers(1, 1, &mPSPerObjectConstantBuffer);
}

void TEngine::PSLitConstantBufferUpdate()
{
	TObjectManager* ObjectManager = TObjectManager::GetInstance();
	D3D11_MAPPED_SUBRESOURCE MappedSubresource;
	SPSLitConstantBuffer* ConstantBuffer;
	md3dDeviceContext->Map(mPSLitConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedSubresource);
	ConstantBuffer = (SPSLitConstantBuffer*)MappedSubresource.pData;
	for (int i = 0; i < ObjectManager->mDirLits.size(); ++i)
	{
		memcpy(ConstantBuffer->DirLit + i, ObjectManager->mDirLits[i].get(), sizeof(SDirectionalLight));
	}
	for (int i = 0; i < ObjectManager->mPointLits.size(); ++i)
	{
		memcpy(ConstantBuffer->PointLit + i, ObjectManager->mPointLits[i].get(), sizeof(SPointLight));
	}
	for (int i = 0; i < ObjectManager->mSpotLits.size(); ++i)
	{
		memcpy(ConstantBuffer->SpotLit + i, ObjectManager->mSpotLits[i].get(), sizeof(SSpotLight));
	}
	md3dDeviceContext->Unmap(mPSLitConstantBuffer, 0);
	md3dDeviceContext->PSSetConstantBuffers(2, 1, &mPSLitConstantBuffer);
}

void TEngine::OnMouseDown(WPARAM BtnState, int x, int y)
{
	mLastMousePos.x = x;
	mLastMousePos.y = y;
	SetCapture(mhMainWnd);
}

void TEngine::OnMouseUp(WPARAM BtnState, int x, int y)
{
	ReleaseCapture();
}

void TEngine::OnMouseMove(WPARAM BtnState, int x, int y)
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

void TEngine::BuildConstantBuffer()
{
	D3D11_BUFFER_DESC VSPerObjectConstantBufferDesc = {};
	VSPerObjectConstantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	VSPerObjectConstantBufferDesc.ByteWidth = sizeof(SVSPerObjectConstantBuffer) + 0xf & 0xfffffff0;
	VSPerObjectConstantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	VSPerObjectConstantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	HR(md3dDevice->CreateBuffer(&VSPerObjectConstantBufferDesc, nullptr, &mVSPerObjectConstantBuffer));

	D3D11_BUFFER_DESC PSPerFrameConstantBufferDesc = {};
	PSPerFrameConstantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	PSPerFrameConstantBufferDesc.ByteWidth = sizeof(SPSPerFrameConstantBuffer) + 0xf & 0xfffffff0;
	PSPerFrameConstantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	PSPerFrameConstantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	HR(md3dDevice->CreateBuffer(&PSPerFrameConstantBufferDesc, nullptr, &mPSPerFrameConstantBuffer));

	D3D11_BUFFER_DESC PSPerObjectConstantBufferDesc = {};
	PSPerObjectConstantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	PSPerObjectConstantBufferDesc.ByteWidth = sizeof(SPSPerObjectConstantBuffer) + 0xf & 0xfffffff0;
	PSPerObjectConstantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	PSPerObjectConstantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	HR(md3dDevice->CreateBuffer(&PSPerObjectConstantBufferDesc, nullptr, &mPSPerObjectConstantBuffer));

	D3D11_BUFFER_DESC PSLitBufferDesc = {};
	PSLitBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	PSLitBufferDesc.ByteWidth = sizeof(SPSLitConstantBuffer) + 0xf & 0xfffffff0;
	PSLitBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	PSLitBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	HR(md3dDevice->CreateBuffer(&PSLitBufferDesc, nullptr, &mPSLitConstantBuffer));
}

void TEngine::BuildShader()
{
	HR(D3DCompileFromFile(L"VertexShader.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VSMain", "vs_5_0", 0, 0, &mVSBlob, nullptr));
	HR(md3dDevice->CreateVertexShader(mVSBlob->GetBufferPointer(), mVSBlob->GetBufferSize(), nullptr, &mVertexShader));
	HR(D3DCompileFromFile(L"PixelShader.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PSMain", "ps_5_0", 0, 0, &mPSBlob, nullptr));
	HR(md3dDevice->CreatePixelShader(mPSBlob->GetBufferPointer(), mPSBlob->GetBufferSize(), nullptr, &mPixelShader));
	HR(D3DCompileFromFile(L"PixelShader.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "ShadowMain", "ps_5_0", 0, 0, &mPSBlob, nullptr));
	HR(md3dDevice->CreatePixelShader(mPSBlob->GetBufferPointer(), mPSBlob->GetBufferSize(), nullptr, &mShadowPixelShader));
}

void TEngine::BuildVertexLayout()
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

void TEngine::InitRenderState()
{
	BuildSamplerState();
	BuildDepthStencilState();
	BuildBlendState();
	BuildRasterizerState();
}

void TEngine::BuildSamplerState()
{
	D3D11_SAMPLER_DESC SamplerDesc = {};
	SamplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerDesc.MipLODBias = 0.f;
	SamplerDesc.MaxAnisotropy = 1;
	SamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	SamplerDesc.BorderColor;
	SamplerDesc.MinLOD = FLT_MIN;
	SamplerDesc.MaxLOD = FLT_MAX;
	md3dDevice->CreateSamplerState(&SamplerDesc, &mSamplerState);
	md3dDeviceContext->PSSetSamplers(0, 1, &mSamplerState);
}

void TEngine::BuildBlendState()
{
	D3D11_BLEND_DESC TransparentDesc = { 0 };
	TransparentDesc.AlphaToCoverageEnable = false;
	TransparentDesc.IndependentBlendEnable = false;
	TransparentDesc.RenderTarget[0].BlendEnable = true;
	TransparentDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	TransparentDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	TransparentDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	TransparentDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	TransparentDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	TransparentDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	TransparentDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	HR(md3dDevice->CreateBlendState(&TransparentDesc, &mTransparentBS));

	D3D11_BLEND_DESC NoRenderTargetWritesDesc = { 0 };
	NoRenderTargetWritesDesc.AlphaToCoverageEnable = false;
	NoRenderTargetWritesDesc.IndependentBlendEnable = false;
	NoRenderTargetWritesDesc.RenderTarget[0].BlendEnable = false;
	NoRenderTargetWritesDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	NoRenderTargetWritesDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	NoRenderTargetWritesDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	NoRenderTargetWritesDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	NoRenderTargetWritesDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	NoRenderTargetWritesDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	NoRenderTargetWritesDesc.RenderTarget[0].RenderTargetWriteMask = 0;			// 이게 중요. 나머지는 딱히?

	HR(md3dDevice->CreateBlendState(&NoRenderTargetWritesDesc, &mNoRenderTargetWritesBS));
}

void TEngine::BuildRasterizerState()
{
	D3D11_RASTERIZER_DESC NoCullDesc;
	ZeroMemory(&NoCullDesc, sizeof(D3D11_RASTERIZER_DESC));
	NoCullDesc.FillMode = D3D11_FILL_SOLID;
	NoCullDesc.CullMode = D3D11_CULL_NONE;
	NoCullDesc.FrontCounterClockwise = false;
	NoCullDesc.DepthClipEnable = true;
	HR(md3dDevice->CreateRasterizerState(&NoCullDesc, &mRSNoCull));

	D3D11_RASTERIZER_DESC CullClockwiseDesc;
	ZeroMemory(&CullClockwiseDesc, sizeof(D3D11_RASTERIZER_DESC));
	CullClockwiseDesc.FillMode = D3D11_FILL_SOLID;
	CullClockwiseDesc.CullMode = D3D11_CULL_BACK;
	CullClockwiseDesc.FrontCounterClockwise = true;
	CullClockwiseDesc.DepthClipEnable = true;
	HR(md3dDevice->CreateRasterizerState(&CullClockwiseDesc, &mCullClockwiseRS));
}

void TEngine::BuildDepthStencilState()
{
	D3D11_DEPTH_STENCIL_DESC MirrorDesc;
	MirrorDesc.DepthEnable = true;
	MirrorDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	MirrorDesc.DepthFunc = D3D11_COMPARISON_LESS;
	MirrorDesc.StencilEnable = true;
	MirrorDesc.StencilReadMask = 0xff;
	MirrorDesc.StencilWriteMask = 0xff;
	MirrorDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	MirrorDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	MirrorDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	MirrorDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	// We are not rendering backfacing polygons, so these settings do not matter.
	MirrorDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	MirrorDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	MirrorDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	MirrorDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	HR(md3dDevice->CreateDepthStencilState(&MirrorDesc, &mMarkMirrorDSS));

	D3D11_DEPTH_STENCIL_DESC DrawReflectionDesc;
	DrawReflectionDesc.DepthEnable = true;
	DrawReflectionDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	DrawReflectionDesc.DepthFunc = D3D11_COMPARISON_LESS;
	DrawReflectionDesc.StencilEnable = true;
	DrawReflectionDesc.StencilReadMask = 0xff;
	DrawReflectionDesc.StencilWriteMask = 0xff;
	DrawReflectionDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	DrawReflectionDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	DrawReflectionDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	DrawReflectionDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
	// We are not rendering backfacing polygons, so these settings do not matter.
	DrawReflectionDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	DrawReflectionDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	DrawReflectionDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	DrawReflectionDesc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;
	HR(md3dDevice->CreateDepthStencilState(&DrawReflectionDesc, &mDrawReflectionDSS));

	D3D11_DEPTH_STENCIL_DESC NoDoubleBlendDesc;
	NoDoubleBlendDesc.DepthEnable = true;
	NoDoubleBlendDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	NoDoubleBlendDesc.DepthFunc = D3D11_COMPARISON_LESS;
	NoDoubleBlendDesc.StencilEnable = true;
	NoDoubleBlendDesc.StencilReadMask = 0xff;
	NoDoubleBlendDesc.StencilWriteMask = 0xff;

	NoDoubleBlendDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	NoDoubleBlendDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	NoDoubleBlendDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
	NoDoubleBlendDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;

	// We are not rendering backfacing polygons, so these settings do not matter.
	NoDoubleBlendDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	NoDoubleBlendDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	NoDoubleBlendDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
	NoDoubleBlendDesc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;

	HR(md3dDevice->CreateDepthStencilState(&NoDoubleBlendDesc, &mNoDoubleBlendDSS));
}

void TEngine::CreateActors()
{
	ACube* Cube = new ACube();
	Cube->SetSRV(L"Textures/WireFence.dds");
	Cube->mTranslation = XMFLOAT3(2.f, 1.f, 0.f);

	/*AHill* Hill = new AHill(500, 500, 50, 50);
	Hill->SetSRV(L"Textures/grass.dds");
	Hill->mTranslation = XMFLOAT3(0.f, -5.f, 0.f);*/
	
	ACylinder* Cylinder = new ACylinder();
	Cylinder->SetSRV(L"Textures/darkbrickdxt1.dds");
	Cylinder->mTranslation = XMFLOAT3(-2.f, 1.f, 0.f);

	ASphere* Sphere = new ASphere();
	Sphere->SetSRV(L"Textures/red.dds");
	Sphere->mTranslation = XMFLOAT3(0.f, 3.f, 0.f);

	/*AWater* Water = new AWater(150, 150, 50, 50);
	Water->SetSRV(L"Textures/water2.dds");
	Water->mTranslation.y = -5.f;*/

	AMirrorCube* MirrorCube = new AMirrorCube();
	MirrorCube->SetSRV(L"Textures/ice.dds");
	MirrorCube->mScale = XMFLOAT3(0.1f, 2.f , 2.f);
}

void TEngine::CreateLits()
{
	SDirectionalLight DirLit1;
	DirLit1.Ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	DirLit1.Diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	DirLit1.Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	DirLit1.Direction = UMathHelper::NormalizeXMFLOAT(XMFLOAT3(1.f, -1.f, 0.f));
	AddLit(DirLit1);

	SPointLight PointLit1;
	PointLit1.Ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	PointLit1.Diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	PointLit1.Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	PointLit1.Position = {};
	PointLit1.Range = 15.f;
	PointLit1.Attenuation = XMFLOAT3(1.f, .1f, 0.01f);
	AddLit(PointLit1);

	SSpotLight SpotLit1;
	SpotLit1.Ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	SpotLit1.Diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	SpotLit1.Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	SpotLit1.Position = { 3.f, 3.f, 0.f };
	SpotLit1.Range = 50.f;
	SpotLit1.Attenuation = XMFLOAT3(1.f, .1f, 0.01f);
	SpotLit1.Direction = UMathHelper::NormalizeXMFLOAT(XMFLOAT3(-1.f, -1.f, 0.f));
	SpotLit1.Spot = 5.f;
	AddLit(SpotLit1);
}

void TEngine::AddLit(SDirectionalLight& DirectionalLight)
{
	TObjectManager::GetInstance()->mDirLits.push_back(make_unique<SDirectionalLight>(DirectionalLight));
}

void TEngine::AddLit(SPointLight& PointLight)
{
	TObjectManager::GetInstance()->mPointLits.push_back(make_unique<SPointLight>(PointLight));
}

void TEngine::AddLit(SSpotLight& SpotLight)
{
	TObjectManager::GetInstance()->mSpotLits.push_back(make_unique<SSpotLight>(SpotLight));
}
