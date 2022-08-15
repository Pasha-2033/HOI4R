#include "../include/window.h"
#include "../../GraphicCore/DirectX/include/dxwindow.h"
//Функции
RECT winmodule::getscreensize() {
    RECT desktop;
    const HWND hDesktop = GetDesktopWindow();
    GetWindowRect(hDesktop, &desktop);
    return desktop;
}
HRESULT winmodule::initwindow(HWND &window, HINSTANCE hinstance, WCHAR* classname, WCHAR* title, WNDPROC wndproc, RECT rect, HICON hicon, HCURSOR hcursor) {
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
    window = CreateWindow(classname, title, WS_OVERLAPPEDWINDOW, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, hinstance, NULL);
    if (!window) return E_FAIL;
    return S_OK;
}
//Классы и их функции
winmodule::window::window(HINSTANCE hinstance, WNDPROC wndproc, WCHAR* winclassname, WCHAR* wintitle, RECT rect) : classname(winclassname), title(wintitle), x(rect.left), y(rect.top), w(rect.right - rect.left), h(rect.bottom - rect.top) {
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
winmodule::windowshowmode winmodule::window::getshowmode() {
    return sm;
}
bool winmodule::window::getV() {
    return v;
}
WCHAR* winmodule::window::gettitle() {
    return title;
}
WCHAR* winmodule::window::getclassname() {
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
void winmodule::window::setshowmode(windowshowmode showmode) {
    sm = showmode;
    LONG HWNDStyle = GetWindowLong(win, GWL_STYLE);
    LONG HWNDStyleEx = GetWindowLong(win, GWL_EXSTYLE);
    RECT screensize = getscreensize();
    uint32_t screenW = screensize.right - screensize.left;
    uint32_t screenH = screensize.bottom - screensize.top;
    switch (sm) {
    case windowshowmode::fullscreen:
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
    case windowshowmode::customsize:
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
void winmodule::window::settitle(WCHAR* wintitle) {
    title = wintitle;
    SetWindowText(win, title);
}
winmodule::winsubproc::winsubproc(WCHAR* name) : name(name) {}
winmodule::winsubproc::~winsubproc() {}
WCHAR* winmodule::winsubproc::getname() {
    return name;
}
winmodule::extendedwinproc::extendedwinproc() {}
winmodule::extendedwinproc::~extendedwinproc() {}
LRESULT CALLBACK winmodule::extendedwinproc::wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    winmodule::extendedwinproc* instance = (winmodule::extendedwinproc*)(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    if (instance) {
        LRESULT lr = DefWindowProc(hwnd, msg, wparam, lparam);
        for (winsubproc* subproc : instance->eventhandler) {
            lr |= subproc->proc(hwnd, msg, wparam, lparam);
        }
        return lr;
    }
    return DefWindowProc(hwnd, msg, wparam, lparam);
}
bool winmodule::extendedwinproc::addsubproc(winsubproc* subproc) {
    for (size_t i = 0; i < size; i++) {
        if (wcscmp((const WCHAR*)eventhandler[i]->getname(), (const WCHAR*)subproc->getname()) == 0) return false; //add log (same name)
    }
    eventhandler.push_back(subproc);
    size++;
    return true;
}
void winmodule::extendedwinproc::deletesubproc(WCHAR* name) {
    for (size_t i = 0; i < size; i++) {
        if (wcscmp((const WCHAR*)eventhandler[i]->getname(), (const WCHAR*)name) == 0) {
            delete eventhandler[i];
            eventhandler.erase(eventhandler.begin() + i);
            size--;
            return;
        }
    }
}
size_t winmodule::extendedwinproc::getsize() {
    return size;
}
winmodule::winsubproc* winmodule::extendedwinproc::getsubproc(WCHAR* name) {
    for (size_t i = 0; i < size; i++) {
        if (wcscmp((const WCHAR*)eventhandler[i]->getname(), (const WCHAR*)name) == 0) {
            return eventhandler[i];
        }
    }
    return nullptr;
}
winmodule::winsubproc* winmodule::extendedwinproc::getsubproc(size_t id) {
    if (id < size) {
        return eventhandler[id];
    }
    return nullptr;
}
winmodule::defaultwinproc::defaultwinproc(WCHAR* name) : winmodule::winsubproc(name) {}
winmodule::defaultwinproc::~defaultwinproc() {}
LRESULT CALLBACK winmodule::defaultwinproc::proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    switch (msg) {
        PAINTSTRUCT ps;
        HDC hdc;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        //case WM_PAINT:
        //    hdc = BeginPaint(hwnd, &ps);
        //    EndPaint(hwnd, &ps);
        //    break;
    }
    return 0;
}