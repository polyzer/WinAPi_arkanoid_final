#pragma once

extern Game CurrentGame;
extern Level CurrentLevel;

struct Platform {
	Block block; // символы используемые в качестве платформы
	wchar_t color; //цвет платформы
	wchar_t length; //длина платформы
	COORD position; // координаты левого конца
	Platform() {
		this->setStandard(); //
		this->length = 3; //
		this->position.X = (CurrentLevel.Size_Columns/2 - 1); // позиция о X
		this->position.Y = (CurrentLevel.Size_Strings - 2); // позиция по Y
	}
	void setStandard() { // установка начальных параметров платформы
		this->block.element = L'P'; //
		this->color = 10; //
	}

	void setStandardPosition() {
		this->position.X = 0; // позиция о X
		this->position.Y = (CurrentLevel.Size_Strings - 1); // позиция по Y
	}
	bool moveControl(int control);//Глобальная функция проверки перемещения
	bool outOfSize(int control);//Проверка выхода за границы экрана
	bool blockCollision(int control);//Проверка столкновения с блоком
	bool ballCollision(int control);//Проверка столкновения с мячом
	void step(int course); //перемещение
};

