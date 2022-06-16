/* 聚光灯 */
matrix wMatrix;
float4 lightDir;
float4 lightPos;

float4 diffuseColor;
float4 ambientColor;
float cutoff;

float attenConst = 1.0f;               //常数衰减
float attenLinear = 0.2f;              //线性衰减
float attenExp = 0.1f;                  //指数衰减
sampler shadowMap;

float uvScale;

struct PS_INPUT
{
    float4 position : TEXCOORD0;                     //模型坐标
    float3 normal : TEXCOORD1;
    float2 scene_uv : TEXCOORD2;
   // float4 shadow_uv : TEXCOORD3;
    float4 worldPos : TEXCOORD4;
    float depth :TEXCOORD5;
    float4 posInLight:TEXCOORD6;
};


float4 my_PS(PS_INPUT pin) : SV_TARGET
{
    //float depInShadow = tex2Dproj(shadowMap, pin.shadow_uv).r;  //获得顶点在阴影图中的深度
   /* float depInShadow;
    float2 suv;
    suv.x = ((pin.posInLight.x / pin.posInLight.w )* 0.5f + 0.5f)/3.5 +0.2 ;
    suv.y =( (pin.posInLight.y/ pin.posInLight.w) * (-0.5f) + 0.5f) /3.5+0.1;
    depInShadow = tex2D(shadowMap, suv).r;*/ 
    float depth = pin.depth;
    float texelSizeX = 0.004f; 
    float texelSizeY = 0.004f;
    float shadowFactor=1.0f;
    float2 s_uv[4];
    float bias = 0.005f;
    float dx = (pin.posInLight.x / pin.posInLight.w) * 0.5f + 0.5f ;
    float dy = (pin.posInLight.y / pin.posInLight.w) * (-0.5f)+ 0.5f  ;
    s_uv[0].x = dx + texelSizeX;  s_uv[0].y = dy;
    s_uv[1].x = dx - texelSizeX;  s_uv[1].y = dy;
    s_uv[2].x = dx ;  s_uv[2].y = dy+ texelSizeY;
    s_uv[3].x = dx;  s_uv[3].y = dy - texelSizeY;
    int sumDepS = 0;
    for (int i = 0; i < 4; i++) {
       float td= tex2D(shadowMap, s_uv[i]).r;
       shadowFactor += depth-bias > td ? 0.1f : 1.0f;
    }
    shadowFactor /= 4.0f;
   /*if (depInShadow < depth) {
        shadowFactor = 0.3f;            //小于说明被遮挡了,光照减弱
    }*/

    float3 lightd =normalize( lightDir.xyz);
    float4 worldPos =pin.worldPos;
    float3 worldNormal = normalize(mul(float4(pin.normal, 0.0f), wMatrix)).xyz;
    float3 lightToPos = normalize(worldPos.xyz - lightPos.xyz);       //光源到顶点的方向
    float disToLight = length(lightToPos);                                      //顶点到光源的距离 
    float spotFact = dot(lightToPos, lightd) / (disToLight * length(lightd));

    float4 diffuse;
    if (spotFact >= cutoff) {
        float NdotL = saturate(dot(-lightd, worldNormal));
        diffuse = float4(diffuseColor.rgb * NdotL, 1.0f);
        float atten = attenConst + attenLinear * disToLight + attenExp * disToLight * disToLight;
        //float atten = 1.0f;
        diffuse = diffuse / atten;
    }
    else {
        diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    }

   float4  finalColor = ambientColor * 0.2f + diffuse  * shadowFactor;
 //   finalColor =float4(shadowFactor, shadowFactor, shadowFactor, shadowFactor);
    //finalColor = float4(depth, depth, depth, depth);
  return finalColor;
}
