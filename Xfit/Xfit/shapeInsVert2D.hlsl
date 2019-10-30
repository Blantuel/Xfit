cbuffer MatrixBuffer {
	matrix mat;
	float2 centerPos;
	float2 reversed;
};

struct VS_INPUT {
	float2 pos : POSITION;
	matrix mat : MATRIX;
	float4 lineColor : LINECOLOR;
	float4 fillColor : FILLCOLOR;
	float lineWidth : LINEWIDTH;
};

struct VS_OUTPUT {
	float4 pos : SV_POSITION;
	float4 lineColor : LINECOLOR;
	float4 fillColor : FILLCOLOR;
	float lineWidth : LINEWIDTH;
	float2 centerPos : CENTERPOS;
};


VS_OUTPUT main(VS_INPUT _input)
{
	VS_OUTPUT output;

	output.pos = mul(float4(_input.pos, 0.f, 1.f), _input.mat);
	output.pos = mul(output.pos, mat);

	output.centerPos = mul(float4(centerPos, 0.f, 1.f), _input.mat).xy;
	output.centerPos = mul(float4(output.centerPos, 0.f, 1.f), mat).xy;

	output.lineColor = _input.lineColor;
	output.fillColor = _input.fillColor;
	output.lineWidth = _input.lineWidth;

	return output;
}

