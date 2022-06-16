struct Ver_INPUT
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    //float2 uv : TEXCOORD;
};

struct Ver_OUTPUT
{
    float4  position : POSITION;
    //float3  normal : NORMAL;
    //float2 uv : TEXCOORD0;
    float4 diffuse : COLOR;
};
//�任����
matrix wMatrix;
matrix vMatrix;
matrix pMatrix;
//��������
float4 lightDir;
float4 lightDiffuse;  
float4 ambient;       
//������ͼ
texture tex;


Ver_OUTPUT my_VS(Ver_INPUT inVer )
{
    Ver_OUTPUT outVer = (Ver_OUTPUT) 0 ;
    outVer.position = mul(float4(inVer.position,1.0f), wMatrix );                  //����ת����������
    float3 lightDirect = normalize(lightDir.xyz);
    float3 N = normalize(mul(float4(inVer.normal.xyz, 0.0f), wMatrix)).xyz;
    //outVer.normal = normalize( mul( float4( inVer.normal.xyz, 0.0f), wMatrix ) ).xyz;        //����ת����������

    float s = dot(lightDirect, N);
    s = saturate(s);
    outVer.diffuse = ambient + s * lightDiffuse;
    outVer.position = mul(inVer.position, vMatrix);
    outVer.position = mul(inVer.position, pMatrix);
    return outVer;
}


float4 my_PS(Ver_OUTPUT pin) : SV_TARGET
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}
/*technique testShader
{
    pass P0 
    {
        
    }
 
}*/