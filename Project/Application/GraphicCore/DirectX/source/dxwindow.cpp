#include "../include/dxwindow.h"
dxwindow::dxwindowclass::dxwindowclass(HINSTANCE hinstance, bool updatedxmodule, RECT rect) : winmodule::window(hinstance, winmodule::extendedwinproc::wndproc, (WCHAR*)L"UNNAMEDCLASS", (WCHAR*)L"", rect), updatedx(updatedxmodule) {
	//инициализация
	if (FAILED(hr)) {
		return;
	}
	dx = new dxmodule::directx(this);
	hr = dx->getHR();
	if (FAILED(hr)) {
		delete dx;
		return;
	}
	winproc = new winmodule::extendedwinproc;
	SetWindowLongPtr(win, GWLP_USERDATA, (LONG_PTR)winproc);
	//настройка процессов окна
	winproc->addsubproc(new winmodule::defaultwinproc((WCHAR*)L"QUIT_MESSAGE"));
	return;
}
dxwindow::dxwindowclass::~dxwindowclass() {}
void dxwindow::dxwindowclass::setupdatedx(bool updatedxmodule) {
	updatedx = updatedxmodule;
}
bool dxwindow::dxwindowclass::getupdatedx() {
	return updatedx;
}
winmodule::extendedwinproc* dxwindow::dxwindowclass::getwinproc() {
	return winproc;
}
dxmodule::directx* dxwindow::dxwindowclass::getdx() {	//temp
	return dx;
}					
void dxwindow::dxwindowclass::setW(uint32_t width) {
	w = width;
	if (updatedx) {
		dx->resizedx(this);
	}
}
void dxwindow::dxwindowclass::setH(uint32_t height) {
	h = height;
	if (updatedx) {
		dx->resizedx(this);
	}
}
bool dxwindow::dxwindowclass::applyvertexshader(size_t shaderid) {
	ID3D11InputLayout* shaderlayout = vso->getshaderlayout(shaderid);
	ID3D11VertexShader* shader = vso->getvertexshader(shaderid);
	if (!(shaderlayout && shader)) return false;
	dx->getdevicecontext()->IASetInputLayout(shaderlayout);
	dx->getdevicecontext()->VSSetShader(shader, NULL, 0);
	return true;
}
bool dxwindow::dxwindowclass::applypixelshader(size_t shaderid) {
	ID3D11PixelShader* shader = pso->getpixelshader(shaderid);
	if (!shader) return false;
	dx->getdevicecontext()->PSSetShader(shader, NULL, 0);
	return true;
}