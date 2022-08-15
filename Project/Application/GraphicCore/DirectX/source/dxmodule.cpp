#include "../include/dxmodule.h"
dxmodule::directx::directx(winmodule::window* win) : cur_drivetype(D3D_DRIVER_TYPE_NULL), device(nullptr), devicecontext(nullptr), swapchain(nullptr), rendertargetview(nullptr) {
	D3D_DRIVER_TYPE drivertypes[] = {
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE
	};
	D3D_FEATURE_LEVEL featurelevels[] = {
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
	UINT createdeviceflags = 0;
	D3D_FEATURE_LEVEL featurelevel = D3D_FEATURE_LEVEL_11_0; //???
	ID3D11Texture2D* backbuffer = nullptr;
	D3D11_VIEWPORT vp;
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
	for (UINT drivertypeindex = 0; drivertypeindex < numdrivertypes; drivertypeindex++) {
		cur_drivetype = drivertypes[drivertypeindex];
		hr = D3D11CreateDeviceAndSwapChain(NULL, cur_drivetype, NULL, createdeviceflags, featurelevels, numfeaturelevels,
			D3D11_SDK_VERSION, &sd, &swapchain, &device, &featurelevel, &devicecontext);
		if (SUCCEEDED(hr)) {
			hr = swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backbuffer);
			if (SUCCEEDED(hr)) {
				hr = device->CreateRenderTargetView(backbuffer, NULL, &rendertargetview);
				backbuffer->Release();
				if (SUCCEEDED(hr)) {
					vp.Width = (FLOAT)win->getW();
					vp.Height = (FLOAT)win->getH();
					vp.MinDepth = 0.0f;
					vp.MaxDepth = 1.0f;
					vp.TopLeftX = 0;
					vp.TopLeftY = 0;
					devicecontext->OMSetRenderTargets(1, &rendertargetview, NULL);
					devicecontext->RSSetViewports(1, &vp);
					break;
				}
			}
		}
	}
}
dxmodule::directx::~directx() {
	if (devicecontext) devicecontext->ClearState();
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
void dxmodule::directx::resizedx(winmodule::window* win) {
	/*ID3D11Texture2D* backbuffer = nullptr;
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
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)win->getW();
	vp.Height = (FLOAT)win->getH();
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	IDXGIDevice* idxgi_device = nullptr;
	IDXGIAdapter* idxgi_adapter = nullptr;
	IDXGIFactory* idxgi_factory = nullptr;
	device->QueryInterface(__uuidof(IDXGIDevice), (void**)&idxgi_device);
	idxgi_device->GetParent(__uuidof(IDXGIAdapter), (void**)&idxgi_adapter);
	idxgi_adapter->GetParent(__uuidof(IDXGIFactory), (void**)&idxgi_factory);
	idxgi_factory->CreateSwapChain(device, &sd, &swapchain);
	idxgi_device->Release();
	idxgi_adapter->Release();
	idxgi_factory->Release();
	swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backbuffer);
	devicecontext->OMSetRenderTargets(1, &rendertargetview, NULL);
	devicecontext->RSSetViewports(1, &vp);*/
	/**/
	devicecontext->ClearState();
	rendertargetview->Release();
	swapchain->Release();
	device->Release();
	devicecontext->Release();
	D3D_DRIVER_TYPE drivertypes[] = {
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE
	};
	D3D_FEATURE_LEVEL featurelevels[] = {
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
	UINT createdeviceflags = 0;
	D3D_FEATURE_LEVEL featurelevel = D3D_FEATURE_LEVEL_11_0; //???
	ID3D11Texture2D* backbuffer = nullptr;
	D3D11_VIEWPORT vp;
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
	for (UINT drivertypeindex = 0; drivertypeindex < numdrivertypes; drivertypeindex++) {
		cur_drivetype = drivertypes[drivertypeindex];
		hr = D3D11CreateDeviceAndSwapChain(NULL, cur_drivetype, NULL, createdeviceflags, featurelevels, numfeaturelevels,
			D3D11_SDK_VERSION, &sd, &swapchain, &device, &featurelevel, &devicecontext);
		if (SUCCEEDED(hr)) {
			hr = swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backbuffer);
			if (SUCCEEDED(hr)) {
				hr = device->CreateRenderTargetView(backbuffer, NULL, &rendertargetview);
				backbuffer->Release();
				if (SUCCEEDED(hr)) {
					vp.Width = (FLOAT)win->getW();
					vp.Height = (FLOAT)win->getH();
					vp.MinDepth = 0.0f;
					vp.MaxDepth = 1.0f;
					vp.TopLeftX = 0;
					vp.TopLeftY = 0;
					devicecontext->OMSetRenderTargets(1, &rendertargetview, NULL);
					devicecontext->RSSetViewports(1, &vp);
					break;
				}
			}
		}
	}/**/
}
HRESULT dxmodule::compileshader(WCHAR* file, LPCSTR entrypoint, LPCSTR profile, ID3DBlob** blobout) {
	HRESULT hr = S_OK;
	ID3DBlob* errorblob = nullptr;
	ID3DBlob* shaderblob = nullptr;
	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
	const D3D_SHADER_MACRO defines[] = {
		"EXAMPLE_DEFINE",
		"1",
		NULL,
		NULL
	};
	hr = D3DCompileFromFile(file, defines, D3D_COMPILE_STANDARD_FILE_INCLUDE, entrypoint, profile, flags, 0, &shaderblob, &errorblob);
	if (FAILED(hr)) {
		if (errorblob) {
			OutputDebugStringA((char*)errorblob->GetBufferPointer());
			errorblob->Release();
		}
		if (shaderblob) shaderblob->Release();
		return hr;//add log
	}
	if (errorblob) errorblob->Release();
	*blobout = shaderblob;
	return S_OK;
}
dxmodule::shaderoperator::shaderoperator() {}
dxmodule::shaderoperator::~shaderoperator() {
	//delete ptr
}
size_t dxmodule::shaderoperator::getshaderid(WCHAR* shadername) {
	for (size_t i = 0; i < size; i++) {
		if (wcscmp((const WCHAR*)shaders[i]->name, (const WCHAR*)shadername) == 0) return i;
	}
	return NULL;
}
size_t dxmodule::shaderoperator::getvectorsize() {
	return size;
}
void dxmodule::shaderoperator::deleteshader(size_t shaderid) {
	if (shaderid < size) {
		delete shaders[shaderid];
		shaders.erase(shaders.begin() + shaderid);
		size--;
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
	HRESULT hr = compileshader(filename, entrypoint, shadermodel, &shaderblob);
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
	HRESULT hr = compileshader(filename, entrypoint, shadermodel, &shaderblob);
	if (FAILED(hr)) return false; //add log
	vertexshader* vertexshaderpointer = new vertexshader;
	hr = device->CreateVertexShader(shaderblob->GetBufferPointer(), shaderblob->GetBufferSize(), NULL, &vertexshaderpointer->shader);
	if (FAILED(hr)) {
		shaderblob->Release();
		return false; //add log
	}
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			0,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		}
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