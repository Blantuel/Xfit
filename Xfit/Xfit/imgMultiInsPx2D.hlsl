struct PS_INPUT {
	float4 pos : SV_POSITION;
	float2 uv : UV;
	matrix colorMat : COLORMATRIX;
	int imgIndex : IMGINDEX;
};

Texture2DArray tex;
SamplerState sam;

float4 main(PS_INPUT _input) :SV_Target{
	return mul(tex.Sample(sam, float3(_input.uv, float(_input.imgIndex))), _input.colorMat);
}
