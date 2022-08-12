#pragma once
#include <functional>
#include "../../../Window/include/window.h"
#include "dxmodule.h"
#include <Windows.h>
namespace dxwindow {
	class dxwindowclass {
		public:
			dxwindowclass(HINSTANCE hinstance = NULL);
			~dxwindowclass();
			winmodule::extendedwinproc* getwinproc();
			winmodule::window* getwindow();
			dxmodule::directx* getdx();
			HRESULT getHR();
		private:
			HRESULT hr;
			winmodule::extendedwinproc* winproc;
			winmodule::window* window;
			dxmodule::directx* dx;
	};

}