//頂点シェーダ→ピクセルシェーダへのやり取りに使用する
//構造体
struct BasicType {
	float4 svpos:SV_POSITION;//システム用頂点座標
	float2 uv:TEXCOORD;//UV値
};

cbuffer ConstData : register(b0)
{
	float4 color;
	matrix mat;
	float2 uv;
};

Texture2D<float4> tex:register(t0);//0番スロットに設定されたテクスチャ
SamplerState smp:register(s0);//0番スロットに設定されたサンプラ
