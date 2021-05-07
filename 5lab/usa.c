#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
#include "task.h"

#define NODES_NUM 21048
#define EDGES_NUM 21693

/*
int main(int argc, char const *argv[])
{
	Graph Cali;
	init_graph(&Cali);

	FILE* nodes = fopen("nodes.txt", "r");
	FILE* edges = fopen("edges.txt", "r");

	int d;
	int index1, index2;
	float x, y;
	Coords coords1, coords2;
	float weight;

	for(int i = 0; i < NODES_NUM; i++){
		fscanf(nodes, "%d %f %f", &d, &coords1.x, &coords1.y);
		insert_node(&Cali, coords1);
	}
	for(int i = 0; i < EDGES_NUM; i++){
		fscanf(edges, "%d %d %d %f", &d, &index1, &index2, &weight);
		insert_edge(&Cali, Cali.array[index1].coords, Cali.array[index2].coords, weight);
		insert_edge(&Cali, Cali.array[index2].coords, Cali.array[index1].coords, weight);
	}

	FILE* usa = fopen("usa.bin", "w");
	write_to_file(Cali, usa);
	fclose(nodes);
	fclose(edges);
	fclose(usa);

	//dot_show(Cali);
	return 0;
}
*/

void print_path_usa(QItem* path, Graph graph){
	int j = 0;
	FILE* file = fopen("path.dot", "w");
	for(QItem* ptr = path; ptr; ptr = ptr->next){
		j++;
		fprintf(file, "%.2f %.2f\n", graph.array[ptr->index].coords.x, graph.array[ptr->index].coords.y);
		//printf("%d %.2f %.2f, \n", j++, graph.array[ptr->index].coords.x, graph.array[ptr->index].coords.y);
	}
	fclose(file);
	printf("%d\n", j);
	printf("\n");
}

int D_Find_BFS(Graph* graph){
	int* check;
	int index1, index2;
	
	check = read_int("\033[1;33mx1\033[0m");

	if(check){
		index1 = *check;
		free(check);
	}

	check = read_int("\033[1;33my1\033[0m");

	if(check){
		index2 = *check;
		free(check);
	}
	else{
		return -1;
	}

	Route path = BFS(*graph, graph->array[index1].coords, graph->array[index2].coords);

	if(path.distance == -1){
		printf("no node pair\n");
	}else if(path.path){
		printf("BFS %.2f :: \n", path.distance);
		print_path_usa(path.path, *graph);
		delete_queue(&path.path);
	}else printf("no way\n");

	return 0;
}

int D_Find_Deikstra(Graph* graph){
	int* check;
	int index1, index2;
	
	check = read_int("\033[1;33mx1\033[0m");

	if(check){
		index1 = *check;
		free(check);
	}

	check = read_int("\033[1;33my1\033[0m");

	if(check){
		index2 = *check;
		free(check);
	}
	else{
		return -1;
	}

	Route path = Deikstra(*graph, graph->array[index1].coords, graph->array[index2].coords);

	if(path.distance == -1){
		printf("no node pair\n");
	}else if(path.path){
		printf("DEI %.2f :: \n", path.distance);
		print_path_usa(path.path, *graph);
		delete_queue(&path.path);
	}else printf("no way\n");

	return 0;
}

int D_Ford_Fulk(Graph* graph){
	int* check;
	int index1, index2;
	
	check = read_int("\033[1;33mx1\033[0m");

	if(check){
		index1 = *check;
		free(check);
	}

	check = read_int("\033[1;33my1\033[0m");

	if(check){
		index2 = *check;
		free(check);
	}
	else{
		return -1;
	}

	Graph net = Ford_Fulk(*graph, graph->array[index1].coords, graph->array[index2].coords);
	if(net.msize != -1) {
		show(net);
		delete_graph(&net);
	}

	return 0;
}

int dialogue(){
	printf("\033[1;33m--------MENU---------\033[0m\n");	
	printf("\033[3;44;33m\n");

	printf("1.Find with BFS\n");
	printf("2.Find with Deikstra\n");
	printf("3.Residual network\n");

	printf("\033[0m\n");
	printf("(Ctrl+D if you want to exit)\n\n");
	return read_answer(4);
}

void py_show(Graph graph){
	FILE* file = fopen("line.py", "w");
	fprintf(file, "import matplotlib.pyplot as plt\n");
	for(int i = 0; i < graph.msize /100 && graph.array[i].busy; i++){
		for(Item* ptr = graph.array[i].head; ptr; ptr = ptr->next){
			fprintf(file, "plt.plot([%f,%f],[%f,%f])\n", 1000*graph.array[i].coords.x, 1000*graph.array[i].coords.y, 1000*graph.array[ptr->index].coords.x, 1000*graph.array[ptr->index].coords.y);
		}
	}
	fprintf(file, "plt.show()");
	fclose(file);
}

void dot_show12(Graph graph){
	FILE* file = fopen("graph.dot", "w");
	fprintf(file, "digraph G {node [shape=point];\n");
	for(int i = 0; i < graph.msize && graph.array[i].busy; i++){
		fprintf(file, "\"%d\" [pos=\"%.2f,%.2f!\"];\n", i, graph.array[i].coords.x, graph.array[i].coords.y);
	}
	for(int i = 0; i < graph.msize && graph.array[i].busy; i++){
		for(Item* ptr = graph.array[i].head; ptr; ptr = ptr->next){
			//fprintf(file, "\"%d\" -> \"%d\";\n", i, ptr->index);
		}
	}
	fprintf(file, "}");
	fclose(file);
}

int main(int argc, char const *argv[])
{
	Graph cali;
	init_graph(&cali);

	FILE* usa = fopen("usa.bin", "rb");
	read_from_file(&cali, usa);
	fclose(usa);

	int answ;
	int done;

	int (*funcs[4])(Graph* graph) = {NULL, &D_Find_BFS, &D_Find_Deikstra, &D_Ford_Fulk};

	while(answ = dialogue()){
		if(funcs[answ]){
			done = funcs[answ](&cali);//funcs[answ](&table);
			if(done < 0)
				break;
		}
	}
	//dot_show12(cali);

	delete_graph(&cali);

	return 0;
}