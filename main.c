#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct node {
    int first;
    int second;
    struct node *next;
} Node;

typedef struct graph {
    int nodes_num;
    int** matrix;
} Graph;

bool* visited;

int *new_node(int first, int second) {
    //creates new node
    Node *new_node = (Node*)malloc(sizeof(Node));
    new_node -> first = first;
    new_node -> second = second;
    new_node -> next = NULL;
    return new_node;
}

Node *add_node(Node *cur_node, int first, int second) {
    //adds node to the list of edges
    Node *new_node_t = new_node(first, second);
    new_node_t -> next = cur_node;
    return new_node_t;
}

void del_nodes(Node *cur_node) {
    //delete nodes
    if (cur_node == NULL)
        return;
    del_nodes(cur_node -> next);
    free(cur_node);
}

void print_rec(Node *cur_node) {
    //prints list of edges
    if (cur_node == NULL)
        return;
    print_rec(cur_node->next);
    printf("%d -- %d\n", cur_node -> first, cur_node -> second);
}

void print_to_file_rec(Node *cur_node, FILE *file){
    //prints list of edges to the dot file
    if (cur_node == NULL)
        return;
    print_to_file_rec(cur_node->next, file);
    fprintf(file, "%d -- %d\n", cur_node -> first, cur_node -> second);
}


void make_list(Node *list){
    //creates list of edges
    FILE *dot_file = fopen("..\\forgraph.dot", "w");
    fputs("graph {\n", dot_file);
    print_to_file_rec(list, dot_file);
    fputs("}", dot_file);
    fclose(dot_file);
}

int find_max_node(Node *list){
    //finds the max value of nodes
    int num_of_nodes = 0;
    while (list != NULL) {
        if (list -> first > num_of_nodes)
            num_of_nodes = list -> first;
        if (list -> second > num_of_nodes)
            num_of_nodes = list -> second;
        list = list -> next;
    }
    return num_of_nodes;
}

Graph *initialize_matrix(int num_nodes) {
    Graph *new_graph = malloc(num_nodes * sizeof(Node));
    new_graph -> matrix = (int**)malloc( num_nodes * sizeof(int*));
    for(int i = 0; i < num_nodes; i++) {
        new_graph -> matrix[i] = (int*)calloc( num_nodes, sizeof(int));
    }
    new_graph -> nodes_num = num_nodes;
    return new_graph;
}

void create_matrix(Graph* graph, Node *list) {
    //creates incidence matrix
    while (list != NULL) {
        int first = list -> first;
        int second = list -> second;
        graph -> matrix[first - 1][second - 1] = 1;
        graph -> matrix[second - 1][first - 1] = 1;
        list = list -> next;
    }
}

void print_matrix(Graph* graph){
    int nodes = graph -> nodes_num;
    int **matrix = graph -> matrix;
    for (int i = 0; i < nodes; i++){
        for (int j = 0; j < nodes; j++){
            printf("%d ", matrix[i][j]);
            if (j == nodes - 1){
                puts("");
            }
        }
    }
}

void DFS(int i, Graph* graph) {
    visited[i] = true;
    for (int j = 0; j < graph -> nodes_num; j++) {
        if (graph -> matrix[i][j] != 0 && !visited[j]) {
            DFS(j, graph);
        }
    }
}

bool isConnected(Graph *graph, int node) {
    //checks the graph for connectivity
    visited = malloc(node * sizeof(bool));
    for (int i = 0; i < graph -> nodes_num; i++) {
        visited[i] = false;
    }
    DFS(0, graph);
    for (int i = 0; i < graph -> nodes_num; i++) {
        if (!visited[i]) {
            return false;
        }
    }
    free(visited);
    return true;
}

int main() {
    FILE *graph_file = fopen("..\\graph.txt", "r");
    //open file with list of edges
    int from, to;
    Node *list = NULL;
    while(fscanf(graph_file, "%d %d", &from, &to) != EOF){
        list = add_node(list, from, to);
    }
    //puts("List of edges:");
    //print_rec(list);
    make_list(list); //making a singly linked list of edges
    fclose(graph_file);
    system("dot -Tpng -O ..\\forgraph.dot");

    Graph *graph = NULL;
    int nodes = find_max_node(list);
    graph = initialize_matrix(nodes);
    create_matrix(graph, list);

    //print_matrix(graph);

    if (isConnected(graph, 0)){
        puts("Graph is connected");
    } else {
        puts("Graph is NOT connected");
    }

    del_nodes(list);
    free(graph);
    return 0;
}

