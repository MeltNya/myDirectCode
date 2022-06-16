#pragma once
#include<d3d9.h>
#include<d3dx9.h>
#include"D3dMaterial.h"
#include"D3dShader.h"
#include"CD3d.h"
using namespace std;
D3dMaterial::~D3dMaterial() {
	 ClearTexBuffer();

}
D3dMaterial::D3dMaterial(){
	
	m_pTextures.resize(8);
}
void D3dMaterial::CreateMatrial(LPDIRECT3DDEVICE9 g_Device, const char* vsFileName, const char* psFileName ) {
	m_shader.CleanUpshader();
	//如果没有shader文件名字参数传入,使用默认的顶点shader
	if (vsFileName == "" && psFileName== "") {
		m_shader.ComplieVertexShader("defaultShader.hlsl", g_Device);
	}
	if (vsFileName != "") {
		m_shader.ComplieVertexShader(vsFileName, g_Device);
	}
	if (psFileName != "") {
		m_shader.CompliePixelShader(psFileName, g_Device);
	}
}
void D3dMaterial::SetWorldMatrix(LPDIRECT3DDEVICE9 g_Device, D3DXMATRIX* mat) {
	m_shader.SetHandle(g_Device, "wMatrix", mat,pt_matrix);
}


void D3dMaterial::SetVPMatrix(LPDIRECT3DDEVICE9 g_Device,D3dCamera camera) {

	m_shader.SetHandle(g_Device, "vMatrix", &camera.m_ViewMatrix, (e_PropertyType)pt_matrix );
	m_shader.SetHandle(g_Device, "pMatrix", &camera.m_ProjMatrix, (e_PropertyType)pt_matrix); 
}



void D3dMaterial::SetPropertyForShader(LPDIRECT3DDEVICE9 g_Device, const char* propertyName, void* pdata, e_PropertyType pt) {
	m_shader.SetHandle(g_Device, propertyName, pdata, pt);
}

void D3dMaterial::Apply(LPDIRECT3DDEVICE9 g_Device, D3DXMATRIX* wMat, D3dLight& light, D3dCamera& camera )
{
	SetVPMatrix(g_Device,camera);
	SetWorldMatrix(g_Device, wMat);
	SetLight(g_Device, light);
	m_shader.SetHandle(g_Device, "lightViewMatrix", light.m_ViewMatrix, pt_matrix);
	m_shader.SetHandle(g_Device, "lightProjMatrix", light.m_ProjMatrix, pt_matrix);
	/*if (isShadow) {
		float fOffsetX = 0.5f ;
		float fOffsetY = 0.5f ;
		D3DXMATRIX biasMatrix
		(	0.5f, 0.0f, 0.0f, 0.0f,
			0.0f, -0.5f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.5f, 0.0f,
			fOffsetX, fOffsetY, 0.0f, 1.0f );

		//传顶点映射到UV的矩阵
		D3DXMATRIX texMat = *wMat * light.m_ViewMatrix * light.m_ProjMatrix* biasMatrix;
		//D3DXMATRIX texMat = *wMat * light.m_ViewMatrix * light.m_ProjMatrix ;
		SetPropertyForShader(g_Device, "shadowTexMatrix", &texMat, pt_matrix);
		//传shadowMap进去
		SetTextureForShaderB(pTex, "shadowMap", g_Device);
	}*/

	m_shader.SetShader(g_Device);
}
void D3dMaterial::ApplyWithMap(LPDIRECT3DDEVICE9 g_Device, D3DXMATRIX* wMat, D3dLight& light, D3dCamera& camera,LPDIRECT3DTEXTURE9 pTex) {

	SetVPMatrix(g_Device, camera);
	SetWorldMatrix(g_Device, wMat);
	SetLight(g_Device, light);
	m_shader.SetHandle(g_Device, "lightViewMatrix", light.m_ViewMatrix, pt_matrix);
	m_shader.SetHandle(g_Device, "lightProjMatrix", light.m_ProjMatrix, pt_matrix);
	float fovOffset = camera.getFovOffset();
	m_shader.SetHandle(g_Device, "uvScale",&fovOffset , pt_float);
	D3DXMATRIX biasMatrix
	(0.5f, 0.0f, 0.0f, 0.0f,
		0.0f,-0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f );

	//传顶点映射到UV的矩阵
	D3DXMATRIX texMat = *wMat * light.m_ViewMatrix * light.m_ProjMatrix * biasMatrix;
	SetPropertyForShader(g_Device, "shadowTexMatrix", &texMat, pt_matrix);
	//传shadowMap进去
	SetTextureForShaderB(pTex, "shadowMap", g_Device);
	m_shader.SetShader(g_Device);
}
void D3dMaterial::ApplyShadow(LPDIRECT3DDEVICE9 g_Device, D3DXMATRIX* wMat, D3dLight& light) {
	SetWorldMatrix(g_Device, wMat);
	SetLight(g_Device, light);
	//设置VP
	m_shader.SetHandle(g_Device, "lightViewMatrix", light.m_ViewMatrix, pt_matrix);
	m_shader.SetHandle(g_Device, "lightProjMatrix", light.m_ProjMatrix, pt_matrix);
	m_shader.SetShader(g_Device);
}
//---------------------------------camera矩阵测试代码
void D3dMaterial::ApplyShadow(LPDIRECT3DDEVICE9 g_Device, D3DXMATRIX* wMat, D3dCamera& camera) {
	m_shader.SetHandle(g_Device, "wMatrix", wMat, pt_matrix);

	//设置VP
	m_shader.SetHandle(g_Device, "lightViewMatrix", camera.m_ViewMatrix, pt_matrix);
	m_shader.SetHandle(g_Device, "lightProjMatrix", camera.m_ProjMatrix, pt_matrix);
	m_shader.SetShader(g_Device);
}
void D3dMaterial::SetLight(LPDIRECT3DDEVICE9 g_Device, D3dLight light) {

	SetPropertyForShader(g_Device, "lightPos", &light.m_LightPos, pt_vector);
	SetPropertyForShader(g_Device, "lightDir", &light.m_LightDir, pt_vector);
	SetPropertyForShader(g_Device, "diffuseColor", &light.m_DiffuseColor, pt_vector);

	SetPropertyForShader(g_Device, "cutoff", &light.m_Cutoff, pt_float);
	SetPropertyForShader(g_Device, "attenConst",&light.m_AttenConst , pt_float);
	SetPropertyForShader(g_Device, "attenLinear", &light.m_AttenLinear, pt_float);
	SetPropertyForShader(g_Device, "attenExp", &light.m_AttenExp, pt_float);

 }

void  D3dMaterial::ClearTexBuffer() {
	for (int i = 0; i < m_pTextures.size(); i++) {
		if (m_pTextures[i] != NULL) 
		{
			m_pTextures[i]->Release();
			m_pTextures[i] = NULL;
		}
	}
	m_pTextures.clear();
}
void  D3dMaterial::SetTextureForShaderA(const char* fileName, const char* propertyName, LPDIRECT3DDEVICE9 g_Device) {
	//从文件创建tex
	m_shader.CreateTexture(fileName, g_Device);
	m_shader.SetTexture(g_Device, propertyName);
}
void  D3dMaterial::SetTextureForShaderB(LPDIRECT3DTEXTURE9 pTex, const char* propertyName, LPDIRECT3DDEVICE9 g_Device) {
	m_shader.GetTextureOutSide(pTex);
	m_shader.SetTexture(g_Device, propertyName);
}

