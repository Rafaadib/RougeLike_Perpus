

#pragma once

#include "raylib.h"
#include "raymath.h"
#include <cstdint>

#include "Struct_dan_Array/Buku.h"


enum class GameState {
	TITLE = 0,
	MAIN_MENU,
	GAMEPLAY,
	LIBRARY
};

class Asset_Manager;
class UI_Manager;
class Card;
class Perpustakaan;


class GameManager {
public:
	GameManager(const int32_t lebar_layar, const int32_t tinggi_layar, const char* judul);
	~GameManager(); //destrukutor

	bool b_berjalan = true;

	void Game_Run();

	int32_t lebar_layar;
	int32_t tinggi_layar;

	GameState current_state = GameState::MAIN_MENU;
	//tranasisi varibale
	bool b_transisi = false;
	bool b_transisi_keluar = true;
	float progress_transisi = 0.0f;
	GameState state_tujuan;

	//shader menu
	int32_t time_loc_menu;
	int32_t resol_loca_menu;

	//shader
	Shader shader;
	RenderTexture2D target_kanvas;
	int32_t timeloc;
	int32_t resloc;

	float total_time = 0.0f;


private:
	void Update();
	void Draw();

	void LoadAsset();
	void UnloadAsset();

	Asset_Manager* Asset;
	UI_Manager* UI;
	Perpustakaan* perpustakaan;

	//array dan structnya
	Buku data_buku[24];
	int32_t jumlah_buku_tersimpan = 0;

	Card* max_tambah_buku[8];
	int32_t jumlah_buku_ditambahkan = 0;


};