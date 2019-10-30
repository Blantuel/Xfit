cbuffer MatrixBuffer {
	matrix viewMat;
};


struct GS_INPUT {
	float4 pos : SV_POSITION;
	float4 lineColor : LINECOLOR;
	float4 fillColor : FILLCOLOR;
	float lineWidth : LINEWIDTH;
	float2 centerPos : CENTERPOS;
};


struct GS_OUTPUT {
	float4 pos : SV_POSITION;
	float4 color : COLOR;
};


[maxvertexcount(7)]
void main(line GS_INPUT _input[2], inout TriangleStream<GS_OUTPUT> _output) {
	GS_OUTPUT output;

	if (_input[0].lineWidth > 0.f) {
		output.color = _input[0].lineColor;

		const float2 dis = float2(_input[0].pos.y - _input[1].pos.y, _input[1].pos.x - _input[0].pos.x) * (_input[0].lineWidth / distance(_input[0].pos.xy, _input[1].pos.xy));

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

		output.color = _input[0].fillColor;

		output.pos = point1;
		_output.Append(output);

		output.pos = point2;
		_output.Append(output);

		output.pos = float4(_input[0].centerPos, 0.f, 1.f);
		output.pos = mul(output.pos, viewMat);
		_output.Append(output);
	} else {
		output.color = _input[0].fillColor;

		output.pos = _input[0].pos;
		output.pos = mul(output.pos, viewMat);
		_output.Append(output);

		output.pos = _input[1].pos;
		output.pos = mul(output.pos, viewMat);
		_output.Append(output);

		output.pos = float4(_input[0].centerPos, 0.f, 1.f);
		output.pos = mul(output.pos, viewMat);
		_output.Append(output);
	}
}