
#include "Kartu.h"
#include "Struct_dan_Array/Buku.h"
#include <iostream>

Card::Card(Vector2 posisi, Buku* data_buku, float rotasi_awal) {

	data = data_buku;

	posisi_sekarang = posisi;
	posisi_target = posisi;
	base_posisi = posisi;

	rotasi = rotasi_awal;
	base_rotasi = rotasi_awal;

	ukuran_sekarang = 1.0f;
	ukuran_target = 1.0f;

	b_hover = false;
	b_tertarik = false;
	b_burning = false;
	b_toggle = false;
	progerss_hancur = 0.0f;
}

Card::~Card() {

}

void Card::Update_Card() {
	Rectangle hit_box = {
		posisi_sekarang.x - (ukuran_lebar * ukuran_sekarang) / 2.0f,
		posisi_sekarang.y - (ukuran_tinggi * ukuran_sekarang) / 2.0f,
		(ukuran_lebar  - 120.0f) * ukuran_sekarang,
		ukuran_tinggi * ukuran_sekarang
	};

	b_hover = CheckCollisionPointRec(GetMousePosition(), hit_box);

	if (getar > 0.0f) {
		getar -= GetFrameTime(); // waktu dikurangi
		//set getarnya
		posisi_sekarang.x = base_posisi.x + sin(GetTime() * 60.0f) * 5.0f;
	}
	else {
		posisi_sekarang.x = Lerp(posisi_sekarang.x, base_posisi.x, 0.1f);
	}

	//animasi
	float waktu = GetTime() + (base_posisi.x, 0.03f);
	float idle_rotasi = base_rotasi + (sin(waktu * 2.0f) * 3.0f);
	float idle_y_rotasi = base_posisi.y + (sin(waktu * 3.0f) * 4.0f);

	if (b_toggle && !b_burning) {
		ukuran_target = 1.1f;

		rotasi = base_rotasi + sin(GetTime() * 15.0f) * 5.0f;
		posisi_sekarang.y = Lerp(posisi_sekarang.y, base_posisi.y - 45.0f, 0.1f);
	}
	else if (b_hover && !b_burning) {
		ukuran_target = 1.05f;

		rotasi = base_rotasi + sin(GetTime() * 15.0f) * 5.0f;
		posisi_sekarang.y = Lerp(posisi_sekarang.y, base_posisi.y - 15.0f, 0.1f);

		//if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		//	Burn();
		//}
	}
	else {
		ukuran_target = 1.0f;

		rotasi = Lerp(rotasi, idle_rotasi, 0.15f);
		posisi_sekarang.y = Lerp(posisi_sekarang.y, idle_y_rotasi, 0.1f);
	}

	/*if (IsKeyPressed(KEY_SPACE)) {
				b_burning = false;
				progerss_hancur = 0.0f;
				std::cout << "KARTU DI-RESET!" << std::endl;
	}*/

	if (b_burning) {
		// Nilai 0.5f membuat proses kehancuran kartu memakan waktu 2 detik pas
		progerss_hancur += GetFrameTime() * 0.5f;

		if (progerss_hancur >= 1.0f) {
			progerss_hancur = 1.0f;
		}
	}

	posisi_sekarang.x = Lerp(posisi_sekarang.x, base_posisi.x, 0.1f);
	ukuran_sekarang = Lerp(ukuran_sekarang, ukuran_target, 0.1f);

}

void Card::Draw_Card(Shader shader, Texture2D kartu, Font font) {
	if (progerss_hancur >= 1.0f) {
		return;
	}

	BeginShaderMode(shader);
		int32_t dissolve_loc = GetShaderLocation(shader, "uDissolve");
		SetShaderValue(shader, dissolve_loc, &progerss_hancur, SHADER_UNIFORM_FLOAT);

		Rectangle dest_rect = {
			posisi_sekarang.x,
			posisi_sekarang.y,
			ukuran_lebar * ukuran_sekarang,
			ukuran_tinggi * ukuran_sekarang
		};

		Vector2 origin = {
			dest_rect.width / 2.0f, 
			dest_rect.height / 2.0f,
		};

		Rectangle card_src = {
			0,
			0,
			(float)kartu.width, 
			(float)kartu.height
		};

		DrawTexturePro(kartu, card_src, dest_rect, origin, rotasi, WHITE);


	EndShaderMode();

	//tulisan judul,id dan penagrang
	Vector2 pos_judul = { posisi_sekarang.x - 40.0f, posisi_sekarang.y - 60.0f };
	Vector2 pos_id = { posisi_sekarang.x - 40.0f, posisi_sekarang.y - 20.0f };
	Vector2 pos_pengarang = { posisi_sekarang.x - 40.0f, posisi_sekarang.y + 40.0f };

	if (data != nullptr) {
		float ukuran_font = 16.0f * ukuran_sekarang;

		//judul
		Vector2 ukuran_tjudul = MeasureTextEx(font, data->judul.c_str(), ukuran_font, 1);
		Vector2 origin_tjudul = { ukuran_tjudul.x / 2.0f, (ukuran_tjudul.y / 2.0f) + 50.0f };
		DrawTextPro(font, data->judul.c_str(), posisi_sekarang, origin_tjudul, rotasi, ukuran_font, 1, BLACK);

		//id
		const char* text_id = TextFormat("ID: %d", data->id);
		Vector2 ukuran_tid = MeasureTextEx(font, text_id, ukuran_font, 1);
		Vector2 origin_tid = { ukuran_tid.x / 2.0f, ukuran_tid.y / 2.0f };
		DrawTextPro(font, text_id, posisi_sekarang, origin_tid, rotasi, ukuran_font, 1, BLACK);

		//pengarang
		Vector2 ukuran_tpengarang = MeasureTextEx(font, data->pengarang.c_str(), ukuran_font, 1);
		Vector2 origin_tpengarang = { ukuran_tpengarang.x / 2.0f, (ukuran_tpengarang.y / 2.0f) - 50.0f };
		DrawTextPro(font, data->pengarang.c_str(), posisi_sekarang, origin_tpengarang, rotasi, ukuran_font, 1, BLACK);

	}
}

void Card::Burn() {
	b_burning = true;
	progerss_hancur = 0.0f;
	std::cout << "INFO [SHADER] Kartu Hancur!" << std::endl;
}
