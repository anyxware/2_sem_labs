#ifndef GRAPH_H
#define GRAPH_H

typedef struct{
	int x, y;
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
}BFSPath;

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

#endif // GRAPH_H