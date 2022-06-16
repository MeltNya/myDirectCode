#pragma once
#include<d3d9.h>
#include<d3dx9.h>
#include<vector>
#include"myHelper.h"

using namespace std;
class CRef
{
	int m_iRefCount;

public:
	CRef() { m_iRefCount = 0; }

	void AddRef()
	{
		++m_iRefCount;
	}

	void Release()
	{
		--m_iRefCount;
		if (m_iRefCount <= 0)
		{
			delete this;
		}
	}
};
//顶点
struct CustomVertex
{
	D3DXVECTOR3 position;
	D3DXVECTOR3 normal;
	unsigned long color ;
	float tu, tv;
	CustomVertex() {
		//color = D3DCOLOR_XRGB(255, 0, 0);
		//color = 0xffff0000;
	}
};
struct LineVertex {
	D3DXVECTOR3 position;
	unsigned long color = D3DCOLOR_XRGB(0, 0, 0);
	LineVertex(D3DXVECTOR3 pos): position(pos){}
	LineVertex(D3DXVECTOR3 pos, unsigned long col) : position(pos) ,color(col){}
	LineVertex(){
	}
};
class D3dGeometryMesh : public CRef
{
protected:	
	LPDIRECT3DVERTEXBUFFER9 m_lpVertexBuffer =NULL;
	LPDIRECT3DVERTEXBUFFER9 m_LpLineVertexBuffer = NULL;                 //法线线段顶点的缓冲区
	LPDIRECT3DINDEXBUFFER9  m_lpIndexBuffer = NULL;
	void ComputeAFaceNormal(D3DXVECTOR3* pA, D3DXVECTOR3* pB, D3DXVECTOR3* pC, D3DXVECTOR3* pOut);
	void NegativeNormal(D3DXVECTOR3* pOut);
	void ClearBuffer();
	void CreateNormalLine(float length, unsigned long color, CustomVertex* _pVertex, LPDIRECT3DDEVICE9 g_Device);

public:
	int i_VertexCount = 0;
	int i_IndexCount = 0;
	int i_TriangleCount = 0;
	void CreateCylinder( float radius, float height, int layer, int slice, LPDIRECT3DDEVICE9 g_Device); //创建一个圆柱体
	void CreateQuad(float length, LPDIRECT3DDEVICE9 g_Device);
	void CreateQuadA(float length, int seg, LPDIRECT3DDEVICE9 g_Device);
	void CreateCube(float length, LPDIRECT3DDEVICE9 g_Device);

	void DrawMesh(LPDIRECT3DDEVICE9 g_Device); 
	void DrawLineForNormal( LPDIRECT3DDEVICE9 g_Device );
	
	D3dGeometryMesh();
	~D3dGeometryMesh();
	
};
