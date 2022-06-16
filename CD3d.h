#pragma once
#include<d3d9.h>
#include <d3dx9.h>
#include"D3dMaterial.h"
#include"D3dLight.h"
#include"D3dCamera.h"
#include"D3dGui.h"
#include"myLight.h"
#include"D3dGeometryMesh.h"
/*class D3dGeometryMesh : public CRef
{
	D3dGeometryMesh() {}
};*/



class CGameObject
{
private:
	D3DXMATRIX m_mWorld;
	D3dGeometryMesh * m_pMesh;
	D3dMaterial * m_pMat;

public:
	CGameObject()
	{
		D3DXMatrixIdentity(&m_mWorld);
	}
	~CGameObject()
	{
		m_pMesh = NULL;
		//delete m_pMesh;
		//m_pMesh->Release();
	}
	void SetPosition(float x, float y, float z) {
		D3DXMATRIX tranMat;
		D3DXMatrixTranslation(&tranMat, x, y, z);
		m_mWorld = tranMat*m_mWorld;
	}
	void SetScale(float x, float y, float z) {
		D3DXMATRIX scaleMat;
		D3DXMatrixScaling(&scaleMat, x, y, z);
		m_mWorld = scaleMat;
	}
	void SetMesh(D3dGeometryMesh* pMesh)
	{
		m_pMesh = pMesh;
		m_pMesh->AddRef();
	}
	void SetMaterial(D3dMaterial* pMat) { 
		m_pMat = NULL;
		m_pMat = pMat; 
	}

	virtual  void Update()
	{
	}
	//渲染物体自身
	void RenderSelf(LPDIRECT3DDEVICE9 lpD3DDevice, D3dLight & light, D3dCamera & camera, LPDIRECT3DTEXTURE9 pTex=NULL)
	{
		//如果没有mesh 就终止
		if (m_pMesh == NULL)
			return;
		if (m_pMat != NULL)
		{
			if (pTex != NULL) {
				m_pMat->ApplyWithMap(lpD3DDevice, &m_mWorld, light, camera, pTex);
			}
			else {
				m_pMat->Apply(lpD3DDevice, &m_mWorld, light, camera);
			}
		}
		m_pMesh->DrawMesh(lpD3DDevice);
	}
	//渲染阴影
	void RenderShadow(LPDIRECT3DDEVICE9 lpD3DDevice, D3dLight& light,D3dMaterial* shadowMat)
	{
		if (m_pMesh == NULL)
			return;
		shadowMat->ApplyShadow(lpD3DDevice,&m_mWorld,light);
		//shadowMat->ApplyShadow(lpD3DDevice, &tMat, light);
		m_pMesh->DrawMesh(lpD3DDevice); 
	}
	void RenderShadow(LPDIRECT3DDEVICE9 lpD3DDevice, D3dCamera& camera, D3dMaterial* shadowMat)
	{
		D3DXMATRIX tMat;
		D3DXMatrixTranslation(&tMat, 0.0f, 0.0f, 1.0f);
		if (m_pMesh == NULL)
			return;
		shadowMat->ApplyShadow(lpD3DDevice, &tMat,camera);
		m_pMesh->DrawMesh(lpD3DDevice);
	}
};

class CD3d
{
protected:
	//D3dShader d3d_Shader;
	LPDIRECT3D9 g_D3D = NULL;									
	LPDIRECT3DDEVICE9 g_D3DDevice = NULL;			   //设备对象
	D3dGeometryMesh d3d_Mesh;									//Mesh对象
	D3dGeometryMesh d3d_Mesh2;
	D3dGeometryMesh d3d_Mesh3;
	D3dGeometryMesh d3d_Mesh4;
	D3dMaterial m_goMat;													//材质对象
	D3dMaterial m_shadowMat;
	D3dMaterial m_testMat;
	D3dMaterial m_testMat2;
	D3dMaterial m_testMat3;
	D3dMaterial m_spMat;
	LPD3DXFONT g_pFont = NULL;										//d3d字体对象
	D3dCamera m_MainCamera;
	RECT m_Rect = { 0, 0, 200,50 };
	vector<CGameObject*> m_vecGameObjcets;
                          
	LPDIRECT3DSURFACE9 m_ShadowMapDS = NULL;
	LPDIRECT3DTEXTURE9 m_ShadowMapRT = NULL;
	bool m_saved = FALSE;

	D3DXVECTOR3 m_Scale1 = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	D3DXVECTOR3 m_Rotate1 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_Trans1 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_Scale2 = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	D3DXVECTOR3 m_Rotate2 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_Trans2 = D3DXVECTOR3(2.0f, 0.0f, 0.0f);

	D3dLight d3d_Light;
	D3DXVECTOR4 m_Ambient;
	DWORD dw_CurrentTime = 0;
	DWORD dw_LastTime = 0;
	int i_FrameCount = 0;
	char s_FpsWord[];

	void GetFps();
	void InitFont();

	void SetupMaterices();                        //设置变换矩阵
	void CreateObject();							  //绘制模型和文字
	//void InitGUI();
	void ComputeWorldMatrix(D3DXVECTOR3 scale, D3DXVECTOR3 rotate, D3DXVECTOR3 trans, D3DXMATRIX* outMat);
	void RenderFont();
	void RenderScene();
	void RenderShadowMap();
public:
	bool  initializeD3D(HWND hwnd, bool fullscreen);
	void  MoveCameraZ(float z);
	void  MoveCameraX(float x);
	void  MoveCameraY(float y);
	void  RotateCameraL(float l);
	void  RotateCameraV(float v);
	void  ScaleCameraFov(float a);
	void  MoveLight(float x, float y, float z);
	void  RotateLightVertical(float angle);
	void  ChangeLight(int lightType,int ShaderType);
	void  Update();
	void  Render();
	void  CleanUp();
	void  RenderShadowAndScene();
	void  CreateShadowTexture();

private:
	void GetConstes(vector<string> & vec, ID3DXConstantTable* pTable)
	{
		vec.clear();

		D3DXCONSTANTTABLE_DESC des;
		pTable->GetDesc(&des);

		D3DXHANDLE handle;
		D3DXCONSTANT_DESC deses;
		for (int i = 0; i < des.Constants; ++i)
		{
			handle = pTable->GetConstant(NULL, i);
			pTable->GetConstantDesc(handle, &deses, NULL);
			vec.push_back(deses.Name);
		}
	}
};