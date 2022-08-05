#pragma once
//���������� ����������
#include <Windows.h>
#include <stdint.h>
//�����������
#define DEFAULT_CURSOR LoadCursor(NULL, IDC_ARROW)
#define DEFAULT_ICON LoadIcon(NULL, IDI_APPLICATION)
namespace winmodule {
	//���������� ������������
	enum windowshowmode {
		fullscreen,
		customsize
	};
	//���������� �������
	RECT getscreensize();
	HRESULT initwindow(HWND &window, HINSTANCE hinstance, LPCTSTR classname, LPCTSTR title, WNDPROC wndproc, RECT rect, HICON hicon = DEFAULT_ICON, HCURSOR hcursor = DEFAULT_CURSOR);
	//���������� �������
	class window {
		public:
			//��������� � ����������
			window(WNDPROC wndproc, LPCTSTR classname, LPCTSTR title, RECT rect = getscreensize());
			~window();
			uint32_t getX();						//��������� � ������� ����
			uint32_t getY();						//��������� � ������� ����
			void setX(uint32_t xpos);				//��������� � ������� ����
			void setY(uint32_t ypos);				//��������� � ������� ����
			uint32_t getW();						//��������� ������ ����
			uint32_t getH();						//��������� ������ ����
			void setW(uint32_t width);				//��������� ������ ����
			void setH(uint32_t height);				//��������� ������ ����
			uint32_t getshowmode();					//�������� ��� �����������
			void setshowmode(uint32_t showmode);	//���������� ��� �����������
			bool getV();							//�������� ���������
			void setV(bool visible);				//���������� ���������
			LPCTSTR gettitle();						//�������� ���������
			void settitle(LPCTSTR wintitle);		//���������� ���������
			LPCTSTR getclassname();					//�������� ��� ������					
			HWND getwindow();						//�������� ��������� �� ����
			HRESULT getHR();						//�������� ������ ���������� �������� ����
		private:
			HRESULT hr;								//������ ���������� �������� ����
			LPCTSTR classname;						//��� ������ ����
			LPCTSTR title;							//��������� ����
			HINSTANCE hinstance = NULL;				//���������� ����������
			HWND win;								//����
			uint32_t x;								//������� ���� �� �
			uint32_t y;								//������� ���� �� �
			uint32_t w;								//������ ����
			uint32_t h;								//������ ����
			uint32_t sm = fullscreen;				//���  ���� (���������� �����)
			uint32_t sw_type = SW_HIDE;				//��� ������ ����
			bool v = false;							//������ �� ����
	};
}