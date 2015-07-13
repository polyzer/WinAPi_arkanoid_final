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
	void setPosition(int x, int y) {
		this->position.X = x;
		this->position.Y = y;
	}
	void setStandardPosition();
	void step(); // шаг м€ча
	void setCourse(int hor_course, int ver_course); //1 - вправо, 0 - влево
	void genCourse();
	bool collision(); //столкновени€ и выход за границы окна
	bool screenOut();
	bool platformCollision();
	void speedControl(); //контроль скорости
	void speedUp(int spd);
};
