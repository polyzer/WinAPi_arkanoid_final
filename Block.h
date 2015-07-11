#pragma once

class Block{

public:
	RECT rect;
	wchar_t element;
	Block(){
		this->element = L' ';
		this->rect.bottom = 0;
		this->rect.left = 0;
		this->rect.right = 0;
		this->rect.top = 0;

	};
	~Block(){};

};