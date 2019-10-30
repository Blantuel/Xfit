
cbuffer MatrixBuffer {
	matrix viewMat;
	float4 fillColor;
	float4 lineColor;
	float lineWidth;
	float2 centerPos;
	float reversed;
};


struct GS_INPUT
{
	float4 pos : SV_POSITION;
};


struct GS_OUTPUT
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
};


[maxvertexcount(7)]
void main(line GS_INPUT _input[2], inout TriangleStream<GS_OUTPUT> _output) {
	GS_OUTPUT output;

	if (lineWidth > 0.f) {
		output.color = lineColor;

		const float2 dis = float2(_input[0].pos.y - _input[1].pos.y, _input[1].pos.x - _input[0].pos.x) * (lineWidth / distance(_input[0].pos.xy, _input[1].pos.xy));

		output.pos.zw = float2(0.f, 1.f);

		output.pos.xy = _input[0].pos.xy;
		output.pos = mul(output.pos, viewMat);
		const float4 point1 = output.pos;
		_output.Append(output);

		output.pos.xy = _input[0].pos.xy + dis;
		output.pos = mul(output.pos, viewMat);
		_output.Append(output);

		output.pos.xy = _input[1].pos.xy;
		output.pos = mul(output.pos, viewMat);
		const float4 point2 = output.pos;
		_output.Append(output);

		output.pos.xy = _input[1].pos.xy + dis;
		output.pos = mul(output.pos, viewMat);
		_output.Append(output);

		_output.RestartStrip();

		output.color = fillColor;

		output.pos = point1;
		_output.Append(output);

		output.pos = point2;
		_output.Append(output);

		output.pos = float4(centerPos,0.f,1.f);
		output.pos = mul(output.pos, viewMat);
		_output.Append(output);
	} else {
		output.color = fillColor;

		output.pos = _input[0].pos;
		output.pos = mul(output.pos, viewMat);
		_output.Append(output);

		output.pos = _input[1].pos;
		output.pos = mul(output.pos, viewMat);
		_output.Append(output);

		output.pos = float4(centerPos, 0.f, 1.f);
		output.pos = mul(output.pos, viewMat);
		_output.Append(output);
	}

}