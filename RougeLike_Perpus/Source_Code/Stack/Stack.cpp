
#include "Stack.h"

Stack::Stack() {
	top = -1; //posisi kososng
}

Stack::~Stack() {

}

void Stack::Push(Buku buku_dihapus) {
	if (top >= MAX_UNDO - 1) {
		std::cout << "[WARNING] Undo Sudah Max!" << std::endl;
		return;
	}

	top++;
	data_riwayat[top] = buku_dihapus;

	std::cout << "[SUCCES] Buku " << buku_dihapus.judul << " Masuk Ke Riwayat Aksi" << std::endl;

}

Buku Stack::Pop() {
	if (Is_Empety()) {
		std::cout << "[WARNING] Tidak Ada Yang Bisa Di Undo!" << std::endl;
		return Buku(-1, "", "", false);
	}

	Buku buku_kembali = data_riwayat[top]; //ambil data paling atas
	top--;

	std::cout << "[SUCCES] Buku " << buku_kembali.judul << " Ditarik Kembali" << std::endl;
	return buku_kembali;
}

bool Stack::Is_Empety() {
	return top == -1;
}

void Stack::Tampil_Stack() {
	std::cout << "\n=== TUMPUKAN RIWAYAT UNDO ===" << std::endl;
	if (Is_Empety()) {
		std::cout << "Riwayat bersih, belum ada yang di hapus" << std::endl;
	}
	else {
		//menapilkan dari atas top
		for (int32_t i = top; i >= 0; i--) {
			std::cout << "Urutan " << top - i + 1 << " -> Id: " << data_riwayat[i].id
				<< " | Judul: " << data_riwayat[i].judul << std::endl;
		}
	}
	std::cout << "==========================\n" << std::endl;
}
