#include "Window.h"
#include <iostream>

using namespace Cefied::Game;

TypeSetup(Window)

Window::Window(std::string windowTitle, int w, int h) :
	m_title(windowTitle)
{
	m_clientSize.cx = w;
	m_clientSize.cy = h;

	if (s_openedWindowsMap == nullptr) {
		s_openedWindowsMap = new std::unordered_map<HWND, Window&>;
	}
}

bool Window::initialize()
{
	//WNDCLASSEX
	ULONG WndStyle = WS_OVERLAPPEDWINDOW;
	WNDCLASSEX wcex;
	ZeroMemory(&wcex, sizeof(WNDCLASSEX));
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.hInstance = GetModuleHandle(0);
	wcex.lpfnWndProc = Window::wndProc;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = "DXAPPWNDCLASS";
	wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wcex))
	{
		std::cerr << "FAILED TO CREATE WINDOW CLASS" << std::endl;
		return false;
	}


	m_clientRect.left = 0;
	m_clientRect.top = 0;
	m_clientRect.right = m_clientSize.cx;
	m_clientRect.bottom = m_clientSize.cy;

	AdjustWindowRect(&m_clientRect, WndStyle, FALSE);
	UINT Width = m_clientRect.right - m_clientRect.left;
	UINT height = m_clientRect.bottom - m_clientRect.top;

	m_hAppWnd = CreateWindow("DXAPPWNDCLASS", m_title.c_str(), WndStyle,
		CW_USEDEFAULT, CW_USEDEFAULT, Width, height, NULL, NULL, wcex.hInstance, NULL);


	if (!m_hAppWnd)
	{
		std::cerr << "FAILED TO CREATE WINDOW CLASS" << std::endl;
		return false;
	}
	//ShowCursor(FALSE);

	if (ShowWindow(m_hAppWnd, SW_SHOW))
		return false;

	s_openedWindowsMap->insert(std::pair<HWND, Window&>(m_hAppWnd, *this));

	reCalculateRect();

	return true;
}

void Window::getPosition(int& left, int& top) const
{
	left = m_clientRect.left;
	top = m_clientRect.top;
}

void Window::getSize(int& width, int& height) const
{
	width = m_clientSize.cx;
	height = m_clientSize.cy;
}

void Window::reCalculateRect()
{
	RECT client, window;
	GetClientRect(m_hAppWnd, &client);
	m_clientSize.cx = client.right - client.left;
	m_clientSize.cy = client.bottom - client.top;
	
	GetWindowRect(m_hAppWnd, &window);
	AdjustWindowRect(&client, WS_OVERLAPPEDWINDOW, FALSE);
	window.top -= client.top;
	window.left -= client.left;
	window.right = window.left + m_clientSize.cx;
	window.bottom = window.top + m_clientSize.cy;

	m_clientRect = window;	
}

LRESULT CALLBACK Window::wndProc(HWND hwnd, UINT umsg, WPARAM wParam, LPARAM lParam)
{

	switch (umsg)
	{
	case WM_MOVE:
	{
		const auto it = s_openedWindowsMap->find(hwnd);
		if (it != s_openedWindowsMap->end()) {
			Window & thisWindow = (*it).second;
			thisWindow.reCalculateRect();
		}
		return DefWindowProc(hwnd, umsg, wParam, lParam);
	}
	case WM_SIZE:
	{
		const auto it = s_openedWindowsMap->find(hwnd);
		if (it != s_openedWindowsMap->end()) {
			Window & thisWindow = (*it).second;

			thisWindow.reCalculateRect();
			if (thisWindow.m_resizedNotify != nullptr)
			{
				thisWindow.m_resizedNotify->resize(thisWindow.m_clientSize.cx, thisWindow.m_clientSize.cy);
			}
		}

		return DefWindowProc(hwnd, umsg, wParam, lParam);
	}

	case WM_DESTROY:
	case WM_QUIT:
	{
		PostQuitMessage(0);
		return 0;
	}

	// Any other messages send to the default message handler as our application won't make use of them.
	default:
		return DefWindowProc(hwnd, umsg, wParam, lParam);
	}
}

Window::~Window()
{
	const auto it = s_openedWindowsMap->find(m_hAppWnd);
	if (it != s_openedWindowsMap->end()) {
		s_openedWindowsMap->erase(it);
	}
	if (s_openedWindowsMap->empty()) {
		delete s_openedWindowsMap;
	}

	DestroyWindow(m_hAppWnd);
}

std::unordered_map<HWND, Window&> *Window::s_openedWindowsMap = nullptr;