Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(t0);

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float4 color;
};

float4 ParticlePixelShader(PixelInputType input) : SV_TARGET
{
    float4 textureColor;
    float4 finalColor;

    textureColor = shaderTexture.Sample(SampleType, input.tex);

    finalColor = textureColor * input.color;

    return finalColor;
}
