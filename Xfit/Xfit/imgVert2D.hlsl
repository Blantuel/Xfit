cbuffer MatrixBuffer {
	matrix mat;
	matrix viewMat;
};

struct VS_INPUT
{
	float2 pos : POSITION;
	float2 uv : UV;
};

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float2 uv : UV;
};


VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output;

	output.pos = mul(float4(input.pos, 0.f, 1.f), mat);
	output.pos = mul(output.pos, viewMat);
	output.uv = input.uv;

	return output;
}

