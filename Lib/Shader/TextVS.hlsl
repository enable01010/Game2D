
struct BasicType {
	float4 svpos:SV_POSITION;//�V�X�e���p���_���W
	float2 uv:TEXCOORD;//UV�l
};


BasicType main(float4 pos : POSITION, float2 uv : TEXCOORD) {
	BasicType output;//�s�N�Z���V�F�[�_�֓n���l
	output.svpos = pos;
	output.uv = uv;
	return output;
}
