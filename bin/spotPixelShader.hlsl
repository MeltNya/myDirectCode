/* �۹�� */
matrix wMatrix;

float4 lightDir;
float4 lightPos;

float4 diffuseColor;
float4 ambientColor;
float cutoff;

float attenConst = 1.0f;               //����˥��
float attenLinear = 0.2f;              //����˥��
float attenExp = 0.1f;                  //ָ��˥��

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
    float3 lightToPos = normalize(worldPos.xyz - lightPos.xyz);       //��Դ������ķ���
    float3 posToLight = normalize(lightPos.xyz - worldPos.xyz);     //���㵽��Դ�ķ���
    float disToLight = length(posToLight);   //���㵽��Դ�ľ��� 
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