#include "pch.h"
#include "Object.h"
#include "Core.h"

OObject::OObject(Vertex* Vertices, size_t VerticesSize, UINT* Indices, size_t IndicesSize) :
	mNumIndex(IndicesSize / sizeof(UINT)),
	mScale({1,1,1}),
	mRotation({0,0,0}),
	mLocation({0,0,0})
{
	D3D11_BUFFER_DESC VertexBufferDesc;
	ZeroMemory(&VertexBufferDesc, sizeof(D3D11_BUFFER_DESC));
	VertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	VertexBufferDesc.ByteWidth = VerticesSize;
	VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VertexBufferDesc.CPUAccessFlags = 0;
	VertexBufferDesc.MiscFlags = 0;
	VertexBufferDesc.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA VertexInitData;
	VertexInitData.pSysMem = Vertices;
	HR(CCore::GetInstance()->CreateD3D11Buffer(&VertexBufferDesc, &VertexInitData, &mVertexBuffer));

	D3D11_BUFFER_DESC IndexBufferDesc;
	ZeroMemory(&IndexBufferDesc, sizeof(D3D11_BUFFER_DESC));
	IndexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	IndexBufferDesc.ByteWidth = IndicesSize;
	IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDesc.CPUAccessFlags = 0;
	IndexBufferDesc.MiscFlags = 0;
	IndexBufferDesc.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA IndexInitData;
	IndexInitData.pSysMem = Indices;
	HR(CCore::GetInstance()->CreateD3D11Buffer(&IndexBufferDesc, &IndexInitData, &mIndexBuffer));
}

OObject::~OObject()
{
	ReleaseCOM(mVertexBuffer);
	ReleaseCOM(mIndexBuffer);
}

XMMATRIX OObject::GetWorldMatrix()
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
		mLocation.x, mLocation.y, mLocation.z,1.f
	};

	return ScaleMat * RotationMat * TranslationMat;
}
