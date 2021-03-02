#include "matr.h"

int main(int argc, char const *argv[]){
	matr M = read_matr();
	matr newM = new_matr(M);
	show_matr(M, "old matrix");
	show_matr(newM, "new matrix");
	delete_matr(M);
	delete_matr(newM);	
	return 0;
}