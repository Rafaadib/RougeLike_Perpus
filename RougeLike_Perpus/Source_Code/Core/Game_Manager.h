

#pragma once

#include "raylib.h"
#include <cstdint>

class GameManager {
public:
	GameManager(const int32_t lebar_layar, const int32_t tinggi_layar, const char* judul);
	~GameManager(); //destrukutor

	bool b_berjalan = true;

	void Game_Run();

	int32_t lebar_layar;
	int32_t tinggi_layar;

private:
	void Update();
	void Draw();

	void LoadAsset();
	void UnloadAsset();

};