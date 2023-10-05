#pragma once
class OObject;

class CObjectManager
{
public:
	SINGLETON(CObjectManager);

	vector<unique_ptr<OObject>> mObjects;
	DirectionalLight mDirLight;
	PointLight mPointLight;
	SpotLight mSpotLight;

private:
	CObjectManager();
	~CObjectManager();

	void CreateCubeObject(const XMFLOAT3 Scale, const XMFLOAT3 Rotation, const XMFLOAT3 Location);
	void CreateGridHillObject(const XMFLOAT3 Scale, const XMFLOAT3 Rotation, const XMFLOAT3 Location);
	void CreateCylinderObject(const XMFLOAT3 Scale, const XMFLOAT3 Rotation, const XMFLOAT3 Location,
		float BottomRadius, float TopRadius, float Height, UINT SliceCount, UINT StackCount);
	void CreateGeoSphereObject(const XMFLOAT3 Scale, const XMFLOAT3 Rotation, const XMFLOAT3 Location, float Radius, UINT NumSubdivisions);
	void PushObjectBuffers(std::vector<Vertex> CubeVertex, std::vector<UINT> CubeIndex,
		const DirectX::XMFLOAT3 Scale, const DirectX::XMFLOAT3 Rotation, const DirectX::XMFLOAT3 Location,
		const WCHAR* TextureFileName = L"Textures/default.dds");
	float GetHillHeight(float x, float z);
	XMFLOAT3 GetHilNormal(float x, float z);
	XMFLOAT3 NormalizeXMFLOAT3(XMFLOAT3 Input);
	XMFLOAT4 NormalizeXMFLOAT4(XMFLOAT4 Input);
};