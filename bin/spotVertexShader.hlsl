/*  spot lighting
     聚光灯
*/
matrix wMatrix;

matrix vMatrix;
matrix pMatrix;

float3 eyePos;
float4 lightPos;
float4 lightDir;
float4 diffuseColor;
float4 ambientColor;
float cutoff;

float attenConst = 1.0f;               //常数衰减
float attenLinear = 0.2f;              //线性衰减
float attenExp = 0.1f;                  //指数衰减

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
    float3 lightd= normalize(lightDir).xyz;
    float4 worldPos = mul(float4(inVer.position, 1.0f), wMatrix);
    float3 worldNormal = normalize(mul(float4(inVer.normal, 0.0f), wMatrix)).xyz;
    float3 lightToPos = normalize(worldPos.xyz - lightPos.xyz);
    float3 posToLight = normalize(lightPos.xyz - worldPos.xyz);
    

    float disToLight = length(posToLight);
    float spotFact = spotFact = dot(lightToPos, lightd) / (disToLight * length(lightd));
    float4 diffuse;

    if (spotFact > cutoff) {
        float NdotL = saturate(dot(posToLight, worldNormal));
        diffuse =float4(diffuseColor.rgb * NdotL,1.0f);
        float atten = attenConst + attenLinear * disToLight + attenExp * disToLight * disToLight;
        diffuse /= atten;
    }
    else {
        diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    }
    outVer.color = ambientColor * 0.4f + diffuse * 2.0f;
    outVer.position = mul(worldPos, vMatrix);
    outVer.position = mul(outVer.position, pMatrix);
    return outVer;
}

