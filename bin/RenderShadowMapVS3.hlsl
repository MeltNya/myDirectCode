//渲染深度贴图的顶点shader
//只做把顶点转到光源空间的变换
//matrix lightWorldMatrix;
matrix lightViewMatrix;
matrix lightProjMatrix;
matrix wMatrix;
matrix vMatrix;
matrix pMatrix;


struct Ver_INPUT
{
    float3 position : POSITION;

};
struct Ver_OUTPUT
{
    float4  position : POSITION;                                //世界空间的pos 
    float4 posInLight : TEXCOORD0;                           //顶点在光源空间的坐标
    float depthInLight : TEXCOORD1;                        //顶点在光源空间的深度

};

Ver_OUTPUT my_VS(Ver_INPUT inVer)
{
    Ver_OUTPUT outVer = (Ver_OUTPUT)0;
    outVer.position = mul(float4(inVer.position, 1.0f), wMatrix);
    //求得灯光空间下POS
    outVer.posInLight = mul(outVer.position, lightViewMatrix);
    outVer.posInLight = mul(outVer.posInLight, lightProjMatrix);
    //计算点在灯光下的深度
    outVer.depthInLight = outVer.posInLight.z / outVer.posInLight.w;
    //计算摄像机空间
    outVer.position = outVer.posInLight;
    outVer.position*= 1.0f;

    return outVer;
}