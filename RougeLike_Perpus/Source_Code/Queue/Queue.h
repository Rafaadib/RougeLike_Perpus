
#pragma once

#include <iostream>
#include <cstdint>

struct Buku;

class Queue {
public:
	static const int32_t MAX_ANTRIAN = 4;
	Buku* data_antrian[MAX_ANTRIAN];

	int32_t depan;
	int32_t belakang;
	int32_t jumlah;

	Queue();
	~Queue();

	//fungsi inti queue untuk nanati
	void EnQueue(Buku* buku_dipinjam); //tambah/ buku yang ingin dipinjam
	Buku* DeQueue(); //kembalikan
	void Tampilkan_Queue(); //tampilkan
};
