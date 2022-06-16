//渲染深度贴图的顶点shader
//只做把顶点转到光源空间的变换
matrix lightViewMatrix;
matrix lightProjMatrix;
matrix wMatrix;

struct Ver_INPUT
{
    float3 position : POSITION;

};
struct Ver_OUTPUT
{
    float4  position : POSITION;                                  //世界空间的pos 
    float4 posInLight : TEXCOORD0;                          //顶点在光源空间的坐标
    float depthInLight : TEXCOORD1;                        //顶点在光源空间的深度

};

Ver_OUTPUT my_VS(Ver_INPUT inVer)
{
    Ver_OUTPUT outVer = (Ver_OUTPUT)0;
    //计算世界空间的顶点坐标
    outVer.position = mul(float4(inVer.position, 1.0f), wMatrix);
    //计算灯光空间下POS
    outVer.posInLight = mul(outVer.position, lightViewMatrix);
    outVer.posInLight = mul(outVer.posInLight, lightProjMatrix);
    //计算点在灯光下的深度
    outVer.depthInLight = outVer.posInLight.z / outVer.posInLight.w;
  //  outVer.depthInLight = outVer.posInLight.z;
    return outVer;
}