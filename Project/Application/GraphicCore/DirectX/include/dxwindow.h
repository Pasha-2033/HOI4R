#pragma once
#include <functional>
#include "../../../Window/include/window.h"
#include "dxmodule.h"
#include <Windows.h>
namespace dxwindow {
	class dxwindowclass {
		public:
			dxwindowclass();
			~dxwindowclass();
			winmodule::extendedwinproc* getwinproc();
			winmodule::window* getwindow();
			dxmodule::directx* getdx();
		private:
			winmodule::extendedwinproc* winproc = new winmodule::extendedwinproc;
			winmodule::window* window = new winmodule::window(winmodule::extendedwinproc::wndproc, (WCHAR*)L"tempclass", (WCHAR*)L"title");
			dxmodule::directx* dx = new dxmodule::directx(window);
	};

}