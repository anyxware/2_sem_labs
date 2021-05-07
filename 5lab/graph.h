#ifndef GRAPH_H
#define GRAPH_H

typedef struct{
	float x, y;
}Coords;

typedef struct Item{
	int index;
	float weight;
	struct Item* next;
}Item;

typedef struct QItem{
	int index;
	struct QItem* next;
}QItem;

typedef struct{
	float distance;
	QItem* path;
}Route;

typedef struct{
	float cap;
	float flow;
}FF;

typedef struct{
	int index1;
	int index2;
}Edge;

typedef struct{
	int busy;
	Item* head;
	Coords coords;
}Node;

typedef struct{
	Node* array;
	unsigned int capacity;
	unsigned int msize;
}Graph;

void init_graph(Graph* graph);
int insert_node(Graph* graph, Coords coords);
int insert_edge(Graph* graph, Coords coords1, Coords coords2, float weight);
int delete_node(Graph* graph, Coords coords);
int delete_edge(Graph* graph, Coords coords1, Coords coords2);
void print_path(QItem* path, Graph graph);
Route BFS(Graph graph, Coords coords1, Coords coords2);
void delete_queue(QItem** queue);
Route Deikstra(Graph graph, Coords coords1, Coords coords2);
Graph Ford_Fulk(Graph graph, Coords coords1, Coords coords2);
void write_to_file(Graph graph, FILE* file);
void read_from_file(Graph* graph, FILE* file);
void delete_graph(Graph* graph);
void show(Graph graph);
void dot_show(Graph graph);

#endif // GRAPH_H