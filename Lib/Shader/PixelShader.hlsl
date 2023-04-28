#include"Header.hlsli"
float4 main(BasicType input) : SV_TARGET{
	float2 texuv = input.uv + uv;
	float4 ret = float4(tex.Sample(smp, texuv));
	ret.r *= color.r;
	ret.g *= color.g;
	ret.b *= color.b;
	ret.a *= color.a;
	return ret;
}
