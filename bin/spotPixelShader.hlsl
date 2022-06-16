/* 聚光灯 */
matrix wMatrix;

float4 lightDir;
float4 lightPos;

float4 diffuseColor;
float4 ambientColor;
float cutoff;

float attenConst = 1.0f;               //常数衰减
float attenLinear = 0.2f;              //线性衰减
float attenExp = 0.1f;                  //指数衰减

struct PS_INPUT
{
    float4 position : TEXCOORD0;
    float3 normal : TEXCOORD1;
};


float4 my_PS(PS_INPUT pin) : SV_TARGET
{
    float3 lightd = normalize(lightDir.xyz);
    float4 worldPos = mul(pin.position,wMatrix);
    float3 worldNormal = normalize(mul(float4(pin.normal, 0.0f), wMatrix)).xyz;
    float3 lightToPos = normalize(worldPos.xyz - lightPos.xyz);       //光源到顶点的方向
    float3 posToLight = normalize(lightPos.xyz - worldPos.xyz);     //顶点到光源的方向
    float disToLight = length(posToLight);   //顶点到光源的距离 
    float spotFact = dot(lightToPos, lightd) / (disToLight * length(lightd) );
    float4 diffuse;
    float at;
    float nt;
    if (spotFact >= cutoff) {
        float NdotL = saturate(dot(-lightd, worldNormal));
        nt = NdotL;
        diffuse = float4(diffuseColor.rgb * NdotL, 1.0f);
        float atten = attenConst + attenLinear * disToLight + attenExp * disToLight * disToLight;
        diffuse  = diffuse/ atten;
        at = atten;
    }
    //Attenuation
    else {
        diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    }
    float4 finalColor = ambientColor * 0.4f + diffuse * 2.0f;

 return finalColor;
}