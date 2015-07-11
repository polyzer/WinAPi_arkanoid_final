#pragma once
#include "arkanoid_includes.h"

extern WNDCLASS wc;
extern HWND hWnd;
extern MSG msg;
extern Platform CurrentPlatform;
extern HDC hdc;
extern int GamePlayTimer;
//extern int ShowMenuTimer; не используется
extern bool showMode;
extern bool Pause;

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;
TCHAR WinName[] = L"MainFrame";
int APIENTRY wWinMain(HINSTANCE This, // Дескриптор текущего приложения
	HINSTANCE Prev,                   // В современных системах всегда 0
	LPWSTR cmd,                       // Командная строка
	int mode)                         // Режим отображения окна
{

	//initing CurrentLevel!!!!!!!!
	CurrentGame.loadCurrentLevelByNumber(); // Загружаем стандартный уровень в текущий
	CurrentGame.loadLevelsFromFile(); // Грузим другие уровни из файлов
	readConfig(); // читаем конфиг и узнаем последние события.
	CurrentGame.CurrentLevelNumber = CurrentLevel.number; //устанавливаем последний номер из конфига!

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
	static int sx = 0, //размер горизонтали
			   sy = 0; //размер окна по вертикали
	switch(message)
	{
		case WM_CREATE:
			SetTimer(hWnd, GamePlayTimer, CurrentGame.FPS, NULL);
		break;
		case WM_SIZE:
			sx = LOWORD(lParam);
			sy = HIWORD(lParam);
		break;
		case WM_TIMER:
			if(showMode)
				CurrentGame.Play();
			else
				CurrentGame.Menu();
		break;
		case WM_PAINT: 
			//костылек... не ясно почему затирается значение размера карты по строкам
//			CurrentLevel.Size_Strings = CurrentGame.Levels[CurrentGame.CurrentLevelNumber]->Size_Strings;
			hdc = BeginPaint(hWnd, &ps);
			if (showMode)
				CurrentGame.render(sx, sy);
			else
				CurrentGame.printMenu(sx, sy);
			EndPaint(hWnd, &ps);
		break;
		case WM_KEYDOWN:
			if (showMode) {
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
					case VK_RETURN:
						CurrentGame.End();
					break;
					case VK_SPACE:
						Pause = !Pause;
						if (Pause == 1){
							KillTimer(hWnd, GamePlayTimer);
						} else {
							SetTimer(hWnd, GamePlayTimer, CurrentGame.FPS, NULL);
						}
					break;
					}
			} else {
				switch(wParam) {
				case VK_UP:
					if (CurrentGame.CurrentLeveNumberControl(CurrentGame.CurrentLevelNumber - 1)) {
						CurrentGame.CurrentLevelNumber--;
					}
				break;
				case VK_DOWN:
					if (CurrentGame.CurrentLeveNumberControl(CurrentGame.CurrentLevelNumber + 1)) {
						CurrentGame.CurrentLevelNumber++;
					}
				break;
				case VK_RETURN:
					CurrentGame.loadCurrentLevelByNumber();
					showMode = true;
				break;
				}
			}
		break;
		case WM_DESTROY : 
			PostQuitMessage(0);
		break; // Завершение программы
		// Обработка сообщения по умолчанию
		default : return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
