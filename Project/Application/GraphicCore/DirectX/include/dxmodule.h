#pragma once
#include <vector>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>			//DirectX::
#include "DirectXPackedVector.h"	//DirectX::PackedVector::
#include "../../../Window/include/window.h"
namespace dxmodule {
	struct simplevertex {
		DirectX::XMFLOAT3 position;	// Координаты точки в пространстве
		DirectX::XMFLOAT2 texture;	// Координаты текстуры
		DirectX::XMFLOAT3 normal;	// Нормаль вершины
	};
	struct constantbufferstruct {
		DirectX::XMMATRIX worldmatrix;		// Матрица мира
		DirectX::XMMATRIX viewmatrix;		// Матрица вида
		DirectX::XMMATRIX projectionmatrix;	// Матрица проекции
	};
	struct graphicobjectmatrixdata {
		float x = 0;
		float y = 0;
		float z = 0;
		float scalex = 0;
		float scaley = 0;
		float scalez = 0;
		float x_rotation = 0;
		float y_rotation = 0;
		float z_rotation = 0;
	};
	enum class graphicobjectdatatype {
		GODT_X,
		GODT_Y,
		GODT_Z,
		GODT_SCALE_X,
		GODT_SCALE_Y,
		GODT_SCALE_Z,
		GODT_ROTATION_X,
		GODT_ROTATION_Y,
		GODT_ROTATION_Z
	};
	class directx {
		public:
			directx(winmodule::window* win, bool useZbuffer = false);
			~directx();
			HRESULT getHR();
			ID3D11Device* getdevice();
			ID3D11DeviceContext* getdevicecontext();
			IDXGISwapChain* getswapchain();
			ID3D11RenderTargetView* getrendertargetview();
			D3D_DRIVER_TYPE getcur_drivetype();
			ID3D11Texture2D* getzbuffertexure();
			ID3D11DepthStencilView* getzbuffer();  
			bool resizedx(winmodule::window* win);
		private:
			HRESULT hr;
			D3D_DRIVER_TYPE cur_drivetype = D3D_DRIVER_TYPE_NULL;
			ID3D11Device* device = nullptr;
			ID3D11DeviceContext* devicecontext = nullptr;
			IDXGISwapChain* swapchain = nullptr;
			ID3D11RenderTargetView* rendertargetview = nullptr;
			ID3D11Texture2D* zbuffertexure = nullptr;             // Текстура буфера глубин
			ID3D11DepthStencilView* zbuffer = nullptr;  // Объект вида, буфер глубин
			bool createZbuffer(UINT w, UINT h, ID3D11Texture2D* &texure, ID3D11DepthStencilView* &buffer);
	};
	template<typename T>
	HRESULT createvertexbuffer(ID3D11Device* device, ID3D11Buffer* &bufferpointer, T structure[], size_t size) {
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = size * sizeof(T);
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = structure;
		HRESULT hr = device->CreateBuffer(&bd, &InitData, &bufferpointer);
		return hr;
	}
	HRESULT createindexbuffer(ID3D11Device* device, ID3D11Buffer* &bufferpointer, WORD* structure, size_t size);	//почему-то нельзя inline
	HRESULT createconstbuffer(ID3D11Device* device, ID3D11Buffer* &bufferpointer);									//почему-то нельзя inline
	inline HRESULT compileshader(WCHAR* file, LPCSTR entrypoint, LPCSTR profile, ID3DBlob* &blobout);
	class shaderoperator {
		public:
			shaderoperator();
			virtual ~shaderoperator();
			virtual bool addshader(WCHAR* filename, WCHAR* shadername, LPCSTR entrypoint, LPCSTR shadermodel, ID3D11Device* device) = 0;
			int getshaderid(WCHAR* shadername);
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
	class pixelshaderoperator : public shaderoperator {
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
	class vertexshaderoperator : public shaderoperator {
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





	class graphicobject {
		public:
			graphicobject(WCHAR* name, simplevertex* bounds, uint32_t id = 0);
			~graphicobject();
			void setPSname(WCHAR* newPSname, shaderoperator* pixelshaders = nullptr);
			void setVSname(WCHAR* newVSname, shaderoperator* vertexshaders = nullptr);
			void optimiseshaderid(shaderoperator* pixelshaders, shaderoperator* vertexshaders);
			void setbounds(simplevertex* newbounds);
			bool addchild(graphicobject* child);
			bool deletechild(WCHAR* childname, uint32_t childid = 0);
			void removechild(WCHAR* childname, uint32_t childid = 0, graphicobject* newparent = nullptr);
			void setparent(graphicobject* newparent);
			void setdatatype(graphicobjectdatatype type, bool isabsolute);
			void setobjectdata(graphicobjectdatatype type, float value);
			std::vector<uint32_t> getidforname(WCHAR* childname);
			uint32_t getid();
			WCHAR* getname();
			WCHAR* getPSname();
			WCHAR* getVSname();
			size_t getPSid();
			size_t getVSid();
			std::vector<graphicobject*> getchilds();
			graphicobject* getparent();
			simplevertex* getbounds();
			bool getuseabsolutedata(graphicobjectdatatype type);
			bool getoptimisedshaderid();
			bool getcanberendered();
			float getobjectdata(graphicobjectdatatype type, bool forceabsolute = false);
		private:
			uint32_t id;							//id объекта, для хранения нескольких объектов с одинаковым именем
			WCHAR* name;							//имя объекта для работы 
			WCHAR* PSname = nullptr;				//имя пиксельного шейдера
			WCHAR* VSname = nullptr;				//имя вершинного шейдера
			size_t PSid = 0;						//id быстрого доступа для пиксельного шейдера
			size_t VSid = 0;						//id быстрого доступа для вершинного шейдера
			std::vector<graphicobject*> childs;		//зависимые объекты
			graphicobject* parent;					//родительский объект
			simplevertex* bounds;					//точки в матрице xzy
			graphicobjectmatrixdata relativedata;	//относительные данные объекта
			bool useabsolutedata[9];				//объект использует абсолютные координаты (если да, то какие параметры)
			bool optimisedshaderid;					//объект имеет id шейдера для быстрого доступа
			bool canberendered = false;				//может ли объект быть отрисован (false, если при поиске по имени шейдера результат -1)
	};
}