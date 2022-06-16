#pragma once
#include <d3dx9.h>
#include<d3d9.h>
#include <string>
#include <vector>
#include"D3dShader.h"
#include"D3dLight.h"
#include"D3dCamera.h"
using namespace std;
class D3dMaterial
{
protected:
	D3dShader m_shader;

	vector< LPDIRECT3DTEXTURE9> m_pTextures ;
	//shader
	void ClearTexBuffer();
	void SetVPMatrix(LPDIRECT3DDEVICE9 g_Device, D3dCamera camera);
	void SetLight(LPDIRECT3DDEVICE9 g_Device,D3dLight light);
		
public:
	void SetTextureForStage(int iStage, const char* szpFileName, LPDIRECT3DDEVICE9 g_Device);
	void SetTextureForShaderA(const char* fileName, const char* propertyName, LPDIRECT3DDEVICE9 g_Device);						 //从外部文件获取纹理贴图给shader
	void SetTextureForShaderB(LPDIRECT3DTEXTURE9 pTex, const char* propertyName, LPDIRECT3DDEVICE9 g_Device);			 //传已有的纹理给shader
	void SetPropertyForShader(LPDIRECT3DDEVICE9 g_Device, const char* propertyName, void* pdata, e_PropertyType pt);

	void Apply(LPDIRECT3DDEVICE9 g_Device, D3DXMATRIX* wMat, D3dLight& light, D3dCamera& camera);
	void ApplyWithMap(LPDIRECT3DDEVICE9 g_Device, D3DXMATRIX* wMat, D3dLight& light, D3dCamera& camera,  LPDIRECT3DTEXTURE9 pTex);
	void ApplyShadow(LPDIRECT3DDEVICE9 g_Device, D3DXMATRIX* wMat, D3dLight& light);
	void ApplyShadow(LPDIRECT3DDEVICE9 g_Device, D3DXMATRIX* wMat, D3dCamera& camera);
	void SetWorldMatrix(LPDIRECT3DDEVICE9 g_Device, D3DXMATRIX* Mat);	

	void CreateMatrial(LPDIRECT3DDEVICE9 g_Device, const char* vsFileName="", const char* psFileName="");		
	
	D3dMaterial();
	~D3dMaterial();
	
};