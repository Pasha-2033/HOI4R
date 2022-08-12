#include "../include/dxwindow.h"
dxwindow::dxwindowclass::dxwindowclass(HINSTANCE hinstance) {
	//инициализация
	window = new winmodule::window(hinstance, winmodule::extendedwinproc::wndproc, (WCHAR*)L"tempclass", (WCHAR*)L"title");
	hr = window->getHR();
	if (FAILED(hr)) {
		delete window;
		return;
	}
	dx = new dxmodule::directx(window);
	hr = dx->getHR();
	if (FAILED(hr)) {
		delete window;
		delete dx;
		return;
	}
	winproc = new winmodule::extendedwinproc;
	SetWindowLongPtr(window->getwindow(), GWLP_USERDATA, (LONG_PTR)winproc);
	//настройка процессов окна
	winproc->addsubproc(new winmodule::defaultwinproc((WCHAR*)L"QUIT_MESSAGE"));
	return;
}
dxwindow::dxwindowclass::~dxwindowclass() {

}
winmodule::extendedwinproc* dxwindow::dxwindowclass::getwinproc() {
	return winproc;
}
winmodule::window* dxwindow::dxwindowclass::getwindow() {
	return window;
}
dxmodule::directx* dxwindow::dxwindowclass::getdx() {
	return dx;
}
HRESULT dxwindow::dxwindowclass::getHR() {
	return hr;
}