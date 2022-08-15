#pragma once
#include <functional>
#include "../../../Window/include/window.h"
#include "dxmodule.h"
#include <Windows.h>
namespace dxwindow {
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

			dxmodule::pixelshaderoperator* pso = new dxmodule::pixelshaderoperator; //temp put to private
			dxmodule::vertexshaderoperator* vso = new dxmodule::vertexshaderoperator;//temp put to private
		private:
			bool updatedx = false;
			HRESULT hr;
			winmodule::extendedwinproc* winproc;
			dxmodule::directx* dx;
			//dxmodule::pixelshaderoperator* pso;
			//dxmodule::vertexshaderoperator* vso;
			ID3D11Buffer* vertexbuffer = nullptr;
			static const UINT offset = 0;
	};

}