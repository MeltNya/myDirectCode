
matrix wMatrix;
matrix vMatrix;
matrix pMatrix;

struct Ver_INPUT
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv:TEXCOORD;
};
struct Ver_OUTPUT
{
    float4  position : POSITION;
    float3 normal : NORMAL;
    float2 uv:TEXCOORD;
};

Ver_OUTPUT my_VS(Ver_INPUT inVer)
{
    Ver_OUTPUT outVer = (Ver_OUTPUT)0;
    outVer.position = mul(float4(inVer.position, 1.0f), wMatrix);
    outVer.position = mul(outVer.position, vMatrix);
    outVer.position = mul(outVer.position, pMatrix);
    outVer.normal = inVer.normal;
    outVer.uv = inVer.uv;
    return outVer;
}