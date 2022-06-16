/* 点光源 */
matrix wMatrix;

float4 lightPos;
float4 diffuseColor;
float4 ambientColor;

float attenConst ;               //常数衰减
float attenLinear;              //线性衰减
float attenExp;         //指数衰减

struct PS_INPUT
{
    float4 position : TEXCOORD0;
    float3 normal : TEXCOORD1;
};

float4 my_PS(PS_INPUT pin) : SV_TARGET
{
    float4 worldPos = mul(pin.position,wMatrix);
    float3 worldNormal = normalize(mul(float4(pin.normal, 0.0f), wMatrix)).xyz;
    float3 toLight = lightPos.xyz - worldPos.xyz;
    float disToLight = length(toLight);
    //diffuse
    toLight = toLight / disToLight;             //归一化
    float NdotL = saturate(dot(toLight, worldNormal));
    float4 diffuse =float4(diffuseColor.rgb * NdotL,1.0);
    //Attenuation
    float atten = attenConst + attenLinear * disToLight + attenExp * disToLight * disToLight;
    diffuse = diffuse / atten;
    float4 finalColor = ambientColor * 0.4f + diffuse * 1.0f;

    return finalColor;
}