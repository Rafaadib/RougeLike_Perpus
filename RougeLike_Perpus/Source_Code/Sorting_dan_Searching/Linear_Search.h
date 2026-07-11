
#pragma once

#include "Struct_dan_Array/Buku.h"

// class static lagi
class Linear_Search {
public:
    // jika tidak ketemu ambil index -1
    static int32_t Cari_Berdasarkan_Id(Buku data_buku[], int32_t jumlah_buku, int32_t target_id);

    static int32_t Cari_Berdasarkan_Judul(Buku data_buku[], int32_t jumlah_buku, std::string judul);
};