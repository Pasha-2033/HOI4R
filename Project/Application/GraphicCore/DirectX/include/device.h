#pragma once
#include <vector>
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
			ID3D11Device* getdevice();
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
	template<typename T>
	HRESULT createvertexbuffer(ID3D11Device* device, ID3D11Buffer** bufferpointer, T structure[]) {
		ID3D11Buffer* buffer = nullptr;
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(T) * 3;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = structure;
		HRESULT hr = device->CreateBuffer(&bd, &InitData, &buffer);
		*bufferpointer = buffer;
		return hr;
	}
	inline HRESULT compileshader(WCHAR* file, LPCSTR entrypoint, LPCSTR profile, ID3DBlob** blobout);
	class shaderoperator {
		public:
			shaderoperator();
			virtual ~shaderoperator();
			virtual bool addshader(WCHAR* filename, WCHAR* shadername, LPCSTR entrypoint, LPCSTR shadermodel, ID3D11Device* device) = 0;
			size_t getshaderid(WCHAR* shadername);
			size_t getvectorsize();
			void deleteshader(size_t shaderid);
			void updatehandled();
			ID3D11Device* getshaderdevice(size_t shaderid);
			WCHAR* getname(size_t shaderid);
			bool getupdatestate();
		protected:
			struct shaderparentclass {	
				ID3D11Device* device = nullptr;					
				WCHAR* name = nullptr;
				virtual ~shaderparentclass();
			};
			std::vector<shaderparentclass*> shaders = {};
			size_t size = 0;
		private:
			bool updatestate = true;
	};
	class pixelshaderoperator : virtual public shaderoperator {
		public:
			pixelshaderoperator();
			~pixelshaderoperator();
			ID3D11PixelShader* getpixelshader(size_t shaderid);
			bool addshader(WCHAR* filename, WCHAR* shadername, LPCSTR entrypoint, LPCSTR shadermodel, ID3D11Device* device);
		private:
			struct pixelshader : shaderparentclass {
				~pixelshader();
				ID3D11PixelShader* shader = nullptr;
			};
	};
	class vertexshaderoperator : virtual public shaderoperator {
		public:
			vertexshaderoperator();
			~vertexshaderoperator();
			ID3D11InputLayout* getshaderlayout(size_t shaderid);
			ID3D11VertexShader* getvertexshader(size_t shaderid);
			bool addshader(WCHAR* filename, WCHAR* shadername, LPCSTR entrypoint, LPCSTR shadermodel, ID3D11Device* device);
		private:
			struct vertexshader : shaderparentclass {
				~vertexshader();
				ID3D11InputLayout* shaderlayout = nullptr;
				ID3D11VertexShader* shader = nullptr;
			};
	};


	/*class shaderoperator {
		public:
			shaderoperator();
			~shaderoperator();
			bool addpixelshader(WCHAR* filename, WCHAR* shadername, LPCSTR entrypoint, LPCSTR shadermodel, ID3D11Device* device);
			bool addvertexshader(WCHAR* filename, WCHAR* shadername, LPCSTR entrypoint, LPCSTR shadermodel, ID3D11Device* device);
			bool getpixelshaderupdatedstate();
			bool getvertexshaderupdatedstate();
			void deletepixelshader(WCHAR* shadername);
			void deletevertexshader(WCHAR* shadername);
		private:
			struct pixelshader {
				ID3D11PixelShader* shader;
				ID3D11Device* device;
				WCHAR* name;
				~pixelshader();
			};
			struct vertexshader {
				ID3D11InputLayout* shaderlayout;
				ID3D11VertexShader* shader;
				ID3D11Device* device;
				WCHAR* name;
				~vertexshader();
			};
			HRESULT compileshader(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
			std::vector<pixelshader*> pixelshaders;
			std::vector<vertexshader*> vertexshaders;
			bool pixelshaderupdated = true;
			bool vertexshaderupdated = true;
	};*/
}