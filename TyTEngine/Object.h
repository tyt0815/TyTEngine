#pragma once

class OObject
{
public:
	OObject(vector<Vertex> Vertices, size_t VerticesSize, vector<UINT> Indices, size_t NumIndex, XMVECTORF32 Color);
	~OObject();
	XMMATRIX GetWorldMatrix();

	UINT mNumIndex;
	ID3D11Buffer* mVertexBuffer;
	ID3D11Buffer* mIndexBuffer;
	XMFLOAT3 mLocation;
	XMFLOAT3 mRotation;	// ����
	XMFLOAT3 mScale;
	Material mMaterial;
};