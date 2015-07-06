#pragma once
#include "arkanoid_includes.h"

extern Platform CurrentPlatform;

bool Ball::screenOut(){
/*	if ((this->position.X <= 0) && (this->course.X < 0)) {
		return true;
	}*/
	if ((this->position.Y <= 0)) {
		return true;
	}
//	if (this->position.X >= (CurrentLevel.Size_Columns - 1)) {
//		return true;
//	}
	return false;
}

bool Ball::platformCollision() {
	//Столкновение с платформой!
	if ((this->position.X >= CurrentPlatform.position.X && //Если шарик находится над платформой и идет в низ
		 this->position.X < (CurrentPlatform.position.X + CurrentPlatform.length)) &&
		 (this->position.Y == (CurrentPlatform.position.Y - 1))
	 )
	{
		return true;
	}
	//столкновение с платформой по диагоналям
	if(
		(this->position.X == (CurrentPlatform.position.X - 1)) &&
		(this->position.Y == (CurrentPlatform.position.Y - 1))
	) {
		return true;
	}
	if(
		(((this->position.X) == (CurrentPlatform.position.X + CurrentPlatform.length))) &&
		((this->position.Y) == (CurrentPlatform.position.Y - 1))
	) {
		return true;
	}
	return false;
	// КОНЕЦ обработки столкновений с платформой

}

bool Ball::collision() {
	if (this->platformCollision() && this->screenOut()) {
		return false;
	}
	// обработка выхода за экран!
	if ((this->position.X <= 0) && (this->course.X < 0)) {
		this->course.X = -(this->course.X);
	}
	if ((this->position.Y <= 0) && (this->course.Y < 0)) {
		this->course.Y = -(this->course.Y);
	}
	if ((this->position.X >= (CurrentLevel.Size_Columns - 1)) && (this->course.X > 0)) {
		this->course.X = -(this->course.X);
	}
	if (this->position.Y == (CurrentLevel.Size_Strings - 1)) {
		// Проигрыш!!!!
		CurrentGame.lifes--;
		this->setStandard(); //установка начального положения шара
		CurrentPlatform.setStandard(); //установка начального положения платформы
		if (CurrentGame.lifes == 0) {
		//	 обработка конца игры!
			CurrentLevel.End(false);
		}
	}
	//Конец обработки выхода за экран
	//Столкновение с платформой!
	if ((this->position.X >= CurrentPlatform.position.X && //Если шарик находится над платформой и идет в низ
		 this->position.X < (CurrentPlatform.position.X + CurrentPlatform.length)) &&
		 (this->position.Y == (CurrentPlatform.position.Y - 1)) &&
		  (this->course.Y > 0)
		 )
	{
		this->course.Y = -(this->course.Y);
	}
	if ((this->position.X >= CurrentPlatform.position.X &&
		 this->position.X < (CurrentPlatform.position.X + CurrentPlatform.length)) &&
		 (this->position.Y == (CurrentPlatform.position.Y + 1)) &&
		  (this->course.Y < 0)
		 )
	{
		this->course.Y = -(this->course.Y);
	}
	if ((this->position.X == (CurrentPlatform.position.X - 1)
		) && (this->position.Y == CurrentPlatform.position.Y)
		&& (this->course.X > 0)
	) {
		this->course.X = -(this->course.X);
	}
	if ((this->position.X == (CurrentPlatform.position.X + CurrentPlatform.length)
		) && this->position.Y == CurrentPlatform.position.Y
		&& (this->course.X < 0)
	) {
		this->course.X = -(this->course.X);
	}
	//столкновение с платформой по диагоналям
	if(
		(this->position.X == (CurrentPlatform.position.X - 1)) &&
		(this->position.Y == (CurrentPlatform.position.Y - 1)) &&
		(this->course.X > 0 && this->course.Y > 0)
	) {
		this->course.X = -(this->course.X);
		this->course.Y = -(this->course.Y);
	}
	if(
		(((this->position.X) == (CurrentPlatform.position.X + CurrentPlatform.length))) &&
		((this->position.Y) == (CurrentPlatform.position.Y - 1)) &&
		(this->course.X < 0 && this->course.Y > 0)
	) {
		this->course.X = -(this->course.X);
		this->course.Y = -(this->course.Y);
	}
	if(
		(this->position.X == (CurrentPlatform.position.X + CurrentPlatform.length)) &&
		(this->position.Y == (CurrentPlatform.position.Y + 1)) &&
		(this->course.X < 0 && this->course.Y < 0)
	) {
		this->course.X = -(this->course.X);
		this->course.Y = -(this->course.Y);
	}
	if(
		(this->position.X == (CurrentPlatform.position.X - 1)) &&
		(this->position.Y == (CurrentPlatform.position.Y + 1)) &&
		(this->course.X > 0 && this->course.Y < 0)
	) {
		this->course.X = -(this->course.X);
		this->course.Y = -(this->course.Y);
	}
	// КОНЕЦ обработки столкновений с платформой
	//Столкновение с блоками
	int i = 0; //счетчик столкновений
	if ((CurrentLevel.Map[this->position.Y - 1][this->position.X] !=
         CurrentLevel.back) &&
         (this->course.Y < 0)) {
		CurrentGame.points += 100; // очки за столкновение
		CurrentLevel.Map[this->position.Y - 1][this->position.X] = CurrentLevel.back;
		this->course.Y = -(this->course.Y);
		i++;
	}
	if ((CurrentLevel.Map[this->position.Y][this->position.X + 1] !=
         CurrentLevel.back) &&
         (this->course.X > 0)) {
		CurrentGame.points += 100; // очки за столкновение
		CurrentLevel.Map[this->position.Y][this->position.X + 1] = CurrentLevel.back;
		this->course.X = -(this->course.X);
		i++;
	}
	if ((CurrentLevel.Map[this->position.Y + 1][this->position.X] !=
         CurrentLevel.back) &&
         (this->course.Y > 0)) {
		CurrentGame.points += 100; // очки за столкновение
		CurrentLevel.Map[this->position.Y + 1][this->position.X] = CurrentLevel.back;
		this->course.Y = -(this->course.Y);
		i++;
	}
	if ((CurrentLevel.Map[this->position.Y][this->position.X - 1]) !=
         CurrentLevel.back &&
         (this->course.X < 0)) {
		CurrentGame.points += 100; // очки за столкновение
		CurrentLevel.Map[this->position.Y][this->position.X - 1] = CurrentLevel.back;
		this->course.X = -(this->course.X);
		i++;
	}
	//Столкновения по диагонали
	if (i == 0) { //Если не случилось столкновений по горизонтали/вертикали, то обрабатываем диагональные
		if (CurrentLevel.Map[this->position.Y + 1][this->position.X + 1] != CurrentLevel.back &&
			(this->course.X > 0) && (this->course.Y > 0) // если блок правее по горизонтали и ниже вертикали и шар идет вправо + вниз
		) {
			CurrentGame.points += 100;// очки за столкновение
			CurrentLevel.Map[this->position.Y + 1][this->position.X + 1] = CurrentLevel.back;
			i++;
		}
		if (CurrentLevel.Map[this->position.Y - 1][this->position.X + 1] != CurrentLevel.back &&
			(this->course.X > 0) && (this->course.Y < 0) // если блок правее по горизонтали и выше вертикали и шар идет вправо + вверх
		) {
			CurrentGame.points += 100;// очки за столкновение
			CurrentLevel.Map[this->position.Y - 1][this->position.X + 1] = CurrentLevel.back;
			i++;
		}
		if (CurrentLevel.Map[this->position.Y - 1][this->position.X - 1] != CurrentLevel.back &&
			(this->course.X < 0) && (this->course.Y < 0) // если блок левее по горизонтали и ниже вертикали и шар идет влево + вниз
		) {
			CurrentGame.points += 100;// очки за столкновение
			CurrentLevel.Map[this->position.Y - 1][this->position.X - 1] = CurrentLevel.back;
			i++;
		}
		if (CurrentLevel.Map[this->position.Y + 1][this->position.X - 1] != CurrentLevel.back &&
			(this->course.X < 0) && (this->course.Y > 0) // если блок левее по горизонтали и выше вертикали и шар идет влево + вверх
		) {
			CurrentGame.points += 100; // очки за столкновение
			CurrentLevel.Map[this->position.Y + 1][this->position.X - 1] = CurrentLevel.back;
			i++;
		}
		if (i != 0) {
			this->course.Y = -(this->course.Y);
			this->course.X = -(this->course.X);
		}
	}
	return true;
	//Конец обработки столкновений с блоками

}

void Ball::step(){
	if (this->course.X > 0) {
		this->position.X++;
	}else {
		this->position.X--;
	}
	if (this->course.Y > 0) {
		this->position.Y++;
	}else {
		this->position.Y--;
	}

}

void Ball::setStandard() {//установка начального положения мяча
		this->course.X = 1; // 1 - вправо, -1 - влево
		this->course.Y = -1;// 1 - вниз, -1 - вверх
		this->position.X = (CurrentLevel.Size_Columns/2);
		this->position.Y = (CurrentLevel.Size_Strings/2);
		this->symbol = 4;
		this->speed = CurrentLevel.minSpeedTime;
		this->timer = 0;
		this->stepNum = 0;
	}
////////////

void Ball::speedUp(int spd) {
	if (this->speed >= CurrentLevel.maxSpeedTime)
		this->speed -= spd;
}
bool Level::allBlocksDestroyed() {
	for(int i = 0; i < CurrentLevel.Size_Strings; i++) {
		for(int j = 0; j < CurrentLevel.Size_Columns; j++){
			if (this->Map[i][j] != this->back) {
				return false;
			}
		}
	}
	return true;
}
