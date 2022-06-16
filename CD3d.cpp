#include<d3d9.h>
#include<d3dx9.h>
#include<math.h>
#include<time.h>
#include"CD3d.h"
#include <stdio.h>
#include"D3dGeometryMesh.h"
#include"D3dMaterial.h"
#include"D3dShader.h"
#include"D3dLight.h"


//定义FVF(坐标 颜色)
//#define D3DFVF_VERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)

bool CD3d::initializeD3D(HWND hwnd, bool fullscreen) {
	D3DDISPLAYMODE displayMode;
	//1.创建d3d对象
	g_D3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_D3D == nullptr) {
		return false;
	}
	//2.获取桌面显示模式
	if (FAILED(g_D3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &displayMode))) {
		return false;
	};
	//3.创建D3DPRESENT_PARAMETERS对象
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	//往D3DPRESENT_PARAMETERS里面填参
	if (fullscreen) {
		d3dpp.Windowed = FALSE;
		d3dpp.BackBufferWidth = 800;
		d3dpp.BackBufferHeight = 600;
	}
	else {
		d3dpp.Windowed = TRUE;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.BackBufferFormat = displayMode.Format;		//设置后台缓冲区格式，与显示模式一致
	}
	//使用z缓存
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;			//设置Z缓存格式为16bit
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;// D3DPRESENT_INTERVAL_DEFAULT;
	//d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	//4.创建d3d设备对象，该对象用于向屏幕渲染图形
	if (FAILED(g_D3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hwnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_D3DDevice
	))) {

		return false;
	}
	g_D3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	g_D3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	g_D3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	//摄像机设置
	m_MainCamera.MoveCamera(0.0f, 0.0f, 0.0f);
	m_MainCamera.RotateCameraVertical(0.0f);
	m_MainCamera.ScaleFov(1.0);
	//---------创建物体
	CreateObject();
	//d3d_Light.SetPosition(0.0f, 0.0f, 0.0f);
	return true;
}
void CD3d::CreateObject() {
	CreateShadowTexture();									//创建阴影用的纹理
	d3d_Mesh.CreateQuadA(300.0f, 100, g_D3DDevice);
	m_shadowMat.CreateMatrial(g_D3DDevice, "RenderShadowMapVS3.hlsl", "RenderShadowMapPS2.hlsl");
	//m_shadowMat.CreateMatrial(g_D3DDevice, "myVertexShader.hlsl", "spotPixelShader.hlsl");

	m_goMat.CreateMatrial(g_D3DDevice, "spotVertexShader2.hlsl", "spotPixelShader3.hlsl");  //带阴影图的spot材质
	m_spMat.CreateMatrial(g_D3DDevice, "myVertexShader.hlsl", "spotPixelShader.hlsl");     //正常渲染的spotlight材质
	m_testMat.CreateMatrial(g_D3DDevice, "myVertexShader.hlsl","testPixelShader.hlsl");   //给物体直接渲染阴影图的材质
	m_testMat2.CreateMatrial(g_D3DDevice, "myVertexShader.hlsl");

	vector<string> vec;
	//GetConstes(vec, m_testMat.m_shader.m_pPSConstTable);
	d3d_Mesh2.CreateCylinder(5.0f, 15.0f, 12, 30, g_D3DDevice);
	d3d_Mesh3.CreateCube(1.0f, g_D3DDevice);
	d3d_Mesh4.CreateCube(5.0f, g_D3DDevice);
	//初始化环境光
	m_Ambient = D3DXVECTOR4 (0.0f, 1.0f, 1.0f, 1.0f);
	m_goMat.SetPropertyForShader(g_D3DDevice, "ambientColor", &m_Ambient, pt_vector);
	m_testMat.SetPropertyForShader(g_D3DDevice, "ambientColor", &m_Ambient, pt_vector);
	d3d_Light.CreateSpotLight(
		D3DXVECTOR4(-3.0f,8.0f, -12.0f, 0.0f),
		D3DXVECTOR4(0.50f,-0.5f, 1.0f, 0.0f),
		D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f),
		1.0f, 0.2f, 0.1f, cos(45* D3DX_PI / 180)
	);

	//D3dGeometryMesh* pMesh = CResourceManager::GetInstance()->ReqMesh("C:/1.obj"); //new D3dGeometryMesh();

	CGameObject* obj1 = new CGameObject();
	obj1->SetMesh(&d3d_Mesh);
	obj1->SetMaterial(&m_goMat);
	//obj1->SetMaterial(&m_testMat);
	//obj1->SetMaterial(&m_spMat);
	obj1->SetPosition(5.0f, -3.0f, 0.0f);
	m_vecGameObjcets.push_back(obj1);

	CGameObject* obj2 = new CGameObject();
	obj2->SetMesh(&d3d_Mesh2);
	obj2->SetMaterial(&m_goMat);
	//obj2->SetMaterial(&m_shadowMat);
	obj2->SetPosition(2.0f, 0.0f, 8.0f);
	m_vecGameObjcets.push_back(obj2);

	CGameObject* obj3 = new CGameObject();
	obj3->SetMesh(&d3d_Mesh4);
	obj3->SetMaterial(&m_goMat);
	obj3->SetScale(0.5f, 0.5f, 0.5f);
	//obj5->SetMaterial(&m_testMat);
	obj3->SetPosition(11.0f, 12.0f, -2.0f);
	//obj1->SetPosition(5.0f, -5.0f, 0.0f);
	m_vecGameObjcets.push_back(obj3);

	CGameObject* obj4 = new CGameObject();
	obj4->SetMesh(&d3d_Mesh4);
	obj4->SetMaterial(&m_goMat);
	//obj4->SetMaterial(&m_shadowMat);
	obj4->SetPosition(-2.0f,5.0f,-2.0f);
	m_vecGameObjcets.push_back(obj4);

	CGameObject* obj5 = new CGameObject();
	obj5->SetMesh(&d3d_Mesh3);
	//obj3->SetMaterial(&m_goMat);
	obj5->SetMaterial(&m_goMat);
	obj5->SetPosition(d3d_Light.m_LightPos.x, d3d_Light.m_LightPos.y, d3d_Light.m_LightPos.z);
	m_vecGameObjcets.push_back(obj5);

	InitFont();
}

void CD3d::Update()
{
	for (size_t st = 0; st < m_vecGameObjcets.size(); ++st)
	{
		m_vecGameObjcets[st]->Update();
	}
//	m_vecGameObjcets[4]->SetPosition(d3d_Light.m_LightPos.x, d3d_Light.m_LightPos.y, d3d_Light.m_LightPos.z);
	GetFps();
}

void CD3d::CleanUp()
{
	if (g_pFont != NULL) {
		g_pFont->Release();
	}
	if (g_D3DDevice != nullptr) {
		g_D3DDevice->Release();
	}
	if (g_D3D != nullptr) {
		g_D3D->Release();
	}
	g_D3DDevice = NULL;
	g_D3D = NULL;
	g_pFont = NULL;
	if (m_ShadowMapRT != NULL) {
		m_ShadowMapRT->Release();
		m_ShadowMapRT = NULL;
	}
	if (m_ShadowMapDS != NULL) {
		m_ShadowMapDS->Release();
		m_ShadowMapDS = NULL;
	}
}

/*创建文字*/
void CD3d::InitFont() {
	D3DXFONT_DESC dfont;
	ZeroMemory(&dfont, sizeof(D3DXFONT_DESC));
	dfont.Height = 20;
	dfont.Width = 15;
	dfont.MipLevels = D3DX_DEFAULT;
	dfont.Italic = false;
	dfont.CharSet = DEFAULT_CHARSET;
	dfont.OutputPrecision = 0;
	dfont.Quality = 0;
	dfont.PitchAndFamily = 0;
	D3DXCreateFontIndirect(g_D3DDevice, &dfont, &g_pFont);
}
void CD3d::RenderFont() {

	g_D3DDevice->SetVertexShader(NULL);
	g_D3DDevice->SetPixelShader(NULL);

	//RECT rect = { 0, 0, 200,50 };
	//绘制文本
	g_pFont->DrawTextA(NULL, s_FpsWord, 20,
		&m_Rect, DT_LEFT, D3DCOLOR_XRGB(255, 0, 0)
	);

}
void CD3d::ComputeWorldMatrix(D3DXVECTOR3 scale, D3DXVECTOR3 rotate, D3DXVECTOR3 trans, D3DXMATRIX* outMat) {
	D3DXMATRIX scaleMat;
	D3DXMatrixScaling(&scaleMat, scale.x, scale.y, scale.z);
	D3DXMATRIX transMat;
	D3DXMatrixTranslation(&transMat, trans.x, trans.y, trans.z);
	D3DXMATRIX rotateXmat;
	D3DXMatrixRotationX(&rotateXmat, rotate.x * D3DX_PI / 180);
	D3DXMATRIX rotateYMat;
	D3DXMatrixRotationY(&rotateYMat, rotate.y * D3DX_PI / 180);
	D3DXMATRIX rotateZMat;
	D3DXMatrixRotationZ(&rotateZMat, rotate.z * D3DX_PI / 180);
	D3DXMATRIX WorldMat;
	*outMat = transMat * rotateXmat * rotateYMat * rotateZMat * scaleMat;
}
void  CD3d::ChangeLight(int lightType, int ShaderType) {
	switch (lightType)
	{
		case (LT_DirectLight): {
			if (ShaderType == ST_VertexShader) 
			{
				m_goMat.CreateMatrial(g_D3DDevice, "directVertexShader.hlsl","");
			}
			else {
				m_goMat.CreateMatrial(g_D3DDevice, "myVertexShader.hlsl", "directPixelShader.hlsl");
			}
			

			m_goMat.SetPropertyForShader(g_D3DDevice, "ambientColor", &m_Ambient, pt_vector);
			d3d_Light.CreateDirectLight(
				D3DXVECTOR4(0.50f, 0.5f, 0.50f, 0.0f),
				D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f)
			);
			break;
		}
		case(LT_PointLight): {
			if (ShaderType == ST_VertexShader) {
				m_goMat.CreateMatrial(g_D3DDevice, "pointVertexShader.hlsl","");
			}
			else {
				m_goMat.CreateMatrial(g_D3DDevice, "myVertexShader.hlsl", "pointPixelShader.hlsl");
			}
			d3d_Light.CreatePointLight(
				D3DXVECTOR4(-5.0f, 1.0f, -2.0f, 0.0f),
				D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f),
				1.0f, 0.1f, 0.1f
			);
			m_goMat.SetPropertyForShader(g_D3DDevice, "ambientColor", &m_Ambient, pt_vector);
			break;
		}
		case(LT_SpotLight): 
		{
			if (ShaderType == ST_VertexShader) {
				m_goMat.CreateMatrial(g_D3DDevice, "spotVertexShader.hlsl","");
			}
			else {
				m_goMat.CreateMatrial(g_D3DDevice, "myVertexShader.hlsl", "spotPixelShader.hlsl");
			}
			d3d_Light.CreateSpotLight(
				D3DXVECTOR4(5.0f, 5.0f, -12.0f, 0.0f),
				D3DXVECTOR4(0.50f, -0.5f, 0.50f, 0.0f),
				D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f),
				1.0f, 0.2f, 0.1f, cos(35 * D3DX_PI / 180)
			);
			m_goMat.SetPropertyForShader(g_D3DDevice, "ambientColor", &m_Ambient, pt_vector);
			break;
		}
		default:
			break;
	}
}
void CD3d::GetFps() {
	float fps=0;
	dw_CurrentTime = timeGetTime();// *0.001f;
	i_FrameCount++;
	if (dw_CurrentTime - dw_LastTime > 1000)
	{
		fps = (float)i_FrameCount / ((dw_CurrentTime - dw_LastTime) * 0.001f);
		sprintf_s(s_FpsWord, 20, "FPS: %.2f", fps);
		dw_LastTime = dw_CurrentTime;
		i_FrameCount = 0;
	}
}
void  CD3d::MoveCameraZ(float z) {
	m_MainCamera.MoveCamera(0.0f, 0.0f, z);
}
void CD3d::MoveCameraX(float x) {
	m_MainCamera.MoveCamera(x, 0.0f, 0.0f);
}
void CD3d::MoveCameraY(float y) {
	m_MainCamera.MoveCamera(0.0f, y, 0.0f);
}
void   CD3d::RotateCameraL(float l) {
	m_MainCamera.RotateCameraLevel(l);
}
void   CD3d::RotateCameraV(float v) {
	m_MainCamera.RotateCameraVertical(v);
}
void  CD3d::ScaleCameraFov(float a) {
	m_MainCamera.ScaleFov(a);
}

void CD3d::RenderShadowMap()
{
	LPDIRECT3DSURFACE9 oldRenderTarget = NULL;
	LPDIRECT3DSURFACE9 ShadowSurface = NULL;
	LPDIRECT3DSURFACE9 oldDepthSurface = NULL;
	if (FAILED(g_D3DDevice->GetRenderTarget(0, &oldRenderTarget))) {
		MessageBox(NULL, "there is no rendertagert", "error:", MB_OK);
	}//保存当前的渲染目标和surface
	if (FAILED(g_D3DDevice->GetDepthStencilSurface(&oldDepthSurface))) {
		MessageBox(NULL, "there is no Depth Surface", "error:", MB_OK);
	}
	m_ShadowMapRT->GetSurfaceLevel(0, &ShadowSurface);								//获得一个surface用来保存纹理对象

	if (FAILED(g_D3DDevice->SetRenderTarget(0, ShadowSurface))) {					//设置新的渲染目标为阴影surface
		MessageBox(NULL, "cant change the render target", "error:", MB_OK);       
	}						 
	if (FAILED(g_D3DDevice->SetDepthStencilSurface(m_ShadowMapDS))){
		MessageBox(NULL, "cant change the denpth surface", "error:", MB_OK);
	}
	//清理surface和深度缓存
	g_D3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(255, 0, 0), 1.0f, 0);
	//渲染
	for (size_t st = 1; st < m_vecGameObjcets.size()-1; ++st)
	{
		
		m_vecGameObjcets[st]->RenderShadow(g_D3DDevice,d3d_Light,&m_shadowMat);
	}
	if (FAILED(g_D3DDevice->SetRenderTarget(0, oldRenderTarget))) {
		MessageBox(NULL, "there is no rendertagert", "error:", MB_OK);
		oldRenderTarget = NULL;
	}
	if (FAILED(g_D3DDevice->SetDepthStencilSurface(oldDepthSurface))) {
		MessageBox(NULL, "there is no dep surface", "error:", MB_OK);
		m_ShadowMapDS = NULL;
	}

	if (!m_saved) {
		D3DXSaveTextureToFile(
			"texShadow.jpg",
			D3DXIFF_JPG,
			m_ShadowMapRT,
			NULL
		);
		m_saved = TRUE;
	}

}

void CD3d::RenderScene()
{
	//清理surface
	g_D3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 100,50), 1.0f, 0);

	for (size_t st = 0; st < m_vecGameObjcets.size(); ++st)
	{

		m_vecGameObjcets[st]->RenderSelf(g_D3DDevice,d3d_Light,m_MainCamera,m_ShadowMapRT);
	//	m_vecGameObjcets[st]->RenderSelf(g_D3DDevice, d3d_Light, m_MainCamera);
	}
}

void  CD3d::Render()
{
	//渲染shadow
	g_D3DDevice->BeginScene();
	g_D3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0);

	RenderShadowMap();
	RenderScene();
	RenderFont();
	g_D3DDevice->EndScene();

	g_D3DDevice->Present(NULL, NULL, NULL, NULL);

}
/*创建阴影纹理和surface*/
void  CD3d::CreateShadowTexture() {
	HRESULT hr;

	hr = g_D3DDevice->CreateTexture(
		800, 600,
		1,
		D3DUSAGE_RENDERTARGET,
		//D3DFMT_A8R8G8B8,
		D3DFMT_R32F,
		D3DPOOL_DEFAULT,
		&m_ShadowMapRT,
		NULL
	);
	if (FAILED(hr)) {
		OutputDebugString("create shadow texture failed \n");
	}

	hr = g_D3DDevice->CreateDepthStencilSurface(
		800, 600,
		D3DFMT_D16,//D3DFMT_A8R8G8B8,
		D3DMULTISAMPLE_NONE,
		0, TRUE,
		&m_ShadowMapDS,
		NULL
	);
	if (FAILED(hr)) {
		OutputDebugString("create shadow depthStencilSurface failed \n");
	}
}
void   CD3d::MoveLight(float x, float y, float z) {
	d3d_Light.SetPosition(x, y, z);
}
void  CD3d::RotateLightVertical(float angle) {
	d3d_Light.RotateVerticcal(angle);
}
