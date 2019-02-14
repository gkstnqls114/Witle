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
////정점 셰이더를 정의한다.
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
////픽셀 셰이더를 정의한다.
//float4 PSMain(PS_INPUT input : SV_POSITION) : SV_TARGET
//{
//	//프리미티브의 모든 픽셀의 색상을 노란색으로 반환한다.
//	// float4 cColor = gtxtTexture.Sample(gSamplerState, input.uv);
//	float4 cColor = float4(1.f, 1.f, 0.f, 0.f);
//	return(cColor);
//}
//정점 셰이더를 정의한다.
float4 VSMain(uint nVertexID : SV_VertexID) : SV_POSITION
{
	float4 output;
//프리미티브(삼각형)를 구성하는 정점의 인덱스(SV_VertexID)에 따라 정점을 반환한다.
//정점의 위치 좌표는 변환이 된 좌표(SV_POSITION)이다. 즉, 투영좌표계의 좌표이다.
if (nVertexID == 0)      output = float4(-1.0,  1.0,  0.5, 1.0);
else if (nVertexID == 1) output = float4(1.0,   1.0,  0.5, 1.0);
else if (nVertexID == 2) output = float4(-1.0, -1.0,  0.5, 1.0);
else if (nVertexID == 3) output = float4(-1.0, -1.0,  0.5, 1.0);
else if (nVertexID == 4) output = float4(1.0,   1.0,  0.5, 1.0);
else if (nVertexID == 5) output = float4(1.0,  -1.0,  0.5, 1.0);

return(output);
}

//픽셀 셰이더를 정의한다.
float4 PSMain(float4 input : SV_POSITION) : SV_TARGET
{
	//프리미티브의 모든 픽셀의 색상을 노란색으로 반환한다.
	return(float4(1.0f, 1.0f, 0.0f, 1.0f)); //Yellow
}