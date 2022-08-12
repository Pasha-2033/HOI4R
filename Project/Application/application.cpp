#include "application.h"
#include "GraphicCore/DirectX/include/dxwindow.h"
int main() {
    dxwindow::dxwindowclass* dxwin = new dxwindow::dxwindowclass;
    if (FAILED(dxwin->getHR())) return -1;
    dxwin->getwindow()->setW(500);
    dxwin->getwindow()->setH(500);
    dxwin->getwindow()->setshowmode(winmodule::windowshowmode::customsize);
    dxwin->getwindow()->setV(true);
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
	return 0;
}