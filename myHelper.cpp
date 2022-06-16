#include"myHelper.h"
 D3DXMATRIX myHelper:: ComputeWorldMat(D3DXVECTOR3 scale, D3DXVECTOR3 translate, D3DXVECTOR3 rotate) {
	 D3DXMATRIX scaleMat;
	 D3DXMatrixScaling(&scaleMat, scale.x, scale.y, scale.z);
	 D3DXMATRIX transMat;
	 D3DXMatrixTranslation(&transMat, translate.x, translate.y, translate.z);
	 D3DXMATRIX rotateXmat;
	 D3DXMatrixRotationX(&rotateXmat, rotate.x * D3DX_PI / 180);
	 D3DXMATRIX rotateYMat;
	 D3DXMatrixRotationY(&rotateYMat, rotate.y * D3DX_PI / 180);
	 D3DXMATRIX rotateZMat;
	 D3DXMatrixRotationZ(&rotateZMat, rotate.z * D3DX_PI / 180);
	 D3DXMATRIX WorldMat;
	 D3DXMatrixIdentity(&WorldMat);
	 WorldMat = scaleMat * rotateXmat * rotateYMat * rotateZMat * scaleMat;
	 return WorldMat;
}