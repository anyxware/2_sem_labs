#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <float.h>
#include "task.h"
#include "graph.h"

int D_Insert_Node(Graph* graph){
	float* check;
	Coords coords;

	check = read_float("\033[1;33mx\033[0m");

	if(check){
		coords.x = *check;
		free(check);
	}
	else{
		return -1;
	}

	check = read_float("\033[1;33my\033[0m");

	if(check){
		coords.y = *check;
		free(check);
	}
	else{
		return -1;
	}

	int status = insert_node(graph,coords);
	if(status) printf("nope\n");

	return 0;
}

int D_Insert_Edge(Graph* graph){
	float* check;
	float* checkf;
	Coords coords1, coords2;
	float weight;

	check = read_float("\033[1;33mx1\033[0m");

	if(check){
		coords1.x = *check;
		free(check);
	}

	check = read_float("\033[1;33my1\033[0m");

	if(check){
		coords1.y = *check;
		free(check);
	}
	else{
		return -1;
	}

	check = read_float("\033[1;33mx2\033[0m");

	if(check){
		coords2.x = *check;
		free(check);
	}
	else{
		return -1;
	}

	check = read_float("\033[1;33my2\033[0m");

	if(check){
		coords2.y = *check;
		free(check);
	}
	else{
		return -1;
	}

	checkf = read_float("\033[1;33mweight\033[0m");

	if(checkf){
		weight = *checkf;
		free(checkf);
	}
	else{
		return -1;
	}

	int status = insert_edge(graph, coords1, coords2, weight);
	if(status) printf("nope\n");
	return 0;
}

int D_Find_BFS(Graph* graph){
	float* check;
	Coords coords1, coords2;
	
	check = read_float("\033[1;33mx1\033[0m");

	if(check){
		coords1.x = *check;
		free(check);
	}

	check = read_float("\033[1;33my1\033[0m");

	if(check){
		coords1.y = *check;
		free(check);
	}
	else{
		return -1;
	}

	check = read_float("\033[1;33mx2\033[0m");

	if(check){
		coords2.x = *check;
		free(check);
	}
	else{
		return -1;
	}

	check = read_float("\033[1;33my2\033[0m");

	if(check){
		coords2.y = *check;
		free(check);
	}
	else{
		return -1;
	}

	Route path = BFS(*graph, coords1, coords2);

	if(path.distance == -1){
		printf("no node pair\n");
	}else if(path.path){
		printf("BFS %.2f :: ", path.distance);
		print_path(path.path, *graph);
		delete_queue(&path.path);
	}else printf("no way\n");

	return 0;
}

int D_Find_Deikstra(Graph* graph){
	float* check;
	Coords coords1, coords2;
	
	check = read_float("\033[1;33mx1\033[0m");

	if(check){
		coords1.x = *check;
		free(check);
	}

	check = read_float("\033[1;33my1\033[0m");

	if(check){
		coords1.y = *check;
		free(check);
	}
	else{
		return -1;
	}

	check = read_float("\033[1;33mx2\033[0m");

	if(check){
		coords2.x = *check;
		free(check);
	}
	else{
		return -1;
	}

	check = read_float("\033[1;33my2\033[0m");

	if(check){
		coords2.y = *check;
		free(check);
	}
	else{
		return -1;
	}

	Route path = Deikstra(*graph, coords1, coords2);

	if(path.distance == -1){
		printf("no node pair\n");
	}else if(path.path){
		printf("BFS %.2f :: ", path.distance);
		print_path(path.path, *graph);
		delete_queue(&path.path);
	}else printf("no way\n");

	return 0;
}

int D_Ford_Fulk(Graph* graph){
	float* check;
	Coords coords1, coords2;
	
	check = read_float("\033[1;33mx1\033[0m");

	if(check){
		coords1.x = *check;
		free(check);
	}else{
		return -1;
	}

	check = read_float("\033[1;33my1\033[0m");

	if(check){
		coords1.y = *check;
		free(check);
	}
	else{
		return -1;
	}

	check = read_float("\033[1;33mx2\033[0m");

	if(check){
		coords2.x = *check;
		free(check);
	}
	else{
		return -1;
	}

	check = read_float("\033[1;33my2\033[0m");

	if(check){
		coords2.y = *check;
		free(check);
	}
	else{
		return -1;
	}

	Graph net = Ford_Fulk(*graph, coords1, coords2);
	if(net.msize != -1) {
		show(net);
		delete_graph(&net);
	}

	return 0;
}

int D_Delete_Node(Graph* graph){
	float* check;
	Coords coords;

	check = read_float("\033[1;33mx\033[0m");

	if(check){
		coords.x = *check;
		free(check);
	}
	else{
		return -1;
	}

	check = read_float("\033[1;33my\033[0m");

	if(check){
		coords.y = *check;
		free(check);
	}
	else{
		return -1;
	}

	int status = delete_node(graph, coords);
	if(status) printf("nope\n");
	return 0;
}

int D_Delete_Edge(Graph* graph){
	float* check;
	Coords coords1, coords2;

	check = read_float("\033[1;33mx1\033[0m");

	if(check){
		coords1.x = *check;
		free(check);
	}

	check = read_float("\033[1;33my1\033[0m");

	if(check){
		coords1.y = *check;
		free(check);
	}
	else{
		return -1;
	}

	check = read_float("\033[1;33mx2\033[0m");

	if(check){
		coords2.x = *check;
		free(check);
	}
	else{
		return -1;
	}

	check = read_float("\033[1;33my2\033[0m");

	if(check){
		coords2.y = *check;
		free(check);
	}
	else{
		return -1;
	}

	int status = delete_edge(graph, coords1, coords2);
	if(status) printf("nope\n");
	return 0;
}

int D_Show(Graph* graph){
	printf("1.show\n");
	printf("2.graphviz\n");
	int answ = read_answer(2);
	if(!answ) return -1;
	else if(answ == 1) show(*graph);
	else dot_show(*graph);
	return 0;
}

int D_Clear(Graph* graph){
	delete_graph(graph);
	init_graph(graph);
	return 0;
}

int dialogue(){
	printf("\033[1;33m--------MENU---------\033[0m\n");	
	printf("\033[3;44;33m\n");
	printf("1.Insert node\n");
	printf("2.Insert edge\n");

	printf("3.Find with BFS\n");
	printf("4.Find with Deikstra\n");
	printf("5.Residual network\n");

	printf("6.Delete Node\n");
	printf("7.Delete Edge\n");

	printf("8.Show\n");
	printf("9.Clear\n");
	printf("\033[0m\n");
	printf("(Ctrl+D if you want to exit)\n\n");
	return read_answer(10);
}

int main(int argc, char const *argv[])
{
	Graph graph;
	init_graph(&graph);
	FILE* file = fopen("graph.bin", "rb");
	read_from_file(&graph, file);
	fclose(file);

	int answ;
	int done;

	int (*funcs[10])(Graph* graph) = {NULL, &D_Insert_Node, &D_Insert_Edge, &D_Find_BFS, &D_Find_Deikstra, &D_Ford_Fulk, &D_Delete_Node, &D_Delete_Edge, &D_Show, &D_Clear};

	while(answ = dialogue()){
		if(funcs[answ]){
			done = funcs[answ](&graph);
			if(done < 0)
				break;
		}
	}

	file = fopen("graph.bin", "wb");
	write_to_file(graph, file);
	fclose(file);

	delete_graph(&graph);

	return 0;
}