cbuffer MatrixBuffer {
	matrix mat;
	matrix viewMat;
};

struct VS_INPUT {
	float2 pos : POSITION;
	float2 uv : UV;
	matrix mat : MATRIX;
	matrix colorMat : COLORMATRIX;
	int imgIndex : IMGINDEX;
};

struct VS_OUTPUT {
	float4 pos : SV_POSITION;
	float2 uv : UV;
	matrix colorMat : COLORMATRIX;
	int imgIndex : IMGINDEX;
};


VS_OUTPUT main(VS_INPUT _input) {
	VS_OUTPUT output;

	output.pos = mul(float4(_input.pos, 0.f, 1.f), _input.mat);
	output.pos = mul(output.pos, mat);
	output.pos = mul(output.pos, viewMat);

	output.uv = _input.uv;
	output.colorMat = _input.colorMat;
	output.imgIndex = _input.imgIndex;

	return output;
}

