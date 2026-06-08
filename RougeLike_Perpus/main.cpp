
//main kode
#include "Core/Game_Manager.h"

int main() {

	const int32_t lebar_layar = 1126;
	const int32_t tinggi_layar = 650;

	GameManager game(lebar_layar, tinggi_layar, "heheheha");
	game.Game_Run();

	return 0;
}
