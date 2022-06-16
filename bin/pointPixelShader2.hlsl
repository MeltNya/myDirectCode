/* ���Դ */
matrix wMatrix;

float4 lightPos;
float4 diffuseColor;
float4 ambientColor;

float attenConst;               //����˥��
float attenLinear;              //����˥��
float attenExp;         //ָ��˥��

sampler shadowMap;

struct PS_INPUT
{
    float4 position : TEXCOORD0;
    float3 normal : TEXCOORD1;
    float2 scene_uv:TEXCOORD2;
    float4 shadow_uv:TEXCOORD3;
    float4 worldPos : TEXCOORD4;
    float depthInLight : TEXCOORD5;
};

float4 my_PS(PS_INPUT pin) : SV_TARGET
{
    float depInShadow = tex2Dproj(shadowMap, pin.shadow_uv).r;  //��ö�������Ӱͼ�е����
    float depth = pin.shadow_uv.z / pin.shadow_uv.w;
    float shadowFactor;
    if (depInShadow < depth) {
        shadowFactor = 0.2f;            //С��˵�����ڵ���,���ռ���
    }
    else {
        shadowFactor = 1.0f;
    }
    float4 worldPos = pin.worldPos;
    float3 worldNormal = normalize(mul(float4(pin.normal, 0.0f), wMatrix)).xyz;
    float3 toLight = lightPos.xyz - worldPos.xyz;
    float disToLight = length(toLight);
    //diffuse
    toLight = toLight / disToLight;             //��һ��
    float NdotL = saturate(dot(toLight, worldNormal));
    float4 diffuse = float4(diffuseColor.rgb * NdotL,1.0);
    //Attenuation
    float atten = attenConst + attenLinear * disToLight + attenExp * disToLight * disToLight;
    diffuse = diffuse / atten;
    float4 finalColor = ambientColor * 0.4f + diffuse * 1.0f* shadowFactor;

    return finalColor;
}