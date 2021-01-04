#pragma once
#include "define.h"
class CWindowEx
{
private:
    HINSTANCE m_hInstance;
    HWND hWnd;
public:
	CWindowEx(HINSTANCE hInstance) {
        m_hInstance = hInstance;
        hWnd = NULL;
        // 设计并注册窗口类
        WNDCLASSEX wce = {0};
        wce.cbSize = sizeof(wce);
        wce.cbClsExtra = 0;
        wce.cbWndExtra = 0;
        wce.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wce.hCursor = NULL;
        wce.hIcon = NULL;
        wce.hIconSm = NULL;
        wce.hInstance = m_hInstance;
        wce.lpfnWndProc = WndProc;
        wce.lpszClassName = WND_CLASS_NAME;
        wce.lpszMenuName = NULL;
        wce.style = CS_HREDRAW | CS_VREDRAW;
        RegisterClassEx(&wce);
	}
	~CWindowEx() {
        // 注销窗口类
        UnregisterClass(WND_CLASS_NAME, m_hInstance);
	}
public:
    // 窗口处理函数（回调函数）
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        switch (uMsg)
        {
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        }
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
    // main 对外调用
    BOOL main(int width, int height) {
        // 创建主窗口
        hWnd = CreateWindowEx(0,
            WND_CLASS_NAME,
            WND_NAME,
            WS_OVERLAPPEDWINDOW,
            WND_X,
            WND_Y,
            width,
            height,
            NULL, NULL, m_hInstance, NULL);
        if (!hWnd)
            return FALSE;
        // 显示窗口
        ShowWindow(hWnd, SW_SHOW);
        UpdateWindow(hWnd);
        // 消息循环
        MSG uMsg = { 0 };
        while (GetMessage(&uMsg, NULL, 0, 0))
        {
            TranslateMessage(&uMsg);
            DispatchMessage(&uMsg);
        }
        return TRUE;
    }
};

