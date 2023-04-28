
struct BasicType {
	float4 svpos:SV_POSITION;//システム用頂点座標
	float2 uv:TEXCOORD;//UV値
};


BasicType main(float4 pos : POSITION, float2 uv : TEXCOORD) {
	BasicType output;//ピクセルシェーダへ渡す値
	output.svpos = pos;
	output.uv = uv;
	return output;
}
