struct PS_INPUT {
	float4 pos : SV_POSITION;
	float4 color : COLOR;
};

float4 main(PS_INPUT _input) :SV_Target{
	return _input.color;
}
