 #include<d3d9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#include<string>
#include"CD3d.h"
#include"myLight.h"

#define WINDOW_CLASS "D3D Class"
#define WINDOW_NAME "MY D3D "
#define id_BTN 005
#define id_DirectLight1 11
#define id_DirectLight2 12
#define id_SpotLight1 21
#define id_SpotLight2 22
#define id_PointLight1 31
#define id_PointLight2 32
void OnMouseMove(LPARAM lParam);
void CameraRotate(LPARAM lParam);
CD3d myD3d;
float lastX=0.0f, lastY=0.f;
float offsetX = 0.0f, offsetY = 0.0f;
bool LbtnDown = false;

LRESULT WINAPI MsgPro(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HMENU hmenu = GetMenu(hwnd);
	HMENU hSubMenu = GetSubMenu(hmenu, 0);

	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		break;
	case WM_LBUTTONDOWN:
		LbtnDown = true;
		lastX = LOWORD(lParam);
		lastY = HIWORD(lParam);
		OutputDebugString("WM_LBUTTONDOWN\n");
		break;
	case WM_MOUSEMOVE :
		if (LbtnDown) {
			CameraRotate(lParam);
			//OutputDebugString(to_string(HIWORD(lParam)).c_str());
			//OutputDebugString("\n");
		}
		break; 
	case WM_LBUTTONUP :
		LbtnDown = false;
		OutputDebugString("WM_LBUTTONUP\n");
		break;
	case WM_MOUSELEAVE:
		LbtnDown = false;
		OutputDebugString("Mouse Leave\n");
		break;
	case WM_MOUSEWHEEL:
	{
		auto zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
		if (zDelta > 0) {
			myD3d.ScaleCameraFov(1 + 0.2);
		}
		else
		{
			myD3d.ScaleCameraFov(1 - 0.2);
		}
	}
	case WM_KEYDOWN:
		switch (wParam)
		{
			case VK_ESCAPE:
				PostQuitMessage(0);
				break;
			case VK_UP:
				myD3d.MoveCameraY(0.3f);
				break;
			case VK_DOWN:
				myD3d.MoveCameraY(-0.3f);
				break;
			case VK_LEFT:
				myD3d.MoveCameraX(-1.0f);
				break;
			case VK_RIGHT:
				myD3d.MoveCameraX(1.0f);
				break;
			case VK_F2:
				myD3d.MoveLight(-0.5f, 0.0f, 0.0f);
				break;
			case VK_F3:
				myD3d.MoveLight(0.5f, 0.0f, 0.0f);
				break;
			case VK_F4:
				//myD3d.MoveLight(0.0f, -0.5f, 0.0f);
				myD3d.RotateLightVertical(2.0f);
				break;
			case VK_F5:
				//myD3d.MoveLight(0.0f, 0.5f, 0.0f);
				myD3d.RotateLightVertical(-2.0f);
				break;
			default:
				break;
		}
		break;
	case WM_COMMAND:
			switch (LOWORD(wParam))
			{
			case id_SpotLight1:
				myD3d.ChangeLight(LT_SpotLight, ST_VertexShader);
				break;
			case id_SpotLight2:
				myD3d.ChangeLight(LT_SpotLight, ST_PixelShader);
				break;	
			case id_DirectLight1:
				myD3d.ChangeLight(LT_DirectLight, ST_VertexShader);
				break;
			case id_DirectLight2:
				myD3d.ChangeLight(LT_DirectLight, ST_PixelShader);
				break;
			case id_PointLight1:
				myD3d.ChangeLight(LT_PointLight, ST_VertexShader);
				break;
			case id_PointLight2:
				myD3d.ChangeLight(LT_PointLight, ST_PixelShader);
				break;
			default:
				break;
			}
		break;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
HMENU hm;

void CreateMyMenu() {
//	HMENU hm;
	hm = CreateMenu();
	HMENU pop1 = CreatePopupMenu();
	HMENU pop2 = CreatePopupMenu();
	
	AppendMenu(hm, MF_POPUP, (UINT_PTR)pop1, "light");
	AppendMenu(hm, MF_POPUP, (UINT_PTR)pop2, "texture");
	HMENU directMenu = CreateMenu();
	AppendMenu(pop1, MF_POPUP, (UINT_PTR)directMenu, "DirectLight");
	AppendMenu(directMenu, MF_STRING, id_DirectLight1, "Vertex");
	AppendMenu(directMenu, MF_STRING, id_DirectLight2, "Pixel");
	HMENU pointMenu = CreateMenu();
	AppendMenu(pop1, MF_POPUP, (UINT_PTR)pointMenu, "PointLight");
	AppendMenu(pointMenu, MF_STRING, id_PointLight1, "Vertex");
	AppendMenu(pointMenu, MF_STRING, id_PointLight2, "Pixel");
	HMENU spotMenu = CreateMenu();
	AppendMenu(pop1, MF_POPUP, (UINT_PTR)spotMenu, "SpotLight");
	AppendMenu(spotMenu, MF_STRING, id_SpotLight1, "Vertex");
	AppendMenu(spotMenu, MF_STRING, id_SpotLight2, "pixel");

}
void CreateMyButton(HWND hwnd) {
	HWND hwndButton = CreateWindow(
		"Button",
		"click me",
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		200, 180,
		100, 50,
		hwnd,
		(HMENU)id_BTN,	//菜单句柄
		(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
		NULL
	);
}

int  WINAPI WinMain(HINSTANCE hInst,HINSTANCE prevhInst, LPSTR cmdLine, int show)
{
	//1.窗口类
	WNDCLASSEX wc =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		MsgPro,
		0L,0L,
		hInst,
		NULL,NULL,NULL,NULL,
		WINDOW_CLASS,
		NULL
	};
	//2.注册窗口类
	RegisterClassEx(&wc);
	CreateMyMenu();
	//3.hwnd句柄
	HWND hWnd = CreateWindow(
		WINDOW_CLASS, WINDOW_NAME, WS_OVERLAPPEDWINDOW,
		100, 100, 640, 480, GetDesktopWindow(), 
		hm, //菜单资源
		wc.hInstance, NULL);
	//4.渲染流程
	//CD3d myD3d;
	if (myD3d.initializeD3D(hWnd, false))
	{
		//显示窗口
		ShowWindow(hWnd,SW_SHOWDEFAULT);
		UpdateWindow(hWnd);
		//CreateMyButton(hWnd);
		//消息循环
		MSG msg;
		ZeroMemory(&msg, sizeof(msg));
		while (msg.message != WM_QUIT)
		{
			if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				//先更新再渲染
				myD3d.Update();
				myD3d.Render();
				
			}
			
		}
		
	}
	

	//退出时释放缓存
	myD3d.CleanUp();
	//5.取消窗口注册
	UnregisterClass(WINDOW_CLASS, wc.hInstance);
	return 0;
}


void CameraRotate(LPARAM lParam)
{
	float curX = LOWORD(lParam);
	float curY = HIWORD(lParam);
	offsetX = curX - lastX;
	offsetY = curY - lastY;
	myD3d.RotateCameraL(-offsetX * 0.08);
	myD3d.RotateCameraV(offsetY * 0.1);
	lastX = curX;
	lastY = curY;

}
void OnMouseUp(LPARAM lParam) {

}

