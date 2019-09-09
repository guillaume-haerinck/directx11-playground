Texture2D tex : register(t0);
SamplerState samplr : register(s0);

struct PSInput {
	float4 Position : SV_POSITION;
	float2 TexCoord : TEXCOORD;
};

float4 main(PSInput pin) : SV_TARGET
{
	return tex.Sample(samplr, pin.TexCoord);
}