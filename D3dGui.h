/*
GUI��
*/
#pragma once
#include "d3dx9.h"
#define D3DFVF_GUI (D3DFVF_XYZRHW |D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define Type_Button 1
struct GUIVERTEX
{
    float x, y, z, rhw;
    unsigned long color;
    float tu, tv;
};

//�ؼ��ṹ
struct GUIControl {
    int type;
    int id;
    unsigned long  color;           //�ı���ɫ
    float posX, posY;
    float width, hight;
    const char* text;                  //�ؼ����ı�����
    LPDIRECT3DTEXTURE9 background;
};

class D3dGui{
public:
	LPD3DXFONT* m_pFonts = NULL;   //D3D�������
    GUIControl* m_pControl = NULL;   //����ؼ�����
    int m_iControlNum = 0;
    LPDIRECT3DVERTEXBUFFER9* m_pVertexBuffer=NULL;    //���㻺��
   // GUIControl m_Background;  //����ͼ����
    int m_WindowWidth;    //ui���ڿ��
    int m_WindowHeight;   //ui���ڸ߶�
    D3dGui();
    ~D3dGui();
    void AddButton(int id, float x, float y, const char* background = NULL);
    void AddText();
    void CreateTextFont();
};