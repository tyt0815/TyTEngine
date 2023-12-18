#include "pch.h"
#include "Object.h"
#include "Engine.h"
#include "ObjectManager.h"

TObject::TObject() :
	mNumIndex(0),
	mScale({1,1,1}),
	mRotation({0,0,0}),
	mTranslation({0,0,0}),
	mTexScale({1.f,1.f,1.f}),
	mTexRotation({ 0.f,0.f,0.f }),
	mTexTranslation({0.f,0.f,0.f}),
	mVertexBuffer(nullptr),
	mIndexBuffer(nullptr),
	mTextureView(nullptr),
	mRunningTime(0)
{
	mMaterial.Ambient = { 1.f,1.f,1.f,1.f };
	mMaterial.Diffuse = { 1.f,1.f,1.f,1.f };
	mMaterial.Specular = { 1.f,1.f,1.f,1.f };
	mMaterial.Reflect = { 1.f,1.f,1.f,1.f };
	SetSRV();
}

TObject::~TObject()
{
	ReleaseCOM(mVertexBuffer);
	ReleaseCOM(mIndexBuffer);
	ReleaseCOM(mTextureView);
}

void TObject::Update(float DeltaTime)
{
	mRunningTime += DeltaTime;
}

void TObject::BeginPlay()
{
}

void TObject::SetD3DBuffers(UGeometryGenerator::MeshData Mesh)
{
	vector<SVertex> Vertices;
	UINT VerticesSize;
	Vertices.resize(Mesh.Vertices.size());
	for (int i = 0; i < Vertices.size(); ++i)
	{
		Vertices[i].Position = Mesh.Vertices[i].Position;
		Vertices[i].Normal = Mesh.Vertices[i].Normal;
		Vertices[i].UV = Mesh.Vertices[i].TexC;
	}
	VerticesSize = static_cast<UINT>(sizeof(SVertex) * Vertices.size());

	vector<UINT> Indices;
	UINT IndicesSize;
	Indices.resize(Mesh.Indices.size());
	Indices.assign(Mesh.Indices.begin(), Mesh.Indices.end());
	IndicesSize = static_cast<UINT>(sizeof(UINT) * Indices.size());

	ID3D11Device* Device = TEngine::GetInstance()->GetD3DDevice();
	D3D11_BUFFER_DESC VertexBufferDesc;
	ZeroMemory(&VertexBufferDesc, sizeof(D3D11_BUFFER_DESC));
	VertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	VertexBufferDesc.ByteWidth = VerticesSize;
	VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VertexBufferDesc.CPUAccessFlags = 0;
	VertexBufferDesc.MiscFlags = 0;
	VertexBufferDesc.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA VertexInitData;
	VertexInitData.pSysMem = &Vertices[0];
	HR(Device->CreateBuffer(&VertexBufferDesc, &VertexInitData, &mVertexBuffer));

	D3D11_BUFFER_DESC IndexBufferDesc;
	ZeroMemory(&IndexBufferDesc, sizeof(D3D11_BUFFER_DESC));
	IndexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	IndexBufferDesc.ByteWidth = IndicesSize;
	IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDesc.CPUAccessFlags = 0;
	IndexBufferDesc.MiscFlags = 0;
	IndexBufferDesc.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA IndexInitData;
	IndexInitData.pSysMem = &Indices[0];
	HR(Device->CreateBuffer(&IndexBufferDesc, &IndexInitData, &mIndexBuffer));
	mNumIndex = UINT(IndicesSize / sizeof(UINT));
}

void TObject::SetSRV(const WCHAR* TextureFileName)
{
	ID3D11Device* Device = TEngine::GetInstance()->GetD3DDevice();
	ScratchImage TexImg;
	ScratchImage DecImg;
	ScratchImage MipCh;
	HR(LoadFromDDSFile(TextureFileName, DDS_FLAGS::DDS_FLAGS_NONE, nullptr, TexImg));
	Decompress(TexImg.GetImages(), TexImg.GetImageCount(), TexImg.GetMetadata(), DXGI_FORMAT_UNKNOWN, DecImg);
	HR(GenerateMipMaps(DecImg.GetImages(), DecImg.GetImageCount(), DecImg.GetMetadata(), TEX_FILTER_DEFAULT, 0, MipCh));
	HR(CreateShaderResourceView(Device, MipCh.GetImages(), MipCh.GetImageCount(), MipCh.GetMetadata(), &mTextureView));
	ScratchImage Custom;
}

XMMATRIX TObject::GetWorldMatrix()
{
	XMMATRIX ScaleMat =
	{
		mScale.x, 0.f, 0.f, 0.f,
		0.f, mScale.y, 0.f, 0.f,
		0.f, 0.f, mScale.z ,0.f,
		0.f, 0.f, 0.f, 1.f
	};

	float SinX = sinf(mRotation.x);
	float CosX = cosf(mRotation.x);
	float SinY = sinf(mRotation.y);
	float CosY = cosf(mRotation.y);
	float SinZ = sinf(mRotation.z);
	float CosZ = cosf(mRotation.z);
	XMMATRIX RotationMat =
	{
		CosY * CosZ, CosY * SinZ, -SinY, 0.f,
		SinX * SinY * CosY - CosX * SinZ, SinX * SinY * SinZ + CosX * CosZ, SinX * CosZ, 0.f,
		CosX * SinY * CosZ + SinX * SinZ, CosX * SinY * SinZ - SinX * CosZ, CosX * CosY, 0.f,
		0.f, 0.f, 0.f, 1.f
	};

	XMMATRIX TranslationMat =
	{
		1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		mTranslation.x, mTranslation.y, mTranslation.z,1.f
	};

	return ScaleMat * RotationMat * TranslationMat;
}

XMMATRIX TObject::GetTexTransform()
{
	XMMATRIX Scale = XMMatrixScalingFromVector(XMLoadFloat3(&mTexScale));
	XMMATRIX Translation = XMMatrixTranslationFromVector(XMLoadFloat3(&mTexTranslation));
	XMMATRIX Result = Scale * Translation;
	return Result;
}

void TObject::SetMaterial(SMaterial Material)
{
	mMaterial = Material;
}
