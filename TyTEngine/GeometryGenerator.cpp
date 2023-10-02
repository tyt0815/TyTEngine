#include "pch.h"
#include "GeometryGenerator.h"

void OGeometryGenerator::CreateCube(float Width, float Height, float Depth, MeshData& Mesh)
{
	//
	// Create the vertices.
	//

	Vertex v[24];

	float w2 = 0.5f * Width;
	float h2 = 0.5f * Height;
	float d2 = 0.5f * Depth;

	// Fill in the front face vertex data.
	v[0] = Vertex(-w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[1] = Vertex(-w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[2] = Vertex(+w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	v[3] = Vertex(+w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

	// Fill in the back face vertex data.
	v[4] = Vertex(-w2, -h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	v[5] = Vertex(+w2, -h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[6] = Vertex(+w2, +h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[7] = Vertex(-w2, +h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	// Fill in the top face vertex data.
	v[8] = Vertex(-w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[9] = Vertex(-w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[10] = Vertex(+w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	v[11] = Vertex(+w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

	// Fill in the bottom face vertex data.
	v[12] = Vertex(-w2, -h2, -d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	v[13] = Vertex(+w2, -h2, -d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[14] = Vertex(+w2, -h2, +d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[15] = Vertex(-w2, -h2, +d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	// Fill in the left face vertex data.
	v[16] = Vertex(-w2, -h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[17] = Vertex(-w2, +h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[18] = Vertex(-w2, +h2, -d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[19] = Vertex(-w2, -h2, -d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	// Fill in the right face vertex data.
	v[20] = Vertex(+w2, -h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
	v[21] = Vertex(+w2, +h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	v[22] = Vertex(+w2, +h2, +d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
	v[23] = Vertex(+w2, -h2, +d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);

	Mesh.Vertices.assign(&v[0], &v[24]);

	//
	// Create the indices.
	//

	UINT i[36];

	// Fill in the front face index data
	i[0] = 0; i[1] = 1; i[2] = 2;
	i[3] = 0; i[4] = 2; i[5] = 3;

	// Fill in the back face index data
	i[6] = 4; i[7] = 5; i[8] = 6;
	i[9] = 4; i[10] = 6; i[11] = 7;

	// Fill in the top face index data
	i[12] = 8; i[13] = 9; i[14] = 10;
	i[15] = 8; i[16] = 10; i[17] = 11;

	// Fill in the bottom face index data
	i[18] = 12; i[19] = 13; i[20] = 14;
	i[21] = 12; i[22] = 14; i[23] = 15;

	// Fill in the left face index data
	i[24] = 16; i[25] = 17; i[26] = 18;
	i[27] = 16; i[28] = 18; i[29] = 19;

	// Fill in the right face index data
	i[30] = 20; i[31] = 21; i[32] = 22;
	i[33] = 20; i[34] = 22; i[35] = 23;

	Mesh.Indices.assign(&i[0], &i[36]);
}

void OGeometryGenerator::CreateGrid(float Width, float Depth, UINT m, UINT n, MeshData& Mesh)
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
	Mesh.Vertices.resize(VertexCount);
	for (UINT i = 0; i < m; ++i)
	{
		float z = HalfDepth - i * dz;
		for (UINT j = 0; j < n; ++j) 
		{
			float x = -HalfWidth + j * dx;
			Mesh.Vertices[i * n + j].Position = XMFLOAT3(x, 0.0f, z);
			// Ignore for now, used for lighting.
			Mesh.Vertices[i * n + j].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
			Mesh.Vertices[i * n + j].TangentU = XMFLOAT3(1.0f, 0.0f, 0.0f);
			// Ignore for now, used for texturing.
			Mesh.Vertices[i * n + j].TexC.x = j * du;
			Mesh.Vertices[i * n + j].TexC.y = i * dv;
		}
	}

	Mesh.Indices.resize(FaceCount * 3); // 3 indices per face
	// Iterate over each quad and compute indices.
	UINT k = 0;
	for (UINT i = 0; i < m - 1; ++i)
	{
		for (UINT j = 0; j < n - 1; ++j)
		{
			Mesh.Indices[k] = i * n + j;
			Mesh.Indices[k + 1] = i * n + j + 1;
			Mesh.Indices[k + 2] = (i + 1) * n + j;
			Mesh.Indices[k + 3] = (i + 1) * n + j;
			Mesh.Indices[k + 4] = i * n + j + 1;
			Mesh.Indices[k + 5] = (i + 1) * n + j + 1;
			k += 6; // next quad
		}
	}

}

void OGeometryGenerator::CreateCylinder(float BottomRadius, float TopRadius, float Height, UINT SliceCount, UINT StackCount, MeshData& Mesh)
{
	Mesh.Vertices.clear();
	Mesh.Indices.clear();
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
			Mesh.Vertices.push_back(vertex);
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
			Mesh.Indices.push_back(i * ringVertexCount + j);
			Mesh.Indices.push_back((i + 1) * ringVertexCount + j);
			Mesh.Indices.push_back((i + 1) * ringVertexCount + j + 1);
			Mesh.Indices.push_back(i * ringVertexCount + j);
			Mesh.Indices.push_back((i + 1) * ringVertexCount + j + 1);
			Mesh.Indices.push_back(i * ringVertexCount + j + 1);
		}
	}
	BuildCylinderTopCap(BottomRadius, TopRadius,
		Height, SliceCount, StackCount, Mesh);
	BuildCylinderBottomCap(BottomRadius, TopRadius,
		Height, SliceCount, StackCount, Mesh);
}

void OGeometryGenerator::CreateGeosphere(float Radius, UINT NumSubdivisions, MeshData& Mesh)
{
	// Put a cap on the number of subdivisions.
	NumSubdivisions = UMathHelper::Min(NumSubdivisions, 5u);
	// Approximate a sphere by tessellating(바둑판모양) an icosahedron(20면체). X^2 + Z^2 == 1
	const float X = 0.525731f;
	const float Z = 0.850651f;
	XMFLOAT3 IcosahedronVertices[12] =
	{
	XMFLOAT3(-X, 0.0f, Z), XMFLOAT3(X, 0.0f, Z),
	XMFLOAT3(-X, 0.0f, -Z), XMFLOAT3(X, 0.0f, -Z),
	XMFLOAT3(0.0f, Z, X), XMFLOAT3(0.0f, Z, -X),
	XMFLOAT3(0.0f, -Z, X), XMFLOAT3(0.0f, -Z, -X),
	XMFLOAT3(Z, X, 0.0f), XMFLOAT3(-Z, X, 0.0f),
	XMFLOAT3(Z, -X, 0.0f), XMFLOAT3(-Z, -X, 0.0f)
	};
	DWORD IcosahedronIndices[60] =
	{
	1,4,0, 4,9,0, 4,5,9, 8,5,4, 1,8,4,
	1,10,8, 10,3,8, 8,3,5, 3,2,5, 3,7,2,
	3,10,7, 10,6,7, 6,11,7, 6,0,11, 6,1,0,
	10,1,6, 11,0,9, 2,11,9, 5,2,9, 11,2,7
	};
	Mesh.Vertices.resize(12);
	Mesh.Indices.resize(60);
	for (size_t i = 0; i < 12; ++i)
		Mesh.Vertices[i].Position = IcosahedronVertices[i];
	for (size_t i = 0; i < 60; ++i)
		Mesh.Indices[i] = IcosahedronIndices[i];
	for (size_t i = 0; i < NumSubdivisions; ++i)
		Subdivide(Mesh);
	// Project vertices onto sphere and scale.
	for (size_t i = 0; i < Mesh.Vertices.size(); ++i)
	{
		// Project onto unit sphere.
		XMVECTOR NormVector = XMVector3Normalize(XMLoadFloat3(
			&Mesh.Vertices[i].Position));
		// Project onto sphere.
		XMVECTOR PositionVector = Radius * NormVector;
		XMStoreFloat3(&Mesh.Vertices[i].Position, PositionVector);
		XMStoreFloat3(&Mesh.Vertices[i].Normal, NormVector);
		// Derive texture coordinates from spherical coordinates.
		float Theta = UMathHelper::AngleFromXY(
			Mesh.Vertices[i].Position.x,
			Mesh.Vertices[i].Position.z);
		float Phi = acosf(Mesh.Vertices[i].Position.y / Radius);
		Mesh.Vertices[i].TexC.x = Theta / XM_2PI;
		Mesh.Vertices[i].TexC.y = Phi / XM_PI;
		// Partial derivative of P with respect to theta
		Mesh.Vertices[i].TangentU.x = -Radius * sinf(Phi) * sinf(Theta);
		Mesh.Vertices[i].TangentU.y = 0.0f;
		Mesh.Vertices[i].TangentU.z = +Radius * sinf(Phi) * cosf(Theta);
		XMVECTOR T = XMLoadFloat3(&Mesh.Vertices[i].TangentU);
		XMStoreFloat3(&Mesh.Vertices[i].TangentU,
			XMVector3Normalize(T));
	}
}

void OGeometryGenerator::BuildCylinderTopCap(float BottomRadius, float TopRadius, float Height, UINT SliceCount, UINT StackCount, MeshData& Mesh)
{
	UINT BaseIndex = (UINT)Mesh.Vertices.size();
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
		Mesh.Vertices.push_back(
			Vertex(x, y, z,
				0.0f, 1.0f, 0.0f,
				1.0f, 0.0f, 0.0f,
				u, v));
	}
	// Cap center vertex.
	Mesh.Vertices.push_back(
		Vertex(0.0f, y, 0.0f,
			0.0f, 1.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			0.5f, 0.5f));
	// Index of center vertex.
	UINT CenterIndex = (UINT)Mesh.Vertices.size() - 1;
	for (UINT i = 0; i < SliceCount; ++i)
	{
		Mesh.Indices.push_back(CenterIndex);
		Mesh.Indices.push_back(BaseIndex + i + 1);
		Mesh.Indices.push_back(BaseIndex + i);
	}

}

void OGeometryGenerator::BuildCylinderBottomCap(float BottomRadius, float TopRadius, float Height, UINT SliceCount, UINT StackCount, MeshData& Mesh)
{
	UINT BaseIndex = (UINT)Mesh.Vertices.size();
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
		Mesh.Vertices.push_back(
			Vertex(x, y, z,
				0.0f, -1.0f, 0.0f,
				1.0f, 0.0f, 0.0f,
				u, v));
	}
	// Cap center vertex.
	Mesh.Vertices.push_back(
		Vertex(0.0f, y, 0.0f,
			0.0f, -1.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			0.5f, 0.5f));
	// Index of center vertex.
	UINT CenterIndex = (UINT)Mesh.Vertices.size() - 1;
	for (UINT i = 0; i < SliceCount; ++i)
	{
		Mesh.Indices.push_back(CenterIndex);
		Mesh.Indices.push_back(BaseIndex + i);
		Mesh.Indices.push_back(BaseIndex + i + 1);
	}
}

void OGeometryGenerator::Subdivide(MeshData& Mesh)
{
	// Save a copy of the input geometry.
	MeshData InputCopy = Mesh;


	Mesh.Vertices.resize(0);
	Mesh.Indices.resize(0);

	//       v1
	//       *
	//      / \
	//     /   \
	//  m0*-----*m1
	//   / \   / \
	//  /   \ /   \
	// *-----*-----*
	// v0    m2     v2

	UINT NumTris = (UINT)InputCopy.Indices.size() / 3;
	for (UINT i = 0; i < NumTris; ++i)
	{
		Vertex v0 = InputCopy.Vertices[InputCopy.Indices[i * 3 + 0]];
		Vertex v1 = InputCopy.Vertices[InputCopy.Indices[i * 3 + 1]];
		Vertex v2 = InputCopy.Vertices[InputCopy.Indices[i * 3 + 2]];

		//
		// Generate the midpoints.
		//

		Vertex m0, m1, m2;

		// For subdivision, we just care about the position component.  We derive the other
		// vertex components in CreateGeosphere.

		m0.Position = XMFLOAT3(
			0.5f * (v0.Position.x + v1.Position.x),
			0.5f * (v0.Position.y + v1.Position.y),
			0.5f * (v0.Position.z + v1.Position.z));

		m1.Position = XMFLOAT3(
			0.5f * (v1.Position.x + v2.Position.x),
			0.5f * (v1.Position.y + v2.Position.y),
			0.5f * (v1.Position.z + v2.Position.z));

		m2.Position = XMFLOAT3(
			0.5f * (v0.Position.x + v2.Position.x),
			0.5f * (v0.Position.y + v2.Position.y),
			0.5f * (v0.Position.z + v2.Position.z));

		//
		// Add new geometry.
		//

		Mesh.Vertices.push_back(v0); // 0
		Mesh.Vertices.push_back(v1); // 1
		Mesh.Vertices.push_back(v2); // 2
		Mesh.Vertices.push_back(m0); // 3
		Mesh.Vertices.push_back(m1); // 4
		Mesh.Vertices.push_back(m2); // 5

		Mesh.Indices.push_back(i * 6 + 0);
		Mesh.Indices.push_back(i * 6 + 3);
		Mesh.Indices.push_back(i * 6 + 5);

		Mesh.Indices.push_back(i * 6 + 3);
		Mesh.Indices.push_back(i * 6 + 4);
		Mesh.Indices.push_back(i * 6 + 5);

		Mesh.Indices.push_back(i * 6 + 5);
		Mesh.Indices.push_back(i * 6 + 4);
		Mesh.Indices.push_back(i * 6 + 2);

		Mesh.Indices.push_back(i * 6 + 3);
		Mesh.Indices.push_back(i * 6 + 1);
		Mesh.Indices.push_back(i * 6 + 4);
	}
}