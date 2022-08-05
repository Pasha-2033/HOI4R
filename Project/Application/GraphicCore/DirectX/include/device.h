#pragma once
#include <vector>
#include <variant>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>			//DirectX::
#include "DirectXPackedVector.h"	//DirectX::PackedVector::
#include "../../../Window/include/window.h"
//есть заметки в cpp!!!
namespace dxmodule {
	class directx {
		public:
			directx(winmodule::window* win);
			~directx();
			HRESULT getHR();
			ID3D11DeviceContext* getdevicecontext();
			IDXGISwapChain* getswapchain();
			ID3D11RenderTargetView* getrendertargetview();
		private:
			HRESULT hr;
			D3D_DRIVER_TYPE cur_drivetype;
			ID3D11Device* device;
			ID3D11DeviceContext* devicecontext;
			IDXGISwapChain* swapchain;
			ID3D11RenderTargetView* rendertargetview;
	};
	class shaderoperator {
		public:
			shaderoperator();
			~shaderoperator();
			ID3D11PixelShader* getpixelshader(LPCWSTR shadername);
			ID3D11VertexShader* getvertexshader(LPCWSTR shadername);
			ID3D11Device* getpixelshaderdevice(LPCWSTR shadername);
			ID3D11Device* getvertexshaderdevice(LPCWSTR shadername);
			bool addpixelshader(WCHAR* filename, LPCWSTR shadername, LPCSTR entrypoint, LPCSTR shadermodel, ID3D11Device* device);
			bool addvertexshader(WCHAR* filename, LPCWSTR shadername, LPCSTR entrypoint, LPCSTR shadermodel, ID3D11Device* device);
			void deletepixelshader(WCHAR* shadername);
			void deletevertexshader(WCHAR* shadername);
		private:
			struct pixelshader {
				ID3D11PixelShader* shader;
				ID3D11Device* device;
				LPCWSTR name;
				~pixelshader();
			};
			struct vertexshader {
				ID3D11InputLayout* shaderlayout;
				ID3D11VertexShader* shader;
				ID3D11Device* device;
				LPCWSTR name;
				~vertexshader();
			};
			HRESULT compileshader(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
			std::vector<pixelshader*> pixelshaders;
			std::vector<vertexshader*> vertexshaders;
	};
	struct simplevertex {
		DirectX::XMFLOAT3 pos;
	};
}