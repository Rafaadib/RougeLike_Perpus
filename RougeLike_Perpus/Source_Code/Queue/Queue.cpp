
#include "Queue.h"
#include "Struct_dan_Array/Buku.h"

Queue::Queue() {
	depan = 0;
	belakang = -1;
	jumlah = 0;
}

Queue::~Queue() {

}

void Queue::EnQueue(Buku* buku_dipinjam) {
	if (jumlah >= MAX_ANTRIAN) {
		std::cout << "[WARNING] Slot Pinjam Penuh 4/4" << std::endl;
		return;
	}

	belakang = (belakang + 1) % MAX_ANTRIAN;
	data_antrian[belakang] = buku_dipinjam;
	jumlah++;

	std::cout << "[SUCCESS] Buku " << buku_dipinjam->judul << " Masuk Ke Slot" << std::endl;

}

Buku* Queue::DeQueue() {
	if (jumlah == 0) {
		std::cout << "[WARNING] Kosong Tidak Ada yang Pinjam!" << std::endl;
		return nullptr;
	}

	//ambil buku yang terdepan
	Buku* buku_keluar = data_antrian[depan];
	
	//geser ke berikutnya
	depan = (depan + 1) % MAX_ANTRIAN;
	jumlah--;

	std::cout << "[SUCCES] Buku " << buku_keluar->judul << " Dikembalikan" << std::endl;
	return buku_keluar;
}

void Queue::Tampilkan_Queue() {
	std::cout << "\n=== BUKU DIPINJAM (" << jumlah << "/" << MAX_ANTRIAN << ") ===" << std::endl;

	if (jumlah == 0) {
		std::cout << "Slot Kosong" << std::endl;
	}
	else {
		int32_t indek = depan;
		for (int32_t i = 0; i < jumlah; i++) {
			std::cout << "Slot " << i + 1 << "-> ID: " << data_antrian[indek]->id
				<< " | Judul: " << data_antrian[indek]->judul << std::endl;

			indek = (indek + 1) % MAX_ANTRIAN;
		}
	}
	std::cout << "=============================\n" << std::endl;
}

