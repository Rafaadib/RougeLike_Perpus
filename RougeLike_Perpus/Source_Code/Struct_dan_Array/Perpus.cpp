

#include <fstream>
#include <sstream>

#include "Perpus.h"

Perpus::Perpus() {
	jumlah_buku = 0;
}

Perpus::~Perpus() {

}

//fungsi CRUD 
//Create
void Perpus::Tambah_Buku(int32_t id, std::string judul, std::string pengarang) {
	if (jumlah_buku < MAX_BUKU) {
		data_buku[jumlah_buku] = Buku(id, judul, pengarang, true);
		jumlah_buku++;
		std::cout << "[SUCCESS] Buku : " << judul << " Berhasil Ditambahkan" << std::endl;
	}
	else {
		std::cout << "[ERROR] Kapasitas Sudah Penuh 24/24!" << std::endl;
	}
}

//Read
void Perpus::Tampil_Buku() {
	std::cout << "====Daftar Buku====" << std::endl;
	if (jumlah_buku == 0) {
		std::cout << "Data Ksoong!" << std::endl;
	}
	else {
		for (int32_t i = 0; i < jumlah_buku; i++) {
			std::cout << i + 1 << "ID:" << data_buku[i].id
				<< " | Judul: " << data_buku[i].judul
				<< " | Pengarang: " << data_buku[i].pengarang
				<< " | Status Buku: " << (data_buku[i].b_tersedia ? "Tersedia" : "DiPinjam")
				<< std::endl;
		}
	}
	std::cout << "=========================" << std::endl;
 }

//Update
void Perpus::Update_Buku(int32_t id, std::string judul, std::string pengarang) {
	bool b_ditemukan = false;
	for (int32_t i = 0; i < jumlah_buku; i++) {
		if (data_buku[i].id == id) {
			data_buku[i].judul = judul;
			data_buku[i].pengarang = pengarang;
			b_ditemukan = true;

			std::cout << "[SUCCES] Buku Dengan ID: " << id << " Berhasil Ditemukan!" << std::endl;
			break;
		}
	}

	if (!b_ditemukan) {
		std::cout << "[ERROR] Gagal Update! Buku Dengan ID: " << id << " Tidak Ditemukan!" << std::endl;
	}

}

//delete via Id, menhapusnya dengan cara mengser array
void Perpus::Hapus_Buku(int32_t id) {
	bool b_ditemukan = false;

	for (int32_t i = 0; i < jumlah_buku; i++) {
		if (data_buku[i].id == id) {
			b_ditemukan = true;
			std::cout << "[SUCCESS] Buku " << data_buku[i].judul << " Berhasil Dihapus!" << std::endl;

			//geser array dari kanan menuju ke kiri 
			for (int32_t j = i; j < jumlah_buku - 1; j++) {
				data_buku[j] = data_buku[j + 1];
			}

			jumlah_buku--;
			break;
		}
	}

	if (!b_ditemukan) {
		std::cout << "[ERROR] Gagal Menhapus! Buku Dengan ID: " << id << " Tidak Ditemukan!" << std::endl;
	}
}

//fusngis untuk fstream nya
//untuk menyimpan riwayat
void Perpus::Simpan_File(std::string nama_file) {
	std::ofstream file(nama_file);
	if (!file.is_open()) {
		std::cout << "[ERROR] Gagal Membuka File Untuk Menyimpan" << std::endl;
		return;
	}

	for (int32_t i = 0; i < jumlah_buku; i++) {
		file << data_buku[i].id << ","
			<< data_buku[i].judul << ","
			<< data_buku[i].pengarang << ","
			<< data_buku[i].b_tersedia << "\n";
	}

	file.close();
	std::cout << "[SUCCESS] Data Berhasil DiSimpan Secara Permanen Ke " << nama_file << "'\n" << std::endl;
}

//baca_file untuk mebuka riwayat sebelumnya
void Perpus::Baca_File(std::string nama_file) {
	std::ifstream file(nama_file);
	if (!file.is_open()) {
		std::cout << "[FILE] File " << nama_file << " Tidak Ditemukan!" << std::endl;
		return;
	}

	std::string baris_text;
	jumlah_buku = 0;

	while (std::getline(file, baris_text)) {
		std::stringstream ss(baris_text);
		std::string text_id, judul, pengarang, text_tersedia;

		std::getline(ss, text_id, ',');
		std::getline(ss, judul, ',');
		std::getline(ss, pengarang, ',');
		std::getline(ss, text_tersedia, ',');

		int32_t id = std::stoi(text_id);
		bool b_tersedia = (text_tersedia == "1");

		data_buku[jumlah_buku] = Buku(id, judul, pengarang, b_tersedia);
		jumlah_buku++;
	}
	file.close();
	std::cout << "[SUCCESS] Berhasil DiMuat " << jumlah_buku << " Buku Dari File " << nama_file << "'\n" << std::endl;

}

