
#include "Game_Manager.h"
#include <fstream>

#include "Utililty/utilility.h"


GameManager::GameManager(const int32_t lebar_layar, const int32_t tinggi_layar, const char* judul)
	: lebar_layar(lebar_layar), tinggi_layar(tinggi_layar) {

	InitWindow(lebar_layar, tinggi_layar, judul);

	SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);

}

GameManager::~GameManager() {
	CloseWindow();
}

void GameManager::LoadAsset() {
	//untuk asset nanti
}

void GameManager::UnloadAsset() {
	//mengahspus aseet di memory
}

void GameManager::Update() {
	//logika struktur data
}

void GameManager::Draw() {
	BeginDrawing();
		ClearBackground(BLACK);

		//voba test bat object(nanti pakai asset)
		Rectangle card_book = {
			250,
			150,
			200,
			300
		};

		DrawRectangleRounded(card_book, 0.3f, 8, RED);

		DrawFPS(FPS::posisi_x, FPS::posisi_y);
	EndDrawing();
}

void GameManager::Game_Run() {
	//gameloop
	while (!WindowShouldClose() && b_berjalan) {
		Update();
		Draw();
	}
}