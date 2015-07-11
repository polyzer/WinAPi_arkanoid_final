#pragma once
#include "Block.h"

class Level {
public:
	int Size_Strings;// размер по Y
	int Size_Columns;//размер по X
	int number; // номер уровня
    std::wstring name;
	bool init;
	wchar_t back; // background
	Block **Map; // карта уровня
	int minSpeedTime; //начальная скорость шара на уровне
	int maxSpeedTime; //максимальная скорость шара на уровне
	int stepNorm; // количество шагов шара для изменения скорости
	Level();
	~Level();
	void setStandard() {
		this->Size_Strings = 10;
		this->Size_Columns = 20;
		this->number = 0; // устанавливаем первый уровень
		//system("pause");
		this->back = L' ';
		this->minSpeedTime = 30;
		this->maxSpeedTime = 7;
		this->stepNorm = 5;
		this->name = L"Default";
		this->init = 1;
	}
	void setNullLevel();
	void reMap(); // задаем количество строк и количество столбцов.
	void End(bool status); // отслеживает окончание уровня
	bool allBlocksDestroyed(); // отслеживает уничтожение блоков
};
