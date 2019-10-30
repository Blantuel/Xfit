cbuffer MatrixBuffer {
	matrix mat;
};

struct VS_INPUT
{
	float2 pos : POSITION;
};

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
};


VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output;

	output.pos = mul(float4(input.pos, 0.f, 1.f), mat);

	return output;
}

