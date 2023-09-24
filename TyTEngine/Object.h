#pragma once

struct Vertex
{
	XMFLOAT3 Pos;
	XMFLOAT4 Color;
};

class OObject
{
public:
	OObject(Vertex* Vertices, size_t VerticesSize, UINT* Indices, size_t NumIndex);
	~OObject();
	XMMATRIX GetWorldMatrix();

	UINT mNumIndex;
	ID3D11Buffer* mVertexBuffer;
	ID3D11Buffer* mIndexBuffer;
	XMFLOAT3 mLocation;
	XMFLOAT3 mRotation;	// ¶óµð¾È
	XMFLOAT3 mScale;
};