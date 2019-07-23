#include "Variables.hlsl"

[numthreads(32, 32, 1)] // 하나의 스레드 그룹에서 실행될 스레드 구조와 개수
void CS(uint3 DTid : SV_DispatchThreadID)
{
    gtxtRWOutput[DTid.xy] = gtxtTexture[DTid.xy] + gtxtTerrainBaseTexture[DTid.xy];
	// gtxtRWOutput[DTid.xy] = float4(1.f, 1.f, 1.f, 1.f);
	
	// GroupMemoryBarrierWithGroupSync();
	// 하나의 스레드 그룹의 모든 그룹의 공유 접근 쓰기가 끝날때까지 그리고
	// 그 그룹의 모든 스레드가 이 함수를 호출할대까지 스레드 실행 멈추고 기다림.

}


#define CacheSize (256 + 2*5)
groupshared float4 gCache[CacheSize];
// 스레드에서 공유메모리 ,  SV_GroupIndex로 인덱싱할수있다.
// 그룹의 각 스레드가 하나의 슬롯을 사용할 수 있다.



[numthreads(256, 1, 1)]
void HorzCS(int3 groupThreadID : SV_GroupThreadID,
	int3 dispatchThreadID : SV_DispatchThreadID)
{
    float weights[11] = { 0.05f, 0.05f, 0.1f, 0.1f, 0.1f, 0.2f, 0.1f, 0.1f, 0.1f, 0.05f, 0.05f };
	 
    if (groupThreadID.x < 5)
    {
        int x = max(dispatchThreadID.x - 5, 0);
        gCache[groupThreadID.x] = gtxtTexture[int2(x, dispatchThreadID.y)];
    }
    if (groupThreadID.x >= 256 - 5)
    {
        int x = min(dispatchThreadID.x + 5, gtxtTexture.Length.x - 1);
        gCache[groupThreadID.x + 2 * 5] = gtxtTexture[int2(x, dispatchThreadID.y)];
    }
	 
    gCache[groupThreadID.x + 5] = gtxtTexture[min(dispatchThreadID.xy, gtxtTexture.Length.xy - 1)];
	 
    GroupMemoryBarrierWithGroupSync();
	 
    float4 blurColor = float4(0, 0, 0, 0);

    for (int i = -5; i <= 5; ++i)
    {
        int k = groupThreadID.x + 5 + i;

        blurColor += weights[i + 5] * gCache[k];
    }

    gtxtRWOutput[dispatchThreadID.xy] = blurColor;
}

[numthreads(1, 256, 1)]
void VertCS(int3 groupThreadID : SV_GroupThreadID,
	int3 dispatchThreadID : SV_DispatchThreadID)
{
    float weights[11] = { 0.05f, 0.05f, 0.1f, 0.1f, 0.1f, 0.2f, 0.1f, 0.1f, 0.1f, 0.05f, 0.05f };
	 
    if (groupThreadID.y < 5)
    {
        int y = max(dispatchThreadID.y - 5, 0);
        gCache[groupThreadID.y] = gtxtTexture[int2(dispatchThreadID.x, y)];
    }
    if (groupThreadID.y >= 256 - 5)
    {
        int y = min(dispatchThreadID.y + 5, gtxtTexture.Length.y - 1);
        gCache[groupThreadID.y + 2 * 5] = gtxtTexture[int2(dispatchThreadID.x, y)];
    }

    gCache[groupThreadID.y + 5] = gtxtTexture[min(dispatchThreadID.xy, gtxtTexture.Length.xy - 1)];


    GroupMemoryBarrierWithGroupSync();


    float4 blurColor = float4(0, 0, 0, 0);

    for (int i = -5; i <= 5; ++i)
    {
        int k = groupThreadID.y + 5 + i;

        blurColor += weights[i + 5] * gCache[k];
    }

    gtxtRWOutput[dispatchThreadID.xy] = blurColor;
}