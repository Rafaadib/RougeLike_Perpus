
#include "Game_Manager.h"
#include <fstream>

#include "Utililty/utilility.h"
#include "Asset_Manager.h"
#include "UI_Manager.h"
#include "Kartu.h"

#include "Struct_dan_Array/Perpus.h"

GameManager::GameManager(const int32_t lebar_layar, const int32_t tinggi_layar, const char* judul)
	: lebar_layar(lebar_layar), tinggi_layar(tinggi_layar) {

	/*SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);*/
	InitWindow(lebar_layar, tinggi_layar, judul);

	Asset = new Asset_Manager();
	UI = new UI_Manager(Asset, lebar_layar, tinggi_layar);

	data_buku[0] = Buku(101, "Bumi", "tere liye", true);
	data_buku[1] = Buku(101, "Bumi", "tere liye", true);
	data_buku[2] = Buku(101, "Bumi", "tere liye", true);
	jumlah_buku_tersimpan = 3;

	max_tambah_buku[0] = new Card({ 380, 555 }, &data_buku[0], -10.0f);
	max_tambah_buku[1] = new Card({ 470, 555 }, &data_buku[1], -8.0f);
	max_tambah_buku[2] = new Card({ 560, 555 }, &data_buku[2], -6.0f);
	jumlah_buku_ditambahkan = 3;

	LoadAsset();

}

GameManager::~GameManager() {
	//delete manual
	for (int32_t i = 0; i < jumlah_buku_ditambahkan; i++) {
		delete max_tambah_buku[i];
	}
	

	UnloadAsset();
	delete Asset;
	delete UI;
	UnloadRenderTexture(target_kanvas);
	CloseWindow();
}

void GameManager::LoadAsset() {

	/*Asset->Add_Texture("backgound", "Asset/table.png");*/
	
	Asset->Add_Texture("card", "Asset/card_book.png");
	Asset->Add_Texture("back", "Asset/card_b.png");

	Asset->Add_Shader("title", nullptr, "Shader/table.fs");
	Asset->Add_Shader("table", nullptr, "Shader/table1.fs");
	Asset->Add_Shader("burn", nullptr, "Shader/burn.fs");
	Asset->Add_Shader("crt", nullptr, "Shader/tv_tabung.fs");

	//shader mainmenu
	Shader menu = Asset->Get_Shader("title");
	time_loc_menu = GetShaderLocation(menu, "uTime");
	resol_loca_menu = GetShaderLocation(menu, "uResolution");
	Vector2 resolusi_menu = { (float)lebar_layar, (float)tinggi_layar };
	SetShaderValue(menu, resol_loca_menu, &resolusi_menu, SHADER_UNIFORM_VEC2);

	//shader table
	Shader shader = Asset->Get_Shader("table");
	timeloc = GetShaderLocation(shader, "uTime");
	resloc = GetShaderLocation(shader, "uResolution");
	Vector2 resolution = { (float)lebar_layar, (float)tinggi_layar };
	SetShaderValue(shader, resloc, &resolution, SHADER_UNIFORM_VEC2);

	target_kanvas = LoadRenderTexture(lebar_layar, tinggi_layar);
	SetTextureFilter(target_kanvas.texture, TEXTURE_FILTER_BILINEAR);
}

void GameManager::UnloadAsset() {
	Asset->Clear_All_Asset();
	//mengahspus aseet di memory
}

void GameManager::Update() {
	//strukture data logik

	Shader shader = Asset->Get_Shader("table");
	Shader shader_dissolve = Asset->Get_Shader("burn");
	Shader background = Asset->Get_Shader("title");

	//logika transisi
	if (b_transisi) {
		if (b_transisi_keluar) {
			progress_transisi += GetFrameTime() * 1.5f;
			if (progress_transisi >= 1.0f) {
				progress_transisi = 1.0f;
				current_state = state_tujuan;
				b_transisi_keluar = false;
			}
		}
		else {
			progress_transisi -= GetFrameTime() * 1.5f;
			if (progress_transisi <= 0.0f) {
				progress_transisi = 0.0f;
				b_transisi = false;
				b_transisi_keluar = true;
			}
		}
		/*return;*/
	}

	switch (current_state) {
		case GameState::MAIN_MENU: {
			float time = (float)GetTime();
			SetShaderValue(background, time_loc_menu, &time, SHADER_UNIFORM_FLOAT);
			if (UI->UpdateMainMenu()) {
				b_transisi = true;
				progress_transisi = 0.0f;
				b_transisi_keluar = true;
				state_tujuan = GameState::GAMEPLAY;
			}
			break;
		}
		case GameState::GAMEPLAY: {
			if (IsKeyPressed(KEY_M)) {
				b_transisi = true;
				progress_transisi = 0.0f;
				b_transisi_keluar = true;
				state_tujuan = GameState::MAIN_MENU;
			}
			if (IsKeyPressed(KEY_L)) {
				b_transisi = true;
				progress_transisi = 0.0f;
				b_transisi_keluar = true;
				state_tujuan = GameState::LIBRARY;
			}
			total_time = (float)GetTime();
			SetShaderValue(shader, timeloc, &total_time, SHADER_UNIFORM_FLOAT);

			AKSI_UI aksi_crud = UI->Update_UI_Gameplay();
			if (aksi_crud == AKSI_UI::TAMBAH_DATA) {
				if (jumlah_buku_tersimpan < 24 && jumlah_buku_ditambahkan < 8) {
					data_buku[jumlah_buku_tersimpan] = Buku(234, "buku baru", "perpus", true);

					float pos_x = 380.0f + (jumlah_buku_ditambahkan * 90.0f);
					max_tambah_buku[jumlah_buku_ditambahkan] = new Card({ pos_x, 555 }, &data_buku[jumlah_buku_tersimpan], 0.0f);

					jumlah_buku_tersimpan++;
					jumlah_buku_ditambahkan++;
				}
			}
			else if (aksi_crud == AKSI_UI::HAPUS_DATA) {
				//logika hapus data
			}
			else if (aksi_crud == AKSI_UI::URUTKAN_ID) {
				//logika urutkan
			}

			for (int32_t i = 0; i < jumlah_buku_ditambahkan; i++) {
				max_tambah_buku[i]->Update_Card();
			}

			break;
		}
		case GameState::LIBRARY: {
			if (IsKeyPressed(KEY_B)) {
				b_transisi = true;
				progress_transisi = 0.0f;
				b_transisi_keluar = true;
				state_tujuan = GameState::GAMEPLAY;
			}
			total_time = (float)GetTime();
			SetShaderValue(shader, timeloc, &total_time, SHADER_UNIFORM_FLOAT);

			break;
		}
								
	}
}

void GameManager::Draw() {
	// 1. kanvas virtual
	BeginTextureMode(target_kanvas);
		ClearBackground(BLACK);
		switch (current_state) {
			case GameState::MAIN_MENU: {
				BeginShaderMode(Asset->Get_Shader("title"));
					Rectangle sumber_tek = { 0, 0, (float)target_kanvas.texture.width, (float)-target_kanvas.texture.height };
					Vector2 posisi_tujuan = { 0, 0 };
					DrawTextureRec(target_kanvas.texture, sumber_tek, posisi_tujuan, WHITE);
				EndShaderMode();

				UI->DrawMenu();
				break;
			}
			case GameState::GAMEPLAY: {
				//  Render shader 
				BeginShaderMode(Asset->Get_Shader("table"));
					Rectangle sumber_tek = { 
						0, 0, 
						(float)target_kanvas.texture.width, 
						(float)-target_kanvas.texture.height 
					};
					Vector2 posisi_tujuan = { 0, 0 };
					DrawTextureRec(target_kanvas.texture, sumber_tek, posisi_tujuan, WHITE);
				EndShaderMode();

				//shadow card
				UI->DrawGameplay();


				// render kartu atau buku
				Texture2D kartu_tex = Asset->Get_Texture("card");
				Texture2D back_card = Asset->Get_Texture("back");

				Shader burn = Asset->Get_Shader("burn");
					
				for (int32_t i = 0; i < jumlah_buku_ditambahkan; i++) {
						max_tambah_buku[i]->Draw_Card(burn, kartu_tex);
				}

				//stack kartu
				Vector2 stack_card = { 1060.0f, 570.0f };
				DrawTextureEx(back_card, stack_card, 0.0f, 1.0f, WHITE);


				break;
			}
			case GameState::LIBRARY: {
				//  Render shader 
				BeginShaderMode(Asset->Get_Shader("table"));
				Rectangle sumber_tek = {
					0, 0,
					(float)target_kanvas.texture.width,
					(float)-target_kanvas.texture.height
				};
				Vector2 posisi_tujuan = { 0, 0 };
				DrawTextureRec(target_kanvas.texture, sumber_tek, posisi_tujuan, WHITE);
				EndShaderMode();

				UI->DrawLibrary();

				break;
			}
		}

		if (b_transisi || progress_transisi > 0.0f) {
			Shader burn_transisi = Asset->Get_Shader("burn");

			BeginShaderMode(burn_transisi);
				float u_api = 1.0f - progress_transisi;

				int loc = GetShaderLocation(burn_transisi, "uDissolve");
				SetShaderValue(burn_transisi, loc, &u_api, SHADER_UNIFORM_FLOAT);

				Rectangle sumber_tek = {
						0, 0,
						(float)target_kanvas.texture.width,
						(float)-target_kanvas.texture.height
				};
				Vector2 posisi_tujuan = { 0, 0 };
				DrawTextureRec(target_kanvas.texture, sumber_tek, posisi_tujuan, BLACK);
			EndShaderMode();
		}

		DrawText(TextFormat("FPS: %i", GetFPS()), FPS::posisi_x, FPS::posisi_y, 20, RED);
	EndTextureMode();

	BeginDrawing();
		ClearBackground(BLACK);
		BeginShaderMode(Asset->Get_Shader("crt"));
		Rectangle sumber = {
			0, 0,
			(float)target_kanvas.texture.width,
			(float)-target_kanvas.texture.height
		};
		
		Rectangle tujuan = {
			0, 0,
			(float)lebar_layar,
			(float)tinggi_layar
		};

		Vector2 origin = { 0, 0 };

		DrawTexturePro(target_kanvas.texture, sumber, tujuan, origin, 0.0f, WHITE);

		EndShaderMode();
	EndDrawing();

}

void GameManager::Game_Run() {
	//gameloop
	while(!WindowShouldClose() && b_berjalan) {
		Update();
		Draw();
	}
}