
matrix wMatrix;
matrix lightViewMatrix;
matrix lightProjMatrix;
matrix vMatrix;
matrix pMatrix;

matrix shadowTexMatrix;     //将顶点映射到阴影uv的矩阵

struct Ver_INPUT
{
    float3 position : POSITION;
    float3 normal: NORMAL;
    float2 uv:TEXCOORD0;
    
};
struct Ver_OUTPUT
{
    float4  position : POSITION;
    float4 vertex:TEXCOORD0;
    float3 normal : TEXCOORD1;
    float2 scene_uv:TEXCOORD2;
    float4 shadow_uv:TEXCOORD3;
    float4 worldPos : TEXCOORD4;                                       //在世界空间下的坐标
    float depthInLight:TEXCOORD5;                                   //在光源空间下的深度
    float4 posInLight:TEXCOORD6;                                      //在光源空间下的坐标
};

Ver_OUTPUT my_VS(Ver_INPUT inVer)
{
    Ver_OUTPUT outVer = (Ver_OUTPUT)0;
    outVer.vertex = float4(inVer.position, 1.0f);
    outVer.position = mul(float4(inVer.position, 1.0f), wMatrix);   
    outVer.posInLight = mul(outVer.position, lightViewMatrix);
    outVer. posInLight= mul(outVer.posInLight, lightProjMatrix);
    //计算点在灯光下的深度
    outVer.depthInLight = outVer.posInLight.z / outVer.posInLight.w;


    outVer.worldPos = outVer.position;
    outVer.position = mul(outVer.position, vMatrix);
    outVer.position = mul(outVer.position, pMatrix);

    outVer.normal = inVer.normal;

    outVer.scene_uv = inVer.uv;
    outVer.shadow_uv = mul(outVer.position, shadowTexMatrix);

    return outVer;
}