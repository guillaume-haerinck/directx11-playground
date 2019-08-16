// Expected input for this last shader in the pipeline
struct PSInput
{
	float4 Position : SV_POSITION;
	float4 Color : COLOR;
	float2 UV : TEXCOORD;
};

// Ressource binding
Texture2D tex : register(t0);
SamplerState smp : register(s0);

float4 main(PSInput pin) : SV_TARGET
{
	return pin.Color * tex.Sample(smp, pin.UV);
}