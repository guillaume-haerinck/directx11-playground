// Get a pos2 return a pos4
float4 main(float2 pos : POSITION) : SV_POSITION
{
	return float4(pos, 0.0f, 1.0f);
}
