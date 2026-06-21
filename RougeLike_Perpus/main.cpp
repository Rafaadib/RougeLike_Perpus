
//main kode
//#include "Core/Game_Manager.h"

#include <iostream>

#include "Struct_dan_Array/Perpus.h"
#include "Linked_list/linked_list.h"
#include "Queue/Queue.h"
#include "Stack/Stack.h"
#include "Sorting_dan_Searching/Bubble_Sort.h"
#include "Sorting_dan_Searching/Linear_Search.h"


int main() {

	std::cout << "-----test sroting dan searching-------" << std::endl;

	Perpus perpus_digital;
	
    
    perpus_digital.Tambah_Buku(105, "kereta agro ngawi gejes", "unkonwn");
    perpus_digital.Tambah_Buku(101, "babershop mas rusdi", "mas rusdi");
    perpus_digital.Tambah_Buku(104, "pelatihan ketat", "mas ryan");
    perpus_digital.Tambah_Buku(102, "puding coklat", "pak hambali");

    std::cout << "\n[KONDISI SEBELUM DIURUTKAN]";
    perpus_digital.Tampil_Buku();

    //fungsi buuble untuk mengruutkan id
    Bubble_Sort::Urutkan_Berdasarkan_Id(perpus_digital.data_buku, perpus_digital.jumlah_buku);

    std::cout << "\n[KONDISI SETELAH DIURUTKAN]";
    perpus_digital.Tampil_Buku();

    int indeks_hasil = Linear_Search::Cari_Berdasarkan_Id(perpus_digital.data_buku, perpus_digital.jumlah_buku, 104);

    // Jika data tidak -1 (artinya ketemu), kita bisa menampilkan rinciannya
    if (indeks_hasil != -1) {
        std::cout << "-> Rincian Hasil Pencarian: "
            << perpus_digital.data_buku[indeks_hasil].judul
            << " karangan " << perpus_digital.data_buku[indeks_hasil].pengarang
            << std::endl;
    }
	std::cin.get();


	//constexpr int32_t lebar_layar = 1280;
	//constexpr int32_t tinggi_layar = 780;

	//GameManager game(lebar_layar, tinggi_layar, "jom tengok sapi");
    //game.Game_Run();

	return 0;
}
