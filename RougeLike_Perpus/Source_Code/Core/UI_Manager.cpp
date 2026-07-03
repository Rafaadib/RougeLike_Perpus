
#include "UI_Manager.h"
#include "Asset_Manager.h"
#include "Text_Box.h"
#include "Button.h"


UI_Manager::UI_Manager(Asset_Manager* asset, int32_t lebar_layar, int32_t tinggi_layar)
	: asset_manager(asset), lebar_layar(lebar_layar), tinggi_layar(tinggi_layar) {

	mtfont = LoadFontEx("Font/mania.ttf", 32, nullptr, 0);
	SetTextureFilter(mtfont.texture, TEXTURE_FILTER_POINT);

	//button
	asset_manager->Add_Texture("btn1", "Asset/tb1.png");
	asset_manager->Add_Texture("btn2", "Asset/tb2.png");
	asset_manager->Add_Texture("btn3", "Asset/tb3.png");
	asset_manager->Add_Texture("btn4", "Asset/tb4l.png");

	asset_manager->Add_Texture("back_b", "Asset/back_b.png");
	asset_manager->Add_Texture("input", "Asset/input.png");

	//implementasi
	Texture2D red_b = asset_manager->Get_Texture("btn1");
	Texture2D mulai_b = asset_manager->Get_Texture("btn2");
	Texture2D yellow_b = asset_manager->Get_Texture("btn3");

	Texture2D gray_b = asset_manager->Get_Texture("btn4");

	//menu utama
	btn = new Button({ 650, 600 }, mulai_b, 1.5f);

	//tombol crud
	btn_tambah = new Button({ 90, 620 }, mulai_b, 1.1f);
	btn_hapus = new Button({ 220, 620 }, red_b, 1.1f);
	btn_show = new Button({ 90, 700 }, mulai_b, 1.1f);
	btn_update = new Button({ 220, 700 }, yellow_b, 1.1f);

	//tombol sort,search,dan undo
	btn_sort = new Button({ 600, 720 }, mulai_b, 1.1f);
	btn_undo = new Button({ 720, 720 }, yellow_b, 0.8f);
	btn_search = new Button({ 840, 720 }, red_b, 1.1f);

	//tombol pinjam dan kembalikan
	btn_pinjam = new Button({ 150, 420 }, gray_b, 1.1f);
	btn_kembalikan = new Button({ 150, 500 }, gray_b, 1.1f);

	//input lokasi
	i_judul = new Text_Box({ 25, 60, 240, 40 }, 15, false);
	i_id = new Text_Box({ 25, 150, 240, 40 }, 4, true);
	i_pengarang = new Text_Box({ 25, 240, 240, 40 }, 15, false);
	
}

UI_Manager::~UI_Manager() {
	UnloadFont(mtfont);
	delete btn;

	//hapus crud button dari memori
	delete btn_tambah;
	delete btn_hapus;
	delete btn_show;
	delete btn_update;

	//hapus sort, search, dan undo
	delete btn_sort;
	delete btn_undo;
	delete btn_search;

	//hapus pinjma dan kembalikan
	delete btn_pinjam;
	delete btn_kembalikan;

	//delete input
	delete i_judul;
	delete i_id;
	delete i_pengarang;

}

bool UI_Manager::UpdateMainMenu() {
	return btn->Update_Button();
}

//tombol crud
AKSI_UI UI_Manager::Update_UI_CRUD() {
	//crud 
	if (btn_tambah->Update_Button()) {
		return AKSI_UI::TAMBAH_DATA;
	}
	if (btn_hapus->Update_Button()) {
		return AKSI_UI::HAPUS_DATA;
	}
	if (btn_show->Update_Button()) {
		return AKSI_UI::SHOW_DATA;
	}
	if (btn_update->Update_Button()) {
		return AKSI_UI::UPDATE_DATA;
	}

	return AKSI_UI::NONE;
}

AKSI_UI UI_Manager::Update_UI_SUS() {
	//sort, search, dan undo
	if (btn_sort->Update_Button()) {
		return AKSI_UI::SORT;
	}
	if (btn_undo->Update_Button()) {
		return AKSI_UI::UNDO;
	}
	if (btn_search->Update_Button()) {
		return AKSI_UI::SEARCH;
	}

	return AKSI_UI::NONE;
}

AKSI_UI UI_Manager::Update_UI_PK() {
	//pinjam kembalikan
	if (btn_pinjam->Update_Button()) {
		return AKSI_UI::PINJAM;
	}
	if (btn_kembalikan->Update_Button()) {
		return AKSI_UI::KEMBALIKAN;
	}

	return AKSI_UI::NONE;
}

void UI_Manager::Update_ITextBox() {
	if (i_judul != nullptr) {
		i_judul->Update_TextBox();
	}
	if (i_id != nullptr) {
		i_id->Update_TextBox();
	}
	if (i_pengarang != nullptr) {
		i_pengarang->Update_TextBox();
	}
}

void UI_Manager::DrawMenu() {
	Texture2D back_button = asset_manager->Get_Texture("back_b");
	Rectangle source = {
		0, 0,
		(float)back_button.width,
		(float)back_button.height,
	};

	Rectangle dest = {
		650,
		600,
		back_button.width * 2,
		back_button.height * 2 
	};

	Vector2 origin = { dest.width / 2.0f, dest.height / 2.0f };
	DrawTexturePro(back_button, source, dest, origin, 0.0f, WHITE);

	btn->Draw_Button();

	Vector2 origin_play = {
		600,
		565
	};
	DrawTextEx(mtfont, "Mulai", origin_play, 60, 2, WHITE);

	Vector2 origin_judul = {
		40,
		100
	};
	DrawTextEx(mtfont, "Random", origin_judul, 240, 2, WHITE);

	Vector2 origin_judul2 = {
		800,
		300
	};
	DrawTextEx(mtfont, "Play", origin_judul2, 240, 2, WHITE);

	/*const char* judul = "Perpus Game";
	const char* intruksi = "Mulai";

	int32_t lebar_judul = MeasureText(judul, 40);
	int32_t lebar_intruksi = MeasureText(intruksi, 20);

	DrawText(judul, (lebar_layar / 2) - (lebar_judul / 2), tinggi_layar / 2 - 50, 40, GOLD);
	DrawText(intruksi, (lebar_layar / 2) - (lebar_intruksi / 2), tinggi_layar / 2 + 20, 20, LIGHTGRAY);*/
}


void UI_Manager::DrawGameplay() {
	//shadow stack_caerd
	Rectangle shadow_stack_card = {
		1080.0f,
		580.0f,
		120.0f,
		153.0f
	};
	DrawRectangleRounded(shadow_stack_card, 0.2f, 8, Fade(BLACK, 0.1f));
	Vector2 origin_stack_card = {
		1120,
		740
	};
	DrawTextEx(mtfont, "24/24", origin_stack_card, 20, 2, WHITE);

	//shadow
	Rectangle shadow_card = {
		360.0f,
		520.0f,
		680.0f,
		140.0f
	};
	DrawRectangleRounded(shadow_card, 0.2f, 8, Fade(BLACK, 0.1f));
	Vector2 origin_shadow = {
		710,
		660
	};
	DrawTextEx(mtfont, "8/8", origin_shadow, 20, 2, WHITE);

	Rectangle board_up = {
		350.0f,
		20.0f,
		480.0f,
		140.0f
	};
	DrawRectangleRounded(board_up, 0.2f, 8, Fade(BLACK, 0.1f));
	Vector2 origin_board_up = {
		590,
		160
	};
	DrawTextEx(mtfont, "4/4", origin_board_up, 20, 2, WHITE);


	Rectangle board_up_right = {
		960.0f,
		20.0f,
		280.0f,
		140.0f
	};
	DrawRectangleRounded(board_up_right, 0.2f, 8, Fade(BLACK, 0.1f));

	//mleft menu
	Rectangle left_menu = {
		15.0f,
		-25.0f,
		280.0f,
		850.0f
	};
	DrawRectangleRounded(left_menu, 0.2f, 8, Fade(DARKGRAY, 0.5f));
	DrawRectangleRoundedLinesEx(left_menu, 0.2f, 8, 3.5f,Fade(GOLD, 0.6f));

	//input judul
	Texture2D input = asset_manager->Get_Texture("input");
	DrawTextureEx(input, { 25, 50 }, 0.0f, 1.3f, WHITE);
	DrawTextEx(mtfont, "Judul", { 30, 25 }, 30, 2, WHITE);

	//input id
	DrawTextureEx(input, { 25, 140 }, 0.0f, 1.3f, WHITE);
	DrawTextEx(mtfont, "ID", { 30, 115 }, 30, 2, WHITE);

	//input pengarang
	DrawTextureEx(input, { 25, 230 }, 0.0f, 1.3f, WHITE);
	DrawTextEx(mtfont, "Pengarang", { 30, 205 }, 30, 2, WHITE);



	//crud
	if (btn_tambah != nullptr) {
		btn_tambah->Draw_Button();
		DrawTextEx(mtfont, "Buat", { 45, 600 }, 30, 2, WHITE);
	}
	if (btn_hapus != nullptr) {
		btn_hapus->Draw_Button();
		DrawTextEx(mtfont, "Hapus", { 190, 600 }, 30, 2, WHITE);
	}
	if (btn_show != nullptr) {
		btn_show->Draw_Button();
		DrawTextEx(mtfont, "Show", { 45, 680 }, 30, 2, WHITE);
	}
	if (btn_update != nullptr) {
		btn_update->Draw_Button();
		DrawTextEx(mtfont, "Update", { 190, 680 }, 30, 2, WHITE);
	}

	//sort, search, dan undo
	if (btn_sort != nullptr) {
		btn_sort->Draw_Button();
		DrawTextEx(mtfont, "Sort", {580, 700 }, 30, 2, WHITE);
	}
	if (btn_undo != nullptr) {
		btn_undo->Draw_Button();
		DrawTextEx(mtfont, "Undo", { 695, 705 }, 24, 2, WHITE);
	}
	if (btn_search != nullptr) {
		btn_search->Draw_Button();
		DrawTextEx(mtfont, "Search", { 800, 700 }, 30, 2, WHITE);
	}

	//pinjam dan kembalikan
	if (btn_pinjam != nullptr) {
		btn_pinjam->Draw_Button();
		DrawTextEx(mtfont, "Pinjam", { 120, 410 }, 24, 2, WHITE);
	}
	if (btn_kembalikan != nullptr) {
		btn_kembalikan->Draw_Button();
		DrawTextEx(mtfont, "Kembali", { 120, 490 }, 24, 2, WHITE);
	}

	//input
	if (i_judul != nullptr) {
		i_judul->Draw_TextBox(mtfont);
	}
	if (i_id != nullptr) {
		i_id->Draw_TextBox(mtfont);
	}
	if (i_pengarang != nullptr) {
		i_pengarang->Draw_TextBox(mtfont);
	}
	
}

void UI_Manager::DrawLibrary() {

	//libabry concept
	Rectangle shadow_stack_card = {
		20.0f,
		20.0f,
		1225.0f,
		740.0f
	};
	DrawRectangleRounded(shadow_stack_card, 0.2f, 8, Fade(BLACK, 0.1f));
	Vector2 origin_library = {
		200,
		400
	};
	DrawTextEx(mtfont, "yang nantinya memnuclkan semua kartu", origin_library, 60, 2, WHITE);
}