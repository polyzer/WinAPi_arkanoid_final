#pragma once

struct Ball {
	COORD position; // позици€ м€ча
	COORD course; // направление м€ча
	Block block; //блок
	int color; //цвет м€ча
	int speed; // текуща€ скорость
	int timer; //количество обновлений
	int stepNum; //счетчик шагов

	Ball() {
		this->setStandard();
	}

	void setStandard();

	void setPosition(COORD pos) {
		this->position.X = pos.X;
		this->position.Y = pos.Y;
	}
	void step(); // шаг м€ча
	void setCourse(int side); //1 - вправо, 0 - влево
	bool collision(); //столкновени€ и выход за границы окна
	bool screenOut();
	bool platformCollision();
	void speedControl(); //контроль скорости
	void speedUp(int spd);
};
