#include "Bubble_Sort.h"

// stactic class
void Bubble_Sort::Urutkan_Berdasarkan_Id(
    Buku data_buku[],
    int32_t jumlah_buku
) {
    std::cout << "\n[SUCCESS] Memulai Proses Sorting Berdasakan ID.."
              << std::endl;

    // pakai nested loop
    for (int32_t i = 0; i < jumlah_buku - 1; i++) {
        for (int32_t j = 0; j < jumlah_buku - i - 1; j++) {
            if (data_buku[j].id > data_buku[j + 1].id) {
                // proses tukar menukar
                Buku temp = data_buku[j];
                data_buku[j] = data_buku[j + 1];
                data_buku[j + 1] = temp;
            }
        }
    }

    std::cout << "[SUCCES] Data Berhasil Di Urutkan Berdasarkan ID"
              << std::endl;
}