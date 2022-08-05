#pragma once
//ќбъ€вление заголовков
#include <Windows.h>
#include <stdint.h>
//ќпределени€
#define DEFAULT_CURSOR LoadCursor(NULL, IDC_ARROW)
#define DEFAULT_ICON LoadIcon(NULL, IDI_APPLICATION)
namespace winmodule {
	//ќбъ€вление перечислений
	enum windowshowmode {
		fullscreen,
		customsize
	};
	//ќбъ€вление функций
	RECT getscreensize();
	HRESULT initwindow(HWND &window, HINSTANCE hinstance, LPCTSTR classname, LPCTSTR title, WNDPROC wndproc, RECT rect, HICON hicon = DEFAULT_ICON, HCURSOR hcursor = DEFAULT_CURSOR);
	//ќбъ€вление классов
	class window {
		public:
			//»нициаци€ и деструкци€
			window(WNDPROC wndproc, LPCTSTR classname, LPCTSTR title, RECT rect = getscreensize());
			~window();
			uint32_t getX();						//получение х позиции окна
			uint32_t getY();						//получение у позиции окна
			void setX(uint32_t xpos);				//установка х позиции окна
			void setY(uint32_t ypos);				//установка у позиции окна
			uint32_t getW();						//получение ширины окна
			uint32_t getH();						//получение высоты окна
			void setW(uint32_t width);				//установка ширины окна
			void setH(uint32_t height);				//установка высоты окна
			uint32_t getshowmode();					//получить тип отображени€
			void setshowmode(uint32_t showmode);	//установить тип отображени€
			bool getV();							//получить видимость
			void setV(bool visible);				//установить видимость
			LPCTSTR gettitle();						//получить заголовок
			void settitle(LPCTSTR wintitle);		//установить заголовок
			LPCTSTR getclassname();					//получить им€ класса					
			HWND getwindow();						//получить указатель на окно
			HRESULT getHR();						//получить статус результата создани€ окна
		private:
			HRESULT hr;								//статус результата создани€ окна
			LPCTSTR classname;						//им€ класса окна
			LPCTSTR title;							//заголовок окна
			HINSTANCE hinstance = NULL;				//дескриптор экземпл€ра
			HWND win;								//окно
			uint32_t x;								//позици€ окна по х
			uint32_t y;								//позици€ окна по у
			uint32_t w;								//ширина окна
			uint32_t h;								//высота окна
			uint32_t sm = fullscreen;				//тип  окна (управление окном)
			uint32_t sw_type = SW_HIDE;				//тип показа окна
			bool v = false;							//видимо ли окно
	};
}