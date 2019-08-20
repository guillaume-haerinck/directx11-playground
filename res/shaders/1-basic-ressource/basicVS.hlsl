// Resource binding with constant buffer
cbuffer cbPerFrame : register(b0)
{
	float4x4 matVP;
	float4x4 matGeo;
};

// Expected input for this first stage of the pipeline
struct VSInput
{
	float3 Position : POSITION;
	float4 Color : COLOR;
};

// Ouput to the next shader in the pipeline
struct VSOutput
{
	float4 Position : SV_POSITION;
	float4 Color : COLOR;
};

VSOutput main(VSInput vin)
{
	VSOutput vout = (VSOutput) 0;
	vout.Position = mul(mul(float4(vin.Position, 1.0f), matGeo), matVP);
	vout.Color = vin.Color;

	return vout;
}
