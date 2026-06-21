
#include "UI_Manager.h"
#include "Asset_Manager.h"
#include "Button.h"


UI_Manager::UI_Manager(Asset_Manager* asset, int32_t lebar_layar, int32_t tinggi_layar)
	: asset_manager(asset), lebar_layar(lebar_layar), tinggi_layar(tinggi_layar) {

	mtfont = LoadFontEx("Font/mania.ttf", 32, nullptr, 0);
	SetTextureFilter(mtfont.texture, TEXTURE_FILTER_POINT);

	asset_manager->Add_Texture("btn1", "Asset/tb2.png");
	asset_manager->Add_Texture("back_b", "Asset/back_b.png");
	Texture2D mulai_b = asset_manager->Get_Texture("btn1");

	btn = new Button({ 650, 600 }, mulai_b, 1.5f);
	btn_tambah = new Button({ 80, 500 }, mulai_b, 1.0f);
	btn_hapus = new Button({ 220, 500 }, mulai_b, 1.0f);
	btn_sort = new Button({ 80, 600 }, mulai_b, 1.0f);
	btn_undo = new Button({ 220, 600 }, mulai_b, 1.0f);

}

UI_Manager::~UI_Manager() {
	UnloadFont(mtfont);
	delete btn;
	delete btn_tambah;
	delete btn_hapus;
	delete btn_sort;
	delete btn_undo;
}

bool UI_Manager::UpdateMainMenu() {
	return btn->Update_Button();
}

AKSI_UI UI_Manager::Update_UI_Gameplay() {
	if (btn_tambah->Update_Button()) {
		return AKSI_UI::TAMBAH_DATA;
	}
	if (btn_hapus->Update_Button()) {
		return AKSI_UI::HAPUS_DATA;
	}
	if (btn_sort->Update_Button()) {
		return AKSI_UI::URUTKAN_ID;
	}
	if (btn_undo->Update_Button()) {
		return AKSI_UI::UNDO_AKSI;
	}

	return AKSI_UI::NONE;
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
		20.0f,
		280.0f,
		740.0f
	};
	DrawRectangleRounded(left_menu, 0.2f, 8, Fade(BLACK, 0.1f));

	if (btn_tambah != nullptr) {
		btn_tambah->Draw_Button();
		DrawTextEx(mtfont, "tambah", { 100, 185 }, 30, 2, WHITE);
	}
	if (btn_hapus != nullptr) {
		btn_hapus->Draw_Button();
	}
	if (btn_sort != nullptr) {
		btn_sort->Draw_Button();
	}
	if (btn_undo != nullptr) {
		btn_undo->Draw_Button();
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