
#include "Text_Box.h"


Text_Box::Text_Box(Rectangle batas, int32_t max_karakter, bool b_angka)
	: batas(batas), max_karakter(max_karakter), b_angka(b_angka) {
	
	text = "";
	b_ketik = false;
}

Text_Box::~Text_Box() {

}

void Text_Box::Update_TextBox() {
	if (CheckCollisionPointRec(GetMousePosition(), batas)) {
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			b_ketik = true;
		}
	}
	else {
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			b_ketik = false;
		}
	}

	if (b_ketik) {
		int32_t key = GetCharPressed();
		while (key > 0) {
			bool b_valid = false;
			//48 dan 57 ini ascii nya
			if (b_angka && (key >= 48 && key <= 57)) {
				b_valid = true;
			}
			else if (!b_angka && (key >= 32 && key <= 125)) {
				b_valid = true;
			}

			if (b_valid && text.length() < max_karakter) {
				text += (char)key;
			}

			key = GetCharPressed();
		}

		//logik backspace
		if (IsKeyPressed(KEY_BACKSPACE) && text.length() > 0) {
			text.pop_back();
		}
	}
}

void Text_Box::Draw_TextBox(Font font) {
	Color warna_bg = b_ketik ? WHITE : LIGHTGRAY;
	DrawRectangleRounded(batas, 0.2f, 8, warna_bg);

	Color warna_garis = b_ketik ? GOLD : DARKGRAY;
	DrawRectangleRoundedLines(batas, 0.2f, 8, warna_bg);

	Vector2 posisi_text = { batas.x + 10, batas.y + 10 };
	DrawTextEx(font, text.c_str(), posisi_text, 24, 1, BLACK);

	if (b_ketik && ((int32_t)(GetTime() * 2) % 2 == 0)) {
		float lebar_text = MeasureTextEx(font, text.c_str(), 24, 1).x;
		DrawLineEx(
			{batas.x + 12 + lebar_text, batas.y + 5},
			{batas.x + 12 + lebar_text, batas.y + 35},
			2.0f, RED
		);
	}
}