
#include "Button.h"
#include "raymath.h"

Button::Button(Vector2 posisi, Texture2D texture_button, float skala_awal) {
	pos_b = posisi;
	tex_b = texture_button;

	b_hover = false;
	ukuran_saat_ini = skala_awal;
	ukuran_target = skala_awal;
	ukuran_base = skala_awal;
}

Button::~Button() {

}

bool Button::Update_Button() {
	bool b_click = false;

	Rectangle hitbox = {
		pos_b.x - (tex_b.width * ukuran_saat_ini) / 2.0f,
		pos_b.y - (tex_b.height * ukuran_saat_ini) / 2.0f,
		(float)tex_b.width * ukuran_saat_ini,
		(float)tex_b.height * ukuran_saat_ini
	};

	b_hover = CheckCollisionPointRec(GetMousePosition(), hitbox);

	if (b_hover) {
		ukuran_target = ukuran_base + 0.2f;
		
		if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
			b_click = true;
		}
	}
	else {
		ukuran_target = ukuran_base;
	}

	ukuran_saat_ini = Lerp(ukuran_saat_ini, ukuran_target, 0.2f);
	return b_click;

}

void Button::Draw_Button() {
	Rectangle source = {
		0, 0,
		(float)tex_b.width,
		(float)tex_b.height,
	};

	Rectangle dest = {
		pos_b.x, 
		pos_b.y,
		tex_b.width *  ukuran_saat_ini,
		tex_b.height * ukuran_saat_ini
	};

	Vector2 origin = {dest.width / 2.0f, dest.height / 2.0f};
	
	Color warna_b = b_hover ? LIGHTGRAY : WHITE;

	DrawTexturePro(tex_b, source, dest, origin, 0.0f, warna_b);
}
