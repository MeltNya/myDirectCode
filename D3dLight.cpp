#include"D3dLight.h"

D3dLight::D3dLight() {
	//m_LightDir = D3DXVECTOR4(0.5f, -1.0f, 0.0f, 0.0f);
	//m_LightPos= D3DXVECTOR4(0.0f, 3.0f, 0.0f, 0.0f);
	//m_DiffuseColor= D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
}

void D3dLight::CreateDirectLight(D3DXVECTOR4 lightDir, D3DXVECTOR4 diffuse) {
	m_DiffuseColor = diffuse;
	m_LightDir = lightDir;
}
void  D3dLight::CreatePointLight(D3DXVECTOR4 lightPos, D3DXVECTOR4 diffuse, float aConst, float aLinear, float aExp) {
	m_DiffuseColor = diffuse;
	m_LightPos = lightPos;
	m_AttenExp = aExp;
	m_AttenConst = aConst;
	m_AttenLinear = aLinear;
	m_LightDir = D3DXVECTOR4(0.0f, 0.0f, 0.0f,0.0f);
	ComputeViewMatrix();
	ComputProjMatirx();
}
void D3dLight::CreateSpotLight(D3DXVECTOR4 lightPos, D3DXVECTOR4 lightDir, D3DXVECTOR4 diffuse, float aConst, float aLinear, float aExp, float angle) {
	m_DiffuseColor = diffuse;
	m_LightDir = lightDir;
	m_LightPos = lightPos;
	m_AttenExp = aExp;
	m_AttenConst = aConst;
	m_AttenLinear = aLinear;
	m_Cutoff =angle;
	ComputeViewMatrix();
	ComputProjMatirx();

}
void D3dLight::ComputeViewMatrix() {
	D3DXVECTOR3 eye = D3DXVECTOR3(m_LightPos.x, m_LightPos.y, m_LightPos.z);
	D3DXVECTOR3 up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXVECTOR3 at = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXMatrixLookAtLH(&m_ViewMatrix, &eye, &m_At, &up);
}
void  D3dLight::ComputProjMatirx() {
	D3DXMatrixPerspectiveFovLH(&m_ProjMatrix,D3DX_PI /2.0f, 1.33f, 1.0f, 100.0f);
}

void D3dLight::SetPosition(float x, float y, float z) {

	D3DXMATRIX transMat;
	D3DXMatrixTranslation(&transMat, x,y,z);
	//D3DXVec3Transform(&m_LightPos, &m_LightPos, &transMat);
	//D3DXVec4Transform(&m_LightPos, &m_LightPos, &transMat);
	m_LightPos.x += x;
	m_LightPos.y += y;
	m_LightPos.z += z;
	m_At.x += x;
	m_At.y += y;
	m_At.z += z;
	ComputeViewMatrix();
}
void D3dLight::MovePosition(float x, float y, float z) {

	D3DXMATRIX transMat;
	D3DXMatrixTranslation(&transMat, x, y, z);
	//D3DXVec3Transform(&m_LightPos, &m_LightPos, &transMat);
	D3DXVec4Transform(&m_LightPos, &m_LightPos, &transMat);
	ComputeViewMatrix();
}
//灯光方向抬头低头
void  D3dLight::RotateVerticcal(float angle){
	float a = -angle * D3DX_PI / 180;
	D3DXVECTOR3 vRight(1.0f, 0.0f, 0.0f);
	D3DXMATRIX rMat;
	D3DXMatrixRotationAxis(&rMat, &vRight, a);
	D3DXVECTOR4 vt;
	D3DXVec4Transform(&vt, &m_LightPos, &rMat);
	m_LightPos = vt;
	D3DXVec4Transform(&vt, &m_LightDir, &rMat);
	m_LightDir = vt;
	ComputeViewMatrix();

}