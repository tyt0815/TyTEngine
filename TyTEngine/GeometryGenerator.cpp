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

void OGeometryGenerator::CreateCylinder(float BottomRadius, float TopRadius, float Height, UINT SliceCount, UINT StackCount, MeshData& MeshData)
{
	MeshData.Vertices.clear();
	MeshData.Indices.clear();
	//
	// Build Stacks.
	//
	float StackHeight = Height / StackCount;
	// Amount to increment radius as we move up each stack
	// level from bottom to top.
	float RadiusStep = (TopRadius - BottomRadius) / StackCount;
	UINT RingCount = StackCount + 1;
	// Compute vertices for each stack ring starting at
	// the bottom and moving up.
	for (UINT i = 0; i < RingCount; ++i)
	{
		float y = -0.5f * Height + i * StackHeight;
		float r = BottomRadius + i * RadiusStep;
		// vertices of ring
		float dTheta = 2.0f * XM_PI / SliceCount;
		for (UINT j = 0; j <= SliceCount; ++j)
		{
			Vertex vertex;
			float c = cosf(j * dTheta);
			float s = sinf(j * dTheta);
			vertex.Position = XMFLOAT3(r * c, y, r * s);
			vertex.TexC.x = (float)j / SliceCount;
			vertex.TexC.y = 1.0f - (float)i / StackCount;
			// Cylinder can be parameterized as follows, where we
			// introduce v parameter that goes in the same direction
			// as the v tex-coord so that the bitangent goes in the
			// same direction as the v tex-coord.
			// Let r0 be the bottom radius and let r1 be the
			// top radius.
			// y(v) = h - hv for v in [0,1].
			// r(v) = r1 + (r0-r1)v
			//
			// x(t, v) = r(v)*cos(t)
			// y(t, v) = h - hv
			// z(t, v) = r(v)*sin(t)
			//
			// dx/dt = -r(v)*sin(t)
			// dy/dt = 0
			// dz/dt = +r(v)*cos(t)
			//
			// dx/dv = (r0-r1)*cos(t)
			// dy/dv = -h
			// dz/dv = (r0-r1)*sin(t)
			// TangentU us unit length.
			vertex.TangentU = XMFLOAT3(-s, 0.0f, c);
			float dr = BottomRadius - TopRadius;
			XMFLOAT3 bitangent(dr * c, -Height, dr * s);
			XMVECTOR T = XMLoadFloat3(&vertex.TangentU);
			XMVECTOR B = XMLoadFloat3(&bitangent);
			XMVECTOR N = XMVector3Normalize(XMVector3Cross(T, B));
			XMStoreFloat3(&vertex.Normal, N);
			MeshData.Vertices.push_back(vertex);
		}
	}
	// Add one because we duplicate the first and last vertex per ring
	// since the texture coordinates are different.
	UINT ringVertexCount = SliceCount + 1;
	// Compute indices for each stack.
	for (UINT i = 0; i < StackCount; ++i)
	{
		for (UINT j = 0; j < SliceCount; ++j)
		{
			MeshData.Indices.push_back(i * ringVertexCount + j);
			MeshData.Indices.push_back((i + 1) * ringVertexCount + j);
			MeshData.Indices.push_back((i + 1) * ringVertexCount + j + 1);
			MeshData.Indices.push_back(i * ringVertexCount + j);
			MeshData.Indices.push_back((i + 1) * ringVertexCount + j + 1);
			MeshData.Indices.push_back(i * ringVertexCount + j + 1);
		}
	}
	BuildCylinderTopCap(BottomRadius, TopRadius,
		Height, SliceCount, StackCount, MeshData);
	BuildCylinderBottomCap(BottomRadius, TopRadius,
		Height, SliceCount, StackCount, MeshData);
}

void OGeometryGenerator::BuildCylinderTopCap(float BottomRadius, float TopRadius, float Height, UINT SliceCount, UINT StackCount, MeshData& MeshData)
{
	UINT BaseIndex = (UINT)MeshData.Vertices.size();
	float y = 0.5f * Height;
	float dTheta = 2.0f * XM_PI / SliceCount;
	// Duplicate cap ring vertices because the texture coordinates
	// and normals differ.
	for (UINT i = 0; i <= SliceCount; ++i)
	{
		float x = TopRadius * cosf(i * dTheta);
		float z = TopRadius * sinf(i * dTheta);
		// Scale down by the height to try and make top cap
		// texture coord area proportional to base.
		float u = x / Height + 0.5f;
		float v = z / Height + 0.5f;
		MeshData.Vertices.push_back(
			Vertex(x, y, z,
				0.0f, 1.0f, 0.0f,
				1.0f, 0.0f, 0.0f,
				u, v));
	}
	// Cap center vertex.
	MeshData.Vertices.push_back(
		Vertex(0.0f, y, 0.0f,
			0.0f, 1.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			0.5f, 0.5f));
	// Index of center vertex.
	UINT CenterIndex = (UINT)MeshData.Vertices.size() - 1;
	for (UINT i = 0; i < SliceCount; ++i)
	{
		MeshData.Indices.push_back(CenterIndex);
		MeshData.Indices.push_back(BaseIndex + i + 1);
		MeshData.Indices.push_back(BaseIndex + i);
	}

}

void OGeometryGenerator::BuildCylinderBottomCap(float BottomRadius, float TopRadius, float Height, UINT SliceCount, UINT StackCount, MeshData& MeshData)
{
	UINT BaseIndex = (UINT)MeshData.Vertices.size();
	float y = -0.5f * Height;
	float dTheta = 2.0f * XM_PI / SliceCount;
	// Duplicate cap ring vertices because the texture coordinates
	// and normals differ.
	for (UINT i = 0; i <= SliceCount; ++i)
	{
		float x = TopRadius * cosf(i * dTheta);
		float z = TopRadius * sinf(i * dTheta);
		// Scale down by the height to try and make top cap
		// texture coord area proportional to base.
		float u = x / Height + 0.5f;
		float v = z / Height + 0.5f;
		MeshData.Vertices.push_back(
			Vertex(x, y, z,
				0.0f, 1.0f, 0.0f,
				1.0f, 0.0f, 0.0f,
				u, v));
	}
	// Cap center vertex.
	MeshData.Vertices.push_back(
		Vertex(0.0f, y, 0.0f,
			0.0f, 1.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			0.5f, 0.5f));
	// Index of center vertex.
	UINT CenterIndex = (UINT)MeshData.Vertices.size() - 1;
	for (UINT i = 0; i < SliceCount; ++i)
	{
		MeshData.Indices.push_back(CenterIndex);
		MeshData.Indices.push_back(BaseIndex + i);
		MeshData.Indices.push_back(BaseIndex + i + 1);
	}
}