cbuffer MatrixBuffer {
	matrix mat;
};

struct VS_INPUT {
	float2 pos : POSITION;
	matrix mat : MATRIX;
	float4 lineColor : LINECOLOR;
	float lineWidth : LINEWIDTH;
};

struct VS_OUTPUT {
	float4 pos : SV_POSITION;
	float4 lineColor : LINECOLOR;
	float lineWidth : LINEWIDTH;
};


VS_OUTPUT main(VS_INPUT _input)
{
	VS_OUTPUT output;

	output.pos = mul(float4(_input.pos, 0.f, 1.f), _input.mat);
	output.pos = mul(output.pos, mat);

	output.lineWidth = _input.lineWidth;
	output.lineColor = _input.lineColor;

	return output;
}

