cbuffer Constants : register(b0)
{
	float4x4 gWorldViewProj;
};

struct VertexIn
{
	float3 PosL : POSITION;
	float4 Color : COLOR;
};

struct VertexOut
{
	float4 PosH : SV_POSITION;
	float4 Color : COLOR;
};

VertexOut VSMain(VertexIn VIn)
{
	VertexOut VOut;
	//VOut.PosH = float4(VIn.PosL, 1.f);
	VOut.PosH = mul(float4(VIn.PosL, 1.f), gWorldViewProj);
	VOut.Color = VIn.Color;
	return VOut;
}