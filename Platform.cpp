#pragma once
#include "arkanoid_includes.h"

extern Ball CurrentBall;

bool Platform::outOfSize(int course){
	// обработка выхода за экран
	if((this->position.Y == 0) && (course == 1)) {
		return false;
	}
	if(((this->position.X + this->length) >= CurrentLevel.Size_Columns) && (course == 2)) {
		return false;
	}
	if((this->position.Y >= (CurrentLevel.Size_Strings - 1)) && (course == 3)) {
		return false;
	}
	if((this->position.X == 0) && (course == 4)) {
		return false;
	}
	return true;

}

bool Platform::blockCollision(int course){
	//Столкновение с блоками
	if (course == 1) {//^
		for(int i = 0; i < this->length; i++) {
			if (CurrentLevel.Map[this->position.Y - 1][this->position.X + i] != CurrentLevel.back) {
				return false;
			}
		}
	}
	if (course == 2) {//->
		if (CurrentLevel.Map[this->position.Y][this->position.X + this->length] != CurrentLevel.back) {
			return false;
		}
	}
	if (course == 3) {//\/
		for(int i = 0; i < this->length; i++) {
			if (CurrentLevel.Map[this->position.Y + 1][this->position.X + i] != CurrentLevel.back) {
				return false;
			}
		}
	}
	if (course == 4) {//<-
		if (CurrentLevel.Map[this->position.Y][this->position.X  - 1] != CurrentLevel.back) {
			return false;
		}
	}

	return true; // если нет препятствий - то возвращаем тру
}


bool Platform::ballCollision(int course) {
		//Столкновение с мячом
	if (course == 3) {//\/
		for(int i = 0; i < this->length; i++) {
			if (((this->position.Y + 1) == CurrentBall.position.Y) && ((this->position.X + i) == CurrentBall.position.X)){
				return false;
			}
		}
	}
	if (course == 2) {//->
		if ((this->position.Y == CurrentBall.position.Y) && ((this->position.X + this->length) == CurrentBall.position.X)) {
			return false;
		}
	}
	if (course == 1) {//^
		for(int i = 0; i < this->length; i++) {
			if (((this->position.Y - 1) == CurrentBall.position.Y) && ((this->position.X + i) == CurrentBall.position.X)){
				return false;
			}
		}
	}
	if (course == 4) {//<-
		if ((this->position.Y == CurrentBall.position.Y) && ((this->position.X - 1) == CurrentBall.position.X)) {
			return false;
		}
	}

	return true; // если нет препятствий - то возвращаем тру
}

void Platform::step(int course){
	switch(course) {
	case 1:
		this->position.Y--;
		break;
	case 2:
		this->position.X++;
		break;
	case 3:
		this->position.Y++;
		break;
	case 4:
		this->position.X--;
		break;
	}
}


bool Platform::moveControl(int course){
	if ((this->outOfSize(course) && this->ballCollision(course)) && this->blockCollision(course)){
		return true;
	} else {
		return false;
	}

}

