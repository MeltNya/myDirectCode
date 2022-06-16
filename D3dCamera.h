#pragma once
#include"d3dx9.h"
class D3dCamera
{
public:

	D3dCamera();
	~D3dCamera();
	D3DXMATRIX m_ViewMatrix;
	D3DXMATRIX m_ProjMatrix;
	void MoveCamera(float x, float y, float z);
	void RotateCameraLevel(float angle);  //��Y��������ת
	void RotateCameraVertical(float angle);
	void ScaleFov(float a);	
	float getFovOffset() {
		return m_fovOffset;
	}
private:
	void SetProj(float fovy, float aspect, float zn, float zf);
	void SetView(D3DXVECTOR3 eye, D3DXVECTOR3 at,D3DXVECTOR3 up);
	float m_rotatedLevel = 0;
	D3DXVECTOR3 m_Eye;
	D3DXVECTOR3 m_At;
	D3DXVECTOR3 m_Up=D3DXVECTOR3(0.0f,1.0f,0.0f) ;	
	D3DXVECTOR3 m_Right = D3DXVECTOR3(1.0f,0.0f,0.0f);
	float m_fovOffset=1.0f;
	float m_fFovY = D3DX_PI / 4.0f;		 //y������ӽ�
	float m_fAspect = 1.0f;   //�ݺ��
	float m_fZN = 1.0f;			 //���ü�ƽ��
	float m_fZF = 100.0f;			 //Զ�ü�ƽ��
	
};

