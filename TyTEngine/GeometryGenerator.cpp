#include "pch.h"
#include "GeometryGenerator.h"

void OGeometryGenerator::CreateGrid(float Width, float Depth, UINT m, UINT n, MeshData& MeshData)
{
	UINT VertexCount = m * n;
	UINT FaceCount = (m - 1) * (n - 1) * 2;
	//
	// Create the vertices.
	//
	float HalfWidth = 0.5f * Width;
	float HalfDepth = 0.5f * Depth;
	float dx = Width / (n - 1);
	float dz = Depth / (m - 1);
	float du = 1.0f / (n - 1);
	float dv = 1.0f / (m - 1);
	MeshData.Vertices.resize(VertexCount);
	for (UINT i = 0; i < m; ++i)
	{
		float z = HalfDepth - i * dz;
		for (UINT j = 0; j < n; ++j) 
		{
			float x = -HalfWidth + j * dx;
			MeshData.Vertices[i * n + j].Position = XMFLOAT3(x, 0.0f, z);
			// Ignore for now, used for lighting.
			MeshData.Vertices[i * n + j].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
			MeshData.Vertices[i * n + j].TangentU = XMFLOAT3(1.0f, 0.0f, 0.0f);
			// Ignore for now, used for texturing.
			MeshData.Vertices[i * n + j].TexC.x = j * du;
			MeshData.Vertices[i * n + j].TexC.y = i * dv;
		}
	}

	MeshData.Indices.resize(FaceCount * 3); // 3 indices per face
	// Iterate over each quad and compute indices.
	UINT k = 0;
	for (UINT i = 0; i < m - 1; ++i)
	{
		for (UINT j = 0; j < n - 1; ++j)
		{
			MeshData.Indices[k] = i * n + j;
			MeshData.Indices[k + 1] = i * n + j + 1;
			MeshData.Indices[k + 2] = (i + 1) * n + j;
			MeshData.Indices[k + 3] = (i + 1) * n + j;
			MeshData.Indices[k + 4] = i * n + j + 1;
			MeshData.Indices[k + 5] = (i + 1) * n + j + 1;
			k += 6; // next quad
		}
	}

}

