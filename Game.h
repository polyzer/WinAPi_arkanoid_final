#pragma once
#include "Level.h"

class Game {
public:
	int lifes; //жизни
	long points; // очки игрока
	int speed; // текущая скорость игры
	int maxSpeed; // максимальная скорость игры
	int minSpeed; // начальная скорость игры
	wchar_t stopSymbol; // символ, по нажатию на который игра прерывается!
	wchar_t pauseSymbol; // символ паузы!
	int saveStatus; // статус сохранения: 0 - новая игра, 1 - загрузка
	int FPS; //количество кадров
	std::vector <Level*> Levels; // Указатели на уровни...
	int CurrentLevelNumber;
	LPCWSTR CurrentLevelName;
	Game();
	~Game();
	void setStandard() { // устанавливает начальные значения
		this->FPS = 30;
		minSpeed = 1000;
		maxSpeed = 50;
		stopSymbol = 113; //символ оканчивающий игру
		pauseSymbol = 32;
		lifes = 3;
		points = 0;
		speed = minSpeed; //стартовая скорость
		saveStatus = 0; //0 - new, 1 - load
	}
	
	bool loadLevelsFromFile();
	bool loadCurrentLevel();
	bool createLevel(LPCWSTR LName); //загружает уровень в зависимости от имени
	void increasePoints(wchar_t c); // увеличивает очки в зависимости от элемента
	void speedUp(int spd); // изменяет скорость
	void setLifes(); //изменяет количество попыток
	void render(int sx, int sy); //рисует все
	void destroyBlock(int y, int x); // обработка уничтожения блоков
	void printInfo();
	void Play(); // Начало игры
	void End(); //Конец уровня!
};
