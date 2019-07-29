#include "Variables.hlsl"

// gtxtAlbedoTexture : gbuffer 0 ( base color + specint)
// gtxtSpecularTexture :  gbuffer 1 (nomral);
// gtxtNormalTexture :  gbuffer 2; (어디다가 쓰는거엿지)
// gtxtMetallicTexture :  gbuffer 4 (깊이);

// 현재 gbuffer 4개 전부 안쓰지만 일단 그 대용으로 ㅠㅠ..
// 여기서 gbuffer0이 HDRTex

groupshared float SharedPosition[1024]; // 공유메모리 그룹에 중간값 저장

float DownScale4x4(uint2 CurPixel, uint groupThreadId)
{
    float avgLum = 0.0;

    // 픽셀 결합 생략
    if(CurPixel.y < Res.y)
    {
        // 4x4 픽셀 결합 그룹 값 합산
        int3 nFullResPos = int3(CurPixel * 4, 0);
        float4 downScaled = float4(0.0, 0.0, 0.0, 0.0);
        [unroll]
        for (int i = 0; i < 4; ++i)
        {
            [unroll]
            for (int j = 0; j < 4; ++j)
            {
                downScaled += gtxtTexture.Load(nFullResPos, int2(j, i)); 
            }
        }
        downScaled /= 16.0;

        // 픽셀별 휘도값 계산
        avgLum = dot(downScaled, LUM_FACTOR);
        
        // 공유메모리에 결과 기록
        SharedPosition[groupThreadId] = avgLum;
    }

    // 동기화 후 다음 단계로
    GroupMemoryBarrierWithGroupSync();

    return avgLum;
}

// float DownScale4x4(uint2 CurPixel, uint groupThreadId) 에서 구한 값을
// 4개의 값으로 다운 스케일한다.
float DownScaled1024to4(uint dispatchThreadID, uint groupThreadID, float avgLum)
{
    // 다운 스케일 코드를 확장
    [unroll]
    for (uint groupSize = 4, step1 = 1, step2 = 2, step3 = 3;
        groupSize < 1024;
        groupSize *= 4, step1 *= 4, step2 *= 4, step3 *=3)
    {
        //팍샐 굘헙 계산 생략
        if(groupThreadID % groupSize == 0 )
        {
            //휘도값 계산
            float stepAvgLum = avgLum;
            stepAvgLum += dispatchThreadID + step1 < Domain ? SharedPosition[groupThreadID + step1] : avgLum;
            stepAvgLum += dispatchThreadID + step2 < Domain ? SharedPosition[groupThreadID + step2] : avgLum;
            stepAvgLum += dispatchThreadID + step3 < Domain ? SharedPosition[groupThreadID + step3] : avgLum;
            
            // 결과값 저장
            avgLum = stepAvgLum;
            SharedPosition[groupThreadID] = stepAvgLum;

        }

        // 동기화후 다음으로
        GroupMemoryBarrierWithGroupSync();
    }
    
    return avgLum;
}

// 네개의 값을 하나의 평균값으로 다운스케일한 후 저장한다.
void DownScaled4to1(uint dispatchThreadID, uint groupThreadID, uint groupID, float avgLum)
{
    if(groupThreadID == 0)
    {
        // 스레드 그룹에 대한 평균 휘도 값 계산
        float fFinalAvgLum = avgLum;
        
        fFinalAvgLum += dispatchThreadID + 256 < Domain ? SharedPosition[groupThreadID + 256] : avgLum;
        fFinalAvgLum += dispatchThreadID + 512 < Domain ? SharedPosition[groupThreadID + 512] : avgLum;
        fFinalAvgLum += dispatchThreadID + 768 < Domain ? SharedPosition[groupThreadID + 768] : avgLum;
        fFinalAvgLum /= 1024.0;

        // 최종 값을 1D UAV에 기록 후
        // 다음 과정으로
        gAverageLum[groupID] = fFinalAvgLum;
    }
}


// 첫번째 과정
[numthreads(1024, 1, 1)]
void DownScaledFirstPass( uint3 groupID : SV_GroupID, uint3 dispatchThreadID : SV_DispatchThreadID , uint3 groupThreadID : SV_GroupThreadID)
{
    uint2 CurPixel = uint2(dispatchThreadID.x % Res.x, dispatchThreadID.x / Res.x);

    // 16 픽셀 메모리 그룹을 하나의 픽셀로 줄여 공유 메모리에 저장
    float avgLum = DownScale4x4(dispatchThreadID.x, groupThreadID.x);

    // 1024에서 4로 다운 스케일
    avgLum = DownScaled1024to4(dispatchThreadID.x, groupThreadID.x, avgLum);

    // 4에서 1로 다운 스케일
    DownScaled4to1(dispatchThreadID.x, groupThreadID.x, groupID.x, avgLum);
}


#define MAX_GROUPS 64
groupshared float ShardAvgFinal[MAX_GROUPS]; // 공유메모리 그룹에 중간값 저장

// 두번째 과정
// 중간값 휘도 srv와 평균 휘도 uav를 지정해 사용
[numthreads(MAX_GROUPS, 1, 1)]
void DownScaledSecondPass(uint3 groupID : SV_GroupID, uint3 dispatchThreadID : SV_DispatchThreadID, uint3 groupThreadID : SV_GroupThreadID)
{
    // 공유 메모리에 1d값 저장 // 1D가 뭐야.. 아이디도 아니고..
    float avgLum = 0.0;

    if(dispatchThreadID.x < GroupSize)
    {
        avgLum = gAverageValues1D[dispatchThreadID.x];
    }
    ShardAvgFinal[dispatchThreadID.x] = avgLum;

    GroupMemoryBarrierWithGroupSync(); // 동기화 후 다음 과정으로

    // 64에서 16으로 다운 스케일...
    if(dispatchThreadID.x % 4 == 0)
    {
        float stepAvgLum = avgLum;
        stepAvgLum += dispatchThreadID.x + 1 < GroupSize ? ShardAvgFinal[dispatchThreadID.x + 1] : avgLum;
        stepAvgLum += dispatchThreadID.x + 2 < GroupSize ? ShardAvgFinal[dispatchThreadID.x + 2] : avgLum;
        stepAvgLum += dispatchThreadID.x + 3 < GroupSize ? ShardAvgFinal[dispatchThreadID.x + 3] : avgLum;

        // 결과 값 저장
        avgLum = stepAvgLum;
        ShardAvgFinal[dispatchThreadID.x] = stepAvgLum;
    }
    
    GroupMemoryBarrierWithGroupSync(); // 동기화 후 다음 과정으로

    // 16에서 4로 다운 스케일...
    if (dispatchThreadID.x % 16 == 0)
    {
        float stepAvgLum = avgLum;
        stepAvgLum += dispatchThreadID.x + 4 < GroupSize ? ShardAvgFinal[dispatchThreadID.x + 4] : avgLum;
        stepAvgLum += dispatchThreadID.x + 8 < GroupSize ? ShardAvgFinal[dispatchThreadID.x + 8] : avgLum;
        stepAvgLum += dispatchThreadID.x + 12 < GroupSize ? ShardAvgFinal[dispatchThreadID.x + 12] : avgLum;

        // 결과 값 저장
        avgLum = stepAvgLum;
        ShardAvgFinal[dispatchThreadID.x] = stepAvgLum;
    }
    
    GroupMemoryBarrierWithGroupSync(); // 동기화 후 다음 과정으로

    // 4에서 1로 다운 스케일...
    if (dispatchThreadID.x == 0)
    {
        float stepAvgLum = avgLum;
        stepAvgLum += dispatchThreadID.x + 16 < GroupSize ? ShardAvgFinal[dispatchThreadID.x + 16] : avgLum;
        stepAvgLum += dispatchThreadID.x + 32 < GroupSize ? ShardAvgFinal[dispatchThreadID.x + 32] : avgLum;
        stepAvgLum += dispatchThreadID.x + 48 < GroupSize ? ShardAvgFinal[dispatchThreadID.x + 48] : avgLum;

        stepAvgLum /= 64.0;

        // 결과 값 저장
        gAverageLum[0] = stepAvgLum;

    }
}
