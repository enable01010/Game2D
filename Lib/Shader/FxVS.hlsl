// ------------------------
//
// FxVS.hlsl
// エフェクト用の頂点シェーダー
// １．プロパティで「構成」と「プラットフォーム」をすべてに変更
// ２．エントリポイント名をmainVSに変更
// ３．ShaderModel 5.0、シェーダーの種類が無ければ頂点シェーダーに変更
//
// ------------------------
#include"Fx_Header.hlsli"

BasicType main(VS_Input vs) {
	BasicType output;//ピクセルシェーダへ渡す値

	matrix wmat = mat[vs.InstanceId];

	// アニメーションフレームのUVを設定
	float2 flame[] = {// Vector2 と同じ
		float2(0.0f,0.0f),
		float2(0.25f,0.0f),
		float2(0.5f,0.0f),
		float2(0.75f,0.0f),

		float2(0.0f,0.333f),
		float2(0.25f,0.333f),
		float2(0.5f,0.333f),
		float2(0.75f,0.333f),

		float2(0.0f,0.666f),
		float2(0.25f,0.666f),
		float2(0.5f,0.666f),
		float2(0.75f,0.666f),
	};
	float anime = wmat[2][3];// 今のアニメのコマの数字

	wmat[2][3] = 0;

	output.svpos = mul(wmat, vs.pos);
	output.uv = vs.uv + flame[(int)anime];
	return output;
}
