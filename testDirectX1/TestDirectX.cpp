#include "CWindowEx.h"

int APIENTRY WinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR     lpCmdLine,
    int       nCmdShow)
{
    CWindowEx ce(hInstance);
    ce.main(800, 600);
    return 0;
}