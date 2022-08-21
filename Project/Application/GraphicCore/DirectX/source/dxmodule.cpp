#include "../include/dxmodule.h"
#include <algorithm>
dxmodule::directx::directx(winmodule::window* win, bool useZbuffer) {
	D3D_DRIVER_TYPE drivertypes[] = {
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE
	};
	D3D_FEATURE_LEVEL featurelevels[] = {
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
	};
	UINT numdrivertypes = ARRAYSIZE(drivertypes);
	UINT numfeaturelevels = ARRAYSIZE(featurelevels);
	UINT createdeviceflags = D3D11_CREATE_DEVICE_DEBUG;
	D3D_FEATURE_LEVEL featurelevel = D3D_FEATURE_LEVEL_11_0; //???
	ID3D11Texture2D* backbuffer = nullptr;
	D3D11_VIEWPORT vp;
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	ZeroMemory(&vp, sizeof(vp));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = win->getW();
	sd.BufferDesc.Height = win->getH();
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 75;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = win->getwindow();
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	for (UINT drivertypeindex = 0; drivertypeindex < numdrivertypes; drivertypeindex++) {
		cur_drivetype = drivertypes[drivertypeindex];
		hr = D3D11CreateDeviceAndSwapChain(nullptr, cur_drivetype, NULL, createdeviceflags, featurelevels, numfeaturelevels,
			D3D11_SDK_VERSION, &sd, &swapchain, &device, &featurelevel, &devicecontext);
		if (SUCCEEDED(hr)) {
			hr = swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backbuffer);
			if (SUCCEEDED(hr)) {
				hr = device->CreateRenderTargetView(backbuffer, nullptr, &rendertargetview);
				backbuffer->Release();
				if (SUCCEEDED(hr)) {
					vp.Width = (FLOAT)win->getW();
					vp.Height = (FLOAT)win->getH();
					vp.MinDepth = 0.0f;
					vp.MaxDepth = 1.0f;
					vp.TopLeftX = 0;
					vp.TopLeftY = 0;
					if (useZbuffer) {
						if (createZbuffer(win->getW(), win->getH(), zbuffertexure, zbuffer)) {
							devicecontext->OMSetRenderTargets(1, &rendertargetview, zbuffer);
							devicecontext->RSSetViewports(1, &vp);
							break;
						}
						else {
							if (devicecontext) devicecontext->ClearState();
							if (rendertargetview) rendertargetview->Release();
							if (swapchain) swapchain->Release();
							if (devicecontext) devicecontext->Release();
							if (device) device->Release();
						}
					}
					else {
						devicecontext->OMSetRenderTargets(1, &rendertargetview, nullptr);
						devicecontext->RSSetViewports(1, &vp);
						break;
					}
				}
			}
		}
	}
}
dxmodule::directx::~directx() {
	if (devicecontext) devicecontext->ClearState();
	if (zbuffer) zbuffer->Release();
	if (zbuffertexure) zbuffertexure->Release();
	if (rendertargetview) rendertargetview->Release();
	if (swapchain) swapchain->Release();
	if (devicecontext) devicecontext->Release();
	if (device) device->Release();
}
HRESULT dxmodule::directx::getHR() {
	return hr;
}
ID3D11Device* dxmodule::directx::getdevice() {
	return device;
}
ID3D11DeviceContext* dxmodule::directx::getdevicecontext() {
	return devicecontext;
}
IDXGISwapChain* dxmodule::directx::getswapchain() {
	return swapchain;
}
ID3D11RenderTargetView* dxmodule::directx::getrendertargetview() {
	return rendertargetview;
}
D3D_DRIVER_TYPE dxmodule::directx::getcur_drivetype() {
	return cur_drivetype;
}
ID3D11Texture2D* dxmodule::directx::getzbuffertexure() {
	return zbuffertexure;
}
ID3D11DepthStencilView* dxmodule::directx::getzbuffer() {
	return zbuffer;
}
bool dxmodule::directx::resizedx(winmodule::window* win) {
	IDXGISwapChain* Swapchain = nullptr;
	ID3D11RenderTargetView* Rendertargetview = nullptr;
	ID3D11Texture2D* Ztexure = nullptr;
	ID3D11DepthStencilView* Zbuffer = nullptr;
	if (zbuffer) {
		if (!createZbuffer(win->getW(), win->getH(), Ztexure, Zbuffer)) return false; //log
	}
	IDXGIDevice* idxgidevice = nullptr;
	IDXGIAdapter* idxgiadapter = nullptr;
	IDXGIFactory* idxgifactory = nullptr;
	ID3D11Texture2D* backbuffer = nullptr;
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)win->getW();
	vp.Height = (FLOAT)win->getH();
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = win->getW();
	sd.BufferDesc.Height = win->getH();
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 75;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = win->getwindow();
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	hr = device->QueryInterface(__uuidof(IDXGIDevice), (void**)&idxgidevice);
	if (FAILED(hr)) return false;	//log
	hr = idxgidevice->GetAdapter(&idxgiadapter);
	if (FAILED(hr)) return false;	//log
	hr = idxgiadapter->GetParent(__uuidof(IDXGIFactory), (void**)&idxgifactory);
	if (FAILED(hr)) return false;	//log
	hr = idxgifactory->CreateSwapChain(device, &sd, &Swapchain);
	if (FAILED(hr)) return false;	//log
	Swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backbuffer);
	if (FAILED(hr)) return false;	//log
	hr = device->CreateRenderTargetView(backbuffer, nullptr, &Rendertargetview);
	if (FAILED(hr)) return false;	//log
	zbuffer->Release();
	zbuffertexure->Release();
	rendertargetview->Release();
	swapchain->Release();
	zbuffer = Zbuffer;
	zbuffertexure = Ztexure;
	swapchain = Swapchain;
	rendertargetview = Rendertargetview;
	devicecontext->OMSetRenderTargets(1, &rendertargetview, zbuffer);
	devicecontext->RSSetViewports(1, &vp);
	return true;
}
bool dxmodule::directx::createZbuffer(UINT w, UINT h, ID3D11Texture2D* &texure, ID3D11DepthStencilView* &buffer) {
	w = w > 0 ? w : 1;
	h = h > 0 ? h : 1;
	D3D11_TEXTURE2D_DESC descdepth;  
	ZeroMemory(&descdepth, sizeof(descdepth));
	descdepth.Width = w;          
	descdepth.Height = h;  
	descdepth.MipLevels = 1;     
	descdepth.ArraySize = 1;
	descdepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descdepth.SampleDesc.Count = 1;
	descdepth.SampleDesc.Quality = 0;
	descdepth.Usage = D3D11_USAGE_DEFAULT;
	descdepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;      
	descdepth.CPUAccessFlags = 0;
	descdepth.MiscFlags = 0;
	hr = device->CreateTexture2D(&descdepth, nullptr, &texure);
	if (SUCCEEDED(hr)) {
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;         
		ZeroMemory(&descDSV, sizeof(descDSV));
		descDSV.Format = descdepth.Format;     
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;
		hr = device->CreateDepthStencilView(texure, &descDSV, &buffer);
	}
	if (FAILED(hr)) {
		if (buffer) buffer->Release();
		if (texure) texure->Release();
		return false;
	}
	return true;
}
HRESULT dxmodule::createindexbuffer(ID3D11Device* device, ID3D11Buffer* &bufferpointer, WORD* structure, size_t size) {
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WORD) * size;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = structure;
	HRESULT hr = device->CreateBuffer(&bd, &InitData, &bufferpointer);
	return hr;
}
HRESULT dxmodule::createconstbuffer(ID3D11Device* device, ID3D11Buffer* &bufferpointer) {
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(constantbufferstruct);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	HRESULT hr = device->CreateBuffer(&bd, nullptr, &bufferpointer);
	return hr;
}
HRESULT dxmodule::compileshader(WCHAR* file, LPCSTR entrypoint, LPCSTR profile, ID3DBlob* &blobout) {
	HRESULT hr = S_OK;
	ID3DBlob* errorblob = nullptr;
	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
	const D3D_SHADER_MACRO defines[] = {
		"EXAMPLE_DEFINE",
		"1",
		NULL,
		NULL
	};
	hr = D3DCompileFromFile(file, defines, D3D_COMPILE_STANDARD_FILE_INCLUDE, entrypoint, profile, flags, 0, &blobout, &errorblob);
	if (FAILED(hr)) {
		if (errorblob) {
			OutputDebugStringA((char*)errorblob->GetBufferPointer());
			errorblob->Release();
		}
		if (blobout) blobout->Release();
		return hr;//add log
	}
	if (errorblob) errorblob->Release();
	return S_OK;
}
dxmodule::shaderoperator::shaderoperator() {}
dxmodule::shaderoperator::~shaderoperator() {
	//delete ptr
}
int dxmodule::shaderoperator::getshaderid(WCHAR* shadername) {
	for (size_t i = 0; i < size; i++) {
		if (wcscmp((const WCHAR*)shaders[i]->name, (const WCHAR*)shadername) == 0) return i;
	}
	return -1;
}
size_t dxmodule::shaderoperator::getvectorsize() {
	return size;
}
void dxmodule::shaderoperator::deleteshader(size_t shaderid) {
	if (shaderid < size) {
		delete shaders[shaderid];
		shaders.erase(shaders.begin() + shaderid);
		size--;
		updatestate = true;
	}
}
void dxmodule::shaderoperator::updatehandled() {
	updatestate = false;
}
ID3D11Device* dxmodule::shaderoperator::getshaderdevice(size_t shaderid) {
	if (shaderid < size) {
		return shaders[shaderid]->device;
	}
	return nullptr;
}
WCHAR* dxmodule::shaderoperator::getname(size_t shaderid) {
	if (shaderid < size) {
		return shaders[shaderid]->name;
	}
	return nullptr;
}
bool dxmodule::shaderoperator::getupdatestate() {
	return updatestate;
}
dxmodule::shaderoperator::shaderparentclass::~shaderparentclass() {}
//класс pixelshaderoperator сохраняет в shaders <pixelshader*>, так что явное приведение shaderparentclass* к pixelshader* не нарушает полиморфизм
dxmodule::pixelshaderoperator::pixelshaderoperator() {}
dxmodule::pixelshaderoperator::~pixelshaderoperator() {}
ID3D11PixelShader* dxmodule::pixelshaderoperator::getpixelshader(size_t shaderid) {
	if (shaderid < size) {
		return ((pixelshader*)shaders[shaderid])->shader;
	}
	return nullptr;
}
bool dxmodule::pixelshaderoperator::addshader(WCHAR* filename, WCHAR* shadername, LPCSTR entrypoint, LPCSTR shadermodel, ID3D11Device* device) {
	for (size_t i = 0; i < size; i++) {
		if (wcscmp((const WCHAR*)shaders[i]->name, (const WCHAR*)shadername) == 0) return false; //add log (same name)
	}
	ID3DBlob* shaderblob = nullptr;
	HRESULT hr = compileshader(filename, entrypoint, shadermodel, shaderblob);
	if (FAILED(hr)) return false; //add log
	pixelshader* pixelshaderpointer = new pixelshader;
	hr = device->CreatePixelShader(shaderblob->GetBufferPointer(), shaderblob->GetBufferSize(), NULL, &pixelshaderpointer->shader);
	if (FAILED(hr)) {
		shaderblob->Release();
		return false; //add log
	}
	shaderblob->Release();
	pixelshaderpointer->name = shadername;
	pixelshaderpointer->device = device;
	shaders.push_back(pixelshaderpointer);
	size++;
	return true;
}
dxmodule::pixelshaderoperator::pixelshader::~pixelshader() {
	delete shader;
}
//класс vertexshaderoperator сохраняет в shaders <vertexshader*>, так что явное приведение shaderparentclass* к vertexshader* не нарушает полиморфизм
dxmodule::vertexshaderoperator::vertexshaderoperator() {}
dxmodule::vertexshaderoperator::~vertexshaderoperator() {}
ID3D11InputLayout* dxmodule::vertexshaderoperator::getshaderlayout(size_t shaderid) {
	if (shaderid < size) {
		return ((vertexshader*)shaders[shaderid])->shaderlayout;
	}
	return nullptr;
}
ID3D11VertexShader* dxmodule::vertexshaderoperator::getvertexshader(size_t shaderid) {
	if (shaderid < size) {
		return ((vertexshader*)shaders[shaderid])->shader;
	}
	return nullptr;
}
bool dxmodule::vertexshaderoperator::addshader(WCHAR* filename, WCHAR* shadername, LPCSTR entrypoint, LPCSTR shadermodel, ID3D11Device* device) {
	for (size_t i = 0; i < size; i++) {
		if (wcscmp((const WCHAR*)shaders[i]->name, (const WCHAR*)shadername) == 0) return false; //add log (same name)
	}
	ID3DBlob* shaderblob = nullptr;
	HRESULT hr = compileshader(filename, entrypoint, shadermodel, shaderblob);
	if (FAILED(hr)) return false; //add log
	vertexshader* vertexshaderpointer = new vertexshader;
	hr = device->CreateVertexShader(shaderblob->GetBufferPointer(), shaderblob->GetBufferSize(), NULL, &vertexshaderpointer->shader);
	if (FAILED(hr)) {
		shaderblob->Release();
		return false; //add log
	}
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	uint32_t numelements = ARRAYSIZE(layout);
	hr = device->CreateInputLayout(layout, numelements, shaderblob->GetBufferPointer(), shaderblob->GetBufferSize(), &vertexshaderpointer->shaderlayout);
	shaderblob->Release();
	if (FAILED(hr)) {
		delete vertexshaderpointer;
		return false; //add log
	}
	vertexshaderpointer->device = device;
	vertexshaderpointer->name = shadername;
	shaders.push_back(vertexshaderpointer);
	size++;
	return true;
}
dxmodule::vertexshaderoperator::vertexshader::~vertexshader() {
	delete shaderlayout;
	delete shader;
}






dxmodule::graphicobject::graphicobject(WCHAR* name, simplevertex* bounds, uint32_t id) : name(name), bounds(bounds), id(id) {
	for (size_t i = 0; i < ARRAYSIZE(useabsolutedata); i++) {
		useabsolutedata[i] = false;
	}
}
dxmodule::graphicobject::~graphicobject() {
	for (graphicobject* child : childs) {
		delete child;
	}
}
void dxmodule::graphicobject::setPSname(WCHAR* newPSname, shaderoperator* pixelshaders) {
	PSname = newPSname;
	if (pixelshaders) {
		int id = pixelshaders->getshaderid(newPSname);
		if (id == -1) {
			canberendered = false;
		}
		else {
			PSid = id;
		}
	}
	else {
		canberendered = false;
		optimisedshaderid = false;
	}
}
void dxmodule::graphicobject::setVSname(WCHAR* newVSname, shaderoperator* vertexshaders) {
	VSname = newVSname;
	if (vertexshaders) {
		int id = vertexshaders->getshaderid(newVSname);
		if (id == -1) {
			canberendered = false;
			optimisedshaderid = false;
		}
		else {
			VSid = id;
		}
	}
	else {
		canberendered = false;
		optimisedshaderid = false;
	}
}
void dxmodule::graphicobject::optimiseshaderid(shaderoperator* pixelshaders, shaderoperator* vertexshaders) {
	int id = pixelshaders->getshaderid(PSname);
	if (id == -1) {
		canberendered = false;
		optimisedshaderid = false;	//log
		return;
	}
	PSid = id;
	id = vertexshaders->getshaderid(VSname);
	if (id == -1) {
		canberendered = false;
		optimisedshaderid = false;	//log
		return;
	}
	VSid = id;
}
void dxmodule::graphicobject::setbounds(simplevertex* newbounds) {
	bounds = newbounds;
}
bool dxmodule::graphicobject::addchild(graphicobject* child) {
	std::vector<uint32_t> vec = getidforname(child->name);
	if (std::find(vec.begin(), vec.end(), child->id) == vec.end()) {
		childs.push_back(child);
	}
	else {
		return false; //log
	}
}
bool dxmodule::graphicobject::deletechild(WCHAR* childname, uint32_t childid) {
	size_t size = childs.size();
	bool rightid = true;
	for (size_t i = 0; i < size; i++) {
		if (wcscmp((const WCHAR*)childs[i]->name, (const WCHAR*)childname) == 0) {
			if (childs[i]->id == childid) {
				delete childs[i];
				childs.erase(childs.begin() + i);
				return true;
			}
			else {
				rightid = false;
			}
		}
	}
	return rightid;	//log
}
void dxmodule::graphicobject::removechild(WCHAR* childname, uint32_t childid, graphicobject* newparent) {
	size_t size = childs.size();
	for (size_t i = 0; i < size; i++) {
		if (wcscmp((const WCHAR*)childs[i]->name, (const WCHAR*)childname) == 0) {
			if (childs[i]->id == childid) {
				childs[i]->parent = newparent;
				childs.erase(childs.begin() + i);
				break;
			}
		}
	}
}
void dxmodule::graphicobject::setparent(graphicobject* newparent) {
	parent = newparent;
}
void dxmodule::graphicobject::setdatatype(graphicobjectdatatype type, bool isabsolute) {
	switch (type) {
		case (graphicobjectdatatype::GODT_X):
			useabsolutedata[0] = isabsolute;
			break;
		case (graphicobjectdatatype::GODT_Y):
			useabsolutedata[1] = isabsolute;
			break;
		case (graphicobjectdatatype::GODT_Z):
			useabsolutedata[2] = isabsolute;
			break;
		case (graphicobjectdatatype::GODT_SCALE_X):
			useabsolutedata[3] = isabsolute;
			break;
		case (graphicobjectdatatype::GODT_SCALE_Y):
			useabsolutedata[4] = isabsolute;
			break;
		case (graphicobjectdatatype::GODT_SCALE_Z):
			useabsolutedata[5] = isabsolute;
			break;
		case (graphicobjectdatatype::GODT_ROTATION_X):
			useabsolutedata[6] = isabsolute;
			break;
		case (graphicobjectdatatype::GODT_ROTATION_Y):
			useabsolutedata[7] = isabsolute;
			break;
		case (graphicobjectdatatype::GODT_ROTATION_Z):
			useabsolutedata[8] = isabsolute;
			break;
		default:
			break;
	}
}
void dxmodule::graphicobject::setobjectdata(graphicobjectdatatype type, float value) {
	switch (type) {
		case (graphicobjectdatatype::GODT_X):
			relativedata.x = value;
			break;
		case (graphicobjectdatatype::GODT_Y):
			relativedata.y = value;
			break;
		case (graphicobjectdatatype::GODT_Z):
			relativedata.z = value;
			break;
		case (graphicobjectdatatype::GODT_SCALE_X):
			relativedata.scalex = value;
			break;
		case (graphicobjectdatatype::GODT_SCALE_Y):
			relativedata.scaley = value;
			break;
		case (graphicobjectdatatype::GODT_SCALE_Z):
			relativedata.scalez = value;
			break;
		case (graphicobjectdatatype::GODT_ROTATION_X):
			relativedata.x_rotation = value;
			break;
		case (graphicobjectdatatype::GODT_ROTATION_Y):
			relativedata.y_rotation = value;
			break;
		case (graphicobjectdatatype::GODT_ROTATION_Z):
			relativedata.z_rotation = value;
			break;
		default:
			break;
	}
}
std::vector<uint32_t> dxmodule::graphicobject::getidforname(WCHAR* childname) {
	std::vector<uint32_t> ids;
	size_t size = childs.size();
	for (size_t i = 0; i < size; i++) {
		if (wcscmp((const WCHAR*)childs[i]->name, (const WCHAR*)childname) == 0) {
			ids.push_back(childs[i]->id);
		}
	}
	return ids;
}
uint32_t dxmodule::graphicobject::getid() {
	return id;
}
WCHAR* dxmodule::graphicobject::getname() {
	return name;
}
WCHAR* dxmodule::graphicobject::getPSname() {
	return PSname;
}
WCHAR* dxmodule::graphicobject::getVSname() {
	return VSname;
}
size_t dxmodule::graphicobject::getPSid() {
	return PSid;
}
size_t dxmodule::graphicobject::getVSid() {
	return VSid;
}
std::vector<dxmodule::graphicobject*> dxmodule::graphicobject::getchilds() {
	return childs;
}
dxmodule::graphicobject* dxmodule::graphicobject::getparent() {
	return parent;
}
dxmodule::simplevertex* dxmodule::graphicobject::getbounds() {
	return bounds;
}
bool dxmodule::graphicobject::getuseabsolutedata(graphicobjectdatatype type) {
	switch (type) {
	case (graphicobjectdatatype::GODT_X):
		return useabsolutedata[0];
	case (graphicobjectdatatype::GODT_Y):
		return useabsolutedata[1];
	case (graphicobjectdatatype::GODT_Z):
		return useabsolutedata[2];
	case (graphicobjectdatatype::GODT_SCALE_X):
		return useabsolutedata[3];
	case (graphicobjectdatatype::GODT_SCALE_Y):
		return useabsolutedata[4];
	case (graphicobjectdatatype::GODT_SCALE_Z):
		return useabsolutedata[5];
	case (graphicobjectdatatype::GODT_ROTATION_X):
		return useabsolutedata[6];
	case (graphicobjectdatatype::GODT_ROTATION_Y):
		return useabsolutedata[7];
	case (graphicobjectdatatype::GODT_ROTATION_Z):
		return useabsolutedata[8];
	default:
		false;
	}
}
bool dxmodule::graphicobject::getoptimisedshaderid() {
	return optimisedshaderid;
}
bool dxmodule::graphicobject::getcanberendered() {
	return canberendered;
}
float dxmodule::graphicobject::getobjectdata(graphicobjectdatatype type, bool forceabsolute) {
	switch (type) {
	case (graphicobjectdatatype::GODT_X):
		if (forceabsolute && parent) {
			float rx = parent->getobjectdata(graphicobjectdatatype::GODT_ROTATION_X, true);
			float ry = parent->getobjectdata(graphicobjectdatatype::GODT_ROTATION_Y, true);
			float rz = parent->getobjectdata(graphicobjectdatatype::GODT_ROTATION_Z, true);
			float x, y, z;
			//x rotation
			y = cos(rx) * relativedata.y + sin(rx) * relativedata.z;
			z = cos(rx) * relativedata.z - sin(rx) * relativedata.y;
			//y rotation
			x = cos(ry) * relativedata.x - sin(ry) * z;
			//z rotation
			x = cos(rz) * x + sin(rz) * y;
			return parent->getobjectdata(graphicobjectdatatype::GODT_X, true) + x;
		}
		else {
			return relativedata.x;
		}
	case (graphicobjectdatatype::GODT_Y):
		if (forceabsolute && parent) {
			float rx = parent->getobjectdata(graphicobjectdatatype::GODT_ROTATION_X, true);
			float ry = parent->getobjectdata(graphicobjectdatatype::GODT_ROTATION_Y, true);
			float rz = parent->getobjectdata(graphicobjectdatatype::GODT_ROTATION_Z, true);
			float x, y, z;
			//x rotation
			y = cos(rx) * relativedata.y + sin(rx) * relativedata.z;
			z = cos(rx) * relativedata.z - sin(rx) * relativedata.y;
			//y rotation
			x = cos(ry) * relativedata.x - sin(ry) * z;
			//z rotation
			y = cos(rz) * y - sin(rz) * x;
			return parent->getobjectdata(graphicobjectdatatype::GODT_Y, true) + y;
		}
		else {
			return relativedata.y;
		}
	case (graphicobjectdatatype::GODT_Z):
		if (forceabsolute && parent) {
			float rx = parent->getobjectdata(graphicobjectdatatype::GODT_ROTATION_X, true);
			float ry = parent->getobjectdata(graphicobjectdatatype::GODT_ROTATION_Y, true);
			float rz = parent->getobjectdata(graphicobjectdatatype::GODT_ROTATION_Z, true);
			float z;
			//x rotation
			z = cos(rx) * relativedata.z - sin(rx) * relativedata.y;
			//y rotation
			z = sin(ry) * relativedata.x + cos(ry) * z;
			return parent->getobjectdata(graphicobjectdatatype::GODT_Z, true) + z;
		}
		else {
			return relativedata.z;
		}
	case (graphicobjectdatatype::GODT_SCALE_X):
		if (forceabsolute && parent) {
			return relativedata.scalex * parent->getobjectdata(graphicobjectdatatype::GODT_SCALE_X, true);
		}
		else {
			return relativedata.scalex;
		}
	case (graphicobjectdatatype::GODT_SCALE_Y):
		if (forceabsolute && parent) {
			return relativedata.scaley * parent->getobjectdata(graphicobjectdatatype::GODT_SCALE_Y, true);
		}
		else {
			return relativedata.scaley;
		}
	case (graphicobjectdatatype::GODT_SCALE_Z):
		if (forceabsolute && parent) {
			return relativedata.scalez * parent->getobjectdata(graphicobjectdatatype::GODT_SCALE_Z, true);
		}
		else {
			return relativedata.scalez;
		}
	case (graphicobjectdatatype::GODT_ROTATION_X):
		if (forceabsolute && parent) {
			return relativedata.x_rotation + parent->getobjectdata(graphicobjectdatatype::GODT_ROTATION_X, true);
		}
		else {
			return relativedata.x_rotation;
		}
	case (graphicobjectdatatype::GODT_ROTATION_Y):
		if (forceabsolute && parent) {
			return relativedata.y_rotation + parent->getobjectdata(graphicobjectdatatype::GODT_ROTATION_Y, true);
		}
		else {
			return relativedata.y_rotation;
		}
	case (graphicobjectdatatype::GODT_ROTATION_Z):
		if (forceabsolute && parent) {
			return relativedata.z_rotation + parent->getobjectdata(graphicobjectdatatype::GODT_ROTATION_Z, true);
		}
		else {
			return relativedata.z_rotation;
		}
	default:
		false;
	}
}