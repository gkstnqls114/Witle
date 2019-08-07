#include "Variables.hlsl"

[numthreads(1024, 1, 1)]
void BloomReveal(uint3 dispatchThreadId : SV_DispatchThreadID)
{
    uint2 CurPixel = uint2(dispatchThreadId.x % Res.x, dispatchThreadId.x / Res.x);

	// Skip out of bound pixels
    if (CurPixel.y < Res.y)
    {
        float4 color = gtxtTexture.Load(int3(CurPixel, 0));
        float Lum = dot(color, LUM_FACTOR);
        float avgLum = gAverageLum[0];

		// Find the color scale
        float colorScale = saturate(Lum - avgLum * fBloomThreshold);

        // Bloom Tex
        gtxtRWOutput[CurPixel.xy] = color * colorScale;
    }
}
