#include "../include/device.h"
//перейти на d3d12, а с нее вызывать 11/10/9, если 12 не удалось создать
dxmodule::directx::directx(winmodule::window* win) : cur_drivetype(D3D_DRIVER_TYPE_NULL), device(nullptr), devicecontext(nullptr), swapchain(nullptr), rendertargetview(nullptr) {
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
		D3D_FEATURE_LEVEL_10_0
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
ID3D11DeviceContext* dxmodule::directx::getdevicecontext() {
	return devicecontext;
}
ID3D11RenderTargetView* dxmodule::directx::getrendertargetview() {
	return rendertargetview;
}
IDXGISwapChain* dxmodule::directx::getswapchain() {
	return swapchain;
}



dxmodule::shaderoperator::shaderoperator() {}
dxmodule::shaderoperator::~shaderoperator() {}
ID3D11PixelShader* dxmodule::shaderoperator::getpixelshader(LPCWSTR shadername) {
	size_t size = pixelshaders.size();
	for (size_t i = 0; i < size; i++) {
		if (pixelshaders[i]->name == shadername) return pixelshaders[i]->shader;
	}
	return nullptr;
}
ID3D11VertexShader* dxmodule::shaderoperator::getvertexshader(LPCWSTR shadername) {
	size_t size = vertexshaders.size();
	for (size_t i = 0; i < size; i++) {
		if (vertexshaders[i]->name == shadername) return vertexshaders[i]->shader;
	}
	return nullptr;
}
bool dxmodule::shaderoperator::addpixelshader(WCHAR* filename, LPCWSTR shadername, LPCSTR entrypoint, LPCSTR shadermodel, ID3D11Device* device) {
	ID3DBlob* shaderblob = nullptr;
	HRESULT hr = compileshader(filename, entrypoint, shadermodel, &shaderblob);
	if (FAILED(hr)) return false; //add log
	pixelshaders.push_back(new pixelshader);
	size_t lastindex = pixelshaders.size() - 1;
	hr = device->CreatePixelShader(shaderblob->GetBufferPointer(), shaderblob->GetBufferSize(), NULL, &pixelshaders[lastindex]->shader);
	if (FAILED(hr)) {
		delete pixelshaders[lastindex];
		pixelshaders.pop_back();
		shaderblob->Release(); 
		return false; //add log
	}
	shaderblob->Release();
	pixelshaders[lastindex]->device = device;
	pixelshaders[lastindex]->name = shadername;
	return true;
}
bool dxmodule::shaderoperator::addvertexshader(WCHAR* filename, LPCWSTR shadername, LPCSTR entrypoint, LPCSTR shadermodel, ID3D11Device* device) {
	ID3DBlob* shaderblob = nullptr;
	HRESULT hr = compileshader(filename, entrypoint, shadermodel, &shaderblob);
	if (FAILED(hr)) return false; //add log
	vertexshaders.push_back(new vertexshader);
	size_t lastindex = vertexshaders.size() - 1;
	hr = device->CreateVertexShader(shaderblob->GetBufferPointer(), shaderblob->GetBufferSize(), NULL, &vertexshaders[lastindex]->shader);
	if (FAILED(hr)) {
		delete vertexshaders[lastindex];
		vertexshaders.pop_back();
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
	hr = device->CreateInputLayout(layout, numelements, shaderblob->GetBufferPointer(),
		shaderblob->GetBufferSize(), &vertexshaders[lastindex]->shaderlayout);
	shaderblob->Release();
	if (FAILED(hr)) {
		delete vertexshaders[lastindex];
		vertexshaders.pop_back();
		return false; //add log
	}
	vertexshaders[lastindex]->device = device;
	vertexshaders[lastindex]->name = shadername;
	return true;
}
void dxmodule::shaderoperator::deletepixelshader(WCHAR* shadername) {
	size_t size = pixelshaders.size();
	for (size_t i; i < size; i++) {
		if (pixelshaders[i]->name == shadername) {
			pixelshaders.erase(pixelshaders.begin() + i);
			break;
		}
	}
}


dxmodule::shaderoperator::pixelshader::~pixelshader() {
	delete shader;
}
dxmodule::shaderoperator::vertexshader::~vertexshader() {
	delete shaderlayout;
	delete shader;
}
HRESULT dxmodule::shaderoperator::compileshader(WCHAR* file, LPCSTR entrypoint, LPCSTR profile, ID3DBlob** blobout) {
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
		if (errorblob != NULL) OutputDebugStringA((char*)errorblob->GetBufferPointer());
		if (errorblob) errorblob->Release(); //add log
		return hr;
	}
	if (errorblob) errorblob->Release();
	*blobout = shaderblob;
	return S_OK;
}