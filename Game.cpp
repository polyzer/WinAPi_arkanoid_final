#pragma once
#include "arkanoid_includes.h"

extern Ball CurrentBall;
extern HWND hWnd;
extern Platform CurrentPlatform;
extern HDC hdc;

Game::Game() {
	this->setStandard();
	Level *lnew = new Level();// Создаем уровень
	lnew->setNullLevel();//Задаем ему значение нулевого уровня
	CurrentGame.Levels.push_back(lnew);
	lnew = NULL; // стираем уровень переменную.
	this->CurrentLevelNumber = (this->Levels.size() - 1);
}

Game::~Game (){
	// При уничтожении игры мы освобождаем память из под уровней
	for (int i = 0; i < this->Levels.size(); i++) {
		delete Levels[i];
		Levels[i] = NULL;
	}
}

void Game::Play() {
	CurrentBall.timer++; // счетчик проходов для замедления скорости шара
	if (CurrentBall.timer >= CurrentBall.speed) {
		CurrentBall.collision();
		CurrentBall.collision();
		CurrentBall.collision();
		if(CurrentBall.collision()){
		CurrentBall.step(); // если достаточно раз прошел - шаг шара
		}
		CurrentBall.timer = 0;
		CurrentBall.stepNum++;
	}
	if (CurrentBall.stepNum >= CurrentLevel.stepNorm) { //если прошли опред кол-во шагов, то ускоряемся
		CurrentBall.speedUp(2);
		CurrentBall.stepNum = 0;
	}
	InvalidateRect(hWnd, NULL, TRUE);
	if (CurrentLevel.allBlocksDestroyed()){
		CurrentLevel.End(true);
	}
}



void Game::increasePoints(wchar_t c) {
	this->points += 100;
	//Прибавление очков в зависимости от разрушенного блока!
}
void Game::destroyBlock(int y, int x) {
	CurrentLevel.Map[y][x] = CurrentLevel.back;
}

void Game::render(static int sx, static int sy) { //рисователь
	COORD cube_size;
	cube_size.X = sx / CurrentLevel.Size_Columns;
	cube_size.Y = sy / CurrentLevel.Size_Strings;
	MoveToEx(hdc, 0, 0, NULL);
	setElementColor((wchar_t) 32);
	for (int i=0; i<CurrentLevel.Size_Strings; i++)
	{
		for (int j=0; j<CurrentLevel.Size_Columns; j++)
		{
			if (CurrentPlatform.position.X == j && CurrentPlatform.position.Y == i)
			{
				for(int k = 0; k < CurrentPlatform.length; k++)
					setElementColor(CurrentPlatform.symbol);
				Rectangle(hdc, j * cube_size.X, i * cube_size.Y, (j + 1) * cube_size.X, (i + 1) * cube_size.Y);
				j += (CurrentPlatform.length - 1); // j
				continue;
			}
			else if (CurrentBall.position.X == j && CurrentBall.position.Y == i) {
				setElementColor(CurrentBall.symbol);
				Rectangle(hdc, j * cube_size.X, i * cube_size.Y, (j + 1) * cube_size.X, (i + 1) * cube_size.Y);				
				continue;
			}else if (CurrentLevel.Map[i][j] != CurrentLevel.back){
				setElementColor(CurrentLevel.Map[i][j]);
				Rectangle(hdc, j * cube_size.X, i * cube_size.Y, (j + 1) * cube_size.X, (i + 1) * cube_size.Y);								
			}
			else {
				setElementColor(CurrentLevel.back);
				Rectangle(hdc, j * cube_size.X, i * cube_size.Y, (j + 1) * cube_size.X, (i + 1) * cube_size.Y);	
			}
		}
	}
	//printInfo();
}

bool Game::createLevel(LPCWSTR LName) { // Создание/загрузка уровней
	Level *newlevel = new Level(); //выделяем память под новый уровень
	newlevel->name = LName; //заполняем данные
	newlevel->number = CurrentGame.Levels.size();
	FILE *level_file;
	if ((level_file = _wfopen(newlevel->name, L"r")) == NULL) {
		MessageBox(hWnd, L"Файл не открывается", 
		L"Файл не открывается", MB_YESNO | MB_ICONQUESTION
		);
	}
	fwscanf(level_file, L"%i%i", &newlevel->Size_Columns, &newlevel->Size_Strings);
	fseek(level_file, 2, SEEK_CUR);
	fwscanf(level_file, L"%c", &newlevel->back);
	fseek(level_file, 2, SEEK_CUR);
	fwscanf(level_file, L"%i%i%i", 
		&newlevel->minSpeedTime, &newlevel->maxSpeedTime, &newlevel->stepNorm
	);
	for (int i = 0; i < newlevel->Size_Strings; i++) {
		for (int j = 0; j < newlevel->Size_Columns; j++) {
			fwscanf(level_file, L"%c", &newlevel->Map[i][j]);
		}
		fseek(level_file, 2, SEEK_CUR);
	}
	fclose(level_file);
	CurrentGame.Levels.push_back(newlevel);
	newlevel = NULL; //Обнуляем, чтобы данные не потерялись
	return true;
}

bool Game::loadCurrentLevel() { // Создание/загрузка уровней
	CurrentLevel.name = CurrentGame.Levels[CurrentGame.CurrentLevelNumber]->name;
	CurrentLevel.number = CurrentGame.CurrentLevelNumber;
	CurrentLevel.back = CurrentGame.Levels[CurrentGame.CurrentLevelNumber]->back;
	CurrentLevel.maxSpeedTime = CurrentGame.Levels[CurrentGame.CurrentLevelNumber]->maxSpeedTime;
	CurrentLevel.minSpeedTime = CurrentGame.Levels[CurrentGame.CurrentLevelNumber]->minSpeedTime;
	CurrentLevel.Size_Columns = CurrentGame.Levels[CurrentGame.CurrentLevelNumber]->Size_Columns;
	CurrentLevel.Size_Strings = CurrentGame.Levels[CurrentGame.CurrentLevelNumber]->Size_Strings;
	CurrentLevel.reMap();
	for (int i = 0; i < CurrentLevel.Size_Strings; i++) {
		for (int j = 0; j < CurrentLevel.Size_Columns; j++) {
			CurrentLevel.Map[i][j] = CurrentGame.Levels[CurrentGame.CurrentLevelNumber]->Map[i][j];
		}
	}
	return true;
}

bool Game::loadLevelsFromFile()
{
	int nff;
	HANDLE hff;
	WIN32_FIND_DATA datas;

	hff = FindFirstFile(L"LEVELS\\*.*", &datas);
	if (hff != INVALID_HANDLE_VALUE) 
	{
		for (;;)
		{
			nff = FindNextFile(hff, &datas);
			if (!nff)
				break;
			CurrentGame.createLevel(datas.cFileName);
		}
	} else 
		return false;
	FindClose(hff);
	return true;
}


void Game::End() { // перенести функцию в Level.End
	int i = MessageBox(hWnd, L"Сохранить игру", 
		L"Сохранение", MB_YESNO | MB_ICONQUESTION
		);
	i = (i == IDYES)? 1 : 0;
	if (i == 1) {
		CurrentGame.saveStatus = 1;
	} else {
		CurrentGame.saveStatus = 0;
	}
	saveConfig();
	exit(0);
}

void Game::printInfo() {
	
}
