#pragma once
class OObject;

class CObjectManager
{
public:
	SINGLETON(CObjectManager);

	vector<unique_ptr<OObject>> mObjects;

private:
	CObjectManager();
	~CObjectManager();

	void CreateCubeObject(const XMFLOAT3 Scale, const XMFLOAT3 Rotation, const XMFLOAT3 Location, const XMVECTORF32 Color);
	void CreateGridHillObject(const XMFLOAT3 Scale, const XMFLOAT3 Rotation, const XMFLOAT3 Location);
	void CreateCylinderObject(const XMFLOAT3 Scale, const XMFLOAT3 Rotation, const XMFLOAT3 Location, const XMVECTORF32 Color,
		float BottomRadius, float TopRadius, float Height, UINT SliceCount, UINT StackCount);
	void PushObjectBuffers(std::vector<Vertex> CubeVertex, std::vector<UINT> CubeIndex, const DirectX::XMFLOAT3 Scale, const DirectX::XMFLOAT3 Rotation, const DirectX::XMFLOAT3 Location);
	float GetHeight(float x, float z);
};