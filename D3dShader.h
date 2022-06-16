#pragma once
#include<d3d9.h>
#include<d3dx9.h>
#include<vector>
#include<map>
#include<typeinfo>
enum e_PropertyType {
	pt_vector,
	pt_matrix,
	pt_float

};
enum e_ShaderType {
	st_VertexShader,
	st_PixelShader
};
using namespace std;
//template <class T>
class D3dShader {
public:
	IDirect3DVertexShader9* m_pVertexShader = NULL;			//顶点着色器
	IDirect3DPixelShader9* m_pPixelShader = NULL;				//像素着色器
	ID3DXConstantTable* m_pVSConstTable = NULL;			    //vsshader的常量表
	ID3DXConstantTable* m_pPSConstTable = NULL;			   //psshader的常量表
	HRESULT m_HResult = 0;
	ID3DXBuffer* m_pErrorBuffer =NULL;
	UINT count;
	LPDIRECT3DTEXTURE9 m_pTex=NULL;
public:
	ID3DXBuffer* m_pVSShader = NULL;
	ID3DXBuffer* m_pPSShader = NULL;
	void ComplieVertexShader(const char* fileName, LPDIRECT3DDEVICE9 g_Device);
	void CompliePixelShader(const char* fileName, LPDIRECT3DDEVICE9 g_Device);
	
	void SetHandle(LPDIRECT3DDEVICE9 g_Device, const char* PropertyName, void* pdata, e_PropertyType pt);
	void SetShader(LPDIRECT3DDEVICE9 g_Device);
	void GetTextureOutSide( LPDIRECT3DTEXTURE9 pSourceTex);
	void SetTexture(LPDIRECT3DDEVICE9 g_Device,const char* PropertyName);

	void CreateTexture(const char* fileName, LPDIRECT3DDEVICE9 g_Device);
	void CleanUpshader();
	~D3dShader();
};

