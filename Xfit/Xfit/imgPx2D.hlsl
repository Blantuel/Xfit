cbuffer ColorMatrixBuffer {
	matrix colorMat;
};


struct PS_INPUT {
	float4 pos : SV_POSITION;
	float2 uv : UV;
};

Texture2D tex;
SamplerState sam;

float4 main(PS_INPUT _input) :SV_Target{
	return mul(tex.Sample(sam, _input.uv), colorMat);
}
