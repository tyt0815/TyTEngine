struct VS_Output
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

float4 ps_main(VS_Output input) : SV_TARGET
{
    return input.color;
}
