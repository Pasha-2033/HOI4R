#include "../include/dxwindow.h"
dxwindow::dxwindowclass::dxwindowclass() {
	winproc->addsubproc((winmodule::winsubproc*) new winmodule::defaultwinproc((WCHAR*)L"name"));
	//здесь начинается первая ступень лохотрона, winproc - вызывает отсутсвие окна
	SetWindowLongPtr(window->getwindow(), GWLP_USERDATA, (LONG_PTR)this);
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