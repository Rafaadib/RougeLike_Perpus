

#pragma once

#include "raylib.h"
#include "raymath.h"
#include <cstdint>

#include "Struct_dan_Array/Buku.h"
#include "Struct_dan_Array/Perpus.h"
#include "Linked_list/linked_list.h"
#include "Queue/Queue.h"
#include "Stack/Stack.h"
#include "Sorting_dan_Searching/Bubble_Sort.h"
#include "Sorting_dan_Searching/Linear_Search.h"


enum class GameState {
	TITLE = 0,
	MAIN_MENU,
	GAMEPLAY,
	LIBRARY
};

class Asset_Manager;
class UI_Manager;
class Card;


class GameManager {
public:
	GameManager(const uint32_t lebar_layar, const uint32_t tinggi_layar, const char* judul);
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

	//font
	Font font;


private:
	void Update();
	void Draw();

	void LoadAsset();
	void UnloadAsset();

	Asset_Manager* Asset;
	UI_Manager* UI;

	//data
	Perpus perpus_data;
	Linked_List node_card;
	Stack riwayat_hapus;

	//fitur pinjam
	Queue queue_pinjam;
	Card* visual_pinjam[4];

	//array dan structnya
	Card* library_card[24];
	int32_t jumlah_buku_tersimpan = 0;

	Card* visual_card[8];
	int32_t jumlah_buku_visual = 0;

	Card* kartu_ditemukan = nullptr;
	bool b_pencarian = false;


};