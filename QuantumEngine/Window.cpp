#include "stdafx.h"
#include "Window.h"

#include "Graphics.h"
#include "InputSystem.h"


static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);



Window::Window()
	: m_wantsToClose(false)
{

}

void Window::initialize(int width, int height, const char* title, bool fullscreen)
{
	const bool DISABLE_RESIZE = true;


	WNDCLASSEX wcex; ZeroMemory(&wcex, sizeof(WNDCLASSEX));
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.hCursor = LoadCursor(0, IDC_ARROW);
	wcex.lpszClassName = "D3D12TestWindow";
	wcex.lpfnWndProc = WndProc;

	RegisterClassEx(&wcex);

	// Adjust window size so client rectangle matches desired size!
	RECT rect = {0, 0, width, height};

	// Create window with our parameters.
	DWORD style = fullscreen ? WS_POPUP : WS_OVERLAPPEDWINDOW;
	if (DISABLE_RESIZE) style = style & ~(WS_MAXIMIZEBOX | WS_THICKFRAME);

	AdjustWindowRect(&rect, style, false);

	m_hwnd = CreateWindowEx(0, "D3D12TestWindow", title, style, 0, 0, rect.right - rect.left, rect.bottom - rect.top, 0, 0, GetModuleHandle(0), 0);

	SetWindowLongPtr(m_hwnd, GWLP_USERDATA, (LONG_PTR) this);

	ShowWindow(m_hwnd, SW_SHOW);
}

void Window::pollEvents()
{
	MSG msg;
	while (PeekMessage(&msg, m_hwnd, 0, 0, true)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void Window::setTitle(const char* title)
{
	SetWindowText(m_hwnd, title);
}

void Window::getClientSize(int& outWidth, int& outHeight) const
{
	RECT r;
	GetClientRect(m_hwnd, &r);

	outWidth = r.right - r.left;
	outHeight = r.bottom - r.top;
}



static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	Window* window = (Window*) GetWindowLongPtr(hwnd, GWLP_USERDATA);

	switch (msg)
	{
	case WM_CLOSE:
		window->setWantsToClose(true);
		break;

	case WM_KEYDOWN: InputSystem::Get().FireKeyDown(wparam); break;
	case WM_KEYUP:   InputSystem::Get().FireKeyUp(wparam);   break;

	case WM_LBUTTONDOWN: InputSystem::Get().FireMouseDown(VK_LBUTTON); break;
	case WM_LBUTTONUP:   InputSystem::Get().FireMouseUp(VK_LBUTTON);   break;
	case WM_RBUTTONDOWN: InputSystem::Get().FireMouseDown(VK_RBUTTON); break;
	case WM_RBUTTONUP:   InputSystem::Get().FireMouseUp(VK_RBUTTON);   break;

	case WM_MOUSEMOVE:
		InputSystem::Get().FireMouseMove((lparam & 0xFFFF), (lparam >> 16 & 0xFFFF));
		break;

		// Prevent Alt key from triggering menu, which freezes the application.
	case WM_SYSCOMMAND:
		if (wparam == SC_KEYMENU) return 0;
		break;

	default:
		break;
	}

	return DefWindowProc(hwnd, msg, wparam, lparam);
}
