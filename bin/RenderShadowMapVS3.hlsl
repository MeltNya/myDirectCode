//��Ⱦ�����ͼ�Ķ���shader
//ֻ���Ѷ���ת����Դ�ռ�ı任
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
    float4  position : POSITION;                                //����ռ��pos 
    float4 posInLight : TEXCOORD0;                           //�����ڹ�Դ�ռ������
    float depthInLight : TEXCOORD1;                        //�����ڹ�Դ�ռ�����

};

Ver_OUTPUT my_VS(Ver_INPUT inVer)
{
    Ver_OUTPUT outVer = (Ver_OUTPUT)0;
    outVer.position = mul(float4(inVer.position, 1.0f), wMatrix);
    //��õƹ�ռ���POS
    outVer.posInLight = mul(outVer.position, lightViewMatrix);
    outVer.posInLight = mul(outVer.posInLight, lightProjMatrix);
    //������ڵƹ��µ����
    outVer.depthInLight = outVer.posInLight.z / outVer.posInLight.w;
    //����������ռ�
    outVer.position = outVer.posInLight;
    outVer.position*= 1.0f;

    return outVer;
}