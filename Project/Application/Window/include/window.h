#pragma once
//ќбъ€вление заголовков
#include <Windows.h>
#include <stdint.h>
#include <vector>
//ќпределени€
#define DEFAULT_CURSOR LoadCursor(NULL, IDC_ARROW)
#define DEFAULT_ICON LoadIcon(NULL, IDI_APPLICATION)
//#define
namespace winmodule {
	//ќбъ€вление перечислений
	enum class windowshowmode {
		fullscreen,
		customsize
	};
	//ќбъ€вление функций
	RECT getscreensize();
	HRESULT initwindow(HWND& window, HINSTANCE hinstance, WCHAR* classname, WCHAR* title, WNDPROC wndproc, RECT rect = {0,0,0,0}, HICON hicon = DEFAULT_ICON, HCURSOR hcursor = DEFAULT_CURSOR);
	//ќбъ€вление классов
	class window {
		public:
			//»нициаци€ и деструкци€
			window(HINSTANCE hinstance, WNDPROC wndproc, WCHAR* classname, WCHAR* title, RECT rect = {0,0,0,0});
			virtual ~window();
			uint32_t getX();								//получение х позиции окна
			uint32_t getY();								//получение у позиции окна
			void setX(uint32_t xpos);						//установка х позиции окна
			void setY(uint32_t ypos);						//установка у позиции окна
			uint32_t getW();								//получение ширины окна
			uint32_t getH();								//получение высоты окна
			virtual void setW(uint32_t width);				//установка ширины окна
			virtual void setH(uint32_t height);				//установка высоты окна
			windowshowmode getshowmode();					//получить тип отображени€
			void setshowmode(windowshowmode showmode);		//установить тип отображени€
			bool getV();									//получить видимость
			void setV(bool visible);						//установить видимость
			WCHAR* gettitle();								//получить заголовок
			void settitle(WCHAR* wintitle);					//установить заголовок
			WCHAR* getclassname();							//получить им€ класса					
			HWND getwindow();								//получить указатель на окно
			HRESULT getHR();								//получить статус результата создани€ окна
		protected:
			HRESULT hr;										//статус результата создани€ окна
			WCHAR* classname;								//им€ класса окна
			WCHAR* title;									//заголовок окна
			HWND win;										//окно
			uint32_t x, y, w, h;							//положение и размер окна									
			windowshowmode sm = windowshowmode::fullscreen;	//тип  окна (управление окном)
			uint32_t sw_type = SW_HIDE;						//тип показа окна
			bool v = false;									//видимо ли окно
	};
	class winsubproc {
		public:
			winsubproc(WCHAR* name);
			virtual ~winsubproc();
			virtual LRESULT CALLBACK proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) = 0;
			WCHAR* getname();
		private:
			WCHAR* name;
	};
	class extendedwinproc {
		public:
			extendedwinproc();
			virtual ~extendedwinproc();
			static LRESULT CALLBACK wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
			bool addsubproc(winsubproc* subproc);
			void deletesubproc(WCHAR* name);
			size_t getsize();
			winsubproc* getsubproc(WCHAR* name);
			winsubproc* getsubproc(size_t id);
		private:
			std::vector<winsubproc*> eventhandler;
			size_t size = 0;
	};
	class defaultwinproc : public winsubproc {
		public:
			defaultwinproc(WCHAR* name);
			virtual ~defaultwinproc();
			LRESULT CALLBACK proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	};
}