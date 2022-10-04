#include "Window.h"
#include <iostream>

int CALLBACK WinMain(
	HINSTANCE hInstnce,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow
) {
	try{
		Window ventana(800, 600, L"Motor Grafico");

		MSG msg;
		BOOL gResult;

		while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0) {

			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (ventana.kbd.KeyIsPressed(VK_SPACE)) {
				MessageBoxA(nullptr, "Funciona", "space", MB_OK | MB_ICONEXCLAMATION);
			}else if(ventana.kbd.KeyIsPressed(VK_MENU)){
				MessageBoxA(nullptr, "Funciona", "alt", MB_OK | MB_ICONEXCLAMATION);
			}
		}
		if (gResult == -1) {
			return -1;
		}
		else {
			return msg.wParam;
		}

	} catch (const Gexception err){
		MessageBoxA(nullptr, err.what(), err.GetType(), MB_OK | MB_ICONEXCLAMATION);
	} catch (const std::exception& err) {
		MessageBoxA(nullptr, err.what(),"STANDARD EXCEPTION", MB_OK | MB_ICONEXCLAMATION);
	} catch (...) {
		MessageBoxA(nullptr,"NO DETAILS AVILABLE", "UNKNOWN EXCEPTION", MB_OK | MB_ICONEXCLAMATION);
	}
	return -1;
}