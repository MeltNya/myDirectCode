#include<d3d9.h>
#include<d3dx9.h>
#include "D3dGeometryMesh.h"

#define D3DFVF_VERTEX (D3DFVF_XYZ |  D3DFVF_NORMAL| D3DFVF_DIFFUSE | D3DFVF_TEX1 )
#define D3DFVF_LINEVERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE)   
D3dGeometryMesh::~D3dGeometryMesh() {
	ClearBuffer();
}
D3dGeometryMesh::D3dGeometryMesh() {
}
void D3dGeometryMesh::ComputeAFaceNormal(D3DXVECTOR3* pA, D3DXVECTOR3* pB, D3DXVECTOR3* pC,D3DXVECTOR3* out) {
	D3DXVECTOR3 a  = *pB - *pA;
	D3DXVECTOR3 b = *pC - *pA;
	D3DXVec3Cross(out, &a, &b);
	D3DXVec3Normalize(out, out);
}
void D3dGeometryMesh::NegativeNormal(D3DXVECTOR3* pOut) {
	pOut->x = -1 * pOut->x;
	pOut->y = -1 * pOut->y;
	pOut->z = -1 * pOut->z;
}
void D3dGeometryMesh::ClearBuffer()
{
	if (m_lpIndexBuffer != NULL) {
		m_lpIndexBuffer->Release();
		m_lpIndexBuffer = NULL;
	}
	
	if (m_lpVertexBuffer != NULL) {
		m_lpVertexBuffer->Release();
		m_lpVertexBuffer = NULL;
	}
	
	if (m_LpLineVertexBuffer != NULL) {
		m_LpLineVertexBuffer->Release();
		m_LpLineVertexBuffer = NULL;
	}
}
void D3dGeometryMesh::CreateCylinder(float radius, float height, int layer, int slice , LPDIRECT3DDEVICE9 g_Device) {
	ClearBuffer();
	int len1 = (slice + 1) * (layer + 1);
	int tcount = 0;
	i_VertexCount = len1+2;
	CustomVertex* p_Vertex;
	g_Device->CreateVertexBuffer(i_VertexCount * sizeof(CustomVertex), 0, D3DFVF_VERTEX, D3DPOOL_MANAGED, &m_lpVertexBuffer, NULL);
	m_lpVertexBuffer->Lock(0, i_VertexCount * sizeof(CustomVertex), (void**)&p_Vertex, 0);
	//曲面
	for (int i = 0; i < layer + 1; i++) {
		for (int j = 0; j <= slice; j++) {
			float angle = j * float(360 / slice) * D3DX_PI / 180;
			p_Vertex[tcount].position = D3DXVECTOR3(cos(angle) * radius, i * height / layer, sin(angle) * radius );
			p_Vertex[tcount].normal = D3DXVECTOR3(cos(angle) ,0, sin(angle) );
			p_Vertex[tcount].color = 0xffff0000;
			p_Vertex[tcount].tu = 0 + (1.0f / slice) * j;
			p_Vertex[tcount].tv = 1 - (1.0f / layer) * i;
			tcount++;
		}
	}
	//顶面和底面的中心顶点
	p_Vertex[tcount].position = D3DXVECTOR3(0.0f, height, 0.0f);
	ComputeAFaceNormal(&p_Vertex[tcount].position, &p_Vertex[tcount - 1].position, &p_Vertex[tcount - 2].position, &p_Vertex[tcount].normal);
	p_Vertex[tcount++].color = D3DCOLOR_XRGB(100, 149, 237);
	p_Vertex[tcount].position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	ComputeAFaceNormal(&p_Vertex[0].position, &p_Vertex[1].position, &p_Vertex[2].position, &p_Vertex[tcount].normal);
	auto tn = p_Vertex[tcount].normal;
	p_Vertex[tcount++].color = D3DCOLOR_XRGB(100, 149, 237);

	CreateNormalLine(0.5f, D3DCOLOR_XRGB(255, 255, 255), p_Vertex, g_Device);
	m_lpVertexBuffer->Unlock();

	tcount = 0;
	i_IndexCount = (slice + 1) * layer * 6  + slice * 3 * 2;
	WORD* p_Index = new WORD[i_IndexCount];
	g_Device->CreateIndexBuffer(
		i_IndexCount * sizeof(WORD),
		0, D3DFMT_INDEX16,
		D3DPOOL_MANAGED, &m_lpIndexBuffer, NULL);

	m_lpIndexBuffer->Lock(0, i_IndexCount * sizeof(WORD), (void**)&p_Index, 0);
	for (int i = 0; i < layer; i++) {
		for (int j = 0; j < 31; j++) {
			p_Index[tcount++] = (i * (slice + 1) + j);
			p_Index[tcount++] = ((i + 1) * (slice + 1) + j);
			p_Index[tcount++] = ((i + 1) * (slice + 1) + j + 1);
			p_Index[tcount++] = (i * (slice + 1) + j);
		    p_Index[tcount++] = ((i + 1) * (slice + 1) + j + 1);
			p_Index[tcount++] = (i * (slice + 1) + j + 1);
			i_TriangleCount += 2;
		}
	}
	int i_TopCenter = i_VertexCount - 2;
	int i_BottomCenter = i_VertexCount - 1;
	for (int i = 0; i < slice; i++) {
		p_Index[tcount++] =i_TopCenter ;
		p_Index[tcount++] = i + (slice+1) * layer;
		p_Index[tcount++] = i + 1 + (slice+1) * layer;
		i_TriangleCount++;
	}
	for (int i = 0; i < slice; i++) {
		p_Index[tcount++] = i_BottomCenter;
		p_Index[tcount++] = i ;
		p_Index[tcount++] = i + 1 ;
		i_TriangleCount++;
	}
	m_lpIndexBuffer->Unlock();
	
}
void D3dGeometryMesh::CreateQuadA(float length, int seg, LPDIRECT3DDEVICE9 g_Device) {
	ClearBuffer();
	float side = length / 2;
	i_VertexCount = (1+seg)*(1+seg);
	CustomVertex* p_Vertex;
	g_Device->CreateVertexBuffer(i_VertexCount * sizeof(CustomVertex), 0, D3DFVF_VERTEX, D3DPOOL_MANAGED, &m_lpVertexBuffer, NULL);
	m_lpVertexBuffer->Lock(0, i_VertexCount * sizeof(CustomVertex), (void**)&p_Vertex, 0);
	int tcount = 0;
	for (int i = 0; i < seg + 1; i++) {
		for (int j = 0; j < seg + 1; j++) {
			auto t =( 2.0f / seg)*j;
			p_Vertex[tcount].position = D3DXVECTOR3(  (-1.0f +(2.0f/seg) * j )* side, 0.0f * side, (-1.0f + (2.0f / seg)* i ) * side);
			p_Vertex[tcount].color = 0xffff0000;
			p_Vertex[tcount].tu = 0.0f + (1.0f / seg) * j;
			p_Vertex[tcount].tv = 1.0f - (1.0f / seg) * i;
			tcount++;
		}
	}
	//法线
	D3DXVECTOR3 v_FaceNormal;
	ComputeAFaceNormal(&p_Vertex[0].position, &p_Vertex[1].position, &p_Vertex[2].position, &v_FaceNormal);
	for (int i = 0; i < i_VertexCount; i++) {
		p_Vertex[i].normal = D3DXVECTOR3(0.0f,1.0f,0.0f);
	}
	CreateNormalLine(1.0f, D3DCOLOR_XRGB(255, 240, 245), p_Vertex, g_Device);
	m_lpVertexBuffer->Unlock();
	tcount = 0;
	i_IndexCount = 6*(seg)*(seg);
	//i_TriangleCount = 2*seg*seg;
	WORD* p_Index = new WORD[i_IndexCount];
	g_Device->CreateIndexBuffer(
		i_IndexCount * sizeof(WORD),
		0, D3DFMT_INDEX16,
		D3DPOOL_MANAGED, &m_lpIndexBuffer, NULL);

	m_lpIndexBuffer->Lock(0, i_IndexCount * sizeof(WORD), (void**)&p_Index, 0);
	for (int i = 0; i < seg ; i++) {
		for (int j = 0; j < seg ; j++) {
			p_Index[tcount++] = i * (seg + 1) + j;
			p_Index[tcount++] = (i+1) * (seg + 1) + j;
			p_Index[tcount++] = (i + 1) * (seg + 1) + j+1;
			p_Index[tcount++] = i * (seg + 1) + j;
			p_Index[tcount++] = (i + 1) * (seg + 1) + j+1;
			p_Index[tcount++] = i * (seg + 1) + j+1;
			i_TriangleCount += 2;
		}
	}
	auto t1 = p_Index[24];
	auto t2= p_Index[23];
	auto t3 = p_Index[22];
	m_lpIndexBuffer->Unlock();

}
void D3dGeometryMesh::CreateQuad(float length, LPDIRECT3DDEVICE9 g_Device) {
	ClearBuffer();
	float side = length/2;
	i_VertexCount = 4;
	CustomVertex* p_Vertex;
	g_Device->CreateVertexBuffer(i_VertexCount * sizeof(CustomVertex), 0, D3DFVF_VERTEX, D3DPOOL_MANAGED, &m_lpVertexBuffer, NULL);
	m_lpVertexBuffer->Lock(0, i_VertexCount * sizeof(CustomVertex), (void**)&p_Vertex, 0);
	p_Vertex[0].position = D3DXVECTOR3(-1.0f * side, 0.0f * side, -1.0f*side);
	p_Vertex[0].color = 0xffff0000;
	p_Vertex[0].tu = 0.0f; p_Vertex[0].tv = 1.0f;
	p_Vertex[1].position = D3DXVECTOR3(-1.0f * side, 0.0f * side, 1.0f*side);
	p_Vertex[1].color = 0xffff0000;
	p_Vertex[1].tu = 0.0f; p_Vertex[1].tv = 0.0f;
	p_Vertex[2].position = D3DXVECTOR3(1.0f * side, 0.0f * side , 1.0f*side);
	p_Vertex[2].color = 0xffff0000;
	p_Vertex[2].tu = 1.0f; p_Vertex[2].tv = 0.0f;
	p_Vertex[3].position = D3DXVECTOR3(1.0f * side, 0.0f * side , -1.0f*side );
	p_Vertex[3].color = 0xffff0000;
	p_Vertex[3].tu = 1.0f; p_Vertex[3].tv = 1.0f;
	//法线向量
	D3DXVECTOR3 v_FaceNormal;
	ComputeAFaceNormal(&p_Vertex[0].position, &p_Vertex[1].position, &p_Vertex[2].position, &v_FaceNormal);
	for (int i = 0; i < i_VertexCount; i++) {
		p_Vertex[i].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	}
	CreateNormalLine(1.0f, D3DCOLOR_XRGB(255, 240, 245), p_Vertex, g_Device);
	m_lpVertexBuffer->Unlock();

	i_IndexCount = 6;
	WORD* p_Index = new WORD[i_IndexCount];
	g_Device->CreateIndexBuffer(
		i_IndexCount * sizeof(WORD),
		0, D3DFMT_INDEX16,
		D3DPOOL_MANAGED, &m_lpIndexBuffer, NULL);

	m_lpIndexBuffer->Lock(0, i_IndexCount * sizeof(WORD), (void**)&p_Index, 0);
	p_Index[0] = 0; p_Index[1] = 1; p_Index[2] = 2;
	p_Index[3] = 0; p_Index[4] = 2; p_Index[5] = 3;
	i_TriangleCount = 2;
	m_lpIndexBuffer->Unlock();

	
}

void  D3dGeometryMesh::CreateCube(float length, LPDIRECT3DDEVICE9 g_Device) {
	ClearBuffer();
	i_VertexCount = 24;
	float side = length / 2;
	CustomVertex* p_Vertex; //= new CustomVertex[i_VertexCount];
	g_Device->CreateVertexBuffer(i_VertexCount * sizeof(CustomVertex), 0, D3DFVF_VERTEX, D3DPOOL_MANAGED, &m_lpVertexBuffer, NULL);
	m_lpVertexBuffer->Lock(0, i_VertexCount * sizeof(CustomVertex), (void**)&p_Vertex, 0);
	//front
	p_Vertex[0].position = D3DXVECTOR3(-1.0f * side, -1.0f * side, -1.0f * side);
	p_Vertex[0].color = 0xffff0000;
	p_Vertex[0].tu = 0.0f; p_Vertex[0].tv = 1.0f;
	p_Vertex[1].position = D3DXVECTOR3(-1.0f * side, 1.0f * side, -1.0f * side);
	p_Vertex[1].color = 0xffff0000;
	p_Vertex[1].tu = 0.0f; p_Vertex[1].tv = 0.0f;
	p_Vertex[2].position = D3DXVECTOR3(1.0f * side, 1.0f * side, -1.0f * side);
	p_Vertex[2].color = 0xffff0000;
	p_Vertex[2].tu = 1.0f; p_Vertex[2].tv = 0.0f;
	p_Vertex[3].position = D3DXVECTOR3(1.0f * side, -1.0f * side, -1.0f * side);
	p_Vertex[3].color = 0xffff0000;
	p_Vertex[3].tu = 1.0f; p_Vertex[3].tv = 1.0f;
	for (int i = 0; i < 4; i++) {
		ComputeAFaceNormal(&p_Vertex[0].position, &p_Vertex[1].position, &p_Vertex[2].position, &p_Vertex[i].normal);
	}
	//back
	p_Vertex[4].position = D3DXVECTOR3(-1.0f * side, -1.0f * side, 1.0f * side);
	p_Vertex[4].color = 0xffff0000;
	p_Vertex[4].tu = 0.0f; p_Vertex[4].tv = 1.0f;
	p_Vertex[5].position = D3DXVECTOR3(-1.0f * side, 1.0f * side, 1.0f * side);
	p_Vertex[5].color = 0xffff0000;
	p_Vertex[5].tu = 0.0f; p_Vertex[5].tv = 0.0f;
	p_Vertex[6].position = D3DXVECTOR3(1.0f * side, 1.0f * side, 1.0f * side);
	p_Vertex[6].color = 0xffff0000;
	p_Vertex[6].tu = 1.0f; p_Vertex[6].tv = 0.0f;
	p_Vertex[7].position = D3DXVECTOR3(1.0f * side, -1.0f * side, 1.0f * side);
	p_Vertex[7].color = 0xffff0000;
	p_Vertex[7].tu = 1.0f; p_Vertex[7].tv = 1.0f;
	for (int i = 4; i <=7; i++) {
		ComputeAFaceNormal(&p_Vertex[4].position, &p_Vertex[5].position, &p_Vertex[6].position, &p_Vertex[i].normal);
		NegativeNormal(&p_Vertex[i].normal);
	}
	//Top
	p_Vertex[8].position = D3DXVECTOR3(-1.0f * side, 1.0f * side, -1.0f * side);
	p_Vertex[8].color = 0xffff0000;
	p_Vertex[8].tu = 0.0f; p_Vertex[8].tv = 1.0f;
	p_Vertex[9].position = D3DXVECTOR3(-1.0f * side, 1.0f * side, 1.0f * side);
	p_Vertex[9].color = 0xffff0000;
	p_Vertex[9].tu = 0.0f; p_Vertex[9].tv = 0.0f;
	p_Vertex[10].position = D3DXVECTOR3(1.0f * side, 1.0f * side, 1.0f * side);
	p_Vertex[10].color = 0xffff0000;
	p_Vertex[10].tu = 1.0f; p_Vertex[10].tv = 0.0f;
	p_Vertex[11].position = D3DXVECTOR3(1.0f * side, 1.0f * side, -1.0f * side);
	p_Vertex[11].color = 0xffff0000;
	p_Vertex[11].tu = 1.0f; p_Vertex[11].tv = 1.0f;
	for (int i = 8; i <= 11; i++) {
		ComputeAFaceNormal(&p_Vertex[8].position, &p_Vertex[9].position, &p_Vertex[10].position, &p_Vertex[i].normal);
	}
	//bottom
	p_Vertex[12].position = D3DXVECTOR3(-1.0f * side, -1.0f * side, -1.0f * side);
	p_Vertex[12].color = 0xffff0000;
	p_Vertex[12].tu = 0.0f; p_Vertex[12].tv = 1.0f;
	p_Vertex[13].position = D3DXVECTOR3(-1.0f * side, -1.0f * side, 1.0f * side);
	p_Vertex[13].color = 0xffff0000;
	p_Vertex[13].tu = 0.0f; p_Vertex[13].tv = 0.0f;
	p_Vertex[14].position = D3DXVECTOR3(1.0f * side, -1.0f * side, 1.0f * side);
	p_Vertex[14].color = 0xffff0000;
	p_Vertex[14].tu = 1.0f; p_Vertex[14].tv = 0.0f;
	p_Vertex[15].position = D3DXVECTOR3(1.0f * side, -1.0f * side, -1.0f * side);
	p_Vertex[15].color = 0xffff0000;
	p_Vertex[15].tu = 1.0f; p_Vertex[15].tv = 1.0f;
	for (int i = 12; i <= 15; i++) {
		ComputeAFaceNormal(&p_Vertex[12].position, &p_Vertex[13].position, &p_Vertex[14].position, &p_Vertex[i].normal);
		NegativeNormal(&p_Vertex[i].normal);
	}
	//left
	p_Vertex[16].position = D3DXVECTOR3(-1.0f * side, -1.0f * side, 1.0f * side);
	p_Vertex[16].color = 0xffff0000;
	p_Vertex[16].tu = 0.0f; p_Vertex[16].tv = 1.0f;
	p_Vertex[17].position = D3DXVECTOR3(-1.0f * side, 1.0f * side, 1.0f * side);
	p_Vertex[17].color = 0xffff0000;
	p_Vertex[17].tu = 0.0f; p_Vertex[17].tv = 0.0f;
	p_Vertex[18].position = D3DXVECTOR3(-1.0f * side, 1.0f * side, -1.0f * side);
	p_Vertex[18].color = 0xffff0000;
	p_Vertex[18].tu = 1.0f; p_Vertex[18].tv = 0.0f;
	p_Vertex[19].position = D3DXVECTOR3(-1.0f * side, -1.0f * side, -1.0f * side);
	p_Vertex[19].color = 0xffff0000;
	p_Vertex[19].tu = 1.0f; p_Vertex[19].tv = 1.0f;
	for (int i = 16; i <= 19; i++) {
		ComputeAFaceNormal(&p_Vertex[16].position, &p_Vertex[17].position, &p_Vertex[18].position, &p_Vertex[i].normal);
		//NegativeNormal(&p_Vertex[i].normal);
	}
	//right
	p_Vertex[20].position = D3DXVECTOR3(1.0f * side, -1.0f * side, -1.0f * side);
	p_Vertex[20].color = 0xffff0000;
	p_Vertex[20].tu = 0.0f; p_Vertex[20].tv = 1.0f;
	p_Vertex[21].position = D3DXVECTOR3(1.0f * side, 1.0f * side,- 1.0f * side);
	p_Vertex[21].color = 0xffff0000;
	p_Vertex[21].tu = 0.0f; p_Vertex[21].tv = 0.0f;
	p_Vertex[22].position = D3DXVECTOR3(1.0f * side, 1.0f * side, 1.0f * side);
	p_Vertex[22].color = 0xffff0000;
	p_Vertex[22].tu = 1.0f; p_Vertex[22].tv = 0.0f;
	p_Vertex[23].position = D3DXVECTOR3(1.0f * side, -1.0f * side, 1.0f * side);
	p_Vertex[23].color = 0xffff0000;
	p_Vertex[23].tu = 1.0f; p_Vertex[23].tv = 1.0f;
	for (int i = 20; i <= 23; i++) {
		ComputeAFaceNormal(&p_Vertex[20].position, &p_Vertex[21].position, &p_Vertex[22].position, &p_Vertex[i].normal);
		//NegativeNormal(&p_Vertex[i].normal);
	}
	CreateNormalLine(0.8f, D3DCOLOR_XRGB(255, 240, 245), p_Vertex, g_Device);
	m_lpVertexBuffer->Unlock();

	i_TriangleCount = 6 * 2;
	i_IndexCount = 6*2*3;
	WORD* p_Index = new WORD[i_IndexCount];
	g_Device->CreateIndexBuffer(
		i_IndexCount * sizeof(WORD),
		0, D3DFMT_INDEX16,
		D3DPOOL_MANAGED, &m_lpIndexBuffer, NULL);
	m_lpIndexBuffer->Lock(0, i_IndexCount * sizeof(WORD), (void**)&p_Index, 0);
	p_Index[0] = 0; p_Index[1] = 1; p_Index[2] = 2;  p_Index[3] = 0; p_Index[4] = 2; p_Index[5] = 3;
	p_Index[6] = 4; p_Index[7] = 5; p_Index[8] = 6; p_Index[9] = 4; p_Index[10] = 6; p_Index[11] = 7;
	p_Index[12] = 8; p_Index[13] = 9; p_Index[14] = 10; p_Index[15] = 8; p_Index[16] = 10; p_Index[17] = 11;
	p_Index[18] = 12; p_Index[19] = 13; p_Index[20] = 14; p_Index[21] = 12; p_Index[22] = 14; p_Index[23] = 15;
	p_Index[24] = 16; p_Index[25] = 17; p_Index[26] = 18; p_Index[27] = 16; p_Index[28] = 18; p_Index[29] = 19;
	p_Index[30] = 20; p_Index[31] = 21; p_Index[32] = 22; p_Index[33] = 20; p_Index[34] = 22; p_Index[35] = 23;
	m_lpIndexBuffer->Unlock();
	
}
void D3dGeometryMesh::DrawMesh(LPDIRECT3DDEVICE9 g_Device) {

	g_Device->SetFVF(D3DFVF_VERTEX);
	g_Device->SetStreamSource(0, m_lpVertexBuffer, 0, sizeof(CustomVertex));
	g_Device->SetIndices(m_lpIndexBuffer);
	g_Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, i_VertexCount, 0, i_TriangleCount);
}

void D3dGeometryMesh::DrawLineForNormal(LPDIRECT3DDEVICE9 g_Device) {
	if (m_LpLineVertexBuffer == NULL) {
		return;
	}
	g_Device->SetFVF(D3DFVF_LINEVERTEX);
	g_Device->SetStreamSource(0, m_LpLineVertexBuffer, 0, sizeof(LineVertex));
	g_Device->DrawPrimitive(D3DPT_LINELIST, 0, i_VertexCount);
	
}
void D3dGeometryMesh::CreateNormalLine(float length, unsigned long color, CustomVertex* p_Vertex, LPDIRECT3DDEVICE9 g_Device) {
	int i_LineVertexCount = i_VertexCount * 2;
	LineVertex* p_LineVertex;
	int tcount = 0;
	g_Device->CreateVertexBuffer(i_LineVertexCount * sizeof(LineVertex), 0, D3DFVF_LINEVERTEX, D3DPOOL_MANAGED, &m_LpLineVertexBuffer, NULL);
	m_LpLineVertexBuffer->Lock(0, i_LineVertexCount * sizeof(LineVertex), (void**)&p_LineVertex, 0);
	for (int i = 0; i < i_VertexCount; i++) {
		p_LineVertex[tcount].position = p_Vertex[i].position;
		p_LineVertex[tcount].color = color;
		tcount++;
		p_LineVertex[tcount].position = D3DXVECTOR3(
			p_Vertex[i].position.x + p_Vertex[i].normal.x * length,
			p_Vertex[i].position.y + p_Vertex[i].normal.y * length,
			p_Vertex[i].position.z + p_Vertex[i].normal.z * length);
		p_LineVertex[tcount].color = color;
		tcount++;
	}
	m_LpLineVertexBuffer->Unlock();
}
