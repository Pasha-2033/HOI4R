#include "../include/dxwindow.h"
dxwindow::camera::camera() {
	recalculatevectros();
}
dxwindow::camera::~camera() {}
DirectX::XMMATRIX dxwindow::camera::getviewpoint() {
	return mymatrix;
}
void dxwindow::camera::setX(float value) {
	x = value;
	recalculatevectros();
}
void dxwindow::camera::setY(float value) {
	y = value;
	recalculatevectros();
}
void dxwindow::camera::setZ(float value) {
	z = value;
	recalculatevectros();
}
void dxwindow::camera::setW(float value) {
	w = value;
	recalculatevectros();
}
void dxwindow::camera::setXY_rotation(float value) {
	xy_rotation = value;
	recalculatevectros();
}
void dxwindow::camera::setXZ_rotation(float value) {
	xz_rotation = value;
	recalculatevectros();
}
void dxwindow::camera::setYZ_rotation(float value) {
	yz_rotation = value;
	recalculatevectros();
}
float dxwindow::camera::getX() {
	return x;
}
float dxwindow::camera::getY() {
	return y;
}
float dxwindow::camera::getZ() {
	return z;
}
float dxwindow::camera::getW() {
	return w;
}
float dxwindow::camera::getXY_rotation() {
	return xy_rotation;
}
float dxwindow::camera::getXZ_rotation() {
	return xz_rotation;
}
float dxwindow::camera::getYZ_rotation() {
	return yz_rotation;
}
void dxwindow::camera::recalculatevectros() {
	float dz = cos(xz_rotation) * cos(yz_rotation);
	DirectX::XMVECTOR Eye = DirectX::XMVectorSet(x, y, z, w);	// Откуда смотрим
	DirectX::XMVECTOR At = DirectX::XMVectorSet(x + sin(xz_rotation), y + sin(yz_rotation), z + dz, w);	// Куда смотрим
	DirectX::XMVECTOR Up = DirectX::XMVectorSet(sin(xy_rotation), cos(xy_rotation), 0.0f, w);	// Направление верха
	mymatrix = DirectX::XMMatrixLookAtLH(Eye, At, Up);
}
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
	//настройка матриц
	//-----настроить----
	hwndcamera->setY(1.0f);
	hwndcamera->setZ(- 10.0f);
	//hwndcamera->xz_rotation = -0.25f;
	//hwndcamera->setYZ_rotation(- 0.15f);
	viewmatrix = hwndcamera->getviewpoint();
	//------------------
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
dxwindow::camera* dxwindow::dxwindowclass::getcamera() {
	return hwndcamera;
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