#include "stdafx.h"
#include "simple_base_converter_msvc.h"

#include <fcntl.h> //???
#include <io.h> //???
#include <iostream>
#include <string>
#include <cstdlib>
//#include <fstream>
#include <Windows.h>
#include <Commctrl.h>


const int EDIT_TEXT_MAXLEN = 40;

wchar_t app_name[] = L"Simple Base Converter";
wchar_t app_description[] = L"Simple base converter by gimadutdin aka marmelad33 aka rgimad aka R G";
HBRUSH main_bk_brush;

bool setEditTextProgramatically = false;


LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void createDebugConsole() {
	AllocConsole();
	// attach the new console to this application's process
	AttachConsole(GetCurrentProcessId());
	// reopen the std I/O streams to redirect I/O to the new console
	FILE *stream;
	freopen_s(&stream, "CON", "w", stdout);
	freopen_s(&stream, "CON", "w", stderr);
	freopen_s(&stream, "CON", "r", stdin);
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//createDebugConsole();
	HWND hwnd; // handle of main window
	MSG msg; // message structure
	WNDCLASS wc; // window class structure
	memset(&wc, 0, sizeof(WNDCLASSA));

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.hInstance = hInstance;
	main_bk_brush = CreateSolidBrush(RGB(52, 152, 219));
	wc.hbrBackground = main_bk_brush;
	wc.lpszClassName = L"MyClass";

	RegisterClass(&wc);

	hwnd = CreateWindow(L"MyClass", app_name, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX, 500, 200, 430, 230, NULL, NULL, hInstance, NULL);

	ShowWindow(hwnd, nCmdShow); // show created windows
	UpdateWindow(hwnd); // redraw window

						//  message processing loop
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	HINSTANCE hInst;
	//PAINTSTRUCT ps;
	static HWND hBtn; // button descriptor
	static HWND hEditDec; // edit for decimal number
	static HWND hEditBin;
	static HWND hEditHex;
	static HWND hEditOct;

	static HWND hLabelDec;
	static HWND hLabelBin;
	static HWND hLabelHex;
	static HWND hLabelOct;

	static HDC hdcStatic;

	switch (Message)
	{
	case WM_CREATE: // if window create message
	{
		hInst = ((LPCREATESTRUCT)lParam)->hInstance; // get application instance
													 //was FW_SEMIBOLD before false
		HFONT hFont1 = CreateFont(18, 0, 0, 0, 0, false, false, false, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, L"Arial");

		// Create and show button
		hBtn = CreateWindow(L"button", L"About", WS_CHILD | WS_VISIBLE | WS_BORDER, 350, 20, 50, 30, hwnd, 0, hInst, NULL);
		SendMessage(hBtn, WM_SETFONT, (LPARAM)GetStockObject(DEFAULT_GUI_FONT), true);
		ShowWindow(hBtn, SW_SHOWNORMAL);//

		hLabelDec = CreateWindow(L"static", L"ST_DEC", WS_CHILD | WS_VISIBLE | WS_TABSTOP, 15, 20 + 2, 50, 25, hwnd, 0, hInst, NULL);
		SetWindowText(hLabelDec, L"DEC:");
		SendMessage(hLabelDec, WM_SETFONT, (WPARAM)hFont1, TRUE);
		ShowWindow(hLabelDec, SW_SHOWNORMAL);

		hEditDec = CreateWindow(L"edit", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL, 70, 20, 270, 25, hwnd, 0, hInst, NULL);
		SendMessage(hEditDec, WM_SETFONT, (WPARAM)hFont1, TRUE);
		//SendMessage(hEditDec, EM_SETLIMITTEXT, 25, 0);
		//SendMessage(hEditDec, EM_SETCUEBANNER, FALSE, (LPARAM)L"azazaz");
		//Edit_SetCueBannerText(hEditDec, L"Enter");
		ShowWindow(hEditDec, SW_SHOWNORMAL);

		//--------------------------------------------------------------------------------
		hLabelBin = CreateWindow(L"static", L"ST_BIN", WS_CHILD | WS_VISIBLE | WS_TABSTOP, 15, 60 + 2, 50, 25, hwnd, 0, hInst, NULL);
		SetWindowText(hLabelBin, L"BIN:");
		SendMessage(hLabelBin, WM_SETFONT, (WPARAM)hFont1, TRUE);
		ShowWindow(hLabelBin, SW_SHOWNORMAL);

		hEditBin = CreateWindow(L"edit", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL, 70, 60, 270, 25, hwnd, 0, hInst, NULL);
		SendMessage(hEditBin, WM_SETFONT, (WPARAM)hFont1, TRUE);
		ShowWindow(hEditBin, SW_SHOWNORMAL);

		//--------------------------------------------------------------------------------
		hLabelHex = CreateWindow(L"static", L"ST_HEX", WS_CHILD | WS_VISIBLE | WS_TABSTOP, 15, 100 + 2, 50, 25, hwnd, 0, hInst, NULL);
		SetWindowText(hLabelHex, L"HEX:");
		SendMessage(hLabelHex, WM_SETFONT, (WPARAM)hFont1, TRUE);
		ShowWindow(hLabelHex, SW_SHOWNORMAL);

		hEditHex = CreateWindow(L"edit", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL, 70, 100, 270, 25, hwnd, 0, hInst, NULL);
		SendMessage(hEditHex, WM_SETFONT, (WPARAM)hFont1, TRUE);
		ShowWindow(hEditHex, SW_SHOWNORMAL);

		//--------------------------------------------------------------------------------
		hLabelOct = CreateWindow(L"static", L"ST_OCT", WS_CHILD | WS_VISIBLE | WS_TABSTOP, 15, 140 + 2, 50, 25, hwnd, 0, hInst, NULL);
		SetWindowText(hLabelOct, L"OCT:");
		SendMessage(hLabelOct, WM_SETFONT, (WPARAM)hFont1, TRUE);
		ShowWindow(hLabelOct, SW_SHOWNORMAL);

		hEditOct = CreateWindow(L"edit", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL, 70, 140, 270, 25, hwnd, 0, hInst, NULL);
		SendMessage(hEditOct, WM_SETFONT, (WPARAM)hFont1, TRUE);
		ShowWindow(hEditOct, SW_SHOWNORMAL);
	}
	break;

	case WM_CTLCOLORSTATIC:
	{
		if ((HWND)lParam == hLabelDec || (HWND)lParam == hLabelBin || (HWND)lParam == hLabelOct || (HWND)lParam == hLabelHex)
		{
			hdcStatic = (HDC)wParam;
			SetTextColor(hdcStatic, RGB(0, 0, 0));
			SetBkMode(hdcStatic, TRANSPARENT);

			return (LRESULT)GetStockObject(NULL_BRUSH);
		}
	}

	case WM_COMMAND:  // if command message
	{
		//std::cout << HIWORD(wParam) << std::endl;
		if (HIWORD(wParam) == BN_CLICKED)
		{
			if (lParam == (LPARAM)hBtn) // if click on hBtn button happened
			{
				MessageBox(NULL, app_description, app_name, MB_OK | MB_ICONINFORMATION);
			}
		}
		else if (HIWORD(wParam) == EN_CHANGE)
		{
			if (setEditTextProgramatically) break;

			//std::cout << "lParam = " << lParam << std::endl;

			char edit_text[EDIT_TEXT_MAXLEN], tmp[EDIT_TEXT_MAXLEN]; size_t pos;
			if ((HWND)lParam == hEditDec)
			{
				GetWindowTextA(hEditDec, edit_text, EDIT_TEXT_MAXLEN);
				try
				{
					int num = stoi(std::string(edit_text), &pos, 10);
					setEditTextProgramatically = true;
					_itoa_s(num, (char*)tmp, 40, 2); SetWindowTextA(hEditBin, tmp); 
					_itoa_s(num, (char*)tmp, 40, 8); SetWindowTextA(hEditOct, tmp);
					_itoa_s(num, (char*)tmp, 40, 16); SetWindowTextA(hEditHex, tmp);
					setEditTextProgramatically = false;
				} catch (...) { }
			} else if ((HWND)lParam == hEditBin)
			{
				//std::cout << "lParam = " << lParam << std::endl;
				GetWindowTextA(hEditBin, edit_text, EDIT_TEXT_MAXLEN);
				//std::cout << stoi(std::string(edit_text), &pos, 2) << " " << edit_text << std::endl;
				try
				{
					int num = stoi(std::string(edit_text), &pos, 2); //std::cout << "num = " << num << std::endl;
					setEditTextProgramatically = true;
					_itoa_s(num, (char*)tmp, 40, 8); SetWindowTextA(hEditOct, tmp);
					_itoa_s(num, (char*)tmp, 40, 10); SetWindowTextA(hEditDec, tmp);
					_itoa_s(num, (char*)tmp, 40, 16); SetWindowTextA(hEditHex, tmp);
					setEditTextProgramatically = false;
				}
				catch (...) {}
			} else if (lParam == (LPARAM)hEditHex)
			{
				GetWindowTextA(hEditHex, edit_text, EDIT_TEXT_MAXLEN);
				try
				{
					int num = stoi(std::string(edit_text), &pos, 16);
					setEditTextProgramatically = true;
					_itoa_s(num, (char*)tmp, 40, 2); SetWindowTextA(hEditBin, tmp);
					_itoa_s(num, (char*)tmp, 40, 8); SetWindowTextA(hEditOct, tmp);
					_itoa_s(num, (char*)tmp, 40, 10); SetWindowTextA(hEditDec, tmp);
					setEditTextProgramatically = false;
				}
				catch (...) {}
			} else if (lParam == (LPARAM)hEditOct)
			{
				GetWindowTextA(hEditOct, edit_text, EDIT_TEXT_MAXLEN);
				try
				{
					int num = stoi(std::string(edit_text), &pos, 8);
					setEditTextProgramatically = true;
					_itoa_s(num, (char*)tmp, 40, 2); SetWindowTextA(hEditBin, tmp);
					_itoa_s(num, (char*)tmp, 40, 10); SetWindowTextA(hEditDec, tmp);
					_itoa_s(num, (char*)tmp, 40, 16); SetWindowTextA(hEditHex, tmp);
					setEditTextProgramatically = false;
				}
				catch (...) {}
			}
		}
	}
	break;

	//case WM_PAINT: // on window repaint
	//break;
	case WM_DESTROY: // close window
		PostQuitMessage(0);
		break;

	default: // processing default message
		return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}

