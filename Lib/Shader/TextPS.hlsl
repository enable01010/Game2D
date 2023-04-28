
struct BasicType {
	float4 svpos:SV_POSITION;//システム用頂点座標
	float2 uv:TEXCOORD;//UV値
};

Texture2D<float4> tex:register(t0);//0番スロットに設定されたテクスチャ
SamplerState smp:register(s0);//0番スロットに設定されたサンプラ
Buffer<uint> srcCommand : register(t2);

float4 main(BasicType input) : SV_TARGET{
	float2 texuv = input.uv ;
	float4 ret = float4(tex.Sample(smp, texuv));
	return ret;
}