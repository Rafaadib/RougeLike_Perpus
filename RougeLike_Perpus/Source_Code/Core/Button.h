
#pragma once

#include "raylib.h"

class Button {
public:
	Button(Vector2 posisi, Texture2D texture_button, float skala_awal = 1.0f);
	~Button();

	bool Update_Button();
	void Draw_Button();

private:
	Vector2 pos_b;
	Texture2D tex_b;

	bool b_hover;
	float ukuran_saat_ini;
	float ukuran_target;
	float ukuran_base;

	/*float ukuran_tombol = 1.5f;*/
};