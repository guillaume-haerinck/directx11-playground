// Resource binding with constant buffer
cbuffer perMesh : register(b0) {
	float4x4 matModel;
}

cbuffer perFrame : register(b1) {
	float4x4 matViewProj;
};

// Expected input for this first stage of the pipeline
// Set by input layout in the context
struct VSInput
{
	float3 Position : POSITION;
	float3 Normal : NORMAL;
	float2 TexCoord : TEXCOORD;
};

// Ouput to the next shader in the pipeline. Must have an SV_POSITION
struct VSOutput
{
	float4 Position : SV_POSITION;
};

VSOutput main(VSInput vin)
{
	VSOutput vout = (VSOutput) 0;
	vout.Position = mul(mul(float4(vin.Position, 1.0f), matModel), matViewProj);
	return vout;
}
