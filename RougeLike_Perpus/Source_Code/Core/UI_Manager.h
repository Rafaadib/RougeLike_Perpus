
#pragma once

#include "raylib.h"
#include <cstdint>

class Asset_Manager;
class Button;

enum class AKSI_UI {
	NONE = 0,
	TAMBAH_DATA,
	HAPUS_DATA,
	URUTKAN_ID,
	UNDO_AKSI
};


class UI_Manager {
public:
	UI_Manager(Asset_Manager* asset, int32_t lebar_layar, int32_t tinggi_layar);
	~UI_Manager();

	void DrawMenu();
	void DrawGameplay();
	void DrawLibrary(); 

	//buton untuk CRUD
	Button* btn_tambah;
	Button* btn_hapus;
	Button* btn_sort;
	Button* btn_undo;

	AKSI_UI Update_UI_Gameplay();

	Font mtfont;

	//buton
	Button* btn;
	bool UpdateMainMenu();

private:
	Asset_Manager* asset_manager;
	int32_t lebar_layar;
	int32_t tinggi_layar;
};