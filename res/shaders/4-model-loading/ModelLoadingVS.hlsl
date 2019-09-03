float4 main(float3 pos : POSITION) : SV_POSITION {
	return float4(pos.x, pos.y, 0.0f, 1.0f);
}
