#include "c_table.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define INT

#ifdef INT
	typedef int Iterator; // index
#else
	typedef long int Iterator; // position
#endif

Iterator begin(Table* table){
	#ifdef INT
		return 0;
	#else
		return sizeof(Table);
	#endif
}

Iterator end(Table* table){
	#ifdef INT
		return table->csize;
	#else
		return sizeof(Table) + table->csize * sizeof(KeySpace1);
	#endif
}

Iterator next(Iterator iterator){
	#ifdef INT
		return iterator + 1;
	#else
		return iterator + sizeof(KeySpace1);
	#endif
}

InfoS* get_info(Table* table, FILE* file, Iterator iterator){
	#ifdef INT
		fseek(file, sizeof(Table) + iterator * sizeof(KeySpace1), SEEK_SET);
	#else
		fseek(file, iterator, SEEK_SET);
	#endif
	KeySpace1 ks1;
	fread(&ks1, sizeof(KeySpace1), 1, file);
	fseek(file, ks1.pos, SEEK_SET);
	Item item;
	fread(&item, sizeof(Item), 1, file);
	InfoS* infos = (InfoS*)malloc(sizeof(InfoS));
	infos->info.x = item.info.x;
	infos->info.y = item.info.y;
	infos->info.len = item.info.len;
	infos->string = (char*)malloc(item.info.len + 1);
	fread(infos->string, sizeof(char), item.info.len, file);
	infos->string[item.info.len] = '\0';
	return infos;
}

void print_info(InfoS* infos){
	if(infos){
		printf("%d %d %s\n", infos->info.x, infos->info.y, infos->string);
		free(infos->string);
		free(infos);
	}
}

void iterator_print_table(Table* table, FILE* file){
	InfoS* infos;

	for(Iterator i = begin(table); i < end(table); i = next(i)){
		infos = get_info(table, file, i);
		print_info(infos);
	}
}

unsigned int hash1(int key){ //HashFAQ6
	char* str = (char*)&key;

    unsigned int hash = 0;

    for (; *str; str++)
    {
        hash += (unsigned char)(*str);
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash;
}

unsigned int hash2_in(int key){ //HashRot13
	char* str = (char*)&key;

    unsigned int hash = 0;

    for(int i = 0; i < sizeof(int); i++)
    {
        hash += (unsigned char)(*str);
        hash -= (hash << 13) | (hash >> 19);
        str++;
    }

    return hash;

}

unsigned int hash2(int key, int msize){

	unsigned int hash = hash2_in(key);
	
	while(!(hash % msize)){
		hash = hash2_in(hash);
	}

    return hash % msize;

}

//----------------------------init, create, insert------------------------------------------------------------


void create_table(FILE* file, Table* table, int msize){
	table->csize = 0;
	table->msize = msize;
	KeySpace1* ks1 = (KeySpace1*)calloc(1, msize * sizeof(KeySpace1));
	KeySpace2* ks2 = (KeySpace2*)calloc(1, msize * sizeof(KeySpace2));

	rewind(file);
	fwrite(table, sizeof(Table), 1, file);
	fwrite(ks1, sizeof(KeySpace1), msize, file);
	fwrite(ks2, sizeof(KeySpace2), msize, file);

	free(ks1);
	free(ks2);
}

void add_key1(FILE* file, Table* table, int j, const char* key1, int rel, KeySpace1* ks1_ind1){
	fseek(file, sizeof(Table) + j * sizeof(KeySpace1), SEEK_SET);
	KeySpace1* move = (KeySpace1*)malloc((table->csize - j) * sizeof(KeySpace1));
	fread(move, sizeof(KeySpace1), table->csize - j, file);
	fseek(file, (j - table->csize + 1) * sizeof(KeySpace1), SEEK_CUR);
	fwrite(move, sizeof(KeySpace1), table->csize - j, file);
	free(move);
	//memmove(&(table->ks1[j+1]), &(table->ks1[j]), (table->csize - j) * sizeof(KeySpace1));

	for(int i = 0; i < table->csize - j; i++){
		fseek(file, sizeof(Table) + (j + i + 1) * sizeof(KeySpace1), SEEK_SET);
		KeySpace1 ks1_j_i_1;
		fread(&ks1_j_i_1, sizeof(KeySpace1), 1, file);

		long int pos = ks1_j_i_1.pos;
		Item item;
		fseek(file, pos, SEEK_SET);
		fread(&item, sizeof(Item), 1, file);
		item.ind1++;
		fseek(file, -sizeof(Item), SEEK_CUR);
		fwrite(&item, sizeof(Item), 1, file);

		//table->ks1[j+i+1].item->ind1 += 1;
	}
	
	strcpy(ks1_ind1->key, key1);
	ks1_ind1->release = rel;
	ks1_ind1->last_release = rel;


	//table->ks1[j].key = (char*)malloc(strlen(key1)+1);
	//strcpy(table->ks1[j].key, key1);
	//table->ks1[j].release = rel;
	//table->ks1[j].last_release = rel;
}

int insert_table(FILE* file, Table* table, const char key1[N], int key2, InfoS infos){
	if(table->csize == table->msize) return -1;
	if(table->csize){
		for(int i = 0; i < table->msize; i++){
			int index = (hash1(key2) + i * hash2(key2, table->msize)) % table->msize;
			fseek(file, sizeof(Table) + table->msize * sizeof(KeySpace1) + index * sizeof(KeySpace2), SEEK_SET);
			KeySpace2 ks2_i;
			fread(&ks2_i, sizeof(KeySpace2), 1, file);
			if(ks2_i.busy && key2 == ks2_i.key) return -1; //check 2-ond place
			else if(!ks2_i.dead) break;
		}
	}
	int ind1 = 0, ind2 = 0;
	KeySpace1 ks1_ind1 = {0};
	KeySpace2 ks2_ind2 = {0};
	//insert into the first table
	if(table->csize){
		int left = 0, right = table->csize - 1;
		int j = 0, status = 1, rel = 0;
		while(left <= right && status){
			j = (left + right) / 2;
			fseek(file, sizeof(Table) + j * sizeof(KeySpace1), SEEK_SET);
			KeySpace1 ks1_j;
			fread(&ks1_j, sizeof(KeySpace1), 1, file);
			status = strcmp(ks1_j.key, key1);
			if(status < 0)
				left = j + 1;
			else
				right = j - 1;
		}
		if(status < 0){
			add_key1(file, table, j + 1, key1, 0, &ks1_ind1);
			ind1 = j + 1;
		}else if(status > 0){
			add_key1(file, table, j, key1, 0, &ks1_ind1);
			ind1 = j;
		}else{
			fseek(file, sizeof(Table) + j * sizeof(KeySpace1), SEEK_SET);
			KeySpace1 ks1_j;
			fread(&ks1_j, sizeof(KeySpace1), 1, file);
			while(!strcmp(ks1_j.key, key1)){
				if(!(j + 1 < table->csize)){
					rel = ks1_j.last_release;
					j++;
					//rel = table->ks1[j++].last_release;
					break;
				}
				rel = ks1_j.last_release;
				j++;
				fread(&ks1_j, sizeof(KeySpace1), 1, file);
				//rel = table->ks1[j++].last_release;
			}
			add_key1(file, table, j, key1, rel + 1, &ks1_ind1);
			fseek(file, sizeof(Table), SEEK_SET);
			for(int i = 0; i < j; i++){
				KeySpace1 ks1_i;
				fread(&ks1_i, sizeof(KeySpace1), 1, file);
				ks1_i.last_release++;
				fseek(file, -sizeof(KeySpace1), SEEK_CUR);
				fwrite(&ks1_i, sizeof(KeySpace1), 1, file);
				//table->ks1[i].last_release++;
			}
			ind1 = j;
		}
	}else{
		strcpy(ks1_ind1.key, key1);
		ks1_ind1.release = 0;
		ks1_ind1.last_release = 0;
		ind1 = 0;
		//table->ks1[0].key = (char*)malloc(strlen(key1)+1);
		//strcpy(table->ks1[0].key, key1);
		//table->ks1[0].release = 0;
		//table->ks1[0].last_release = 0;
		//ind1 = 0;
	}
	//insert into the second table
	for(int i = 0; i < table->msize; i++){
		int index = (hash1(key2) + i * hash2(key2, table->msize)) % table->msize;
		KeySpace2 ks2_index;
		fseek(file, sizeof(Table) + table->msize * sizeof(KeySpace1) + index * sizeof(KeySpace2), SEEK_SET);
		fread(&ks2_index, sizeof(KeySpace2), 1, file);
		if(!(ks2_index.busy)){
			ks2_ind2.busy = 1;
			ks2_ind2.dead = 1;
			ks2_ind2.key = key2;
			ind2 = index;
			break;
		}
	}

	//create the Item

	Item item;
	item.info.x = infos.info.x;
	item.info.y = infos.info.y;
	item.info.len = strlen(infos.string);
	item.ind1 = ind1;
	item.ind2 = ind2;
	item.del = 0;

	fseek(file, 0, SEEK_END);
	fwrite(&item, sizeof(Item), 1, file);
	long int pos = ftell(file) - sizeof(Item);
	fwrite(infos.string, sizeof(char), strlen(infos.string), file);

	
	ks1_ind1.pos = pos;
	fseek(file, sizeof(Table) + ind1 * sizeof(KeySpace1), SEEK_SET);
	fwrite(&ks1_ind1, sizeof(KeySpace1), 1, file);

	//printf("%d %d\n", ks2_ind2.key, ks2_ind2.dead);
	ks2_ind2.pos = pos;
	fseek(file, sizeof(Table) + table->msize * sizeof(KeySpace1) + ind2 * sizeof(KeySpace2), SEEK_SET);
	fwrite(&ks2_ind2, sizeof(KeySpace2), 1, file);
	/*
	table->ks1[ind1].item = (Item*)malloc(sizeof(Item));
	table->ks1[ind1].item->info.x = info.x;
	table->ks1[ind1].item->info.y = info.y;
	table->ks1[ind1].item->info.string = (char*)malloc(strlen(info.string)+1);
	strcpy(table->ks1[ind1].item->info.string, info.string);
	table->ks1[ind1].item->ind1 = ind1;
	table->ks1[ind1].item->ind2 = ind2;
	table->ks2[ind2].item = table->ks1[ind1].item;
	*/
	table->csize++;
	fseek(file, 0, SEEK_SET);
	fwrite(table, sizeof(Table), 1, file);
	return 0;
}

void show_table(FILE* file, Table* table){
	for(int i = 0; i < 60; i++){
		printf("-");
	}
	printf("\n");
	for(int i = 0; i < table->csize; i++){
		printf("| %d |", i);
		fseek(file, sizeof(Table) + i * sizeof(KeySpace1), SEEK_SET);
		KeySpace1 ks1_i;
		fread(&ks1_i, sizeof(KeySpace1), 1, file);
		long int pos = ks1_i.pos;
		fseek(file, pos, SEEK_SET);
		Item item;
		fread(&item, sizeof(Item), 1, file);
		char* string = (char*)malloc(item.info.len + 1);
		fread(string, sizeof(char), item.info.len, file);
		string[item.info.len] = '\0';
		printf(" Info | %d | %d | %s |", item.info.x, item.info.y, string);
		printf(" Key1 | %s |", ks1_i.key);
		printf(" %d %d |", ks1_i.release, ks1_i.last_release);
		int ind2 = item.ind2;
		fseek(file, sizeof(Table) + table->msize * sizeof(KeySpace1) + ind2 * sizeof(KeySpace2), SEEK_SET);\
		KeySpace2 ks2_ind2;
		fread(&ks2_ind2, sizeof(KeySpace2), 1, file);
		printf(" Key2 | %d |\n", ks2_ind2.key);	
		for(int i = 0; i < 60; i++){
			printf("-");
		}
		free(string);
	printf("\n");
	}
}

int bin_search(FILE* file, Table* table, char key1[N]){
	int left = 0, right = table->csize - 1;
	int ind1, status = 1;
	KeySpace1 ks1_ind1;
	while(left <= right && status){
		ind1 = (left + right) / 2;
		fseek(file, sizeof(Table) + ind1 * sizeof(KeySpace1), SEEK_SET);
		fread(&ks1_ind1, sizeof(KeySpace1), 1, file);
		status = strcmp(ks1_ind1.key, key1);
		if(status < 0)
			left = ind1 + 1;
		else
			right = ind1 - 1;
	}
	if(status != 0) return -1;

	for(;ind1 >= 0; ind1--){
		fseek(file, sizeof(Table) + ind1 * sizeof(KeySpace1), SEEK_SET);
		fread(&ks1_ind1, sizeof(KeySpace1), 1, file);
		if(strcmp(ks1_ind1.key, key1)){
			break;
		}
	}
	ind1++;
	return ind1;
}

InfoS* read_infos(FILE* file, Item item, long int pos){
	InfoS* infos = (InfoS*)malloc(sizeof(InfoS));
	infos->info.x = item.info.x;
	infos->info.y = item.info.y;
	infos->info.len = item.info.len;
	infos->string = (char*)malloc(item.info.len + 1);
	fseek(file, pos + sizeof(Item), SEEK_SET);
	fread(infos->string, sizeof(char), item.info.len, file);
	infos->string[item.info.len] = '\0';
	return infos;
}

InfoS* KS1_1_search_table(FILE* file, Table* table, char key1[N], int key2){ //bin search  and delete

	if(!table->csize){
		return NULL;
	}
	int ind1, ind2;

	ind1 = bin_search(file, table, key1);
	if(ind1 == -1){
		return NULL;
	}

	KeySpace1 ks1_ind1;
	fseek(file, sizeof(Table) + ind1 * sizeof(KeySpace1), SEEK_SET);
	fread(&ks1_ind1, sizeof(KeySpace1), 1, file);

	for(;ind1 < table->csize && !strcmp(ks1_ind1.key, key1); ind1++){
		Item item;
		fseek(file, ks1_ind1.pos, SEEK_SET);
		fread(&item, sizeof(Item), 1, file);
		ind2 = item.ind2;
		KeySpace2 ks2_ind2;
		fseek(file, sizeof(Table) + table->msize * sizeof(KeySpace1) + ind2 * sizeof(KeySpace2), SEEK_SET);
		fread(&ks2_ind2, sizeof(KeySpace2), 1, file);
		//ind2 = table->ks1[ind1].item->ind2;
		if(ks2_ind2.key == key2){
			return read_infos(file, item, ks1_ind1.pos);
		}
		fseek(file, sizeof(Table) + ind1 * sizeof(KeySpace1), SEEK_SET);
		fread(&ks1_ind1, sizeof(KeySpace1), 1, file);
	}
	return NULL;
}

void free_item(FILE* file, KeySpace2 ks2_ind2, Table* table, Item item, int ind1, int ind2){
	ks2_ind2.busy = 0;
	fseek(file, sizeof(Table) + table->msize * sizeof(KeySpace1) + ind2 * sizeof(KeySpace2), SEEK_SET);
	fwrite(&ks2_ind2, sizeof(KeySpace2), 1, file);
	item.del = 1;
	fseek(file, ks2_ind2.pos, SEEK_SET);
	fwrite(&item, sizeof(Item), 1, file);

	fseek(file, sizeof(Table) + (ind1 + 1) * sizeof(KeySpace1), SEEK_SET);
	KeySpace1* move = (KeySpace1*)malloc((table->csize - ind1 - 1) * sizeof(KeySpace1));
	fread(move, sizeof(KeySpace1), table->csize - ind1 - 1, file);
	fseek(file, sizeof(Table) + (ind1) * sizeof(KeySpace1), SEEK_SET);
	fwrite(move, sizeof(KeySpace1), table->csize - ind1 - 1, file);
	free(move);
	for(int i = ind1; i < table->csize - 1; i++){
		fseek(file, sizeof(Table) + i * sizeof(KeySpace1), SEEK_SET);
		KeySpace1 ks1_i;
		fread(&ks1_i, sizeof(KeySpace1), 1, file);
		fseek(file, ks1_i.pos, SEEK_SET);
		fread(&item, sizeof(Item), 1, file);
		item.ind1--;
		fseek(file, -sizeof(Item), SEEK_CUR);
		fwrite(&item, sizeof(Item), 1, file);
	}
}

void KS1_1_delete_table(FILE* file, Table* table, char key1[N], int key2){ //bin search  and delete

	if(!table->csize){
		return;
	}

	int ind1, ind2;

	ind1 = bin_search(file, table, key1);

	if(ind1 == -1){
		return;
	}

	KeySpace1 ks1_ind1;	

	for(;ind1 < table->csize; ind1++){
		fseek(file, sizeof(Table) + ind1 * sizeof(KeySpace1), SEEK_SET);
		fread(&ks1_ind1, sizeof(KeySpace1), 1, file);
		if(strcmp(ks1_ind1.key, key1)){
			break;
		}
		Item item;
		fseek(file, ks1_ind1.pos, SEEK_SET);
		fread(&item, sizeof(Item), 1, file);
		ind2 = item.ind2;
		KeySpace2 ks2_ind2;
		fseek(file, sizeof(Table) + table->msize * sizeof(KeySpace1) + ind2 * sizeof(KeySpace2), SEEK_SET);
		fread(&ks2_ind2, sizeof(KeySpace2), 1, file);
		//Item item = read_item1(file, ks1, ind1);
		//ind2 = item.ind2;
		if(ks2_ind2.key == key2){
			free_item(file, ks2_ind2, table, item, ind1, ind2);
			table->csize--;
			//free_item(item, table, ks1, ks2, file, ind1, ind2);
			break;
		}	
	}
}

InfoS* KS2_1_search_table(FILE* file, Table* table, char* key1, int key2){ //hash search and delete

	if(!table->csize){
		return NULL;
	}

	int ind1, ind2;
	for(int i = 0; i < table->msize; i++){
		ind2 = (hash1(key2) + i * hash2(key2, table->msize)) % table->msize;
		KeySpace2 ks2_ind2;
		fseek(file, sizeof(Table) + table->msize * sizeof(KeySpace1) + ind2 * sizeof(KeySpace2), SEEK_SET);//-------------------------------------------------
		fread(&ks2_ind2, sizeof(KeySpace2), 1, file);
		if(ks2_ind2.busy && ks2_ind2.key == key2){
			Item item;
			fseek(file, ks2_ind2.pos, SEEK_SET);
			fread(&item, sizeof(Item), 1, file);
			ind1 = item.ind1;
			fseek(file, sizeof(Table) + ind1 * sizeof(KeySpace1), SEEK_SET);
			KeySpace1 ks1_ind1;
			fread(&ks1_ind1, sizeof(KeySpace1), 1, file);
			if(!strcmp(ks1_ind1.key, key1)){
				return read_infos(file, item, ks1_ind1.pos);
			}
		}else if(!ks2_ind2.dead){
			break;
		}
	}
	return NULL;
}

void KS2_1_delete_table(FILE* file, Table* table, char* key1, int key2){ //hash search and delete

	if(!table->csize){
		return ;
	}

	int ind1, ind2;
	for(int i = 0; i < table->msize; i++){
		ind2 = (hash1(key2) + i * hash2(key2, table->msize)) % table->msize;
		KeySpace2 ks2_ind2;
		fseek(file, sizeof(Table) + table->msize * sizeof(KeySpace1) + ind2 * sizeof(KeySpace2), SEEK_SET);//-------------------------------------------------
		fread(&ks2_ind2, sizeof(KeySpace2), 1, file);
		if(ks2_ind2.busy && ks2_ind2.key == key2){
			Item item;
			fseek(file, ks2_ind2.pos, SEEK_SET);
			fread(&item, sizeof(Item), 1, file);
			ind1 = item.ind1;
			fseek(file, sizeof(Table) + ind1 * sizeof(KeySpace1), SEEK_SET);
			KeySpace1 ks1_ind1;
			fread(&ks1_ind1, sizeof(KeySpace1), 1, file);
			if(!strcmp(ks1_ind1.key, key1)){
				free_item(file, ks2_ind2, table, item, ind1, ind2);
				table->csize--;
				//free_item(item, table, ks1, ks2, file, ind1, ind2);
				break;
			}
		}else if(!ks2_ind2.dead){
			break;
		}
	}
}

InfoSR* KS1_2_search_table(FILE* file, Table* table, char key1[N]){ //bin search  and delete

	if(!table->csize){
		return NULL;
	}

	int ind1, ind2, ind0;

	ind1 = bin_search(file, table, key1);

	if(ind1 == -1){
		return NULL;
	}

	ind0 = ind1;

	int count = 0;
	KeySpace1 ks1_ind1;
	for(;ind1 < table->csize; ind1++){
		fseek(file, sizeof(Table) + ind1 * sizeof(KeySpace1), SEEK_SET);
		fread(&ks1_ind1, sizeof(KeySpace1), 1, file);
		if(strcmp(ks1_ind1.key, key1)){
			break;
		}
		count+=1;
	}
	ind1 = ind0;

	InfoSR* information = (InfoSR*)calloc(1,(count + 1) * sizeof(InfoSR));
	information[ind1 - ind0].string = NULL;
	information[ind1 - ind0].info.len = count+1;
	ind1++;
	for(;ind1 - ind0 < count + 1; ind1++){
		fseek(file, sizeof(Table) + (ind1 - 1) * sizeof(KeySpace1), SEEK_SET);
		fread(&ks1_ind1, sizeof(KeySpace1), 1, file);
		fseek(file, ks1_ind1.pos, SEEK_SET);
		Item item;
		fread(&item, sizeof(Item), 1, file);
		//Item item = read_item1(file, ks1, ind1-1);
		information[ind1 - ind0].info.x = item.info.x;
		information[ind1 - ind0].info.y = item.info.y;
		information[ind1 - ind0].string = (char*)malloc(item.info.len + 1);
		fread(information[ind1 - ind0].string, sizeof(char), item.info.len, file);
		information[ind1 - ind0].string[item.info.len] = '\0';
		information[ind1 - ind0].release = ks1_ind1.release;
	}
	return information;


	//InfoSR* information = read_infosr(table, ks1, ind1, ind0, key1, count, file);
	
}

void KS1_2_delete_table(FILE* file, Table* table, char key1[N]){ //bin search  and delete

	if(!table->csize){
		return;
	}

	int ind1, ind2, ind0;

	ind1 = bin_search(file, table, key1);
	if(ind1 == -1){
		return;
	}
	int count = 0;
	ind0 = ind1;
	KeySpace1 ks1_ind1;
	for(;ind1 < table->csize; ind1++){
		fseek(file, sizeof(Table) + ind1 * sizeof(KeySpace1), SEEK_SET);
		fread(&ks1_ind1, sizeof(KeySpace1), 1, file);
		if(strcmp(ks1_ind1.key, key1)){
			break;
		}
		count+=1;
	}
	ind1 = ind0;

	for(;ind1 < table->csize; ind1++){
		fseek(file, sizeof(Table) + ind1 * sizeof(KeySpace1), SEEK_SET);
		fread(&ks1_ind1, sizeof(KeySpace1), 1, file);
		if(strcmp(ks1_ind1.key, key1)){
			break;
		}
		Item item;
		fseek(file, ks1_ind1.pos, SEEK_SET);
		fread(&item, sizeof(Item), 1, file);
		item.del = 1;
		fseek(file, -sizeof(Item), SEEK_CUR);
		fwrite(&item, sizeof(Item), 1, file);
		ind2 = item.ind2;
		fseek(file, sizeof(Table) + table->msize * sizeof(KeySpace1) + ind2 * sizeof(KeySpace2), SEEK_SET);
		KeySpace2 ks2_ind2;
		fread(&ks2_ind2, sizeof(KeySpace2), 1, file);
		ks2_ind2.busy = 0;
		fseek(file, -sizeof(KeySpace2), SEEK_CUR);
		fwrite(&ks2_ind2, sizeof(KeySpace2), 1, file);
	}
	fseek(file, sizeof(Table) + ind1 * sizeof(KeySpace1), SEEK_SET);
	KeySpace1* move = (KeySpace1*)calloc(1,(table->csize - ind1) * sizeof(KeySpace1));
	fread(move, sizeof(KeySpace1), table->csize - ind1, file);
	fseek(file, sizeof(Table) + ind0 * sizeof(KeySpace1), SEEK_SET);
	fwrite(move, sizeof(KeySpace1), table->csize - ind1, file);
	free(move);
	table->csize -= ind1 - ind0;
	for(int i = ind0; i < ind1 && i < table->csize; i++){
		KeySpace1 ks1_i;
		fseek(file, sizeof(Table) + i * sizeof(KeySpace1), SEEK_SET);
		fread(&ks1_i, sizeof(KeySpace1), 1, file);
		fseek(file, ks1_i.pos, SEEK_SET);
		Item item;
		fread(&item, sizeof(Item), 1, file);
		item.ind1 -= ind1 - ind0;
		fseek(file, -sizeof(Item), SEEK_CUR);
		fwrite(&item, sizeof(Item), 1, file);
	}
}

InfoS* KS2_2_search_table(FILE* file, Table* table, int key2){ //hash search and delete

	if(!table->csize){
		return NULL;
	}

	int ind1, ind2;

	for(int i = 0; i < table->msize; i++){
		ind2 = (hash1(key2) + i * hash2(key2, table->msize)) % table->msize;
		KeySpace2 ks2_ind2;
		fseek(file, sizeof(Table) + table->msize * sizeof(KeySpace1) + ind2 * sizeof(KeySpace2), SEEK_SET);
		fread(&ks2_ind2, sizeof(KeySpace2), 1, file);
		if(ks2_ind2.busy && ks2_ind2.key == key2){
			Item item;
			fseek(file, ks2_ind2.pos, SEEK_SET);
			fread(&item, sizeof(Item), 1, file);
			ind1 = item.ind1;
			fseek(file, sizeof(Table) + ind1 * sizeof(KeySpace1), SEEK_SET);
			KeySpace1 ks1_ind1;
			fread(&ks1_ind1, sizeof(KeySpace1), 1, file);
			
			return read_infos(file, item, ks1_ind1.pos);
		}else if(!ks2_ind2.dead){
			break;
		}
	}
	return NULL;
}

void KS2_2_delete_table(FILE* file, Table* table, int key2){ //hash search and delete

	if(!table->csize){
		return;
	}

	int ind1, ind2;

	for(int i = 0; i < table->msize; i++){
		ind2 = (hash1(key2) + i * hash2(key2, table->msize)) % table->msize;
		KeySpace2 ks2_ind2;
		fseek(file, sizeof(Table) + table->msize * sizeof(KeySpace1) + ind2 * sizeof(KeySpace2), SEEK_SET);
		fread(&ks2_ind2, sizeof(KeySpace2), 1, file);
		if(ks2_ind2.busy && ks2_ind2.key == key2){
			Item item;
			fseek(file, ks2_ind2.pos, SEEK_SET);
			fread(&item, sizeof(Item), 1, file);
			ind1 = item.ind1;
			fseek(file, sizeof(Table) + ind1 * sizeof(KeySpace1), SEEK_SET);
			KeySpace1 ks1_ind1;
			fread(&ks1_ind1, sizeof(KeySpace1), 1, file);
			
			free_item(file, ks2_ind2, table, item, ind1, ind2);
			table->csize--;
			//free_item(item, table, ks1, ks2, file, ind1, ind2);
			break;
		}else if(!ks2_ind2.dead){
			break;
		}
	}
}

InfoS* search_releases_table(FILE* file, Table* table, char key1[N], int rel){ //bin search  and delete

	if(!table->csize){
		return NULL;
	}

	int ind1;

	ind1 = bin_search(file, table, key1);

	if(ind1 == -1){
		return NULL;
	}

	KeySpace1 ks1_ind1;
	fseek(file, sizeof(Table) + ind1 * sizeof(KeySpace1), SEEK_SET);
	fread(&ks1_ind1, sizeof(KeySpace1), 1, file);

	for(;ind1 < table->csize && !strcmp(ks1_ind1.key, key1); ind1++){
		//ind2 = table->ks1[ind1].item->ind2;
		if(ks1_ind1.release == rel){
			Item item;
			fseek(file, ks1_ind1.pos, SEEK_SET);
			fread(&item, sizeof(Item), 1, file);
			
			return read_infos(file, item, ks1_ind1.pos);
		}
		fseek(file, sizeof(Table) + ind1 * sizeof(KeySpace1), SEEK_SET);
		fread(&ks1_ind1, sizeof(KeySpace1), 1, file);
	}
	return NULL;
}

void delete_releases_table(FILE* file, Table* table, char key1[N], int rel){ //bin search  and delete

	if(!table->csize){
		return;
	}

	int ind1, ind2;

	ind1 = bin_search(file, table, key1);

	if(ind1 == -1){
		return;
	}

	KeySpace1 ks1_ind1;

	for(;ind1 < table->csize; ind1++){
		//Item item = read_item1(file, ks1, ind1);
		//ind2 = item.ind2;
		fseek(file, sizeof(Table) + ind1 * sizeof(KeySpace1), SEEK_SET);
		fread(&ks1_ind1, sizeof(KeySpace1), 1, file);
		if(strcmp(ks1_ind1.key, key1)){
			break;
		}
		if(ks1_ind1.release == rel){
			Item item;
			fseek(file, ks1_ind1.pos, SEEK_SET);
			fread(&item, sizeof(Item), 1, file);
			item.del = 1;
			fseek(file, -sizeof(Item), SEEK_CUR);
			fwrite(&item, sizeof(Item), 1, file);

			ind2 = item.ind2;

			KeySpace2 ks2_ind2;
			fseek(file, sizeof(Table) + table->msize * sizeof(KeySpace1) + ind2 * sizeof(KeySpace2), SEEK_SET);
			fread(&ks2_ind2, sizeof(KeySpace2), 1, file);
			free_item(file, ks2_ind2, table, item, ind1, ind2);
			table->csize--;
			//free_item(item, table, ks1, ks2, file, ind1, ind2);
			break;
		}	
	}
}

void clear_table(){
	FILE* file1 = fopen("c_table.bin", "wb");
	fclose(file1);
}
/*
void garbage_collector_wtable(Table* table, KeySpace1** ks1, KeySpace2** ks2, FILE* file){

	fseek(file, sizeof(Table) + table->msize * (sizeof(KeySpace1) + sizeof(KeySpace2)), SEEK_CUR);

	long int start = ftell(file);
	long int position = start;
	fseek(file, 0, SEEK_END);
	long int end = ftell(file);

	long int delta = 0;

	char* payload = (char*)malloc(end - start);
	fseek(file, start - end, SEEK_CUR);
	fread(payload, sizeof(char), end - start, file);

	Item item;
	while(position < end){
		memcpy(&item, payload + position - start, sizeof(Item));
		if(!item.del){
			(*ks1)[item.ind1].pos = position;
			(*ks2)[item.ind2].pos = position;
			position += sizeof(Item) + item.info.len;
		}
		else{
			memmove(payload + position - start, payload + position + sizeof(Item) + item.info.len - start, end - (position + sizeof(Item) + item.info.len));
			end -= sizeof(Item) + item.info.len;
		}

	}

	rewind(file);
	fseek(file, start, SEEK_CUR);
	fwrite(payload, sizeof(char), end - start, file);

	rewind(file);
	ftruncate(fileno(file), end);

	free(payload);
}
*/

/*
int main(int argc, char const *argv[])
{
	FILE* file = fopen("c_table.bin", "wb");
	fclose(file);
	file = fopen("c_table.bin", "r+b");

	Table table;
	create_table(file, &table, 3);


	InfoS infos;
	infos.info.x = 3;
	infos.info.y = 4;
	infos.string = "qwewer";
	insert_table(file, &table, "1", 1, infos);
	infos.info.x = 6;
	infos.info.y = 5;
	infos.string = "22222";
	insert_table(file, &table, "1", 2, infos);
	infos.info.x = 2;
	infos.info.y = 1;
	infos.string = "3333333";
	insert_table(file, &table, "1", 3, infos);

	InfoS* infos1 = search_releases_table(file, &table, "1", 1);
	if(infos1){
		printf("%d %d %s\n", infos1->info.x, infos1->info.y, infos1->string);
		free(infos1);
	}
	printf("\n");

	InfoSR* infosr = KS1_2_search_table(file, &table, "1");
	if(infosr){
		for(int i = 1;i < infosr[0].info.len; i++){
			printf("%d %d %d %s\n", infosr[i].release, infosr[i].info.x, infosr[i].info.y, infosr[i].string);
			free(infosr[i].string);
		}
		free(infosr);
	}

	KS1_1_delete_table(file, &table, "1", 3);

	show_table(file, &table);
	fclose(file);
	//system("xxd c_table.bin");
	return 0;
}*/