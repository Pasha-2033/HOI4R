#pragma once
#include <functional>
#include "../../../Window/include/window.h"
#include "dxmodule.h"
#include <Windows.h>
namespace dxwindow {
	static const UINT stride = sizeof(dxmodule::simplevertex); //to do (dxmodule::simplevertex) 
	static const UINT offset = 0;
	class camera {
		public:
			camera();
			~camera();
			DirectX::XMMATRIX getviewpoint();
			void setX(float value);
			void setY(float value);
			void setZ(float value);
			void setW(float value);
			void setXY_rotation(float value);
			void setXZ_rotation(float value);
			void setYZ_rotation(float value);
			float getX();
			float getY();
			float getZ();
			float getW();
			float getXY_rotation();
			float getXZ_rotation();
			float getYZ_rotation();
		private:
			void recalculatevectros();
			float x = 0.0f;
			float y = 0.0f;
			float z = 0.0f;
			float xy_rotation = 0.0f;
			float xz_rotation = 0.0f;
			float yz_rotation = 0.0f;
			float w = 1.0f;	//Разделение перспективы - трансформация в усеченное пространство (очень интересно, но ничего не понятно)
			DirectX::XMMATRIX mymatrix;
	};
	class dxwindowclass : public winmodule::window {
		public:
			dxwindowclass(HINSTANCE hinstance = NULL, bool updatedxmodule = false, RECT rect = {0,0,0,0});
			~dxwindowclass();

			void setupdatedx(bool updatedxmodule);
			bool getupdatedx();		
			winmodule::extendedwinproc* getwinproc();
			dxmodule::directx* getdx();					//temp
			void setW(uint32_t width);					//установка ширины окна (override)
			void setH(uint32_t height);					//установка высоты окна (override)

			bool applyvertexshader(size_t shaderid);
			bool applypixelshader(size_t shaderid);
			bool addshader(WCHAR* filename, WCHAR* shadername, LPCSTR entrypoint, LPCSTR shadermodel, bool ispixelshader);
			bool hasshader(WCHAR* shadername, bool ispixelshader);
			void deleteshader(WCHAR* shadername, bool ispixelshader);
			size_t getshadervectorsize(bool ispixelshader);
			std::vector<WCHAR*> getshaderlist(bool ispixelshader);

			DirectX::XMMATRIX worldmatrix = DirectX::XMMatrixIdentity();//temp
			DirectX::XMMATRIX viewmatrix;								//
			DirectX::XMMATRIX projectionmatrix;							//
			camera* getcamera();

			void setznear(float z);
			void setzfar(float z);
			float getznear();
			float getzfar();

			//void forceupdatedx(<some_mode = all>); //чтобы изменения применились не вовремя установки параметров, а только когда это нужно (будет полезно только при updatedx = true)
			//void renderobject(SimpleVertex verticles, size_t vertexsize, WORD indices, size_t indexsize);
		private:
			bool updatedx = false;
			HRESULT hr;
			winmodule::extendedwinproc* winproc;
			dxmodule::directx* dx;
			dxmodule::pixelshaderoperator* pso = new dxmodule::pixelshaderoperator;
			dxmodule::vertexshaderoperator* vso = new dxmodule::vertexshaderoperator;
			ID3D11Buffer* vertexbuffer = nullptr;
			ID3D11Buffer* indexbuffer = nullptr;
			ID3D11Buffer* constbuffer = nullptr;
			//DirectX::XMMATRIX worldmatrix = DirectX::XMMatrixIdentity();
			//DirectX::XMMATRIX viewmatrix;
			//DirectX::XMMATRIX projectionmatrix;
			float znear = 0.001f;	//то насколько близко мы не видим
			float zfar = 1.0f;		//то насколько далеко мы видим
			camera* hwndcamera = new camera;
	};

}