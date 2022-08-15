#pragma once
//���������� ����������
#include <Windows.h>
#include <stdint.h>
#include <vector>
//�����������
#define DEFAULT_CURSOR LoadCursor(NULL, IDC_ARROW)
#define DEFAULT_ICON LoadIcon(NULL, IDI_APPLICATION)
//#define
namespace winmodule {
	//���������� ������������
	enum class windowshowmode {
		fullscreen,
		customsize
	};
	//���������� �������
	RECT getscreensize();
	HRESULT initwindow(HWND& window, HINSTANCE hinstance, WCHAR* classname, WCHAR* title, WNDPROC wndproc, RECT rect = {0,0,0,0}, HICON hicon = DEFAULT_ICON, HCURSOR hcursor = DEFAULT_CURSOR);
	//���������� �������
	class window {
		public:
			//��������� � ����������
			window(HINSTANCE hinstance, WNDPROC wndproc, WCHAR* classname, WCHAR* title, RECT rect = {0,0,0,0});
			virtual ~window();
			uint32_t getX();								//��������� � ������� ����
			uint32_t getY();								//��������� � ������� ����
			void setX(uint32_t xpos);						//��������� � ������� ����
			void setY(uint32_t ypos);						//��������� � ������� ����
			uint32_t getW();								//��������� ������ ����
			uint32_t getH();								//��������� ������ ����
			virtual void setW(uint32_t width);				//��������� ������ ����
			virtual void setH(uint32_t height);				//��������� ������ ����
			windowshowmode getshowmode();					//�������� ��� �����������
			void setshowmode(windowshowmode showmode);		//���������� ��� �����������
			bool getV();									//�������� ���������
			void setV(bool visible);						//���������� ���������
			WCHAR* gettitle();								//�������� ���������
			void settitle(WCHAR* wintitle);					//���������� ���������
			WCHAR* getclassname();							//�������� ��� ������					
			HWND getwindow();								//�������� ��������� �� ����
			HRESULT getHR();								//�������� ������ ���������� �������� ����
		protected:
			HRESULT hr;										//������ ���������� �������� ����
			WCHAR* classname;								//��� ������ ����
			WCHAR* title;									//��������� ����
			HWND win;										//����
			uint32_t x, y, w, h;							//��������� � ������ ����									
			windowshowmode sm = windowshowmode::fullscreen;	//���  ���� (���������� �����)
			uint32_t sw_type = SW_HIDE;						//��� ������ ����
			bool v = false;									//������ �� ����
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