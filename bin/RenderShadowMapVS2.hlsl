//��Ⱦ�����ͼ�Ķ���shader
//ֻ���Ѷ���ת����Դ�ռ�ı任
matrix lightViewMatrix;
matrix lightProjMatrix;
matrix wMatrix;

struct Ver_INPUT
{
    float3 position : POSITION;

};
struct Ver_OUTPUT
{
    float4  position : POSITION;                                  //����ռ��pos 
    float4 posInLight : TEXCOORD0;                          //�����ڹ�Դ�ռ������
    float depthInLight : TEXCOORD1;                        //�����ڹ�Դ�ռ�����

};

Ver_OUTPUT my_VS(Ver_INPUT inVer)
{
    Ver_OUTPUT outVer = (Ver_OUTPUT)0;
    //��������ռ�Ķ�������
    outVer.position = mul(float4(inVer.position, 1.0f), wMatrix);
    //����ƹ�ռ���POS
    outVer.posInLight = mul(outVer.position, lightViewMatrix);
    outVer.posInLight = mul(outVer.posInLight, lightProjMatrix);
    //������ڵƹ��µ����
    outVer.depthInLight = outVer.posInLight.z / outVer.posInLight.w;
  //  outVer.depthInLight = outVer.posInLight.z;
    return outVer;
}