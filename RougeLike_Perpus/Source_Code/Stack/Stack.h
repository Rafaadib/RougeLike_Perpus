
#pragma once

#include "Struct_dan_Array/Buku.h"

class Stack {
public:
	static const int32_t MAX_UNDO = 10; //aku batasi maksimal undo 10x
	Buku data_riwayat[MAX_UNDO];

	int32_t top;

	Stack();
	~Stack();

	//funsgi inti stack
	void Push(Buku buku_dihapus);
	Buku Pop();
	bool Is_Empety();

	void Tampil_Stack();

};