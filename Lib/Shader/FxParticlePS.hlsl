// ------------------------
//
// FxPS.hlsl
// エフェクト用のピクセルシェーダー
// 設定はVS参考に。
//
// ------------------------
#include"FxParticle_Header.hlsli"

float4 main(BasicType input) : SV_TARGET{
	float2 texuv = input.uv;

	float4 ret = float4(tex.Sample(smp, texuv)) * data[input.uv.z].color;
	return ret;
}