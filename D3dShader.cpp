#pragma once
#include "D3dShader.h"
#include<typeinfo>
#include<string>
using namespace std;

void D3dShader::ComplieVertexShader(const char* fileName, LPDIRECT3DDEVICE9 g_Device) {
	m_HResult = D3DXCompileShaderFromFile(
		fileName, 0, 0,
		"my_VS",
		"vs_3_0",
		D3DXSHADER_DEBUG  /*D3DXSHADER_USE_LEGACY_D3DX9_31_DLL*/,
		&m_pVSShader,
		&m_pErrorBuffer,
		&m_pVSConstTable );
	if (m_pErrorBuffer)
	{
		//处理错误和警告
		const char* szp = (const char *)m_pErrorBuffer->GetBufferPointer();
		MessageBox(NULL, szp, "Tips :", MB_OK);
	}
	if (FAILED(m_HResult)) {
		return;
	}
	m_HResult = g_Device->CreateVertexShader(
		(DWORD*)m_pVSShader->GetBufferPointer(),
		&m_pVertexShader);
}
void D3dShader::CompliePixelShader(const char* fileName, LPDIRECT3DDEVICE9 g_Device) {
	m_HResult = D3DXCompileShaderFromFile(
		fileName, 0, 0,
		"my_PS",
		"ps_3_0",
		D3DXSHADER_DEBUG ,
		&m_pPSShader,
		&m_pErrorBuffer,
		&m_pPSConstTable);
	if (m_pErrorBuffer)
	{
		//处理错误和警告
		const char* szp = (const char*)m_pErrorBuffer->GetBufferPointer();
		MessageBox(NULL, szp, "Tips :", MB_OK);
	}
	if (FAILED(m_HResult)) {
		MessageBox(NULL, "compile failed", "Tips :", MB_OK);
		return;
	}
	m_HResult = g_Device->CreatePixelShader(
		(DWORD*)m_pPSShader->GetBufferPointer(),
		&m_pPixelShader );
}
void D3dShader::SetHandle(LPDIRECT3DDEVICE9 g_Device, const char* PropertyName, void* pdata, e_PropertyType pt) {
	D3DXHANDLE vsHandle =NULL;
	D3DXHANDLE psHandle = NULL;
	if (m_pVSConstTable != NULL) {
		 vsHandle = m_pVSConstTable->GetConstantByName(NULL, PropertyName);
	}
	if(m_pPSConstTable != NULL) {
		 psHandle = m_pPSConstTable->GetConstantByName(NULL, PropertyName);
		 D3DXCONSTANTTABLE_DESC des;
		 m_pPSConstTable->GetDesc(&des);
		 des.Version = 100;
	}



	if (vsHandle != NULL) {
		switch (pt)
		{
		case pt_vector:
			if (FAILED(m_pVSConstTable->SetVector(g_Device, vsHandle, (const D3DXVECTOR4*)pdata))) {
				string s = "cannot set handle ";
				s += PropertyName;
				MessageBox(NULL, s.c_str(), "Tips :", MB_OK);
			}
			break;
		case pt_matrix:
			if (FAILED(m_pVSConstTable->SetMatrix(g_Device, vsHandle, (const D3DXMATRIX*)pdata))) {
				string s = "cannot set handle ";
				s += PropertyName;
				MessageBox(NULL, s.c_str(), "Tips :", MB_OK);
			}
			break;
		case pt_float:
			if (FAILED(m_pVSConstTable->SetFloat(g_Device, vsHandle, *(float*)pdata))) {
				string s = "cannot set handle ";
				s += PropertyName;
				MessageBox(NULL, s.c_str(), "Tips :", MB_OK);
			}
		default:
			break;
		}
	}
	if (psHandle != NULL) {
		switch (pt)
		{
		case pt_vector:
			if (FAILED(m_pPSConstTable->SetVector(g_Device, psHandle, (const D3DXVECTOR4*)pdata))) {
				string s = "cannot set handle ";
				s += PropertyName;
				MessageBox(NULL, s.c_str(), "Tips :", MB_OK);
			}
			break;
		case pt_matrix:
			if (FAILED(m_pPSConstTable->SetMatrix(g_Device, psHandle, (const D3DXMATRIX*)pdata))) {
				string s = "cannot set handle ";
				s += PropertyName;
				MessageBox(NULL, s.c_str(), "Tips :", MB_OK);
			}
			break;
		case pt_float:
			if (FAILED(m_pPSConstTable->SetFloat(g_Device, psHandle, *(float*)pdata))) {
				string s = "cannot set handle ";
				s += PropertyName;
				MessageBox(NULL, s.c_str(), "Tips :", MB_OK);
			}
		default:
			break;
		}
	}
}
void D3dShader::CreateTexture(const char* fileName, LPDIRECT3DDEVICE9 g_Device) {
	if (fileName == "") {
		return;
	}
	D3DXCreateTextureFromFile(g_Device, fileName, &m_pTex);
}

void D3dShader::SetTexture(LPDIRECT3DDEVICE9 g_Device,const char* PropertyName) {
	D3DXCONSTANT_DESC texDesc;
	D3DXHANDLE handle=NULL;
	if (m_pPSConstTable != NULL) {
		 handle = m_pPSConstTable->GetConstantByName(NULL, PropertyName);
	}
	if (handle == NULL)
	{
		string s;
		s = "PropertyName : ";
		s += PropertyName;
		s += " is not found.";
	//	MessageBox(NULL, s.c_str(), "Error :", MB_OK);
		return;
	}
	m_pPSConstTable->GetConstantDesc(handle,&texDesc,&count);
	m_pPSConstTable->SetDefaults(g_Device);
	g_Device->SetTexture(texDesc.RegisterIndex, m_pTex);
	g_Device->SetSamplerState(texDesc.RegisterIndex, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_Device->SetSamplerState(texDesc.RegisterIndex, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_Device->SetSamplerState(texDesc.RegisterIndex, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	g_Device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_BORDER);
	g_Device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_BORDER);
	
}

void D3dShader::SetShader(LPDIRECT3DDEVICE9 g_Device) {
	if (m_pVertexShader != NULL) {
		g_Device->SetVertexShader(m_pVertexShader);
	}
	if (m_pPixelShader != NULL) {
		g_Device->SetPixelShader(m_pPixelShader);
	}
	else
	{
		g_Device->SetPixelShader(NULL);
	}
}
void D3dShader::GetTextureOutSide( LPDIRECT3DTEXTURE9 pSourceTex) {
	m_pTex = pSourceTex;
}

D3dShader::~D3dShader() {
	if (m_pPixelShader != NULL) {
		m_pPixelShader->Release();
		m_pPixelShader = NULL;
	}
	if (m_pVertexShader != NULL){
		m_pVertexShader->Release();
		m_pVertexShader = NULL;
		}
	if (m_pVSShader != NULL) {
		m_pVSShader->Release();
		m_pVSShader = NULL;
	}
	if (m_pPSShader != NULL) {
		m_pPSShader->Release();
		m_pPSShader = NULL;
	}
	if (m_pVSConstTable != NULL) {
		m_pVSConstTable->Release();
		m_pVSConstTable = NULL;
	}
	if (m_pPSConstTable != NULL) {
		m_pPSConstTable->Release();
		m_pVSConstTable = NULL;
	}
	if (m_pTex != NULL) {
		m_pTex->Release();
		m_pTex = NULL;
	}
}

void D3dShader::CleanUpshader() {
	if (m_pPixelShader != NULL) {
		m_pPixelShader->Release();
		m_pPixelShader = NULL;
	}
	if (m_pVertexShader != NULL) {
		m_pVertexShader->Release();
		m_pVertexShader = NULL;
	}
	if (m_pVSShader != NULL) {
		m_pVSShader->Release();
		m_pVSShader = NULL;
	}
	if (m_pPSShader != NULL) {
		m_pPSShader->Release();
		m_pPSShader = NULL;
	}
	if (m_pVSConstTable != NULL) {
		m_pVSConstTable->Release();
		m_pVSConstTable = NULL;
	}
	if (m_pPSConstTable != NULL) {
		m_pPSConstTable->Release();
		m_pPSConstTable = NULL;
	}
	
}