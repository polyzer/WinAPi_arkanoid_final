#pragma once
#include "arkanoid_includes.h"

extern WNDCLASS wc;
extern HWND hWnd;
extern MSG msg;
extern Platform CurrentPlatform;
extern HDC hdc;

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;
TCHAR WinName[] = L"MainFrame";
int APIENTRY wWinMain(HINSTANCE This, // Дескриптор текущего приложения
	HINSTANCE Prev,                   // В современных системах всегда 0
	LPWSTR cmd,                       // Командная строка
	int mode)                         // Режим отображения окна
{

	//initing CurrentLevel!!!!!!!!
	CurrentGame.loadCurrentLevel();
	readConfig();

	// Определение класса окна
	wc.hInstance = This;
	wc.lpszClassName = WinName;                // Имя класса окна
	wc.lpfnWndProc = WndProc;                  // Функция окна
	wc.style = CS_HREDRAW | CS_VREDRAW;       // Стиль окна
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);    // Стандартная иконка
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); // Стандартный курсор
	wc.lpszMenuName = NULL;       // Нет меню
	wc.cbClsExtra = 0;            // Нет дополнительных данных класса
	wc.cbWndExtra = 0;            // Нет дополнительных данных окна
	// Заполнение окна белым цветом
	wc.hbrBackground = (HBRUSH) (COLOR_WINDOW+1);
	if (!RegisterClass(&wc)) return 0;   // Регистрация класса окна
// Создание окна
	hWnd = CreateWindow(WinName, // Имя класса окна
    L"Каркас Windows-приложения",  // Заголовок окна
	WS_OVERLAPPEDWINDOW,         // Стиль окна
	CW_USEDEFAULT, // x
	CW_USEDEFAULT, // y   Размеры окна
	CW_USEDEFAULT, // width
	CW_USEDEFAULT, // Height
	HWND_DESKTOP, // Дескриптор родительского окна
	NULL,         // Нет меню
	This,         // Дескриптор приложения
	NULL);        // Дополнительной информации нет
	ShowWindow(hWnd, mode); // Показать окно
// Цикл обработки сообщений
	while(GetMessage(&msg, NULL, 0, 0))
	{
	TranslateMessage(&msg); // Функция трансляции кодов нажатой клавиши
	DispatchMessage(&msg);  // Посылает сообщение функции WndProc()
	}
	return 0;
}
// Оконная функция вызывается операционной системой
// и получает сообщения из очереди для данного приложения
LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
	          WPARAM wParam, LPARAM lParam)
{       // Обработчик сообщений
	PAINTSTRUCT ps;
	static int sx, sy;
	switch(message)
	{
		case WM_CREATE:
			SetTimer(hWnd, 1, CurrentGame.FPS, NULL);
		break;
		case WM_SIZE:
			sx = LOWORD(lParam);
			sy = HIWORD(lParam);
		break;
		case WM_TIMER:
			CurrentGame.Play();
		break;
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			CurrentGame.render(sx, sy);
			EndPaint(hWnd, &ps);
		break;
		case WM_KEYDOWN:
			switch(wParam){
			case VK_UP:
				if(CurrentPlatform.moveControl(1))
				{
					CurrentPlatform.step(1);
				}			
			break;
			case VK_RIGHT:
				if(CurrentPlatform.moveControl(2))
				{
					CurrentPlatform.step(2);
				}
			
			break;
			case VK_DOWN:
				if(CurrentPlatform.moveControl(3))
				{
					CurrentPlatform.step(3);
				}
			
			break;
			case VK_LEFT:
				if(CurrentPlatform.moveControl(4))
				{
					CurrentPlatform.step(4);
				}			
			break;
			case VK_SPACE:
				CurrentGame.End();
			break;
			}
		break;
		case WM_DESTROY : PostQuitMessage(0);
			          break; // Завершение программы
		// Обработка сообщения по умолчанию
		default : return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
