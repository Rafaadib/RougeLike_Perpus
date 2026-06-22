#include "Linear_Search.h"

int32_t Linear_Search::Cari_Berdasarkan_Id(
    Buku data_buku[],
    int32_t jumlah_buku,
    int32_t target_id
) {
    std::cout << "\n[SUCCES] Mencari Buku Dengan Id: "
              << target_id << std::endl;

    for (int32_t i = 0; i < jumlah_buku; i++) {
        if (data_buku[i].id == target_id) {
            std::cout << "[SUCCES] Buku Dengan "
                      << target_id
                      << " Ditemukan Pada Indek ke: "
                      << i << std::endl;

            return i;
        }
    }

    std::cout << "[ERROR] Buku Dengan Id: "
              << target_id
              << " Tidak Ditemukan!"
              << std::endl;

    return -1;
}

int32_t Linear_Search::Cari_Berdasarkan_Pengarang(
    Buku data_buku[],
    int32_t jumlah_buku,
    std::string judul
) {
    std::cout << "\n[SUCCES] Mencari Buku Dengan Judul: "
              << judul << std::endl;

    for (int32_t i = 0; i < jumlah_buku; i++) {
        if (data_buku[i].judul == judul) {
            std::cout << "[SUCCES] Buku Dengan Judul "
                      << judul
                      << " Ditemukan Pada Indek ke: "
                      << i << std::endl;

            return i;
        }
    }

    std::cout << "[ERROR] Buku Dengan Judul: "
              << judul
              << " Tidak Ditemukan!"
              << std::endl;

    return -1;
}