#include<d3d9.h>
#include<d3dx9.h>
#include "D3dQuad.h"
/*D3dQuad::D3dQuad() {
	p_Vertices= new CustomVertex[4];
	p_Vertices[0].position= D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
	p_Vertices[1].position = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
	p_Vertices[2].position = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	p_Vertices[3].position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);

	/*p_Vertices[0].tu = 0.0f, p_Vertices[0].tv = 1.0f;
	p_Vertices[1].tu = 0.0f, p_Vertices[1].tv = 0.0f;
	p_Vertices[2].tu = 1.0f, p_Vertices[2].tv = 1.0f;
	p_Vertices[3].tu = 1.0f, p_Vertices[3].tv = 1.0f;*/
/*
	p_Indices = new WORD[6];
	p_Indices[0] = 0; p_Indices[1] = 1; p_Indices[2] = 2;
	p_Indices[3] = 0; p_Indices[4] = 2; p_Indices[5] = 3;
	D3DXVECTOR3 n;
	ComputeAFaceNormal(&p_Vertices[0].position, &p_Vertices[1].position, &p_Vertices[2].position, &n);
	p_Vertices[0].normal = D3DXVECTOR3(p_Vertices[0].position.x + n.x, p_Vertices[0].position.y + n.y, p_Vertices[0].position.z + n.z);
	p_Vertices[1].normal = D3DXVECTOR3(p_Vertices[1].position.x + n.x, p_Vertices[1].position.y + n.y, p_Vertices[1].position.z + n.z);
	p_Vertices[2].normal = D3DXVECTOR3(p_Vertices[2].position.x + n.x, p_Vertices[2].position.y + n.y, p_Vertices[2].position.z + n.z);
	p_Vertices[3].normal = D3DXVECTOR3(p_Vertices[3].position.x + n.x, p_Vertices[3].position.y + n.y, p_Vertices[2].position.z + n.z);
}
*/

