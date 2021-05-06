#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <float.h>
#include "graph.h"

#define GRAPH_SIZE 1

void init_graph(Graph* graph){
	graph->msize = GRAPH_SIZE;
	graph->array = (Node*)calloc(graph->msize, sizeof(Node));
	graph->capacity = 0;
}

int compare(Coords coords1, Coords coords2){
	return coords1.x == coords2.x && coords1.y == coords2.y;
}

int insert_node(Graph* graph, Coords coords){
	int index = -1;
	int i = 0;
	for(; i < graph->msize && graph->array[i].busy; i++){
		if(graph->array[i].busy == 1 && compare(graph->array[i].coords, coords)){
			return -1;
		}
		if(index < 0 && graph->array[i].busy != 1){
			index = i;
		}	
	}

	if(index < 0){
		index = i;
	}

	graph->array[index].busy = 1;
	graph->array[index].coords = coords;
	graph->array[index].head = NULL;

	if(index + 1 == graph->msize){
		graph->array = realloc(graph->array, (graph->msize + GRAPH_SIZE) * sizeof(Node));
		memset(&graph->array[graph->msize], 0, GRAPH_SIZE * sizeof(Node));
		graph->msize += GRAPH_SIZE;
	}
}

void prepend_item(Item** head, int index, float weight){
	Item* ptr = (Item*)malloc(sizeof(Item));
	ptr->index = index;
	ptr->weight = weight;
	ptr->next = *head;
	*head = ptr;
}

int in_list(Item* head, int index){
	for(Item* ptr = head; ptr; ptr = ptr->next){
		if(ptr->index == index){
			return 1;
		}
	}
	return 0;
}

int insert_edge(Graph* graph, Coords coords1, Coords coords2, float weight){
	int index1 = -1, index2 = -1;
	for(int i = 0; graph->array[i].busy; i++){
		if(compare(graph->array[i].coords,coords1)){
			index1 = i;
		}
		if(compare(graph->array[i].coords,coords2)){
			index2 = i;
		}

		if(index1 > 0 && index2 > 0) break;
	}

	if(index1 < 0 || index2 < 0){
		return -1;
	}

	if(!in_list(graph->array[index1].head, index2)){
		prepend_item(&graph->array[index1].head, index2, weight);
	}
}

void remove_item(Item** head, int index){
	Item *ptr, *prev;
	for(ptr = *head, prev = NULL; ptr && ptr->index != index; prev = ptr, ptr = ptr->next){}
	if(!ptr) return;
	if(ptr == *head) *head = ptr->next;
	if(prev) prev->next = ptr->next;
	free(ptr);
}

void delete_list(Item** head){
	Item* ptr = *head, *prev;
    while(ptr){
        prev = ptr;
        ptr = ptr->next;
        free(prev);
    }
    *head = NULL;
}

int delete_node(Graph* graph, Coords coords){
	int index = -1;
	for(int i = 0; i < graph->msize && graph->array[i].busy; i++){
		if(graph->array[i].busy == 1 && compare(graph->array[i].coords, coords)){
			index = i;
			break;
		}	
	}

	if(index < 0){
		return -1;
	}

	for(int i = 0; i < graph->msize && graph->array[i].busy; i++){
		if(graph->array[i].busy == 1 && i != index){
			remove_item(&graph->array[i].head, index);
		}
	}

	graph->array[index].busy = -1;
	delete_list(&graph->array[index].head);

	return 0;
}

int delete_edge(Graph* graph, Coords coords1, Coords coords2){
	int index1 = -1, index2 = -1;
	for(int i = 0; graph->array[i].busy; i++){
		if(compare(graph->array[i].coords,coords1)){
			index1 = i;
		}
		if(compare(graph->array[i].coords,coords2)){
			index2 = i;
		}

		if(index1 > 0 && index2 > 0) break;
	}

	if(index1 < 0 || index2 < 0){
		return -1;
	}

	remove_item(&graph->array[index1].head, index2);
}

int graph_size(Graph graph){
	int size = 0;
	for(int i = 0; graph.array[i].busy; i++, size++){}
	return size;
}

void push_queue(QItem** queue, int index){
	QItem* ptr = (QItem*)malloc(sizeof(QItem));
	ptr->index = index;
	ptr->next = *queue;
	*queue = ptr;
}

int pop_queue(QItem** queue){
	QItem *ptr, *prev;
	if(!queue) return -1;

	for(ptr = *queue, prev = NULL; ptr && ptr->next; prev = ptr, ptr = ptr->next){}

	if(ptr == *queue) *queue = NULL;
	if(prev) prev->next = NULL;
	int index = ptr->index;
	free(ptr);
	return index;
}

void delete_queue(QItem** queue){
	QItem* ptr = *queue, *prev;
    while(ptr){
        prev = ptr;
        ptr = ptr->next;
        free(prev);
    }
    *queue = NULL;
}

void print_path(QItem* path, Graph graph){
	for(QItem* ptr = path; ptr; ptr = ptr->next){
		printf("%d %d, ", graph.array[ptr->index].coords.x, graph.array[ptr->index].coords.y);
	}
	printf("\n");
}

void find_path(Graph graph, int size, int parents[size], int index1, int index2, QItem** path){
	if(index1 == index2){
		push_queue(path, index1);
	}
	else{
		if(parents[index2] == -1){
			delete_queue(path);
		}else{
			push_queue(path, index2);
			find_path(graph, size, parents, index1, parents[index2], path);
		}
	}
}

BFSPath BFS(Graph graph, Coords coords1, Coords coords2){
	int index1 = -1, index2 = -1;
	for(int i = 0; i < graph.msize && graph.array[i].busy; i++){
		if(graph.array[i].busy == 1 && compare(graph.array[i].coords, coords1)){
			index1 = i;
		}
		if(graph.array[i].busy == 1 && compare(graph.array[i].coords, coords2)){
			index2 = i;
		}
		if(index1 != -1 && index2 != -1){
			break;
		}
	}
	
	if(index1 < 0 || index2 < 0){
		BFSPath BFSpath;
		BFSpath.distance = -1;
		return BFSpath;
	}

	int size = graph_size(graph);
	char visited[size];
	memset(visited, 0, size * sizeof(char));
	int parents[size];
	memset(parents, -1, size * sizeof(int));
	float distance[size];
	float infinity = FLT_MAX;
	for(int i = 0; i < size; i++){
		distance[i] = infinity;
	}

	QItem* queue = NULL;
	push_queue(&queue, index1);
	visited[index1] = 1;
	distance[index1] = 0;


	while(queue){
		int cur_index = pop_queue(&queue);
		for(Item* ptr = graph.array[cur_index].head; ptr; ptr = ptr->next){
			if(!visited[ptr->index]){
				visited[ptr->index] = 1;
				parents[ptr->index] = cur_index;
				distance[ptr->index] = distance[cur_index] + ptr->weight;
				push_queue(&queue, ptr->index);
			}
		}
		visited[cur_index] = 1;
		if(cur_index == index2){
			break;
		}
	}
	if(queue){
		delete_queue(&queue);
	}

	QItem* path = NULL;;
	find_path(graph, size, parents, index1, index2, &path);
	BFSPath BFSpath = {distance[index2], path};

	return BFSpath;
}

void push_priority_queue(QItem** queue, int index, int size, float distance[size]){
	QItem* ptr, *prev;
	for(ptr = *queue, prev = NULL; ptr && distance[ptr->index] > distance[index]; prev = ptr, ptr = ptr->next){}
	QItem* new = (QItem*)malloc(sizeof(QItem));
	new->index = index;
	new->next = ptr;
	if(prev) prev->next = new;
	else *queue = new;
}

float Deikstra(Graph graph, Coords coords1, Coords coords2){
	int index1 = -1, index2 = -1;
	for(int i = 0; i < graph.msize && graph.array[i].busy; i++){
		if(graph.array[i].busy == 1 && compare(graph.array[i].coords, coords1)){
			index1 = i;
		}
		if(graph.array[i].busy == 1 && compare(graph.array[i].coords, coords2)){
			index2 = i;
		}
		if(index1 != -1 && index2 != -1){
			break;
		}
	}
	
	if(index1 < 0 || index2 < 0){
		return -2;
	}

	int size = graph_size(graph);
	char visited[size];
	memset(visited, 0, size * sizeof(char));
	int parents[size];
	memset(parents, -1, size * sizeof(int));
	float distance[size];
	float infinity = FLT_MAX;
	for(int i = 0; i < size; i++){
		distance[i] = infinity;
	}

	distance[index1] = 0;
	QItem* queue = NULL;

	for(int i = 0; i < graph.msize && graph.array[i].busy; i++){
		if(graph.array[i].busy == 1){
			push_priority_queue(&queue, i, size, distance);
		}
	}

	while(queue){
		int cur_index = pop_queue(&queue);
		visited[cur_index] = 1;
		for(Item* ptr = graph.array[cur_index].head; ptr; ptr = ptr->next){
			if(distance[ptr->index] > distance[cur_index] + ptr->weight){
				distance[ptr->index] = distance[cur_index] + ptr->weight;
				parents[ptr->index] = cur_index;
			}
		}
	}


	return distance[index2];
}

void write_to_file(Graph graph, FILE* file){
	char cont = 0, end = 1, full_end = 2;
	for(int i = 0; i < graph.msize && graph.array[i].busy; i++){
		if(graph.array[i].busy == 1){
			fwrite(&cont, sizeof(char), 1, file);
			fwrite(&graph.array[i].coords.x, sizeof(int), 1, file);
			fwrite(&graph.array[i].coords.y, sizeof(int), 1, file);
		}
	}
	fwrite(&end, sizeof(char), 1, file);
	for(int i = 0; i < graph.msize && graph.array[i].busy; i++){
		if(graph.array[i].busy == 1){
			for(Item* ptr = graph.array[i].head; ptr; ptr = ptr->next){
				fwrite(&cont, sizeof(char), 1, file);
				fwrite(&graph.array[ptr->index].coords.x, sizeof(int), 1, file);
				fwrite(&graph.array[ptr->index].coords.y, sizeof(int), 1, file);
				fwrite(&ptr->weight, sizeof(float), 1, file);
			}
			fwrite(&end, sizeof(char), 1, file);
		}
	}
}

void read_from_file(Graph* graph, FILE* file){
	char status;
	for(;;){
		fread(&status, sizeof(char), 1, file);
		if(status == 0){
			Coords coords;
			fread(&coords.x, sizeof(int), 1, file);
			fread(&coords.y, sizeof(int), 1, file);
			insert_node(graph, coords);
		}else{
			break;
		}
	}
	for(int i = 0; i < graph->msize && graph->array[i].busy; i++){
		for(;;){
			fread(&status, sizeof(char), 1, file);
			if(status == 0){
				Coords coords;
				float weight;
				fread(&coords.x, sizeof(int), 1, file);
				fread(&coords.y, sizeof(int), 1, file);
				fread(&weight, sizeof(float), 1, file);
				insert_edge(graph, graph->array[i].coords, coords, weight);
			}
			else{
				break;
			}
		}
	}
}

void delete_graph(Graph* graph){
	for(int i = 0; i < graph->msize && graph->array[i].busy; i++){
		if(graph->array[i].busy == 1){
			delete_list(&graph->array[i].head);
		}
	}
	free(graph->array);
}

void show(Graph graph){
	for(int i = 0; graph.array[i].busy; i++){
		if(graph.array[i].busy != 1){
			continue;
		}
		printf("%d %d: ", graph.array[i].coords.x, graph.array[i].coords.y);
		Item* ptr = graph.array[i].head;
		while(ptr){
			printf("%d %d %f, ", graph.array[ptr->index].coords.x, graph.array[ptr->index].coords.y, ptr->weight);
			ptr = ptr->next;
		}
		printf("\n");
	}
}

void dot_show(Graph graph){
	FILE* file = fopen("graph.dot", "w");
	fprintf(file, "digraph G {graph [ dpi = 150 ];nodesep=0.3;ranksep=0.2;margin=0.1;node [shape=circle];edge [arrowsize=0.8];\n");
	for(int i = 0; i < graph.msize && graph.array[i].busy; i++){
		if(graph.array[i].busy == 1){
			for(Item* ptr = graph.array[i].head; ptr; ptr = ptr->next){
				fprintf(file, "\"%d,%d\" -> \"%d,%d\" [label = \"%f\"];\n", graph.array[i].coords.x, graph.array[i].coords.y, graph.array[ptr->index].coords.x, graph.array[ptr->index].coords.y, ptr->weight);
			}
		}
	}
	fprintf(file, "}");
	fclose(file);
}

int main(int argc, char const *argv[])
{
	Graph graph;

	init_graph(&graph);

	FILE* file = fopen("graph.bin", "rb");
	read_from_file(&graph, file);
	fclose(file);

	Coords c1  = {0,1}, c2 = {0,2}, c3 = {0,3}, c4 = {0,4}, c5 = {0,5}, c6 = {0, 6};

	/*
	insert_node(&graph,c1);
	insert_node(&graph,c2);
	insert_node(&graph,c3);
	insert_node(&graph,c4);
	insert_node(&graph,c5);
	insert_node(&graph,c6);

	insert_edge(&graph, c1, c2, 1);
	insert_edge(&graph, c1, c3, 1);
	insert_edge(&graph, c1, c4, 1);
	insert_edge(&graph, c1, c5, 1);
	insert_edge(&graph, c2, c1, 1);
	insert_edge(&graph, c3, c1, 1);
	insert_edge(&graph, c3, c4, 1);
	insert_edge(&graph, c4, c1, 1);
	insert_edge(&graph, c4, c3, 1);
	insert_edge(&graph, c4, c5, 1);
	insert_edge(&graph, c5, c1, 1);
	insert_edge(&graph, c5, c4, 1);
	insert_edge(&graph, c5, c6, 1);
	insert_edge(&graph, c6, c5, 1);
	*/

	BFSPath path = BFS(graph, c2, c6);

	if(path.distance == -1){
		printf("no node pair\n");
	}else if(path.path){
		printf("BFS %f :: ", path.distance);
		print_path(path.path, graph);
		delete_queue(&path.path);
	}else printf("no way\n");

	float dist = Deikstra(graph, c2, c6);
	if(dist == -2){
		printf("no pair\n");
	}else if(dist == FLT_MAX){
		printf("no way\n");
	}else{
		printf("DEI %f\n", dist);
	}

	show(graph);
	dot_show(graph);

	file = fopen("graph.bin", "wb");
	write_to_file(graph, file);
	fclose(file);

	delete_graph(&graph);

	return 0;
}