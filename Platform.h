#pragma once

extern Game CurrentGame;
extern Level CurrentLevel;

struct Platform {
	wchar_t symbol; // символы используемые в качестве платформы
	wchar_t color; //цвет платформы
	wchar_t length; //длина платформы
	COORD position; // координаты левого конца
	Platform() {
		this->setStandard(); //
	}

	void setStandard() { // установка начальных параметров платформы
		this->symbol = 110; //
		this->color = 10; //
		this->length = 3; //
		this->position.X = (CurrentLevel.Size_Columns/2 - 1); // позиция о X
		this->position.Y = (CurrentLevel.Size_Strings - 2); // позиция по Y
	}

	void setPosition(COORD pos) {
		this->position.X = pos.X;
		this->position.Y = pos.Y;
	}
	bool moveControl(int control);//Глобальная функция проверки перемещения
	bool outOfSize(int control);//Проверка выхода за границы экрана
	bool blockCollision(int control);//Проверка столкновения с блоком
	bool ballCollision(int control);//Проверка столкновения с мячом
	void step(int course); //перемещение
};

