// Resource binding with constant buffer
cbuffer cbPerFrame : register(b0)
{
	float4x4 matVP;
	float4x4 matGeo;
};

// Expected input for this first stage of the pipeline
// Set by input layout in the context
struct VSInput
{
	float3 Position : POSITION;
};

// Ouput to the next shader in the pipeline. Must have an SV_POSITION
struct VSOutput
{
	float4 Position : SV_POSITION;
};

VSOutput main(VSInput vin)
{
	VSOutput vout = (VSOutput) 0;
	vout.Position = mul(mul(float4(vin.Position, 1.0f), matGeo), matVP);
	return vout;
}
