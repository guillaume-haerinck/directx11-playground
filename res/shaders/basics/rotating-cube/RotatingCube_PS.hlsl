cbuffer cbPerFrame : register(b0)
{
	float4 faceColor[6];
};

// Expected input for this last shader in the pipeline
struct PSInput
{
	float4 Position : SV_POSITION;
	uint tid : SV_PrimitiveID;
};

float4 main(PSInput pin) : SV_TARGET
{
	return faceColor[pin.tid / 2];
}