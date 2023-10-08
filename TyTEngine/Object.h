#pragma once

class TObject
{
public:
	TObject();
	~TObject();
	virtual void Update(float DeltaTime);
	virtual void BeginPlay();

	void SetD3DBuffers(UGeometryGenerator::MeshData Mesh);
	void SetSRV(const WCHAR* TextureFileName = L"Textures/default.dds");

	ID3D11Buffer* mVertexBuffer;
	ID3D11Buffer* mIndexBuffer;
	UINT mNumIndex;
	ID3D11ShaderResourceView* mTextureView;
	XMFLOAT3 mTranslation;
	XMFLOAT3 mRotation;	// ¶óµð¾È
	XMFLOAT3 mScale;
	XMFLOAT3 mTexScale;
	XMFLOAT3 mTexRotation;
	XMFLOAT3 mTexTranslation;
	SMaterial mMaterial;

protected:
	float mRunningTime;
private:

public:
	XMMATRIX GetWorldMatrix();
	XMMATRIX GetTexTransform();
	void SetMaterial(SMaterial Material);
};