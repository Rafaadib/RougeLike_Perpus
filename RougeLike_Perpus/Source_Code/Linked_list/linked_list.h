
#pragma once

#include <cstdint>

struct Buku;

class Linked_List {
public:
	Buku* head;
	int32_t jumlah_ditampilkan;
	const int32_t MAX_TAMPIL = 8;

	Linked_List();
	~Linked_List();

	//funsgi memanmipulaisi buku/kartu di laayr nanti
	void Tambah_Ke_Layar(Buku* buku_database);
	void Hapus_Dari_Layar(int32_t id);
	void Tampilkan_di_Layar();
};




