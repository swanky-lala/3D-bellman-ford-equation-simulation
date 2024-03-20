#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <pthread.h>
#include <time.h>

#define X_SIZE 50
#define Y_SIZE 50
#define Z_SIZE 50

#define INFINITY_UINT INT_MAX
#define NUM_THREADS 10

typedef struct {
    int x, y, z;
} Vertex;

typedef struct {
    unsigned int cost;
    Vertex direction;
} VertexInfo;

int edgeCosts[6] = {1, 1, 1, 1, 1, 1};
VertexInfo ***graph;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_barrier;
int barrier_count = 0;
double total_execution_time = 0.0;

void custom_barrier_init() {
    pthread_cond_init(&cond_barrier, NULL);
    barrier_count = 0;
}

void custom_barrier() {
    pthread_mutex_lock(&mutex);
    barrier_count++;
    if (barrier_count == NUM_THREADS) {
        barrier_count = 0;
        pthread_cond_broadcast(&cond_barrier);
    } else {
        pthread_cond_wait(&cond_barrier, &mutex);
    }
    pthread_mutex_unlock(&mutex);
}

void initializeGraph() {
    graph = (VertexInfo ***)malloc(X_SIZE * sizeof(VertexInfo **));
    for (int x = 0; x < X_SIZE; x++) {
        graph[x] = (VertexInfo **)malloc(Y_SIZE * sizeof(VertexInfo *));
        for (int y = 0; y < Y_SIZE; y++) {
            graph[x][y] = (VertexInfo *)malloc(Z_SIZE * sizeof(VertexInfo));
            for (int z = 0; z < Z_SIZE; z++) {
                graph[x][y][z].cost = INFINITY_UINT;
                graph[x][y][z].direction.x = -1;
                graph[x][y][z].direction.y = -1;
                graph[x][y][z].direction.z = -1;
            }
        }
    }
}

void freeGraph() {
    for (int x = 0; x < X_SIZE; x++) {
        for (int y = 0; y < Y_SIZE; y++) {
            free(graph[x][y]);
        }
        free(graph[x]);
    }
    free(graph);
}

void setEqualEdgeCosts() {
    for (int i = 0; i < 6; i++) {
        edgeCosts[i] = 1;
    }
}
 
/* void setRandomEdgeCosts() {
    for (int i = 0; i < 6; i++) {
        edgeCosts[i] = rand() % 10 + 1;  // Assign random costs between 1 and 10
    }
} */

void setSourceVertex(Vertex source) {
    graph[source.x][source.y][source.z].cost = 0;
}

void relax(Vertex u, Vertex v, int edgeCost) {
    if (v.x >= 0 && v.x < X_SIZE && v.y >= 0 && v.y < Y_SIZE && v.z >= 0 && v.z < Z_SIZE) {
        if (u.x >= 0 && u.x < X_SIZE && u.y >= 0 && u.y < Y_SIZE && u.z >= 0 && u.z < Z_SIZE) {
            if (graph[u.x][u.y][u.z].cost != INFINITY_UINT &&
                graph[u.x][u.y][u.z].cost + edgeCost < graph[v.x][v.y][v.z].cost) {
                graph[v.x][v.y][v.z].cost = graph[u.x][u.y][u.z].cost + edgeCost;
                graph[v.x][v.y][v.z].direction = u;
            }
        }
    }
}

void* parallelBellmanFord(void* arg) {
    clock_t start_time = clock();
    int thread_id = *((int*)arg);
    int chunk_size = X_SIZE / NUM_THREADS;

    int start = thread_id * chunk_size;
    int end = (thread_id == NUM_THREADS - 1) ? X_SIZE : (thread_id + 1) * chunk_size;

    //int progress_interval = 20000; // Print progress every 10000 iterations

    for (int iter = 0; iter < X_SIZE * Y_SIZE * Z_SIZE - 1; iter++) {
        for (int x = start; x < end; x++) {
            for (int y = 0; y < Y_SIZE; y++) {
                for (int z = 0; z < Z_SIZE; z++) {
                    Vertex currentVertex = {x, y, z};

                    relax(currentVertex, (Vertex){x - 1, y, z}, edgeCosts[0]);
                    relax(currentVertex, (Vertex){x + 1, y, z}, edgeCosts[1]);
                    relax(currentVertex, (Vertex){x, y - 1, z}, edgeCosts[2]);
                    relax(currentVertex, (Vertex){x, y + 1, z}, edgeCosts[3]);
                    relax(currentVertex, (Vertex){x, y, z - 1}, edgeCosts[4]);
                    relax(currentVertex, (Vertex){x, y, z + 1}, edgeCosts[5]);
                }
            }
        }

        custom_barrier();
         // Print progress at regular intervals
       
       /*  if (iter % progress_interval == 0) {
            printf("Thread %d, Iteration %d/%d\n", thread_id, iter, X_SIZE * Y_SIZE * Z_SIZE - 1);
            printf("***************************************************************************\n"); 
        }
        */
    }

    clock_t end_time = clock();
    double execution_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    printf("Process Id: %d, Execution Time: %f seconds\n", thread_id, execution_time);

    pthread_mutex_lock(&mutex);
    total_execution_time += execution_time;
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

void findShortestPath(Vertex source, Vertex destination) {
    Vertex path[X_SIZE * Y_SIZE * Z_SIZE];
    int pathLength = 0;

    if (destination.x < 0 || destination.x >= X_SIZE || destination.y < 0 || destination.y >= Y_SIZE ||
        destination.z < 0 || destination.z >= Z_SIZE || graph[destination.x][destination.y][destination.z].cost == INFINITY_UINT) {
        printf("No path found to destination (%d, %d, %d).\n", destination.x, destination.y, destination.z);
        return;
    }

    printf("Shortest Path from source (%d, %d, %d) to coordinate (%d, %d, %d): Cost/Distance=%d, Path=",
           source.x, source.y, source.z, destination.x, destination.y, destination.z, graph[destination.x][destination.y][destination.z].cost);

    Vertex pathVertex = destination;
    while (pathVertex.x != -1) {
        // Check if path length is within valid range
        if (pathLength == X_SIZE * Y_SIZE * Z_SIZE) {
            printf("Error: Path length exceeds maximum size.\n");
            return;
        }

        path[pathLength++] = pathVertex;

        // Add boundary checks for pathVertex
        if (pathVertex.x < 0 || pathVertex.x >= X_SIZE || pathVertex.y < 0 || pathVertex.y >= Y_SIZE ||
            pathVertex.z < 0 || pathVertex.z >= Z_SIZE) {
            printf("Error: Path vertex out of bounds.\n");
            return;
        }

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


int main(){
    printf("Number of Thread: %d\n", NUM_THREADS);

    srand(time(NULL));
    custom_barrier_init();
    initializeGraph();
    //setRandomEdgeCosts();
    setEqualEdgeCosts();
    setSourceVertex((Vertex){0, 0, 0});

    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, parallelBellmanFord, (void*)&thread_ids[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    //printf("Total Process Sum of Execution Time: %f seconds\n", total_execution_time);
    printf("THIS IS THE RESULT FOR ALL EGDES set to 1 FOR PARALLEL CODE");
    printf("\n");
    findShortestPath((Vertex){0, 0, 0}, (Vertex){7, 7, 7});
    printf("**************************************************************************************************************************************************************************\n");
    findShortestPath((Vertex){0, 0, 0}, (Vertex){3, 9, 19});
    printf("**************************************************************************************************************************************************************************\n");
    findShortestPath((Vertex){0, 0, 0}, (Vertex){23, 40, 7});
    printf("**************************************************************************************************************************************************************************\n");
    findShortestPath((Vertex){0, 0, 0}, (Vertex){17, 13, 27});
    printf("**************************************************************************************************************************************************************************\n");
    findShortestPath((Vertex){0, 0, 0}, (Vertex){15, 30, 49});
   
    printf("\n");
    printf("**************************************************************************************************************************************************************************\n");
    printf("Cordinate (12, 12, 12 and (37,37,37)\n");
    printf("**************************************************************************************************************************************************************************\n");

    findShortestPath((Vertex){0, 0, 0}, (Vertex){12, 12, 12});
    printf("\n");
    findShortestPath((Vertex){0, 0, 0}, (Vertex){37, 37, 37});


    // Free dynamically allocated memory
    freeGraph();

    return 0;
}
