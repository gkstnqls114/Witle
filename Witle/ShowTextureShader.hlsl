//
//Texture2D gtxtTexture : register(t0);
//
//SamplerState gSamplerState : register(s0);
//
//
//struct PS_INPUT
//{
//	float4 pos : SV_POSITION;
//	float2 uv : TEXTURE;
//};
//
////���� ���̴��� �����Ѵ�.
//PS_INPUT VSMain(uint nVertexID : SV_VertexID) : SV_POSITION
//{
//	PS_INPUT output;
//
//
//	if (nVertexID == 0) {
//		output.pos = float4(-1.0f, 1.0f, 1.0f, 1.0f);
//		output.uv = float2(0.f, 0.f);
//	}
//	else if (nVertexID == 1) {
//		output.pos = float4(1.0f, 1.0f, 1.0f, 1.0f);
//		output.uv = float2(1.f, 0.f);
//	}
//	else if (nVertexID == 2) {
//		output.pos = float4(-1.0f, -1.0f, 1.0f, 1.0f);
//		output.uv = float2(0.f, 1.f);
//	}
//	else if (nVertexID == 3) {
//		output.pos = float4(-1.0f, -1.0f, 1.0f, 1.0f);
//		output.uv = float2(0.f, 1.f);
//	}
//	else if (nVertexID == 4) {
//		output.pos = float4(1.0f, 1.0f, 1.0f, 1.0f);
//		output.uv = float2(1.f, 0.f);
//	}
//	else if (nVertexID == 5) {
//		output.pos = float4(1.0f, -1.0f, 1.0f, 1.0f);
//		output.uv = float2(1.f, 1.f);
//	}
//
//	return(output);
//}
//
////�ȼ� ���̴��� �����Ѵ�.
//float4 PSMain(PS_INPUT input : SV_POSITION) : SV_TARGET
//{
//	//������Ƽ���� ��� �ȼ��� ������ ��������� ��ȯ�Ѵ�.
//	// float4 cColor = gtxtTexture.Sample(gSamplerState, input.uv);
//	float4 cColor = float4(1.f, 1.f, 0.f, 0.f);
//	return(cColor);
//}
//���� ���̴��� �����Ѵ�.
float4 VSMain(uint nVertexID : SV_VertexID) : SV_POSITION
{
	float4 output;
//������Ƽ��(�ﰢ��)�� �����ϴ� ������ �ε���(SV_VertexID)�� ���� ������ ��ȯ�Ѵ�.
//������ ��ġ ��ǥ�� ��ȯ�� �� ��ǥ(SV_POSITION)�̴�. ��, ������ǥ���� ��ǥ�̴�.
if (nVertexID == 0)      output = float4(-1.0,  1.0,  0.5, 1.0);
else if (nVertexID == 1) output = float4(1.0,   1.0,  0.5, 1.0);
else if (nVertexID == 2) output = float4(-1.0, -1.0,  0.5, 1.0);
else if (nVertexID == 3) output = float4(-1.0, -1.0,  0.5, 1.0);
else if (nVertexID == 4) output = float4(1.0,   1.0,  0.5, 1.0);
else if (nVertexID == 5) output = float4(1.0,  -1.0,  0.5, 1.0);

return(output);
}

//�ȼ� ���̴��� �����Ѵ�.
float4 PSMain(float4 input : SV_POSITION) : SV_TARGET
{
	//������Ƽ���� ��� �ȼ��� ������ ��������� ��ȯ�Ѵ�.
	return(float4(1.0f, 1.0f, 0.0f, 1.0f)); //Yellow
}