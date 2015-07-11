#pragma once
#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#include <Windows.h>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <vector>
#include "Game.h"
#include "Level.h"
#include "Ball.h"
#include "Platform.h"
#include "Menu.h"
#include "Block.h"

bool createConfig();
bool saveConfig();
bool readConfig();
std::string ws2mb( LPCWSTR src);
void setElementColor(Block block);