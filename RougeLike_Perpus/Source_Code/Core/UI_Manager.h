
#pragma once

#include "raylib.h"
#include <cstdint>

class Asset_Manager;
class Button;
class Text_Box;

enum class AKSI_UI {
	NONE = 0,
	TAMBAH_DATA,
	HAPUS_DATA,
	UPDATE_DATA,
	SHOW_DATA,
	SEARCH,
	PINJAM,
	KEMBALIKAN,
	SORT,
	UNDO
};


class UI_Manager {
public:
	UI_Manager(Asset_Manager* asset, int32_t lebar_layar, int32_t tinggi_layar);
	~UI_Manager();

	void DrawMenu();
	void DrawGameplay(int32_t jml_visualcard, int32_t jml_library, int32_t jml_pinjam);
	void DrawLibrary(); 

	//buton untuk CRUD
	//show nya klik kartu
	Button* btn_tambah;
	Button* btn_update;
	Button* btn_hapus;
	

	//aksi lain seperti urutkan, undo, searching
	Button* btn_sort;
	Button* btn_undo;
	Button* btn_search;

	//pinjam dan kembalikan
	Button* btn_pinjam;
	Button* btn_kembalikan;

	//text input
	Text_Box* i_judul;
	Text_Box* i_id;
	Text_Box* i_pengarang;
	void Update_ITextBox();

	AKSI_UI Update_UI_CRUD();
	AKSI_UI Update_UI_SUS();
	AKSI_UI Update_UI_PK();

	//untuk font 
	Font mtfont;

	//buton
	Button* btn;
	bool UpdateMainMenu();

private:
	Asset_Manager* asset_manager;
	int32_t lebar_layar;
	int32_t tinggi_layar;
};