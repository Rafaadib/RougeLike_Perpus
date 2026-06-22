#pragma once

#include "Struct_dan_Array/Buku.h"

// class static
class Bubble_Sort {
public:
    // buat fungsi static biar langsung dipanggil,
    // nggak usah bikin object dari class ini
    static void Urutkan_Berdasarkan_Id(
        Buku data_buku[],
        int32_t jumlah_buku
    );
};