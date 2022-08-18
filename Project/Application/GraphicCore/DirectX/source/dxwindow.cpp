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
	//настройка dx
	dx->getdevicecontext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//
	projectionmatrix = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV4, getW() / (FLOAT)getH(), znear, zfar);
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
		projectionmatrix = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV4, getW() / (FLOAT)getH(), znear, zfar);
	}
	MoveWindow(win, x, y, w, h, TRUE);
}
void dxwindow::dxwindowclass::setH(uint32_t height) {
	h = height;
	if (updatedx) {
		dx->resizedx(this);
		projectionmatrix = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV4, getW() / (FLOAT)getH(), znear, zfar);
	}
	MoveWindow(win, x, y, w, h, TRUE);
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
//void optimise(vector<graphicobject>* gov);	//для того чтобы перевести shadername в shaderid, для более быстрого доступа
size_t dxwindow::dxwindowclass::getshadervectorsize(bool ispixelshader) {
	if (ispixelshader) {
		return pso->getvectorsize();
	}
	else {
		return vso->getvectorsize();
	}
}
bool dxwindow::dxwindowclass::addshader(WCHAR* filename, WCHAR* shadername, LPCSTR entrypoint, LPCSTR shadermodel, bool ispixelshader) {
	if (ispixelshader) {
		return pso->addshader(filename, shadername, entrypoint, shadermodel, dx->getdevice());
	}
	else {
		return vso->addshader(filename, shadername, entrypoint, shadermodel, dx->getdevice());
	}
}
bool dxwindow::dxwindowclass::hasshader(WCHAR* shadername, bool ispixelshader) {
	if (ispixelshader) {
		if (pso->getshaderid(shadername) < 0) return false;
	}
	else {
		if (vso->getshaderid(shadername) < 0) return false;
	}
	return true;
}
void dxwindow::dxwindowclass::deleteshader(WCHAR* shadername, bool ispixelshader) {
	if (ispixelshader) {
		pso->deleteshader(pso->getshaderid(shadername));
	}
	else {
		vso->deleteshader(vso->getshaderid(shadername));
	}
	//update graphic objects (to do)
}
std::vector<WCHAR*> dxwindow::dxwindowclass::getshaderlist(bool ispixelshader) {
	std::vector<WCHAR*> v = {};
	if (ispixelshader) {
		size_t size = pso->getvectorsize();
		for (size_t i = 0; i < size; i++) {
			v.push_back(pso->getname(i));
		}
	}
	else {
		size_t size = vso->getvectorsize();
		for (size_t i = 0; i < size; i++) {
			v.push_back(vso->getname(i));
		}
	}
	return v;
}
void dxwindow::dxwindowclass::setznear(float z) {
	znear = z;
	if (updatedx) {
		projectionmatrix = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV4, getW() / (FLOAT)getH(), znear, zfar);
	}
}
void dxwindow::dxwindowclass::setzfar(float z) {
	zfar = z;
	if (updatedx) {
		projectionmatrix = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV4, getW() / (FLOAT)getH(), znear, zfar);
	}
}
float dxwindow::dxwindowclass::getznear() {
	return znear;
}
float dxwindow::dxwindowclass::getzfar() {
	return zfar;
}