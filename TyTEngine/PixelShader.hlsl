struct VertexOut
{
	float4 PosH : SV_POSITION;
	float4 Color : COLOR;
};

float4 PSMain(VertexOut VOut) : SV_TARGET
{
	return VOut.Color;
}