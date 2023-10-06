#pragma once

class OObject
{
public:
	OObject(vector<Vertex> Vertices, size_t VerticesSize, vector<UINT> Indices, size_t NumIndex,
		const WCHAR* TextureFileName = L"Textures/default.dds");
	~OObject();
	virtual void Update(float DeltaTime);
	XMMATRIX GetWorldMatrix();
	XMMATRIX GetTexTransform();


	UINT mNumIndex;
	ID3D11Buffer* mVertexBuffer;
	ID3D11Buffer* mIndexBuffer;
	ID3D11ShaderResourceView* mTextureView;
	float mLifeTime;

	XMFLOAT3 mLocation;
	XMFLOAT3 mRotation;	// ¶óµð¾È
	XMFLOAT3 mScale;
	Material mMaterial;
	XMFLOAT3 mTexScale;
	XMFLOAT3 mTexTranslation;
	XMFLOAT3 mTexVelocity;
};