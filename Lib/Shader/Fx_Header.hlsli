// ------------------------
//
// Fx_Header.hlsli(最後にiがあるので注意。新規作成で「HLSLヘッダーファイル」を選択)
// エフェクト用のピクセルシェーダー
//
// ------------------------
Texture2D<float4> tex:register(t0);//0番スロットに設定されたテクスチャ
SamplerState smp:register(s0);//0番スロットに設定されたサンプラ


// 定数バッファ
cbuffer ConstData : register(b0)
{
	float4 color;
	matrix mat[100];// 弾の位置が入ってる
};

struct BasicType {
	float4 svpos:SV_POSITION;//システム用頂点座標
	float2 uv:TEXCOORD;//UV値
};

struct VS_Input
{
	float4 pos : POSITION;
	float2 uv : TEXCOORD;
	uint InstanceId : SV_InstanceID;// 
};
