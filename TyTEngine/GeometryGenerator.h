#pragma once

class UGeometryGenerator
{
public:
	struct SVertex
	{
		SVertex() :
			Position({0.f, 0.f, 0.f}),
			Normal({ 0.f, 0.f, 0.f }),
			TangentU({ 0.f, 0.f, 0.f }),
			TexC({ 0.f, 0.f})
		{};
		SVertex(const XMFLOAT3& p,
			const XMFLOAT3& n,
			const XMFLOAT3& t,
			const XMFLOAT2& uv)
			: Position(p), Normal(n), TangentU(t), TexC(uv) {}
		SVertex(
			float px, float py, float pz,
			float nx, float ny, float nz,
			float tx, float ty, float tz,
			float u, float v)
			: Position(px, py, pz), Normal(nx, ny, nz),
			TangentU(tx, ty, tz), TexC(u, v) {}
		XMFLOAT3 Position;
		XMFLOAT3 Normal;
		XMFLOAT3 TangentU;
		XMFLOAT2 TexC;

	};
	struct MeshData
	{
		std::vector<SVertex> Vertices;
		std::vector<UINT> Indices;
	};
	static void CreateCube(float Width, float Height, float Depth, MeshData& Mesh);
	static void CreateCylinder(float BottomRadius, float TopRadius, float Height, UINT SliceCount, UINT StackCount, MeshData& Mesh);
	static void CreateGeosphere(float Radius, UINT NumSubdivisions, MeshData& Mesh);
	static void CreateGrid(float Width, float Depth, UINT m, UINT n, MeshData& Mesh);
	static void CreateHill(float Width, float Depth, UINT m, UINT n, MeshData& Mesh);
	static void CreateWater(float Width, float Depth, UINT m, UINT n, MeshData& Mesh);

private:
	static void BuildCylinderTopCap(float BottomRadius, float TopRadius, float Height, UINT SliceCount, UINT stackCount, MeshData& Mesh);
	static void BuildCylinderBottomCap(float BottomRadius, float TopRadius, float Height, UINT SliceCount, UINT stackCount, MeshData& Mesh);
	static XMFLOAT3 GetGridNormal(float x, float z);
	static float GetHillHeight(float x, float z);
	static float GetWatersHeight(float x, float z);
	static void Subdivide(MeshData& Mesh);
};