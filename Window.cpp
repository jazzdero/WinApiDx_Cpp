#include "Window.h"
#include <sstream>
#include "resource.h"

Window::WindowClass Window::WindowClass::wndClass;

Window::WindowClass::WindowClass() noexcept :
	hInst(GetModuleHandle(nullptr)) {
		
	WNDCLASSEX wc = { 0 };

	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = HandleMsgSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetInstance();
	wc.hIcon = static_cast<HICON>(LoadImage(hInst,MAKEINTRESOURCE(IDIMAGE_ICON4),IMAGE_ICON,16,16,0));
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = (wchar_t*)GetName();
	wc.hIconSm = static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(IDIMAGE_ICON4), IMAGE_ICON, 16, 16, 0));;

	RegisterClassEx(&wc);
	}

Window::WindowClass::~WindowClass() {
	UnregisterClass((wchar_t*)wndClassName, GetInstance());
}

const char* Window::WindowClass::GetName() noexcept {
	return Window::WindowClass::wndClassName;
}

HINSTANCE Window::WindowClass::GetInstance()noexcept {
	return Window::WindowClass::wndClass.hInst;
}

Window::Window(int nWidth, int nHeight, const wchar_t* name):
width(nWidth),
height(nHeight)
{
	RECT wr;
	wr.left = 100;
	wr.right = nWidth + wr.left;
	wr.top = 100;
	wr.bottom = nHeight + wr.top;

	// calculate window size based on desired client region size

	if (AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE) == 0){
		throw CHWND_LAST_EXCEPT();
	}


	Window::hWnd = CreateWindowEx(
		0, (wchar_t*)(WindowClass::GetName()),name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, nWidth, nHeight,
		nullptr, nullptr, WindowClass::GetInstance(), this
	);
	if (hWnd == nullptr) {
		throw CHWND_LAST_EXCEPT();
	}

	ShowWindow(Window::hWnd, SW_SHOWDEFAULT);
}
Window::~Window() {
		DestroyWindow(Window::hWnd);
}


LRESULT CALLBACK WINAPI Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept{
	if (msg == WM_NCCREATE) {
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pWnd = static_cast <Window*>(pCreate->lpCreateParams);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgTunk));
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK WINAPI Window::HandleMsgTunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
//clear keystatne when window loses focus 
	case WM_KILLFOCUS:
		kbd.ClearState();
		break;
//keyboard messages
	case WM_KEYDOWN:// nonsystem keys (space, k.. etc)
	case WM_SYSKEYDOWN: //system keys (alt,ctrl..etc)
		// if not ( lParam masked with bit 30 ) or AotorepeatIsEnabled?
		if (!(lParam & 0x40000000) || kbd.AutorepeatIsEnabled()) {
			kbd.OnKeyPressed(static_cast<unsigned char>(wParam));
		}
		break;
	case WM_KEYUP: // nonsystemkeys
	case WM_SYSKEYUP: // system keys
		kbd.OnKeyReleased(static_cast<unsigned char>(wParam));
		break;
	case WM_CHAR:
		kbd.OnChar(static_cast<unsigned char>(wParam));
		break;
	}
// end keyboard messages 
	return DefWindowProc(hWnd, msg, wParam, lParam);
}


// Window Exception

Window::Exception::Exception( int line, const char* file,HRESULT hr) noexcept :
	Gexception(line,file),
	hr(hr)
{}

const char* Window::Exception::what() const noexcept {
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< " [Error Code] " << GetErrorCode() << std::endl
		<< " [Description] " << GetErrorString() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
 }

const char* Window::Exception::GetType() const noexcept {
	return "Window Exception";
}

std::string Window::Exception::TranslateErrorCode(HRESULT hr) noexcept {
	char* pMsgBuf = nullptr;
	DWORD nMsgLen = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPWSTR>(&pMsgBuf), 0, nullptr
	);
	if (nMsgLen == 0){
		return "UNDEFINED ERROR CODE";
	}
	std::string errorString = pMsgBuf;
	LocalFree(pMsgBuf);
	return errorString;

}

HRESULT Window::Exception::GetErrorCode() const noexcept {
	return hr;
}

std::string Window::Exception::GetErrorString() const noexcept {
	return TranslateErrorCode( hr );
}