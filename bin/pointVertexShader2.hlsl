
matrix wMatrix;

matrix vMatrix;
matrix pMatrix;

matrix shadowTexMatrix;     //将顶点映射到阴影uv的矩阵

struct Ver_INPUT
{
    float3 position : POSITION;
    float3 normal: NORMAL;
    float2 uv:TEXCOORD0;

};
struct Ver_OUTPUT
{
    float4  position : POSITION;
    float4 vertex:TEXCOORD0;
    float3 normal : TEXCOORD1;
    float2 scene_uv:TEXCOORD2;
    float4 shadow_uv:TEXCOORD3;
    float4 worldPos : TEXCOORD4;
};

Ver_OUTPUT my_VS(Ver_INPUT inVer)
{
    Ver_OUTPUT outVer = (Ver_OUTPUT)0;
    outVer.vertex = float4(inVer.position, 1.0f);
    outVer.position = mul(float4(inVer.position, 1.0f), wMatrix);
    outVer.worldPos = outVer.position;
    outVer.position = mul(outVer.position, vMatrix);
    outVer.position = mul(outVer.position, pMatrix);

    outVer.normal = inVer.normal;

    outVer.scene_uv = inVer.uv;
    outVer.shadow_uv = mul(outVer.position, shadowTexMatrix);

    return outVer;
}