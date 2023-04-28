// ------------------------
//
// FxVS.hlsl
// エフェクト用の頂点シェーダー
// １．プロパティで「構成」と「プラットフォーム」をすべてに変更
// ２．エントリポイント名をmainVSに変更
// ３．ShaderModel 5.0、シェーダーの種類が無ければ頂点シェーダーに変更
//
// ------------------------
#include"FxParticle_Header.hlsli"

BasicType main(VS_Input vs) {
	BasicType output;//ピクセルシェーダへ渡す値

	matrix wmat = data[vs.InstanceId].mat;
	output.uv.xy = vs.uv;// 
	output.uv.z = vs.InstanceId;

	// 位置情報
	wmat[2][3] = 0;// Z軸にアニメーションフレームを入れてるのでクリア
	output.svpos = mul(wmat, vs.pos);

	return output;
}
