#include "Variables.hlsl"

[numthreads(32, 32, 1)] // �ϳ��� ������ �׷쿡�� ����� ������ ������ ����
void CS(uint3 DTid : SV_DispatchThreadID)
{
    gtxtRWOutput[DTid.xy] = gtxtTexture[DTid.xy] + gtxtTerrainBaseTexture[DTid.xy];
	// gtxtRWOutput[DTid.xy] = float4(1.f, 1.f, 1.f, 1.f);
	
	// GroupMemoryBarrierWithGroupSync();
	// �ϳ��� ������ �׷��� ��� �׷��� ���� ���� ���Ⱑ ���������� �׸���
	// �� �׷��� ��� �����尡 �� �Լ��� ȣ���Ҵ���� ������ ���� ���߰� ��ٸ�.

}


#define CacheSize (256 + 2*5)
groupshared float4 gCache[CacheSize];
// �����忡�� �����޸� ,  SV_GroupIndex�� �ε����Ҽ��ִ�.
// �׷��� �� �����尡 �ϳ��� ������ ����� �� �ִ�.



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