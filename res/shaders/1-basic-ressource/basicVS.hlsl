// Resource binding with constant buffer
cbuffer cbPerFrame : register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
	float4 ColorTest : COLOR_TEST;
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
	vout.Position = float4(vin.Position, 1.0f);
	//vout.Position = mul(vin.Position, World);
	//vout.Position = mul(vout.Position, View);
	//vout.Position = mul(vout.Position, Projection);
	vout.Color = ColorTest;

	return vout;
}
