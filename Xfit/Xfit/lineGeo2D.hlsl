
cbuffer MatrixBuffer {
	matrix viewMat;
	float lineWidth;
	float3 reversed;
};


struct GS_INPUT
{
	float4 pos : SV_POSITION;
};


struct GS_OUTPUT
{
	float4 pos : SV_POSITION;
};


[maxvertexcount(4)]
void main(line GS_INPUT _input[2], inout TriangleStream<GS_OUTPUT> _output) {
	GS_OUTPUT output;

	const float2 dis = float2(_input[0].pos.y - _input[1].pos.y, _input[1].pos.x - _input[0].pos.x) * ((lineWidth/2.f) / distance(_input[0].pos.xy, _input[1].pos.xy));

	output.pos.zw = float2(0.f, 1.f);

	output.pos.xy = _input[0].pos.xy + dis;
	output.pos = mul(output.pos, viewMat);
	_output.Append(output);

	output.pos.xy = _input[0].pos.xy - dis;
	output.pos = mul(output.pos, viewMat);
	_output.Append(output);

	output.pos.xy = _input[1].pos.xy + dis;
	output.pos = mul(output.pos, viewMat);
	_output.Append(output);

	output.pos.xy = _input[1].pos.xy - dis;
	output.pos = mul(output.pos, viewMat);
	_output.Append(output);
}