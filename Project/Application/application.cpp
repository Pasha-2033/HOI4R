#include "application.h"
using namespace std;
HWND win;
winmodule::window* winclass;
dxmodule::directx* dx;
LRESULT CALLBACK WndProc(HWND hWnd, uint32_t message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        PAINTSTRUCT ps;
        HDC hdc;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;
    }
    return 0;
}
void render();
int main() {
    winclass = new winmodule::window(WndProc, L"tempclass", L"title");
    winclass->setshowmode(winmodule::windowshowmode::customsize);
    winclass->setW(500);
    winclass->setH(500);
    winclass->setV(true);
    dx = new dxmodule::directx(winclass);
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
            render();      
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
	return 0;
}
void render() {
    float ClearColor[4] = { 0.0f, 0.0f, 1.0f, 1.0f }; // красный, зеленый, синий, альфа-канал
    dx->getdevicecontext()->ClearRenderTargetView(dx->getrendertargetview(), ClearColor);
    dx->getswapchain()->Present(0, 0);
}