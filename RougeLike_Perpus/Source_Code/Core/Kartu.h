
#pragma once

#include "raylib.h"
#include "raymath.h"

struct Buku;

class Card {
public:
	Card(Vector2 posisi,Buku* data_buku, float rotasi_awal = 0.0f);
	~Card();

	Buku* data;

	Vector2 posisi_sekarang;
	Vector2 posisi_target;
	Vector2 base_posisi;

	float ukuran_sekarang;
	float ukuran_target;
	float rotasi;
	float base_rotasi;

	bool b_hover;
	bool b_tertarik;
	bool b_burning;
	float progerss_hancur;

	void Update_Card();
	void Draw_Card(Shader shader, Texture2D kartu);
	void Burn();

private:
	float ukuran_lebar = 200.0f;
	float ukuran_tinggi = 200.0f;
};
