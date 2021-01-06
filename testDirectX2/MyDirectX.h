#pragma once
#include "define.h"

#include "d3d9.h"

#pragma comment(lib, "d3d9.lib")

class MyDirectX
{
private:
	LPDIRECT3D9 m_pD3D;	// 用于创建D3D设备
	LPDIRECT3DDEVICE9 m_pd3dDevice;	// 我们的渲染设备
public:
	static MyDirectX* Get() {
		static MyDirectX myD3D;
		return &myD3D;
	}
	MyDirectX();
	BOOL InitD3D(HWND hWnd, BOOL isFull);
	void Render();
	void CleanUp();
	virtual ~MyDirectX();
};

