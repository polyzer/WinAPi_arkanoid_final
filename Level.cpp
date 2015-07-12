#pragma once
#include "arkanoid_includes.h"

extern Game CurrentGame;
extern HWND hWnd;
extern Ball CurrentBall;
extern Platform CurrentPlatform;
extern int GamePlayTimer;

Level::Level() 
{
	this->setStandard();
	this->reMap();
}

Level::~Level() {
    for (int i = 0; i < this->Size_Strings; i++) {
         delete [] Map[i];
    }
    delete [] Map;
}

void Level::End(bool status) {
	KillTimer(hWnd, GamePlayTimer);
	if (status == true) {
		if (CurrentGame.CurrentLevelNumber == (CurrentGame.Levels.size() - 1)){
			MessageBox(hWnd, L"Вы прошли все доступные уровни! Поехали сначала...", 
				L"Конец", MB_OK | MB_ICONQUESTION
			);
			CurrentGame.CurrentLevelNumber = 0;
		} else {
			CurrentGame.CurrentLevelNumber++;			
		}
		CurrentGame.setStandard();
		CurrentPlatform.setStandard();
		CurrentBall.setStandard();
		CurrentGame.loadCurrentLevelByNumber();
		CurrentGame.saveStatus = 1;
		saveConfig();
		SetTimer(hWnd, GamePlayTimer, CurrentGame.FPS, NULL);
	} else {
		int i = MessageBox(hWnd, L"Начать заново", 
		L"Проигрыш", MB_YESNO | MB_ICONQUESTION
		);
		if (i == IDYES) {
			CurrentGame.loadCurrentLevelByNumber();
			CurrentGame.setStandard();
			CurrentPlatform.setStandard();
			CurrentBall.setStandard();
			SetTimer(hWnd, GamePlayTimer, CurrentGame.FPS, NULL);
		} else {
			CurrentGame.End();
		}
	}
}


void Level::reMap()
{
	if (!this->init){ 
		for(int i = 0; i < this->Size_Strings; i++)
			delete [] Map[i];
		delete [] Map;
	} else 
		this->init = false;// убираем значение инициализациии, чтобы очищать память в след раз

	Map = new Block *[this->Size_Strings];
	for (int i = 0; i < this->Size_Strings; i++)
		Map[i] = new Block [this->Size_Columns];
}

void Level::setNullLevel()// устанавливает нулевой уровень
{
	for (int i = 0; i < this->Size_Strings; i++) {
		for (int j = 0; j < this->Size_Columns; j++) {
			if (i == 0) {
				this->Map[i][j].element = L'c';
			}else{
				this->Map[i][j].element = this->back;
			}
		}
	}

	CurrentBall.setStandard();
	CurrentGame.setStandard();
	CurrentPlatform.setStandard();
}
