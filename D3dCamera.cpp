#include"D3dCamera.h"
#include <d3dx9math.h>
#include<string>
using namespace std;
void D3dCamera::SetProj(float fovy, float aspect, float zn, float zf ) {
	m_fAspect = aspect;
	m_fFovY = fovy;
	m_fZN = zn;
	m_fZF = zf;
	D3DXMatrixPerspectiveFovLH(&m_ProjMatrix, m_fFovY, m_fAspect,m_fZN, m_fZF);
}
void D3dCamera::SetView(D3DXVECTOR3 eye, D3DXVECTOR3 at, D3DXVECTOR3 up) {
	m_Eye = eye;
	m_At = at;
	m_Up = up;
	
	D3DXMatrixLookAtLH(&m_ViewMatrix, &eye, &at, &up);
}
D3dCamera::D3dCamera(){
	D3DXVECTOR3 vEye(0.0f, 30.0f, -40.0f);
	D3DXVECTOR3 vAt(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vUp(0.0f, 1.0f, 0.0f);
	SetView(vEye, vAt, vUp);
	SetProj(D3DX_PI / 4.0f, 1.0f, 1.0f, 100.0f); 
}
D3dCamera::~D3dCamera() {

}
void D3dCamera:: MoveCamera(float x, float y, float z) {
	D3DXMATRIX transMat;
	D3DXMatrixTranslation(&transMat, x, y, z);
	D3DXVECTOR4 newEye;
	D3DXVec3Transform(&newEye, &m_Eye, &transMat);
	m_Eye = D3DXVECTOR3(newEye.x, newEye.y, newEye.z);
	D3DXVECTOR4 newAt;
	D3DXVec3Transform(&newAt, &m_At, &transMat);
	m_At = D3DXVECTOR3(newAt.x, newAt.y, newAt.z);

	SetView(m_Eye, m_At, m_Up);
}
//绕Y轴左右旋转
void D3dCamera::RotateCameraLevel(float angle) {
	m_rotatedLevel += angle;
	string s = std::to_string(m_rotatedLevel) + "\n";
	OutputDebugString(s.c_str());
	float a =- angle * D3DX_PI / 180;
	D3DXMATRIX rMat;
	D3DXMatrixRotationY(&rMat, a);
	//at旋转
	D3DXVECTOR4 tempMat;
	D3DXVec3Transform(&tempMat, &m_Eye, &rMat);
	m_Eye = D3DXVECTOR3(tempMat.x, tempMat.y, tempMat.z);

	D3DXMATRIX rMat0;
	D3DXMatrixRotationAxis(&rMat0, &m_Up, a);
	D3DXVECTOR4 tempRight;
	D3DXVec3Transform(&tempRight, &m_Right, &rMat0);
	m_Right = D3DXVECTOR3(tempRight.x, tempRight.y, tempRight.z);

	SetView(m_Eye, m_At, m_Up);
}
//绕x轴上下旋转
void D3dCamera::RotateCameraVertical(float angle) {
	if (angle >= 90.0f) {
		angle = 89.9f;
	}
	if (angle <= -90.0f) {
		angle =- 89.9f;
	}
	float a = angle * D3DX_PI / 180;

	D3DXMATRIX rMat;
	D3DXMatrixRotationAxis(&rMat, &m_Right, a);
	//D3DXMatrixRotationX(&rMat, a);
	D3DXVECTOR4 tempMat;
	D3DXVec3Transform(&tempMat, &m_Eye, &rMat);
	m_Eye = D3DXVECTOR3(tempMat.x, tempMat.y, tempMat.z);

	SetView(m_Eye, m_At, m_Up);
	
}
//放大
void D3dCamera::ScaleFov(float a) {
	m_fovOffset = a;
	m_fFovY /= a;
	SetProj(m_fFovY, m_fAspect, m_fZN, m_fZF);
}