#include "Variables.hlsl"

// gtxtAlbedoTexture : gbuffer 0 ( base color + specint)
// gtxtSpecularTexture :  gbuffer 1 (nomral);
// gtxtNormalTexture :  gbuffer 2; (���ٰ� ���°ſ���)
// gtxtMetallicTexture :  gbuffer 4 (����);

// ���� gbuffer 4�� ���� �Ⱦ����� �ϴ� �� ������� �Ф�..
// ���⼭ gbuffer0�� HDRTex

groupshared float SharedPosition[1024]; // �����޸� �׷쿡 �߰��� ����

float DownScale4x4(uint2 CurPixel, uint groupThreadId)
{
    float avgLum = 0.0;

    // �ȼ� ���� ����
    if(CurPixel.y < Res.y)
    {
        // 4x4 �ȼ� ���� �׷� �� �ջ�
        int3 nFullResPos = int3(CurPixel * 4, 0);
        float4 downScaled = float4(0.0, 0.0, 0.0, 0.0);
        [unroll]
        for (int i = 0; i < 4; ++i)
        {
            [unroll]
            for (int j = 0; j < 4; ++j)
            {
                downScaled += gtxtAlbedoTexture.Load(nFullResPos, int2(j, i));
                
            }
        }
        downScaled /= 16.0;

        // �ȼ��� �ֵ��� ���
        avgLum = dot(downScaled, LUM_FACTOR);
        
        // �����޸𸮿� ��� ���
        SharedPosition[groupThreadId] = avgLum;
    }

    // ����ȭ �� ���� �ܰ��
    GroupMemoryBarrierWithGroupSync();

    return avgLum;
}

// float DownScale4x4(uint2 CurPixel, uint groupThreadId) ���� ���� ����
// 4���� ������ �ٿ� �������Ѵ�.
float DownScaled1024to4(uint dispatchThreadID, uint groupThreadID, float avgLum)
{
    // �ٿ� ������ �ڵ带 Ȯ��
    [unroll]
    for (uint groupSize = 4, step1 = 1, step2 = 2, step3 = 3;
        groupSize < 1024;
        groupSize *= 4, step1 *= 4, step2 *= 4, step3 *=3)
    {
        //�Ż� ���� ��� ����
        if(groupThreadID % groupSize == 0 )
        {
            //�ֵ��� ���
            float stepAvgLum = avgLum;
            stepAvgLum += dispatchThreadID + step1 < Domain ? SharedPosition[groupThreadID + step1] : avgLum;
            stepAvgLum += dispatchThreadID + step2 < Domain ? SharedPosition[groupThreadID + step2] : avgLum;
            stepAvgLum += dispatchThreadID + step3 < Domain ? SharedPosition[groupThreadID + step3] : avgLum;
            
            // ����� ����
            avgLum = stepAvgLum;
            SharedPosition[groupThreadID] = stepAvgLum;

        }

        // ����ȭ�� ��������
        GroupMemoryBarrierWithGroupSync();
    }
    
    return avgLum;
}

// �װ��� ���� �ϳ��� ��հ����� �ٿ������ �� �����Ѵ�.
void DownScaled4to1(uint dispatchThreadID, uint groupThreadID, uint groupID, float avgLum)
{
    if(groupThreadID == 0)
    {
        // ������ �׷쿡 ���� ��� �ֵ� �� ���
        float fFinalAvgLum = avgLum;
        
        fFinalAvgLum += dispatchThreadID + 256 < Domain ? SharedPosition[groupThreadID + 256] : avgLum;
        fFinalAvgLum += dispatchThreadID + 512 < Domain ? SharedPosition[groupThreadID + 512] : avgLum;
        fFinalAvgLum += dispatchThreadID + 768 < Domain ? SharedPosition[groupThreadID + 768] : avgLum;
        fFinalAvgLum /= 1024.0;

        // ���� ���� 1D UAV�� ��� ��
        // ���� ��������
        gAverageLum[groupID] = fFinalAvgLum;
    }
}


// ù��° ����
[numthreads(1024, 1, 1)]
void DownScaledFirstPass( uint3 groupID : SV_GroupID, uint3 dispatchThreadID : SV_DispatchThreadID , uint3 groupThreadID : SV_GroupThreadID)
{
    uint2 CurPixel = uint2(dispatchThreadID.x % Res.x, dispatchThreadID.x / Res.x);

    // 16 �ȼ� �޸� �׷��� �ϳ��� �ȼ��� �ٿ� ���� �޸𸮿� ����
    float avgLum = DownScale4x4(dispatchThreadID.x, groupThreadID.x);

    // 1024���� 4�� �ٿ� ������
    avgLum = DownScaled1024to4(dispatchThreadID.x, groupThreadID.x, avgLum);

    // 4���� 1�� �ٿ� ������
    DownScaled4to1(dispatchThreadID.x, groupThreadID.x, groupID.x, avgLum);
}


#define MAX_GROUPS 64
groupshared float ShardAvgFinal[MAX_GROUPS]; // �����޸� �׷쿡 �߰��� ����

// �ι�° ����
// �߰��� �ֵ� srv�� ��� �ֵ� uav�� ������ ���
[numthreads(MAX_GROUPS, 1, 1)]
void DownScaledSecondPass(uint3 groupID : SV_GroupID, uint3 dispatchThreadID : SV_DispatchThreadID, uint3 groupThreadID : SV_GroupThreadID)
{
    // ���� �޸𸮿� 1d�� ���� // 1D�� ����.. ���̵� �ƴϰ�..
    float avgLum = 0.0;

    if(dispatchThreadID.x < GroupSize)
    {
        avgLum = gAverageValues1D[dispatchThreadID.x];
    }
    ShardAvgFinal[dispatchThreadID.x] = avgLum;

    GroupMemoryBarrierWithGroupSync(); // ����ȭ �� ���� ��������

    // 64���� 16���� �ٿ� ������...
    if(dispatchThreadID.x % 4 == 0)
    {
        float stepAvgLum = avgLum;
        stepAvgLum += dispatchThreadID.x + 1 < GroupSize ? ShardAvgFinal[dispatchThreadID.x + 1] : avgLum;
        stepAvgLum += dispatchThreadID.x + 2 < GroupSize ? ShardAvgFinal[dispatchThreadID.x + 2] : avgLum;
        stepAvgLum += dispatchThreadID.x + 3 < GroupSize ? ShardAvgFinal[dispatchThreadID.x + 3] : avgLum;

        // ��� �� ����
        avgLum = stepAvgLum;
        ShardAvgFinal[dispatchThreadID.x] = stepAvgLum;
    }
    
    GroupMemoryBarrierWithGroupSync(); // ����ȭ �� ���� ��������

    // 16���� 4�� �ٿ� ������...
    if (dispatchThreadID.x % 16 == 0)
    {
        float stepAvgLum = avgLum;
        stepAvgLum += dispatchThreadID.x + 4 < GroupSize ? ShardAvgFinal[dispatchThreadID.x + 4] : avgLum;
        stepAvgLum += dispatchThreadID.x + 8 < GroupSize ? ShardAvgFinal[dispatchThreadID.x + 8] : avgLum;
        stepAvgLum += dispatchThreadID.x + 12 < GroupSize ? ShardAvgFinal[dispatchThreadID.x + 12] : avgLum;

        // ��� �� ����
        avgLum = stepAvgLum;
        ShardAvgFinal[dispatchThreadID.x] = stepAvgLum;
    }
    
    GroupMemoryBarrierWithGroupSync(); // ����ȭ �� ���� ��������

    // 4���� 1�� �ٿ� ������...
    if (dispatchThreadID.x == 0)
    {
        float stepAvgLum = avgLum;
        stepAvgLum += dispatchThreadID.x + 16 < GroupSize ? ShardAvgFinal[dispatchThreadID.x + 16] : avgLum;
        stepAvgLum += dispatchThreadID.x + 32 < GroupSize ? ShardAvgFinal[dispatchThreadID.x + 32] : avgLum;
        stepAvgLum += dispatchThreadID.x + 48 < GroupSize ? ShardAvgFinal[dispatchThreadID.x + 48] : avgLum;

        stepAvgLum /= 64.0;

        // ��� �� ����
        gAverageLum[0] = stepAvgLum;

    }
}
