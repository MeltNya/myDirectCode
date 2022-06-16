/*
GUI类
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

//控件结构
struct GUIControl {
    int type;
    int id;
    unsigned long  color;           //文本颜色
    float posX, posY;
    float width, hight;
    const char* text;                  //控件的文本内容
    LPDIRECT3DTEXTURE9 background;
};

class D3dGui{
public:
	LPD3DXFONT* m_pFonts = NULL;   //D3D字体对象
    GUIControl* m_pControl = NULL;   //多个控件对象
    int m_iControlNum = 0;
    LPDIRECT3DVERTEXBUFFER9* m_pVertexBuffer=NULL;    //顶点缓存
   // GUIControl m_Background;  //背景图对象
    int m_WindowWidth;    //ui窗口宽度
    int m_WindowHeight;   //ui窗口高度
    D3dGui();
    ~D3dGui();
    void AddButton(int id, float x, float y, const char* background = NULL);
    void AddText();
    void CreateTextFont();
};