/* 点光源 */
matrix wMatrix;

matrix vMatrix;
matrix pMatrix;

float3 eyePos;
float4 lightPos;
//float   lightRange;
float4 diffuseColor;
float4 ambientColor;

float attenConst=1.0f;               //常数衰减
float attenLinear=0.2f;              //线性衰减
float attenExp=0.1f;                  //指数衰减

struct Ver_INPUT
{
    float3 position : POSITION;
    float3 normal : NORMAL;
};
struct Ver_OUTPUT
{
    float4  position : POSITION;
    float4  color : COLOR;
};

Ver_OUTPUT my_VS(Ver_INPUT inVer)
{
    Ver_OUTPUT outVer = (Ver_OUTPUT)0;
    float4 worldPos = mul(float4(inVer.position, 1.0f), wMatrix);
    float3 worldNormal = normalize(mul(float4(inVer.normal, 0.0f), wMatrix)).xyz;
    float3 toLight = lightPos.xyz - worldPos.xyz;
    float disToLight = length(toLight);
    //diffuse
    toLight = toLight / disToLight;             //归一化
    float NdotL = saturate (dot(toLight, worldNormal));
    float3 diffuse = diffuseColor.rgb * NdotL;
    //Attenuation
    float atten = attenConst + attenLinear * disToLight + attenExp * disToLight * disToLight ;
    diffuse =diffuse / atten;
    outVer.color = ambientColor * 0.4f + float4(diffuse, 1.0f)*1.5f;


    outVer.position = mul(worldPos, vMatrix);
    outVer.position = mul(outVer.position, pMatrix);
    return outVer;
}