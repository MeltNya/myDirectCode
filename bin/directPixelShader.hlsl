/*Æ½ÐÐ¹â*/
matrix wMatrix;

float4 lightDir;
float4 ambientColor;
float4 diffuseColor;

struct PS_INPUT {
	float3 position :POSITION;
	float3 normal : TEXCOORD1;
};

float4 my_PS(PS_INPUT pin) : SV_TARGET
{
	//float3 worldPos = mul(float4(pin.position,1.0f),wMatrix).xyz;
	float3 worldNormal = normalize(mul(float4(pin.normal, 0.0f), wMatrix)).xyz;
	float3 L = normalize(lightDir).xyz;
	float NdotL = dot(L, worldNormal);
	NdotL = saturate(NdotL);
	float4 finalCol = ambientColor * 0.2 + NdotL * diffuseColor * 0.8;
	return finalCol;
}