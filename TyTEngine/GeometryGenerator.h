#pragma once

class OGeometryGenerator
{
public:
	struct Vertex
	{
		Vertex() {};
		Vertex(const XMFLOAT3& p,
			const XMFLOAT3& n,
			const XMFLOAT3& t,
			const XMFLOAT2& uv)
			: Position(p), Normal(n), TangentU(t), TexC(uv) {}
		Vertex(
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
		std::vector<Vertex> Vertices;
		std::vector<UINT> Indices;
	};

	static void CreateGrid(float Width, float Depth, UINT m, UINT n, MeshData& MeshData);
	static void CreateCylinder(float BottomRadius, float TopRadius, float Height, UINT SliceCount, UINT StackCount, MeshData& MeshData);

private:
	static void BuildCylinderTopCap(float BottomRadius, float TopRadius, float Height, UINT SliceCount, UINT stackCount, MeshData& MeshData);
	static void BuildCylinderBottomCap(float BottomRadius, float TopRadius, float Height, UINT SliceCount, UINT stackCount, MeshData& MeshData);
};