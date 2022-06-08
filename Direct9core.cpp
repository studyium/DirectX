#include <Windows.h>

#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")

#define WINDOW_CLASS "UGPDX"
#define WINDOW_TITLE "Demo Window"
#define WIN_WIDTH 640
#define WIN_HEIGHT 480

bool InitializeD3D(HWND hWnd, bool fullscreen);
bool InitializeObjects();
void RenderScreen();
void Shutdown();

LPDIRECT3D9 g_D3D = NULL;
LPDIRECT3DDEVICE9 g_D3DDevice = NULL;

D3DXMATRIX g_projection;	//投影矩阵
D3DXMATRIX g_ViewMatrix; 	//观察矩阵
D3DXMATRIX g_WorldMatrix;	//世界矩阵

LRESULT CALLBACK MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		break;
	case WM_KEYUP:
		if(wParam == VK_ESCAPE) PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int APIENTRY WinMain(HINSTANCE hInst,
	HINSTANCE prevhInst,
	LPSTR     cmdLine,
	int       show)
{
	WNDCLASSEX wc = {sizeof(WNDCLASSEX),CS_CLASSDC,MsgProc,0,0,
		hInst,NULL,NULL,NULL,NULL,WINDOW_CLASS,NULL};
	RegisterClassEx(&wc);

	HWND hWnd = CreateWindowEx(NULL,WINDOW_CLASS,WINDOW_TITLE,WS_OVERLAPPEDWINDOW,
		100,100,WIN_WIDTH,WIN_HEIGHT,GetDesktopWindow(),NULL,hInst,NULL);

	if (InitializeD3D(hWnd,false))
	{
		ShowWindow(hWnd, SW_SHOWDEFAULT);
		UpdateWindow(hWnd);

		MSG msg;
		ZeroMemory(&msg, sizeof(msg));
		SetCursorPos(0,0);
		while (msg.message!=WM_QUIT)
		{
			if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				RenderScreen();
			}
		}
	}

	Shutdown();

	UnregisterClass(WINDOW_CLASS, hInst);
	return 0;
}

bool InitializeD3D(HWND hWnd, bool fullscreen)
{
	D3DDISPLAYMODE displayMode; //D3D显示模式

	g_D3D = Direct3DCreate9(D3D_SDK_VERSION);
	if(g_D3D == NULL) return false;

	//查询显卡模式
	if(FAILED(g_D3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &displayMode)))
	{
		return false;
	}

	//定义后台缓存表面的参数（包括深度模板缓存是否定义和像素格式)
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	//是否全屏
	if (fullscreen)
	{
		d3dpp.Windowed = FALSE;
		d3dpp.BackBufferWidth = WIN_WIDTH;
		d3dpp.BackBufferHeight = WIN_HEIGHT;
	}
	else
	{
		d3dpp.Windowed = TRUE;
	}
	d3dpp.BackBufferFormat = displayMode.Format; //后台缓存格式
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD; //交换效果，速度最快
	//创建D3D设备
	if (FAILED(g_D3D->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,hWnd,D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE,&d3dpp,&g_D3DDevice)))
	{
		return false;
	}

	if (!InitializeObjects())
	{
		return false;
	}
	return true;
}

bool InitializeObjects()
{
	//设置投影矩阵
	D3DXMatrixPerspectiveFovLH(&g_projection,45.0f,WIN_WIDTH/WIN_HEIGHT,0.1f,1000);
	g_D3DDevice->SetTransform(D3DTS_PROJECTION, &g_projection);
	
	//SetRenderState(); 设置渲染状态
	//关闭光照
	g_D3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//获取 世界->视图 变换矩阵
	D3DXVECTOR3 cameraPos(0.0f, 0.0f, -1.0f); //camera在世界坐标系中的位置向量
	D3DXVECTOR3 lookAtPos(0.0f, 0.0f, 0.0f); //camera的朝向向量，可以是原点，也可以是其它观察点
	D3DXVECTOR3 upDir(0.0f, 1.0f, 0.0f); //定义向上的方向，一般是[0,1,0]
	//根据上面给出的3个值建立一个基于左手坐标系的观察矩阵
	D3DXMatrixLookAtLH(&g_ViewMatrix, &cameraPos, &lookAtPos, &upDir);

	return true;
}

void RenderScreen()
{
	//设置视口矩形为黑色
	g_D3DDevice->Clear(0,NULL,D3DCLEAR_TARGET,D3DCOLOR_XRGB(0,0,0),1.0f,0);
	//Direct3D中规定所有绘制方法都必须在BeginScene()和EndScene()之间完成
	//开始渲染
	g_D3DDevice->BeginScene();

	//设置该矩阵为D3D的当前观察矩阵
	g_D3DDevice->SetTransform(D3DTS_VIEW, &g_ViewMatrix);
	
	
	//结束渲染
	g_D3DDevice->EndScene();
	//进行屏幕显示
	g_D3DDevice->Present(NULL,NULL,NULL,NULL);
}

void Shutdown()
{
	//释放所有DirectX资源
	if(g_D3DDevice != NULL) g_D3DDevice->Release();
	if(g_D3D != NULL) g_D3D->Release();

	g_D3DDevice = NULL;
	g_D3D = NULL;
}