#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "graph.h"

//#define CLOCKS_PER_SEC 1000

int main(int argc, char const *argv[])
{
	Graph graph;
	init_graph(&graph);

	int count;
	printf("number: ");
	scanf("%d", &count);

	srand(time(NULL));
	clock_t all_t = 0;

	//insert------------------------------------
	for(int i = 0; i < count; i++){
		Coords coords;
		coords.x = rand() % count;
		coords.y = rand() % count;
		clock_t time = clock();
		insert_node(&graph, coords);
		all_t += clock() - time;
		if(!(i % 1000) && i){
			printf("%d %.1lf\n", i / 1000, (double)(all_t) / 1000);
			all_t = 0;
		}
	}
	all_t = 0;
	for(int i = 0; i < 10 * count; i++){
		Coords coords1 = graph.array[rand() % count].coords;
		Coords coords2 = graph.array[rand() % count].coords;
		clock_t time = clock();
		insert_edge(&graph, coords1, coords2, 1);
		all_t += clock() - time;
		if(!(i % 1000) && i){
			printf("%d %.1lf\n", i / 1000, (double)(all_t) / 1000);
			all_t = 0;
		}
	}

	//alg----------------------------------------------------------
	all_t = 0;
	for(int i = 0; i < 50; i++){
		Coords coords1 = graph.array[rand() % count].coords;
		Coords coords2 = graph.array[rand() % count].coords;
		clock_t time = clock();
		BFS(graph, coords1, coords2);
		all_t += clock() - time;
		if(!(i % 5) && i){
			printf("BFS %d %.1lf\n", i / 5, (double)(all_t) / 5);
			all_t = 0;
		}
	}
	all_t = 0;
	for(int i = 0; i < 50; i++){
		Coords coords1 = graph.array[rand() % count].coords;
		Coords coords2 = graph.array[rand() % count].coords;
		clock_t time = clock();
		Deikstra(graph, coords1, coords2);
		all_t += clock() - time;
		if(!(i % 5) && i){
			printf("DEI %d %.1lf\n", i / 5, (double)(all_t) / 5);
			all_t = 0;
		}
	}
	all_t = 0;
	for(int i = 0; i < 50; i++){
		Coords coords1 = graph.array[rand() % count].coords;
		Coords coords2 = graph.array[rand() % count].coords;
		clock_t time = clock();
		Ford_Fulk(graph, coords1, coords2);
		all_t += clock() - time;
		if(!(i % 5) && i){
			printf("FF %d %.1lf\n", i / 5, (double)(all_t) / 5);
			all_t = 0;
		}
	}



	//delete----------------------------------------------------
	if(1){
		all_t = 0;
		for(int i = 0; i < 10 * count; i++){
			Coords coords1 = graph.array[rand() % count].coords;
			Coords coords2 = graph.array[rand() % count].coords;
			clock_t time = clock();
			delete_edge(&graph, coords1, coords2);
			all_t += clock() - time;
			if(!(i % 1000) && i){
				printf("%d %.1lf\n", i / 1000, (double)(all_t) / 1000);
				all_t = 0;
			}
		}
		all_t = 0;
		for(int i = 0; i < count; i++){
			Coords coords;
			coords.x = rand() % count;
			coords.y = rand() % count;
			clock_t time = clock();
			delete_node(&graph, coords);
			all_t += clock() - time;
			if(!(i % 1000) && i){
				printf("%d %.1lf\n", i / 1000, (double)(all_t) / 1000);
				all_t = 0;
			}
		}
	}
	else{
		all_t = 0;
		for(int i = 0; i < count; i++){
			Coords coords;
			coords.x = rand() % count;
			coords.y = rand() % count;
			clock_t time = clock();
			delete_node(&graph, coords);
			all_t += clock() - time;
			if(!(i % 1000) && i){
				printf("%d %.1lf\n", i / 1000, (double)(all_t) / 1000);
				all_t = 0;
			}
		}
		all_t = 0;
		for(int i = 0; i < 10 * count; i++){
			Coords coords1 = graph.array[rand() % count].coords;
			Coords coords2 = graph.array[rand() % count].coords;
			clock_t time = clock();
			delete_edge(&graph, coords1, coords2);
			all_t += clock() - time;
			if(!(i % 1000) && i){
				printf("%d %.1lf\n", i / 1000, (double)(all_t) / 1000);
				all_t = 0;
			}
		}
	}

	return 0;
}