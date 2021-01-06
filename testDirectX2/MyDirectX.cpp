#include "MyDirectX.h"

MyDirectX::MyDirectX() {
	m_pD3D = NULL;	// 用于创建D3D设备
	m_pd3dDevice = NULL;	// 我们的渲染设备
}

//-----------------------------------------------------------------------------
	// Name: InitD3D()
	// Desc: 初始化 Direct3D
//-----------------------------------------------------------------------------
BOOL MyDirectX::InitD3D(HWND hWnd, BOOL isFull) {
	//该函数的参数必须是D3D_SDK_VERSION,只有如此方能保证应用程序使用正确的头文件，
	//如果该函数调用失败，将返回一个NULL指针
	//该函数的返回值是一个指向接口IDirect3D9的指针
	//注意：这个函数的使用需要有库文件“d3d9.lib”的存在。
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (!m_pD3D)
		return FALSE;
	//定义显卡显示模式
	D3DDISPLAYMODE displayMode;
	//获得显卡的显示模式 参数1：显卡号(一般为D3DADAPTER_DEFAULT)
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &displayMode)))
		return FALSE;
	//定义后台缓存表面的参数（包括深度模板缓存是否定义和像素格式)
	D3DPRESENT_PARAMETERS d3dpp;	 //参数对象，用于下面的设备创建
	//使用结构前清零
	ZeroMemory(&d3dpp, sizeof(d3dpp));	//用0来填充d3dpp内存区域，也就是初始化内存
	//是否窗口化
	if (isFull) {
		//全屏
		d3dpp.Windowed = false;
		d3dpp.BackBufferHeight = WND_H;	//交换链的后台缓存的像素高度
		d3dpp.BackBufferWidth = WND_W;	//交换链的后台缓存的像素宽度
	}
	else {
		d3dpp.Windowed = TRUE;	//设置为窗口模式
		//交换缓冲支持的效果类型，指定表面在交换链中是如何被交换的。
		//如果设定为 D3DSWAPEFFECT_DISCARD，
		//则后备缓冲区的东西被复制到屏幕上后，后备缓冲区的东西就没有什么用了，可以丢弃（discard）了。
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	//这里为后备的缓存加载之后就删除。
		//后备缓冲的格式。如果对这D3DFORMAT不熟悉的话，可以把它设为D3DFMT_UNKNOWN，这时候它将使用桌面的格式。
		//d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
		//这里我们使用自己定义的，其实跟上面一样，都是0
		d3dpp.BackBufferFormat = displayMode.Format;	//显示模式

		//创建Direct3D 设备
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,	//默认适配器（显卡）
			D3DDEVTYPE_HAL,	//硬件驱动 
			hWnd,	//设备所使用的窗体
			//D3DCREATE_PUREDEVICE, //创建纯设备
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,	//顶点像素渲染方式
			&d3dpp,	//参数
			&m_pd3dDevice	//驱动设备
		))) {
			return FALSE;
		}
	}

	return TRUE;
}
//-----------------------------------------------------------------------------
	// Name: Render()
	// Desc: 绘制场景
//-----------------------------------------------------------------------------
void MyDirectX::Render() {
	if (NULL == m_pd3dDevice)
		return;
	//清除背景缓存并重新设置为蓝色
	m_pd3dDevice->Clear(0,	// pRects中的矩形数，如果 pRects为NULL着该参数必须为0
		NULL,	//一个指向D3DRECT结构体的指针
		D3DCLEAR_TARGET,	//D3DCLEAR标志，现在为清空当前的渲染器
		D3DCOLOR_XRGB(0, 0, 255),	//颜色    
		1.0f,	// depth buffer的新z值
		0	//模板
	);
	// 开始渲染场景
	if (SUCCEEDED(m_pd3dDevice->BeginScene()))
	{
		// 可以在这里渲染场景

		// 结束渲染
		m_pd3dDevice->EndScene();
	}

	// 清空当前缓存中的数据
	m_pd3dDevice->Present(NULL, NULL, NULL, NULL);
}

//-----------------------------------------------------------------------------
	// Name: CleanUp()
	// Desc: 释放预先初始化的对象
//-----------------------------------------------------------------------------
void MyDirectX::CleanUp() {
	if (m_pd3dDevice != NULL) {
		m_pd3dDevice->Release();
		m_pd3dDevice = NULL;
	}
		
	if (m_pD3D != NULL) {
		m_pD3D->Release();
		m_pD3D = NULL;
	}
}

MyDirectX::~MyDirectX() {

}