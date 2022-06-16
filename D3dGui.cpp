#pragma once
#include "D3dGui.h"

D3dGui::D3dGui() {
	m_WindowHeight = 640;
	m_WindowHeight = 480;

}
D3dGui::~D3dGui(){

}
/*void D3dGui::AddButton(int id, float x, float y, const char* background = NULL) {
	GUIControl uiButton;
	uiButton.type = Type_Button;
	uiButton.posX = x;
	uiButton.posY = y;
	uiButton.width = 100;
	uiButton.hight = 50;
	if (m_pControl == NULL) {
		m_pControl = new GUIControl[1];
	}
	else
	{
		GUIControl* pTempControl = new GUIControl[m_iControlNum + 1];
	
	}
	//m_pControl = &uiButton;

}*/