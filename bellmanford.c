#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

#define X_SIZE 50
#define Y_SIZE 50
#define Z_SIZE 50

#define INFINITY_UINT UINT_MAX

typedef struct {
    int x, y, z;
} Vertex;

typedef struct {
    unsigned int cost;
    Vertex direction;
} VertexInfo;

int edgeCosts[6] = {1, 1, 1, 1, 1, 1};
VertexInfo graph[X_SIZE][Y_SIZE][Z_SIZE];

void initializeGraph() {
    for (int x = 0; x < X_SIZE; x++) {
        for (int y = 0; y < Y_SIZE; y++) {
            for (int z = 0; z < Z_SIZE; z++) {
                graph[x][y][z].cost = INFINITY_UINT;
                graph[x][y][z].direction.x = -1;
                graph[x][y][z].direction.y = -1;
                graph[x][y][z].direction.z = -1;
            }
        }
    }
}

/* void setEqualEdgeCosts() {
    for (int i = 0; i < 6; i++) {
        edgeCosts[i] = 1;
    }
} */
void setRandomEdgeCosts() {
    for (int i = 0; i < 6; i++) {
        edgeCosts[i] = rand() % 10 + 1;  // Assign random costs between 1 and 10
    }
} 

void setSourceVertex(Vertex source) {
    graph[source.x][source.y][source.z].cost = 0;
}

void relax(Vertex u, Vertex v, int edgeCost) {
    if (graph[u.x][u.y][u.z].cost != INFINITY_UINT &&
        graph[u.x][u.y][u.z].cost + edgeCost < graph[v.x][v.y][v.z].cost) {
        
        graph[v.x][v.y][v.z].cost = graph[u.x][u.y][u.z].cost + edgeCost;
        graph[v.x][v.y][v.z].direction = u;

        /* printf("Relaxed: (%d, %d, %d) -> (%d, %d, %d) with cost %u\n",
               u.x, u.y, u.z, v.x, v.y, v.z, graph[v.x][v.y][v.z].cost); */
    }
}

void bellmanFord(Vertex source) {
	clock_t start_time = clock();
    
    for (int iter = 0; iter < X_SIZE * Y_SIZE * Z_SIZE - 1; iter++) {
        for (int x = 0; x < X_SIZE; x++) {
            for (int y = 0; y < Y_SIZE; y++) {
                for (int z = 0; z < Z_SIZE; z++) {
                    Vertex currentVertex = {x, y, z};

                    if (x > 0) {
                        relax(currentVertex, (Vertex){x - 1, y, z}, edgeCosts[0]);
                    }
                    if (x < X_SIZE - 1) {
                        relax(currentVertex, (Vertex){x + 1, y, z}, edgeCosts[1]);
                    }
                    if (y > 0) {
                        relax(currentVertex, (Vertex){x, y - 1, z}, edgeCosts[2]);
                    }
                    if (y < Y_SIZE - 1) {
                        relax(currentVertex, (Vertex){x, y + 1, z}, edgeCosts[3]);
                    }
                    if (z > 0) {
                        relax(currentVertex, (Vertex){x, y, z - 1}, edgeCosts[4]);
                    }
                    if (z < Z_SIZE - 1) {
                        relax(currentVertex, (Vertex){x, y, z + 1}, edgeCosts[5]);
                    }
                }
            }
        }
    }
    
    clock_t end_time = clock();
    double execution_time = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;
    printf("Execution Time: %f seconds\n", execution_time);
}

void findShortestPath(Vertex source, Vertex destination) {
    Vertex path[X_SIZE * Y_SIZE * Z_SIZE];
    int pathLength = 0;

    printf("Shortest Path from (%d, %d, %d) to (%d, %d, %d): Cost=%u, Path=",
           source.x, source.y, source.z, destination.x, destination.y, destination.z, graph[destination.x][destination.y][destination.z].cost);

    Vertex pathVertex = destination;
    while (pathVertex.x != -1) {
        path[pathLength++] = pathVertex;
        pathVertex = graph[pathVertex.x][pathVertex.y][pathVertex.z].direction;
    }

    for (int i = pathLength - 1; i >= 0; i--) {
        printf("(%d, %d, %d) ", path[i].x, path[i].y, path[i].z);
        if (i > 0) {
            printf("-> ");
        }
    }

    printf("\n");
}

int main() {
    Vertex source = {25, 25, 25};
    Vertex destination1 = {45, 31, 29};
    Vertex destination2 = {33, 48, 27};
    Vertex destination3 = {38, 35, 42};
    Vertex destination4 = {42, 34, 49};
    Vertex destination5 = {30, 26, 43};
    Vertex destination6 = {12, 12, 12};
    Vertex destination7 = {37, 37, 37};
   
  
    initializeGraph();
    //setEqualEdgeCosts();
    setRandomEdgeCosts();
    setSourceVertex(source);
    bellmanFord(source);
      //printf("Total Process Sum of Execution Time: %f seconds\n", total_execution_time);
    printf("THIS IS THE RESULT FOR ALL EGDES SET TO RANDOM NUMBER FROM 1-10 FOR SEQUENTIAL CODE");
    printf("\n");
    findShortestPath(source, destination1);
     printf("**************************************************************************************************************************************************************************\n");
    findShortestPath(source, destination2);
     printf("**************************************************************************************************************************************************************************\n");
    findShortestPath(source, destination3);
     printf("**************************************************************************************************************************************************************************\n");
    findShortestPath(source, destination4);
     printf("**************************************************************************************************************************************************************************\n");
    findShortestPath(source, destination5);
     printf("**************************************************************************************************************************************************************************\n");

      printf("\n");
    printf("**************************************************************************************************************************************************************************\n");
    printf("Cordinate (12, 12, 12 and (37,37,37)\n");
    printf("**************************************************************************************************************************************************************************\n");

    findShortestPath(source, destination6);
    printf("\n");
    findShortestPath(source, destination7);


    return 0;
}
