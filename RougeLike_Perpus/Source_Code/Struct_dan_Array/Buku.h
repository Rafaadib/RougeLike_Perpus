
#pragma once

#include <iostream>
#include <cstdint>
#include <string>

//array dan struct nya dan membpunyai atribut berikut
struct Buku {
	int32_t id;
	std::string judul;
	std::string pengarang;

	bool b_tersedia;

	Buku* next; //memakai single linekd list untuk nanti

	Buku(int32_t id = 0, std::string judul = "", std::string pengarang = "", bool b_tersedia = true);
};
