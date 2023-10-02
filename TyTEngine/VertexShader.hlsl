
cbuffer PerObject : register(b1)
{
	float4x4 gWorld;
	float4x4 gWorldInverseTranspose;
	float4x4 gWorldViewProj;
};

struct VertexIn
{
	float3 PosL : POSITION;
	float3 NormalL : NORMAL;
	float4 Color : COLOR;
};

struct VertexOut
{
	float4 PosH : SV_POSITION;
	float3 PosW : POSITION;
	float3 NormalW : NORMAL;
	float4 Color : COLOR;
};

VertexOut VSMain(VertexIn VIn)
{
	VertexOut VOut;
	VOut.PosH = mul(float4(VIn.PosL, 1.f), gWorldViewProj);
	VOut.PosW = mul(float4(VIn.PosL, 1.f), gWorld).xyz;
	VOut.NormalW = mul(float4(VIn.NormalL, 0.f), gWorldInverseTranspose).xyz;
	VOut.Color = VIn.Color;
	return VOut;
}