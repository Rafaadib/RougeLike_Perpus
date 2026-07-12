
#include "Game_Manager.h"
#include <fstream>

#include "Utililty/utilility.h"
#include "Asset_Manager.h"
#include "UI_Manager.h"
#include "Text_Box.h"
#include "Kartu.h"


GameManager::GameManager(const uint32_t lebar_layar, const uint32_t tinggi_layar, const char* judul)
	: lebar_layar(lebar_layar), tinggi_layar(tinggi_layar) {

	/*SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);*/
	InitWindow(lebar_layar, tinggi_layar, judul);
	SetTargetFPS(240);

	Asset = new Asset_Manager();
	UI = new UI_Manager(Asset, lebar_layar, tinggi_layar);

	font = LoadFontEx("Font/mania.ttf", 32, nullptr, 0);
	SetTextureFilter(font.texture, TEXTURE_FILTER_POINT);
	
	perpus_data.Baca_File("Data_Buku.txt");

	//max buku yang tampil
	int32_t batas_visual = (perpus_data.jumlah_buku < 8) ? perpus_data.jumlah_buku : 8;
	for (int32_t i = 0; i < batas_visual; i++) {
		node_card.Tambah_Ke_Layar(&perpus_data.data_buku[i]);
	}

	//linked listnya
	Buku* temp = node_card.head;
	jumlah_buku_visual = 0;
	float base_x = 380.0f;
	while (temp != nullptr) {
		float pos_x = base_x + (jumlah_buku_visual * 95.0f);

		visual_card[jumlah_buku_visual] = new Card({ pos_x, 555 }, temp, -10.0f + (jumlah_buku_visual * 2.0f));
		jumlah_buku_visual++;
		temp = temp->next;
	}

	LoadAsset();

}

GameManager::~GameManager() {
	//delete manual
	//simpan dulu sebelum dihapus
	perpus_data.Simpan_File("Data_Buku.txt");

	for (int32_t i = 0; i < jumlah_buku_visual; i++) {
		if (visual_card[i] != nullptr) {
			delete visual_card[i];
		}
	}

	for (int32_t i = 0; i < jumlah_buku_tersimpan; i++) {
		if (library_card[i] != nullptr) {
			delete library_card[i];
		}
	}

	if (kartu_ditemukan != nullptr) {
		delete kartu_ditemukan;
	}
	
	//hapus asset dari memory
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
	Asset->Add_Texture("card1", "Asset/card.png");

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
			if (IsKeyPressed(KEY_F1)) {
				b_transisi = true;
				progress_transisi = 0.0f;
				b_transisi_keluar = true;
				state_tujuan = GameState::MAIN_MENU;
			}
			if (IsKeyPressed(KEY_F2)) {
				b_transisi = true;
				progress_transisi = 0.0f;
				b_transisi_keluar = true;
				state_tujuan = GameState::LIBRARY;
			}
			total_time = (float)GetTime();
			SetShaderValue(shader, timeloc, &total_time, SHADER_UNIFORM_FLOAT);

			//klik tumpukan
			Rectangle deck_hitbox = {
				1110.0f,
				570.0f,
				120.0f,
				150.0f
			};

			if (CheckCollisionPointRec(GetMousePosition(), deck_hitbox) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
				
				//bebaskan memory
				for (int32_t i = 0; i < jumlah_buku_tersimpan; i++) {
					if (library_card[i] != nullptr) {
						delete library_card[i];
					}
				}
				jumlah_buku_tersimpan = 0;

				for (int32_t i = 0; i < perpus_data.jumlah_buku; i++) {
					int32_t kolom = i % 8;
					int32_t baris = i / 8;

					float pos_x = 180.0f + (kolom * 125.0f);
					float pos_y = 180.0f + (baris * 200.0f);

					float rotasi_acak = (i % 2 == 0) ? 2.0f : -2.0f;
					library_card[i] = new Card({ pos_x, pos_y }, &perpus_data.data_buku[i], rotasi_acak);
					jumlah_buku_tersimpan++;

				}

				b_transisi = true;
				progress_transisi = 0.0f;
				b_transisi_keluar = true;
				state_tujuan = GameState::LIBRARY;

				std::cout << "[INFO] Membuka Library" << std::endl;
			}

			UI->Update_ITextBox();

			//crud
			AKSI_UI aksi_crud = UI->Update_UI_CRUD();
			if (aksi_crud == AKSI_UI::TAMBAH_DATA || aksi_crud == AKSI_UI::HAPUS_DATA || aksi_crud == AKSI_UI::UPDATE_DATA) {
				if (kartu_ditemukan != nullptr) {
					delete kartu_ditemukan;
					kartu_ditemukan = nullptr;
					b_pencarian = false;
				}
			}
			if (aksi_crud == AKSI_UI::TAMBAH_DATA) {
				//cek inputan apakah kosong
				if (UI->i_id->text.empty() || UI->i_judul->text.empty() || UI->i_pengarang->text.empty()) {
					std::cout << "[WARNING] Di Isi Wak!" << std::endl;
				}
				else if (perpus_data.jumlah_buku >= 24) {
					std::cout << "[WARNING] Library Penuh" << std::endl;
				}
				else {
					int32_t id_baru = std::stoi(UI->i_id->text);
					std::string judul_baru = UI->i_judul->text;
					std::string penagrang_baru = UI->i_pengarang->text;

					perpus_data.Tambah_Buku(id_baru, judul_baru, penagrang_baru);
					if (jumlah_buku_visual < 8) {
						//menghubungkan ke linkedlist
						Buku* buku_baru = &perpus_data.data_buku[perpus_data.jumlah_buku - 1];
						node_card.Tambah_Ke_Layar(buku_baru);

						float pos_x = 380.0f + (jumlah_buku_visual * 95.0f);
						visual_card[jumlah_buku_visual] = new Card({ pos_x, 555 }, buku_baru, 0.0f);
						visual_card[jumlah_buku_visual]->b_toggle = false;
						jumlah_buku_visual++;
					}

					//kosoongkna inputan
					UI->i_id->text = "";
					UI->i_judul->text = "";
					UI->i_pengarang->text = "";

					std::cout << "[SUCCES] Buku Ditambhakan" << std::endl;
				}
			}
			else if (aksi_crud == AKSI_UI::HAPUS_DATA) {
				//logika hapus
				//cek dulu apakah ada antrian
				if (queue_pinjam.jumlah > 0) {
					std::cout << "[WARNING] Saat antrian pinjam tida dapat hapus" << std::endl;
				}
				else {
					bool ada_yang_dihapus = false;

					// Cari kartu mana yang sedang berstatus TERPILIH (Menonjol)
					for (int32_t i = 0; i < jumlah_buku_visual; i++) {

						if (visual_card[i]->b_toggle && visual_card[i]->data != nullptr) {

							int32_t id_hapus = visual_card[i]->data->id;

							//Amankan salinan data murni ke Stack (Riwayat Undo)
							riwayat_hapus.Push(*(visual_card[i]->data));

							//Hapus dari Linked List dan Array Database
							node_card.Hapus_Dari_Layar(id_hapus);
							perpus_data.Hapus_Buku(id_hapus);

							for (int32_t k = i + 1; k < jumlah_buku_visual; k++) {
								if ((visual_card[k] != nullptr && visual_card[k]->data != nullptr)) {
									visual_card[k]->data = visual_card[k]->data - 1;
								}
							}

							//Putuskan pointer data dari visual kartu!
							visual_card[i]->data = nullptr;

							//Picu animasi shader terbakar
							visual_card[i]->Burn();

							ada_yang_dihapus = true;
							std::cout << "[SUCCESS] Buku ID " << id_hapus << " Dihapus!" << std::endl;

							// Hentikan pencarian karena kartu sudah ketemu dan dihapus
							break;
						}
					}

					// Jika tombol Hapus ditekan tapi tidak ada kartu yang sedang diklik/menonjol
					if (!ada_yang_dihapus) {
						std::cout << "[WARNING] klik dulu wak!" << std::endl;
					}
				}
			}
			else if (aksi_crud == AKSI_UI::SHOW_DATA) {
				//logika tampilakn
			}
			else if (aksi_crud == AKSI_UI::UPDATE_DATA) {
				//logika update
				bool b_update = false;

				for (int32_t i = 0; i < jumlah_buku_visual; i++) {
					if (visual_card[i]->b_toggle && visual_card[i]->data != nullptr) {
						b_update = true;

						if (UI->i_id->text.empty() && UI->i_judul->text.empty() && UI->i_pengarang->text.empty()) {
							std::cout << "[WARNING] Minimal Isi 1 " << std::endl;
						}
						else {
							//update
							//jika id
							if (!UI->i_id->text.empty()) {
								visual_card[i]->data->id = std::stoi(UI->i_id->text);
							}

							//jika judul
							if (!UI->i_judul->text.empty()) {
								visual_card[i]->data->judul = UI->i_judul->text;
							}

							//jika pengarang
							if (!UI->i_pengarang->text.empty()) {
								visual_card[i]->data->pengarang = UI->i_pengarang->text;
							}

							UI->i_id->text = "";
							UI->i_pengarang->text = "";
							UI->i_judul->text = "";

							visual_card[i]->b_toggle = false;
							std::cout << "[SUCCES] Data Buku Berhasil Berhasil Di Perbaruai" << std::endl;

						}
						break;
					}
				}
				if (!b_update) {
					std::cout << "[WARNING] Klik Dulu Wak" << std::endl;
				}
			}
			

			//search, sort, undo
			AKSI_UI aksi_sus = UI->Update_UI_SUS();
			if (aksi_sus == AKSI_UI::SORT) {
				//logika sorting
				//soritng nya lansgung se librabry
				if (perpus_data.jumlah_buku == 0) {
					std::cout << "[WARNING] Perpus Kosong wak!" << std::endl;
				}
				else {
					//fungsi dari class buuble short
					//urutu bedasra id
					Bubble_Sort::Urutkan_Berdasarkan_Id(perpus_data.data_buku, perpus_data.jumlah_buku);
					for (int32_t i = 0; i < jumlah_buku_visual; i++) {
						if (visual_card[i] != nullptr) {
							visual_card[i]->data = &perpus_data.data_buku[i];

							//visual sedkit
							visual_card[i]->rotasi += 5.0f;
						}
					}
					std::cout << "[SUCCES] Soerting Berhasil" << std::endl;
				}

			}
			else if (aksi_sus == AKSI_UI::UNDO) {
				//logika undo
				//cek apakah ada isisnya
				if (riwayat_hapus.Is_Empety()) {
					std::cout << "[WARNING] Riwayat Kosong!" << std::endl;
				}
				else if (perpus_data.jumlah_buku >= 24) {
					std::cout << "[WARNING] Library Penuh" << std::endl;
				}
				else {
					Buku buku_kembali = riwayat_hapus.Pop();
					//pastikan indexnya tidak -1
					if (buku_kembali.id != -1) {
						perpus_data.Tambah_Buku(buku_kembali.id, buku_kembali.judul, buku_kembali.pengarang);

						//spwan hanya jika kurang dari 8 di layar gampelay
						if (jumlah_buku_visual < 8) {
							Buku* ref_buku_baru = &perpus_data.data_buku[perpus_data.jumlah_buku - 1];

							//tapilkanya mengugnakan linked list
							node_card.Tambah_Ke_Layar(ref_buku_baru);
							float pos_x = 380.0f + (jumlah_buku_visual * 95.0f);
							float rotasi_baru = -10.0f + (jumlah_buku_visual * 2.0f);
							visual_card[jumlah_buku_visual] = new Card({ pos_x, 555 }, ref_buku_baru, rotasi_baru);
							visual_card[jumlah_buku_visual]->b_toggle = false;

							jumlah_buku_visual++;
						}
						std::cout << "[SUCCES] Undo Succes" << std::endl;
					}
				}
			}
			else if (aksi_sus == AKSI_UI::SEARCH) {
				//logika searching
				//cek apakah id dan judul sudah di klik
				if (UI->i_id->text.empty() && UI->i_judul->text.empty()) {
					std::cout << "[WARNING] Masukan id atau judul dulu" << std::endl;
				}
				else {
					int32_t indek_ketemu = -1;

					//cari beradsar id
					if (!UI->i_id->text.empty()) {
						int32_t target_id = std::stoi(UI->i_id->text);
						indek_ketemu = Linear_Search::Cari_Berdasarkan_Id(perpus_data.data_buku, perpus_data.jumlah_buku, target_id);
					}
					else if (!UI->i_judul->text.empty()) {
						indek_ketemu = Linear_Search::Cari_Berdasarkan_Judul(perpus_data.data_buku, perpus_data.jumlah_buku, UI->i_judul->text);
					}

					//jika ketemu
					if (indek_ketemu != -1) {
						//cek apakah buku inni sedang dipinjam atau tidak
						if (!perpus_data.data_buku[indek_ketemu].b_tersedia) {
							std::cout << "[INFO] Buku " << perpus_data.data_buku[indek_ketemu].judul << " Sedang Dipinjam!" << std::endl;
						}
						else {
							if (kartu_ditemukan != nullptr) {
								delete kartu_ditemukan;
							}

							kartu_ditemukan = new Card({ 710.0f, 320.0f }, &perpus_data.data_buku[indek_ketemu], 0.0f);
							b_pencarian = true;

							//kosongkan setealh diisi
							UI->i_id->text = "";
							UI->i_judul->text = "";
							UI->i_pengarang->text = "";

							std::cout << "[SUCCESS] Buku DItemukan" << std::endl;
						}
						
					}

				}
			}

			//pinjam dan kembalikan
			AKSI_UI aksi_pk = UI->Update_UI_PK();
			if (aksi_pk == AKSI_UI::PINJAM) {
				//logika pinjam
				for (int32_t i = 0; i < jumlah_buku_visual; i++) {

					// Jika kartu di meja terpilih
					if (visual_card[i]->b_toggle && visual_card[i]->data != nullptr) {

						// Cek kapasitas antrean (maks 4)
						if (queue_pinjam.jumlah < 4) {
							Buku* buku_target = visual_card[i]->data;

							//masukan data ke queue
							queue_pinjam.EnQueue(buku_target);

							int32_t index_antrian = queue_pinjam.jumlah - 1;
							float pos_x = 580.0f + (index_antrian * 120.0f);
							visual_pinjam[index_antrian] = new Card({ pos_x, 110.0f }, buku_target, 0.0f);

							//putuskan data linkedlistnya
							visual_card[i]->data = nullptr;
							visual_card[i]->Burn();
							
						}
						break;
					}
				}

			}
			else if (aksi_pk == AKSI_UI::KEMBALIKAN) {
				//logika pinjam
				if (queue_pinjam.jumlah > 0 && visual_pinjam[0]->b_toggle) {
					Buku* buku_kembali = queue_pinjam.DeQueue();

					//bersihan yang terdepan
					delete visual_pinjam[0];

					for (int32_t i = 0; i < queue_pinjam.jumlah; i++) {
						visual_pinjam[i] = visual_pinjam[i + 1];
						visual_pinjam[i]->base_posisi.x -= 105.0f;
					}
					visual_pinjam[queue_pinjam.jumlah] = nullptr; //ujung nya diksooginn

					if (jumlah_buku_visual < 8) {
						node_card.Tambah_Ke_Layar(buku_kembali);
						float pos_x = 380.0f + (jumlah_buku_visual * 95.0f);
						float rotasi_baru = -10.0f + (jumlah_buku_visual * 2.0f);

						visual_card[jumlah_buku_visual] = new Card({ pos_x, 555 }, buku_kembali, rotasi_baru);
						visual_card[jumlah_buku_visual]->b_toggle = false;
						jumlah_buku_visual++;
					}
				}
				else if(queue_pinjam.jumlah > 0) {
					std::cout << "[WARNING] klik kartu paling ujung untuk mengbalikan!" << std::endl;
				}
			}


			for (int32_t i = 0; i < jumlah_buku_visual; i++) {
				visual_card[i]->Update_Card();
				
				//hapus bekas di memory
				if (visual_card[i]->b_burning && visual_card[i]->progerss_hancur >= 0.5f) {
					delete visual_card[i];

					for (int32_t j = i; j < jumlah_buku_visual - 1; j++) {
						visual_card[j] = visual_card[j + 1];

						//kordinat ke kiri
						visual_card[j]->base_posisi.x -= 95.0f;
						visual_card[j]->base_rotasi -= 2.0f;
					}
					jumlah_buku_visual--;
					i--;

					//ambil data dari library untuk di tampilkan
					if (jumlah_buku_visual < 8 && perpus_data.jumlah_buku > jumlah_buku_visual ) {
						Buku* buku_cadangan = nullptr;

						// Cari buku pertama di gudang yang BELUM ada di meja dan TIDAK sedang dipinjam
						for (int32_t b = 0; b < perpus_data.jumlah_buku; b++) {
							bool sudah_di_meja = false;

							// Cek apakah buku ini sudah mejeng di meja
							for (int32_t v = 0; v < jumlah_buku_visual; v++) {
								if (visual_card[v]->data != nullptr && visual_card[v]->data->id == perpus_data.data_buku[b].id) {
									sudah_di_meja = true;
									break;
								}
							}

							// Jika belum ada di meja dan statusnya b_tersedia == true
							if (!sudah_di_meja && perpus_data.data_buku[b].b_tersedia) {
								buku_cadangan = &perpus_data.data_buku[b];
								break; // Ketemu! Hentikan pencarian
							}
						}

						// Jika ketemu buku yang cocok, munculkan di meja
						if (buku_cadangan != nullptr) {
							node_card.Tambah_Ke_Layar(buku_cadangan);

							float pos_x = 380.0f + (jumlah_buku_visual * 95.0f);
							float rotasi_baru = -10.0f + (jumlah_buku_visual * 2.0f);

							visual_card[jumlah_buku_visual] = new Card({ pos_x , 555 }, buku_cadangan, rotasi_baru);
							visual_card[jumlah_buku_visual]->b_toggle = false;

							jumlah_buku_visual++;
							std::cout << "[INFO] Buku baru ditarik dari gudang!" << std::endl;
						}
					}


					//biar tidak crash kembalikan ke i++
					continue;
				}

				//klik 
				if (visual_card[i]->b_hover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
					bool b_terpilih = visual_card[i]->b_toggle;
					for (int32_t k = 0; k < jumlah_buku_visual; k++) {
						visual_card[k]->b_toggle = false;
					}
					if (!b_terpilih) {
						visual_card[i]->b_toggle = true;
					/*	if (visual_card[i]->data != nullptr) {
							UI->i_id->text = std::to_string(visual_card[i]->data->id);
							UI->i_judul->text = visual_card[i]->data->judul;
							UI->i_pengarang->text = visual_card[i]->data->pengarang;
						}*/
					}
					/*else {
						UI->i_id->text = "";
						UI->i_judul->text = "";
						UI->i_pengarang->text = "";
					}*/
				}
			}

			//updaet hasil pencarian
			if (b_pencarian && kartu_ditemukan != nullptr) {
				kartu_ditemukan->Update_Card();

				if (kartu_ditemukan->b_hover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
					delete kartu_ditemukan;
					kartu_ditemukan = nullptr;
					b_pencarian = false;
					std::cout << "[INFO] Pencarian Berhenti" << std::endl;
				}
			}

			//antrian loop
			for (int32_t i = 0; i < queue_pinjam.jumlah; i++) {
				visual_pinjam[i]->Update_Card();

				if (visual_pinjam[i]->b_hover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
					//hanya kartu index pertama yang bisa di klik
					if (i == 0) {
						bool b_terpilih = visual_pinjam[0]->b_toggle; //hanya index eprtama
						for (int32_t k = 0; k < queue_pinjam.jumlah; k++) {
							visual_pinjam[k]->b_toggle = false;
						}

						visual_pinjam[0]->b_toggle = !b_terpilih;
					}
					else {
						visual_pinjam[i]->getar = 0.3f;
					}
				}
			}

			break;
		}
		case GameState::LIBRARY: {
			if (IsKeyPressed(KEY_TAB)) {
				for (int32_t i = 0; i < jumlah_buku_tersimpan; i++) {
					delete library_card[i];
					library_card[i] = nullptr;
				}
				jumlah_buku_tersimpan = 0;

				b_transisi = true;
				progress_transisi = 0.0f;
				b_transisi_keluar = true;
				state_tujuan = GameState::GAMEPLAY;
			}
			total_time = (float)GetTime();
			SetShaderValue(shader, timeloc, &total_time, SHADER_UNIFORM_FLOAT);

			//interaksi di library
			for (int32_t i = 0; i < jumlah_buku_tersimpan; i++) {
				library_card[i]->Update_Card();

				//logika klik nya
				if (library_card[i]->b_hover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
					bool b_terpilih = library_card[i]->b_toggle;
					for (int32_t k = 0; k < jumlah_buku_tersimpan; k++) {
						library_card[k]->b_toggle = false;
					}

					//nya
					if (!b_terpilih) {
						library_card[i]->b_toggle = true;
					}
				}
			}

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

				Texture2D card_menu = Asset->Get_Texture("card");
				DrawTextureEx(card_menu, { 650, 200 }, 5.0f, 1.5f, WHITE);
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
				//int32_t jumlah_di_pinjam = 0; //sementara
				UI->DrawGameplay(jumlah_buku_visual, perpus_data.jumlah_buku, queue_pinjam.jumlah);


				// render kartu atau buku
				Texture2D kartu_tex = Asset->Get_Texture("card1");
				Texture2D back_card = Asset->Get_Texture("back");

				Shader burn = Asset->Get_Shader("burn");
				
				//render 8 kartu dan libary
				for (int32_t i = 0; i < jumlah_buku_visual; i++) {
						visual_card[i]->Draw_Card(burn, kartu_tex, font);
				}

				//stack kartu
				Vector2 stack_card = { 1090.0f, 570.0f };
				DrawTextureEx(back_card, stack_card, 0.0f, 1.0f, WHITE);

				if (b_pencarian && kartu_ditemukan != nullptr) {
					DrawRectangle(0, 0, lebar_layar, tinggi_layar, Fade(BLACK, 0.5f));

					kartu_ditemukan->Draw_Card(burn, kartu_tex, font);
					const char* instruksi = "Klik kartu untuk menutup";
					int lebar_teks = MeasureText(instruksi, 20);
					DrawText(instruksi, 710 - (lebar_teks / 2), 430, 20, LIGHTGRAY);
				}

				//render kartu pinjam 
				for (int32_t i = 0; i < queue_pinjam.jumlah; i++) {
					if (visual_pinjam[i] != nullptr) {
						visual_pinjam[i]->Draw_Card(burn, kartu_tex, font);
					}
				}

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

				//gambar seluruh kartunya
				Texture2D card_tex = Asset->Get_Texture("card1");
				Shader burn = Asset->Get_Shader("burn");

				for(int32_t i = 0; i < jumlah_buku_tersimpan; i++) {
					library_card[i]->Draw_Card(burn, card_tex, font);
				}

				break;
			}
		}

		//logika transisi state
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

		DrawText(TextFormat("FPS: %i", GetFPS()), FPS::posisi_x, FPS::posisi_y, 20, WHITE);
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