#pragma once
#include "arkanoid_includes.h"

extern Ball CurrentBall;
extern HWND hWnd;
extern Platform CurrentPlatform;
extern HDC hdc;
extern bool showMode;
extern int GamePlayTimer;

Game::Game() {
	this->setStandard();
	this->CurrentLevelNumber = 0; //устанавливаем значение текущего номера на 0
	this->CurrentLevelName = L"Default"; // им€ стандартного левела
	Level *lnew = new Level();// —оздаем уровень
	lnew->setNullLevel();//«адаем ему значение нулевого уровн€
	CurrentGame.Levels.push_back(lnew);
	lnew = NULL; // стираем уровень переменную.
	RedBlackBrush = CreateSolidBrush(RGB(128, 0, 0));
	RedLightBrush = CreateSolidBrush(RGB(255, 0, 0));
	GreenBlackBrush = CreateSolidBrush(RGB(0, 128, 0));
	GreenLightBrush = CreateSolidBrush(RGB(0, 255, 0));
	BlueBlackBrush = CreateSolidBrush(RGB(0, 0, 128));
	BlueLightBrush = CreateSolidBrush(RGB(0, 0, 255));
	YellowBlackBrush = CreateSolidBrush(RGB(128, 128, 0));
	YellowLightBrush = CreateSolidBrush(RGB(255, 255, 0));
	GreyBlackBrush = CreateSolidBrush(RGB(128, 128, 128));
	GreyLightBrush = CreateSolidBrush(RGB(255, 255, 255));
	White = CreateSolidBrush(RGB(255, 255, 0));
	Black = CreateSolidBrush(RGB(255, 255, 0));

}

Game::~Game (){
	// ѕри уничтожении игры мы освобождаем пам€ть из под уровней
	for (int i = 0; i < this->Levels.size(); i++) {
		delete Levels[i];
		Levels[i] = NULL;
	}
	DeleteObject(RedBlackBrush);
	DeleteObject(RedLightBrush);
	DeleteObject(GreenBlackBrush);
	DeleteObject(GreenLightBrush);
	DeleteObject(BlueBlackBrush);
	DeleteObject(BlueLightBrush);
	DeleteObject(YellowBlackBrush);
	DeleteObject(YellowLightBrush);
	DeleteObject(GreyBlackBrush);
	DeleteObject(GreyLightBrush);
	DeleteObject(White);
	DeleteObject(Black);

}

void Game::Play() {
	CurrentBall.timer++; // счетчик проходов дл€ замедлени€ скорости шара
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
	if (CurrentBall.stepNum >= CurrentLevel.stepNorm) { //если прошли опред кол-во шагов, то ускор€емс€
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
	//ѕрибавление очков в зависимости от разрушенного блока!
}
void Game::destroyBlock(int y, int x) {
	CurrentLevel.Map[y][x].element = CurrentLevel.back;

}

void Game::render(static int sx, static int sy) { //рисователь
	COORD cube_size;
	cube_size.X = sx / CurrentLevel.Size_Columns;
	cube_size.Y = sy / CurrentLevel.Size_Strings;
	MoveToEx(hdc, 0, 0, NULL);
	for (int i=0; i<CurrentLevel.Size_Strings; i++)
	{
		for (int j=0; j<CurrentLevel.Size_Columns; j++)
		{
			if (CurrentPlatform.position.X == j && CurrentPlatform.position.Y == i)
			{
				for(int k = 0; k < CurrentPlatform.length; k++){
					SetRect(&CurrentPlatform.block.rect, (j + k) * cube_size.X, i * cube_size.Y, (j + k + 1) * cube_size.X, (i + 1) * cube_size.Y);
					setElementColor(CurrentPlatform.block);
				}
				j += (CurrentPlatform.length - 1); // j
				continue;
			}
			else if (CurrentBall.position.X == j && CurrentBall.position.Y == i) {
				SetRect(&CurrentBall.block.rect, j * cube_size.X, i * cube_size.Y, (j + 1) * cube_size.X, (i + 1) * cube_size.Y);				
				setElementColor(CurrentBall.block);
				continue;
			}else if (CurrentLevel.Map[i][j].element != CurrentLevel.back){
				SetRect(&CurrentLevel.Map[i][j].rect, j * cube_size.X, i * cube_size.Y, (j + 1) * cube_size.X, (i + 1) * cube_size.Y);								
				setElementColor(CurrentLevel.Map[i][j]);
			}
			else {
				SetRect(&CurrentLevel.Map[i][j].rect, j * cube_size.X, i * cube_size.Y, (j + 1) * cube_size.X, (i + 1) * cube_size.Y);	
				setElementColor(CurrentLevel.Map[i][j]);
			}
		}
	}
	//printInfo();
}

bool Game::createLevel(LPCWSTR LName) { // —оздание/загрузка уровней
		//—читывание!!!
		//—труктура файла
		//LevelName
		//size_X size_Y
		//********************** elements
		//**********************
	Level *newlevel = new Level(); //выдел€ем пам€ть под новый уровень
	newlevel->number = this->Levels.size();
	FILE *file_Fp;
	if ((file_Fp = _wfopen(LName, L"r")) != NULL)
	{
		wchar_t sym;
		newlevel->name.clear();
		fwscanf(file_Fp, L"%c", &sym);
		while (sym != L'\n') 
		{
			newlevel->name.push_back(sym);
			fwscanf(file_Fp, L"%c", &sym);
		}
		fwscanf(file_Fp, L"%i", &newlevel->Size_Columns);
		fwscanf(file_Fp, L"%i", &newlevel->Size_Strings);
		fseek(file_Fp, 3, SEEK_CUR);

		for (int i = 0; i < newlevel->Size_Strings; i++) 
		{
			for (int j = 0; j < newlevel->Size_Columns; j++) 
			{
				fwscanf(file_Fp, L"%c", &(newlevel->Map[i][j].element));
			}
			fseek(file_Fp, 2, SEEK_CUR);
		}
	} else {
		exit(0);
	}
	fclose(file_Fp);
	this->Levels.push_back(newlevel);
	newlevel = NULL; //ќбнул€ем, чтобы данные не потер€лись
	return true;
}

bool Game::loadCurrentLevelByNumber() { // —оздание/загрузка уровней
	int k = 0;
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
			CurrentLevel.Map[i][j].element = CurrentGame.Levels[CurrentGame.CurrentLevelNumber]->Map[i][j].element;
			if (CurrentGame.Levels[CurrentGame.CurrentLevelNumber]->Map[i][j].element == L'P')	//≈сли встречаетс€ символ P, то		
			{	
				CurrentPlatform.position.X = j; //устанавливаем положение платформы
				CurrentPlatform.position.Y = i;
				for (k = j; k < CurrentLevel.Size_Columns; k++) {
					if(CurrentGame.Levels[CurrentGame.CurrentLevelNumber]->Map[i][k].element == L'P') {
						CurrentPlatform.length = (k - j + 1); // и ”станавливаем длину платформы
						CurrentLevel.Map[i][k].element = L' ';
					} else 
						break;
				}
				j = k;
			}
		}
	}
	return true;
}

bool Game::loadLevelsFromFile()
{
	int nff;
	HANDLE hff;
	std::wstring str = L"LEVELS\\";
	WIN32_FIND_DATA datas;
	hff = FindFirstFile(L"LEVELS\\*.*", &datas);
	nff = FindNextFile(hff, &datas);
	if (hff != INVALID_HANDLE_VALUE) 
	{
		for (;;)
		{
			nff = FindNextFile(hff, &datas);
			if (!nff)
				break;
			this->createLevel((str.append(datas.cFileName)).c_str());
			str = L"LEVELS\\";
		}
	} else 
		return false;
	FindClose(hff);
	return true;
}


void Game::End() {
	int i = MessageBox(hWnd, L"—охранить игру", 
		L"—охранение", MB_YESNO | MB_ICONQUESTION
		);
	if (i == IDYES) {
		this->saveStatus = 1;
	} else {
		this->saveStatus = 0;
	}

	saveConfig();
	CurrentGame.setStandard();
	CurrentPlatform.setStandard();
	CurrentBall.setStandard();
	showMode = 0;
}

void Game::Menu() {
	InvalidateRect(hWnd, NULL, TRUE);
}
void Game::printMenu(int sx, int sy) {
	TEXTMETRICW tm;
	HFONT newfont, oldfont;
	tm.tmInternalLeading = 0;
	tm.tmDescent = 0;
	tm.tmExternalLeading = 0;
	int curTextPosX = (int)(sx / 5), curTextPosY = (int)(sy / 5);
	SetBkColor(hdc, RGB(200,200,200));
	SetTextColor(hdc, RGB(0,0,0));
	newfont = CreateFontW(30, 20, 0, 0, 500, 0, 0, 0, 
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial");	
	oldfont = (HFONT) SelectObject(hdc, newfont);
	GetTextMetricsW(hdc, &tm);	
	//сдвигаемс€ вниз дл€ отображени€ нужного нам номера;
	curTextPosY = curTextPosY - CurrentGame.CurrentLevelNumber * ( tm.tmHeight + tm.tmExternalLeading);
	for (int i = 0; i < CurrentGame.Levels.size(); i++) {
		if(this->CurrentLevelNumber == this->Levels[i]->number)
			SetTextColor(hdc, RGB(100, 220, 220));
		else
			SetTextColor(hdc, RGB(0, 0, 0));
		TextOutW(hdc, curTextPosX, curTextPosY, this->Levels[i]->name.c_str(), this->Levels[i]->name.length());
		curTextPosY += tm.tmExternalLeading + tm.tmHeight;
	}
	SelectObject(hdc, oldfont);
	DeleteObject(newfont);	
}
bool Game::CurrentLeveNumberControl(int num){
	if (num >= 0 && num < CurrentGame.Levels.size())		
		return true;
	else
		return false;
}


