
#include "Buku.h"

Buku::Buku(int32_t id, std::string judul, std::string pengarang, bool b_tersedia)
	: id(id), judul(judul), pengarang(pengarang), b_tersedia(b_tersedia) {

	next = nullptr;
}

