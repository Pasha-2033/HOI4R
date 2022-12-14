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
			void setX_rotation(float value);
			void setY_rotation(float value);
			void setZ_rotation(float value);
			float getX();
			float getY();
			float getZ();
			float getW();
			float getX_rotation();
			float getY_rotation();
			float getZ_rotation();
		private:
			void recalculatevectros();
			float x = 0.0f;
			float y = 0.0f;
			float z = 0.0f;
			float x_rotation = 0.0f;
			float y_rotation = 0.0f;
			float z_rotation = 0.0f;
			float w = 1.0f;	//?????????? ??????????? - ????????????? ? ????????? ???????????? (????? ?????????, ?? ?????? ?? ???????)
			DirectX::XMMATRIX mymatrix;
	};
	class dxwindowclass : public winmodule::window {
		public:
			dxwindowclass(HINSTANCE hinstance = NULL, bool updatedxmodule = false, bool useZbuffer = false, RECT rect = {0,0,0,0});
			~dxwindowclass();

			void setupdatedx(bool updatedxmodule);
			bool getupdatedx();		
			winmodule::extendedwinproc* getwinproc();
			dxmodule::directx* getdx();					//temp
			void setW(uint32_t width);					//????????? ?????? ???? (override)
			void setH(uint32_t height);					//????????? ?????? ???? (override)

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
			ID3D11Buffer* vertexbuffer = nullptr;						//
			ID3D11Buffer* indexbuffer = nullptr;						//
			ID3D11Buffer* constbuffer = nullptr;						//
			camera* getcamera();

			void setznear(float z);
			void setzfar(float z);
			float getznear();
			float getzfar();

		private:
			bool updatedx = false;
			HRESULT hr;
			winmodule::extendedwinproc* winproc;
			dxmodule::directx* dx;
			dxmodule::pixelshaderoperator* pso = new dxmodule::pixelshaderoperator;
			dxmodule::vertexshaderoperator* vso = new dxmodule::vertexshaderoperator;
			//ID3D11Buffer* vertexbuffer = nullptr;
			//ID3D11Buffer* indexbuffer = nullptr;
			//ID3D11Buffer* constbuffer = nullptr;
			//DirectX::XMMATRIX worldmatrix = DirectX::XMMatrixIdentity();
			//DirectX::XMMATRIX viewmatrix;
			//DirectX::XMMATRIX projectionmatrix;
			float znear = 0.001f;	//?? ????????? ?????? ?? ?? ?????
			float zfar = 1.0f;		//?? ????????? ?????? ?? ?????
			camera* hwndcamera = new camera;
	};

}