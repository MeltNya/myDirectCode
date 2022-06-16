/* 方向光 */
matrix wMatrix;

matrix vMatrix;
matrix pMatrix;

float4 lightDir;
float4 ambientColor;
float4 diffuseColor;

struct Ver_INPUT
{
    float3 position : POSITION;
    float3 normal : NORMAL;
};
struct Ver_OUTPUT
{
    float4  position : POSITION;
    float4  diffuse : COLOR;
};

Ver_OUTPUT my_VS(Ver_INPUT inVer)
{
    Ver_OUTPUT outVer = (Ver_OUTPUT)0;
    outVer.position = mul(float4(inVer.position, 1.0f), wMatrix);
    //光照
    float3 L = normalize(lightDir).xyz;
    float3 N = normalize( mul(float4(inVer.normal, 0.0f), wMatrix) ).xyz;
    float NdotL = dot(L, N);
    NdotL = saturate(NdotL);
 //   outVer.diffuse = diffuseColor;
    outVer.diffuse = ambientColor * 0.2 + NdotL * diffuseColor * 0.8;

    outVer.position = mul(outVer.position, vMatrix);
    outVer.position = mul(outVer.position, pMatrix);
    return outVer;
}