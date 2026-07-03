
#pragma once
#include "raylib.h"
#include <string>

//class untuk input
class Text_Box {
public:
	Rectangle batas;
	std::string text;

	bool b_ketik;
	int32_t max_karakter;
	bool b_angka;

	Text_Box(Rectangle batas, int32_t max_karakter, bool b_angka = false);
	~Text_Box();

	void Update_TextBox();
	void Draw_TextBox(Font font);
};