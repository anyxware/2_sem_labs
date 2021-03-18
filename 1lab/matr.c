#include <stdio.h>
#include <stdlib.h>
#include "matr.h"

void show_matr(matr M, char* s){
	for(int i = 0; i < M.m; i++){
		for(int j = 0; j < M.matrix[i].n; j++){
			printf("%d ", M.matrix[i].string[j]);
		}
		printf("\n");
	}
	printf("\n");
}

matr read_matr(){
	matr M;
	int status = 0;
	while(status <= 0){
		printf("m: ");
		status = scanf("%d", &(M.m));
		if(status <= 0) scanf("%*[^\n]");
		if(M.m < 0) status = 0;
	}
	M.matrix = (line*)malloc(M.m * sizeof(line));
	for(int i = 0; i < M.m; i++){
		line N;
		status = 0;
		while(status <= 0){
			printf("n%d: ", i);
			status = scanf("%d", &(N.n));
			if(status <= 0) scanf("%*[^\n]");
			if(N.n < 0) status = 0;
		}
		if(N.n) {
			N.string = (int*)malloc(N.n * sizeof(int));
			status = 0;
			while(status <= 0){
				printf("elements: ");
				int j = 0;
				while(j < N.n){
					 status = scanf("%d", &(N.string[j++]));
				}
				if(status <= 0) scanf("%*[^\n]");
			}
		}
		M.matrix[i] = N;
	}
	return M;
}

//while(status = scanf("%[^\n]d"), &x) && status != -1)

matr new_matr(matr M){
	matr newM;
	newM.m = M.m;
	newM.matrix = (line*)malloc(newM.m * sizeof(line));
	for(int i = 0; i < newM.m; i++){
		if(!(M.matrix[i].n)){
			newM.matrix[i] = M.matrix[i];
			continue;
		}
		int k = 0, min = M.matrix[i].string[0];
		for(int j = 0; j < M.matrix[i].n; j++){
			if(M.matrix[i].string[j] < min){
				k = j;
				min = M.matrix[i].string[j];
			}
		}
		line N;
		N.n = M.matrix[i].n - k;
		N.string = (int*)malloc(N.n * sizeof(int));
		for(int j = 0; j < N.n; j++){
			N.string[j] = M.matrix[i].string[k + j];
		}
		newM.matrix[i] = N;
	}
	return newM;
}

void delete_matr(matr M){
	for(int i = 0; i < M.m; i++){
		if(M.matrix[i].n) free(M.matrix[i].string);
	}
	free(M.matrix);
}