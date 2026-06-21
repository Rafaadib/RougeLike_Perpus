
#include "linked_list.h"
#include "Struct_dan_Array/Buku.h"

Linked_List::Linked_List() {
	head = nullptr; //head kosong beararti
	jumlah_ditampilkan = 0;
}

Linked_List::~Linked_List() {
	//kosongngin
}

void Linked_List::Tambah_Ke_Layar(Buku* buku_database) {
	if (jumlah_ditampilkan >= MAX_TAMPIL) {
		std::cout << "[WARNING] Tidak Bisa Menampilkan Lebih Dari 8!" << std::endl;
		return;
	}

	//putus ikatan dari data asli sehingga nanti saat ditampilkan tidak infinit loop yang mnahbiskan emmeory
	buku_database->next = nullptr;

	if (head == nullptr) {
		head = buku_database; // jika kosong bearti menajdi buku/kartu pertama
	}
	else {
		Buku* temp = head; // buat weadah nya temp
		while (temp->next != nullptr) {
			temp = temp->next;
		}
		//sambungkna lagi ke ujung
		temp->next = buku_database;
	}

	jumlah_ditampilkan++;
	std::cout << "[SUCCESS] Buku " << buku_database->judul << " Behasil Ditampilkan" << std::endl;

}

void Linked_List::Hapus_Dari_Layar(int32_t id) {
	if (head == nullptr) {
		std::cout << "[ERROR] Tidak Boleh Kosong" << std::endl;
		return;
	}

	Buku* temp = head;
	Buku* prev = nullptr; //untuk ingatan sementara

	if (temp != nullptr && temp->id == id) {
		head = temp->next;
		jumlah_ditampilkan--;
		std::cout << "[SUCCESS] Buku Dengan ID: "<< id  << " Berhasil Disingkirkan/Dihapus!" << std::endl;
		return;
	}

	while (temp != nullptr && temp->id != id) {
		prev = temp;
		temp = temp->next;
	}

	if (temp == nullptr) {
		std::cout << "[ERROR] Buku Dengan ID: " << id << " Tidak Ada" << std::endl;
		return;
	}

	//ptuurskan sambungan atau node dengan melompatinya
	prev->next = temp->next;
	jumlah_ditampilkan--;
	std::cout << "[SUCCESS] Buku Dengan ID: " << id << " Berhasil Di Singkirkan/Dihapus!" << std::endl;

}

//cara fungsi ini menapilkan dengan menlusuri pointer 1 per 1
void Linked_List::Tampilkan_di_Layar() {
	std::cout << "\n=== Jumalh Kartu/buku ( " << jumlah_ditampilkan << "/8)" << std::endl;
	Buku* temp = head;
	int32_t pos = 1;

	while (temp != nullptr) {
		std::cout << "Posisi " << pos << "-> ID:" << temp->id << " | Judul: " << temp->judul << std::endl;
		temp = temp->next;
		pos++;
	}
	std::cout << "===============================" << std::endl;
}


