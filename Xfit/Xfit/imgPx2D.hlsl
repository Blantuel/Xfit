interface iSample {
	float4 Sample(float2 _uv);
};

class LoadSample : iSample {
	float reversed;
	float4 Sample(float2 _uv);
};

class SamplerSample : iSample {
	float reversed;
	float4 Sample(float2 _uv);
};

cbuffer ColorMatrixBuffer {
	matrix colorMat;
	LoadSample loadSample;
	SamplerSample samplerSample;
	float2 reversed;
};


struct PS_INPUT {
	float4 pos : SV_POSITION;
	float2 uv : UV;
};

Texture2D tex;
SamplerState sam;


float4 LoadSample::Sample(float2 _uv) {
	uint width, height;
	tex.GetDimensions(width, height);

	return tex.Load(int3(_uv.x * width, _uv.y * height, 0));//0 is mipmapLevel
}
float4 SamplerSample::Sample(float2 _uv) {
	return tex.Sample(sam, _uv);
}

iSample isample;


float4 main(PS_INPUT _input) :SV_Target{
	return mul(isample.Sample(_input.uv), colorMat);
}
