#pragma once
#include <functional>
#include "../../../Window/include/window.h"
#include "dxmodule.h"
#include <Windows.h>
namespace dxwindow {
	static const UINT stride = sizeof(dxmodule::simplevertex);
	class dxwindowclass : public winmodule::window {
		public:
			dxwindowclass(HINSTANCE hinstance = NULL, bool updatedxmodule = false, RECT rect = {0,0,0,0});
			~dxwindowclass();

			void setupdatedx(bool updatedxmodule);
			bool getupdatedx();		
			winmodule::extendedwinproc* getwinproc();
			dxmodule::directx* getdx();					//temp
			void setW(uint32_t width);					//��������� ������ ���� (override)
			void setH(uint32_t height);					//��������� ������ ���� (override)

			bool applyvertexshader(size_t shaderid);
			bool applypixelshader(size_t shaderid);
			//void optimise(vector<graphicobject>* gov);	//��� ���� ����� ��������� shadername � shaderid, ��� ����� �������� �������
			bool addshader(WCHAR* filename, WCHAR* shadername, LPCSTR entrypoint, LPCSTR shadermodel, bool ispixelshader);
			bool hasshader(WCHAR* shadername, bool ispixelshader);
			void deleteshader(WCHAR* shadername, bool ispixelshader);
			size_t getshadervectorsize(bool ispixelshader);
			std::vector<WCHAR*> getshaderlist(bool ispixelshader);


		private:
			bool updatedx = false;
			HRESULT hr;
			winmodule::extendedwinproc* winproc;
			dxmodule::directx* dx;
			dxmodule::pixelshaderoperator* pso = new dxmodule::pixelshaderoperator;
			dxmodule::vertexshaderoperator* vso = new dxmodule::vertexshaderoperator;
			static const UINT offset = 0;
	};

}