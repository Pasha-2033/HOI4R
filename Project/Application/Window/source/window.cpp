#include "../include/window.h"
//Функции
RECT winmodule::getscreensize() {
    RECT desktop;
    const HWND hDesktop = GetDesktopWindow();
    GetWindowRect(hDesktop, &desktop);
    return desktop;
}
HRESULT winmodule::initwindow(HWND &window, HINSTANCE hinstance, LPCTSTR classname, LPCTSTR title, WNDPROC wndproc, RECT rect, HICON hicon, HCURSOR hcursor) {
    //Регистрация класса
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = wndproc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hinstance;
    wcex.hIcon = hicon;
    wcex.hCursor = hcursor;
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = classname;
    wcex.hIconSm = hicon;
    if (!RegisterClassEx(&wcex)) return E_FAIL;
    //Создание окна
    window = CreateWindow(classname, title, WS_OVERLAPPEDWINDOW, rect.left, rect.top, rect.right, rect.bottom, NULL, NULL, hinstance, NULL);
    if (!window) return E_FAIL;
    return S_OK;
}
//Классы и их функции
winmodule::window::window(WNDPROC wndproc, LPCTSTR winclassname, LPCTSTR wintitle, RECT rect) : classname(winclassname), title(wintitle), x(rect.left), y(rect.top), w(rect.right - rect.left), h(rect.bottom - rect.top) {
    hr = initwindow(win, hinstance, classname, title, wndproc, rect);
}
winmodule::window::~window() {}
uint32_t winmodule::window::getX() {
    return x;
}
uint32_t winmodule::window::getY() {
    return y;
}
uint32_t winmodule::window::getW() {
    return w;
}
uint32_t winmodule::window::getH() {
    return h;
}
uint32_t winmodule::window::getshowmode() {
    return sm;
}
bool winmodule::window::getV() {
    return v;
}
LPCTSTR winmodule::window::gettitle() {
    return title;
}
LPCTSTR winmodule::window::getclassname() {
    return classname;
}
HWND winmodule::window::getwindow() {
    return win;
}
HRESULT winmodule::window::getHR() {
    return hr;
}
void winmodule::window::setX(uint32_t xpos) {
    x = xpos;
    MoveWindow(win, x, y, w, h, TRUE);
}
void winmodule::window::setY(uint32_t ypos) {
    y = ypos;
    MoveWindow(win, x, y, w, h, TRUE);
}
void winmodule::window::setW(uint32_t width) {
    w = width;
    MoveWindow(win, x, y, w, h, TRUE);
}
void winmodule::window::setH(uint32_t height) {
    h = height;
    MoveWindow(win, x, y, w, h, TRUE);
}
void winmodule::window::setshowmode(uint32_t showmode) {
    sm = showmode;
    LONG HWNDStyle = GetWindowLong(win, GWL_STYLE);
    LONG HWNDStyleEx = GetWindowLong(win, GWL_EXSTYLE);
    RECT screensize = getscreensize();
    uint32_t screenW = screensize.right - screensize.left;
    uint32_t screenH = screensize.bottom - screensize.top;
    switch (sm) {
        case fullscreen:
            //полноэкранный режим
            x, y = 0;
            w = screenW;
            h = screenH;
            sw_type = SW_MAXIMIZE;
            HWNDStyle &= ~WS_BORDER;
            HWNDStyle &= ~WS_DLGFRAME;
            HWNDStyle &= ~WS_THICKFRAME;
            HWNDStyleEx &= ~WS_EX_WINDOWEDGE;
            SetWindowLong(win, GWL_STYLE, HWNDStyle | WS_POPUP);
            SetWindowLong(win, GWL_EXSTYLE, HWNDStyleEx | WS_EX_TOPMOST);
            MoveWindow(win, x, y, w, h, TRUE);
            ShowWindow(win, v & sw_type);
            break;
        case customsize:
            //обычное окно, произвольный размер
            if (w > screenW) {
                w = screenW;
                x = 0;
            }
            else {
                x = (screenW - w) / 2;
            }
            if (h > screenH) {
                h = screenH;
                y = 0;
            }
            else {
                y = (screenH - h) / 2;
            }
            sw_type = SW_NORMAL;
            HWNDStyle |= WS_BORDER;
            HWNDStyle |= WS_DLGFRAME;
            HWNDStyle |= WS_THICKFRAME;
            HWNDStyleEx |= WS_EX_WINDOWEDGE;
            SetWindowLong(win, GWL_STYLE, HWNDStyle | WS_POPUP);
            SetWindowLong(win, GWL_EXSTYLE, HWNDStyleEx | WS_EX_TOPMOST);
            MoveWindow(win, x, y, w, h, TRUE);
            ShowWindow(win, v & sw_type);
            break;
    }
}
void winmodule::window::setV(bool visible) {
    v = visible;
    ShowWindow(win, v & sw_type);
}
void winmodule::window::settitle(LPCTSTR wintitle) {
    title = wintitle;
    SetWindowText(win, title);
}