
#pragma once


#include "Buku.h"

//kelas yang menhadle file stream dan operasi CRUD nya
class Perpus {
public:
	Perpus();
	~Perpus();

	static const int32_t MAX_BUKU = 24;
	Buku data_buku[MAX_BUKU];
	int32_t jumlah_buku;

	//operasi CRUD
	void Tambah_Buku(int32_t id, std::string judul, std::string pengarang);
	void Tampil_Buku();
	void Update_Buku(int32_t id, std::string judul, std::string pengarang);
	void Hapus_Buku(int32_t id);

	//file hanlding fstream
	void Simpan_File(std::string nama_file);
	void Baca_File(std::string nama_file);
};



