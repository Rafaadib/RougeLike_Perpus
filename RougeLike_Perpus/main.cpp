
//main kode
#include "Core/Game_Manager.h"


int main() {

	
	const uint32_t lebar_layar = 1280;  
	const uint32_t tinggi_layar = 780; 

	GameManager game(lebar_layar, tinggi_layar, "jom tengok sapi");
    game.Game_Run();

	return 0;
}
