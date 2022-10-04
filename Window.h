#pragma once

#include "ChiliWin.h"
#include "Gexcpetions.h"
#include "Keyboard.h"

class Window {
//exceotions
public:
	class Exception : public Gexception {
	public:
		Exception(int line, const char* file, HRESULT hr) noexcept;
		const char* what() const noexcept override;
		virtual const char* GetType() const noexcept;
		static std::string TranslateErrorCode( HRESULT hr) noexcept;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
	private:
		HRESULT hr;
	};

//window
private:
	class WindowClass
	{
	public:
		static const char* GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	private:
		WindowClass() noexcept;
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator = (const WindowClass&) = delete;
		static constexpr const char*  wndClassName = "Ventana Motor grafico";
		static WindowClass wndClass;
		HINSTANCE hInst;
	};
public:
	Window(int width, int heigh, const wchar_t* name);
	~Window();
	Window(const Window&) = delete;
	Window& operator = (const Window&) = delete;
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMsgTunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
public:
	Keyboard kbd;
private:
	int width;
	int height;
	HWND hWnd;

};

// Error exception helper macro

#define CHWND_EXCEPT( hr ) Window::Exception(__LINE__,__FILE__,hr)
#define CHWND_LAST_EXCEPT()	Window::Exception(__LINE__,__FILE__,GetLastError())