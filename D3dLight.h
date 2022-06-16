#pragma once
#include <d3dx9.h>
class D3dLight {
protected:
	D3DXVECTOR3 m_Eye;
	D3DXVECTOR3 m_At;
	D3DXVECTOR3 m_Up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	void ComputProjMatirx();
	void ComputeViewMatrix();
	//void ComputeWorldMatrix();
public:
	D3DXVECTOR4 m_LightDir;
	D3DXVECTOR4 m_LightPos;
	D3DXVECTOR4 m_DiffuseColor;
	float m_Cutoff;
	float m_AttenConst=1.0f;
	float m_AttenLinear=0.1f;
	float m_AttenExp = 0.1f;
	D3DXMATRIX m_ViewMatrix;    //光源的view矩阵
	D3DXMATRIX m_ProjMatrix;	  //光源的投影矩阵
	D3DXMATRIX m_WorldMatirx;    //光源的世界矩阵
	D3dLight();
	//D3dLight(D3DXVECTOR4 lightDir, D3DXVECTOR4 lightPos, D3DXVECTOR4 diffuse);
	void CreateDirectLight(D3DXVECTOR4 lightDir, D3DXVECTOR4 diffuse);
	void CreatePointLight(D3DXVECTOR4 lightPos, D3DXVECTOR4 diffuse, float aConst, float aLinear, float aExp);
	void CreateSpotLight(D3DXVECTOR4 lightPos, D3DXVECTOR4 lightDir, D3DXVECTOR4 diffuse, float aConst, float aLinear, float aExp,float angle);
	void SetPosition(float x, float y, float z);
	void MovePosition(float x, float y, float z);
	void RotateVerticcal(float angle);
	//~D3dLight();
};