# 3D-bellman-ford-equation-simulation
This is a simulation of Bellman ford Equation parallelization using 3D graph. 

Bellman-Ford Shortest Path Algorithm

This repository contains both serial and parallel implementations of the Bellman-Ford algorithm to find the shortest path from a specified source vertex to multiple destination vertices in a 3D grid graph.

Serial Implementation

Introduction
The serial implementation (shortest_path.c) of the Bellman-Ford algorithm works as follows:

Initialization: The graph is initialized with all distances set to infinity and the source vertex distance set to 0.
Main Loop: The main loop runs for X_SIZE * Y_SIZE * Z_SIZE - 1 iterations. Within each iteration:
Each vertex is relaxed, meaning its distance is updated if a shorter path is found via one of its neighbors.
Relaxation: For each edge (u, v) with weight w, if the distance to u plus the weight w is less than the current distance to v, update the distance to v to the new shorter distance.
Path Reconstruction: After the main loop, the shortest paths from the source vertex to all other vertices are reconstructed.
How to Use
To use the serial implementation:

Compile the Program: Compile the shortest_path.c file using a C compiler. For example:
Copy code
```
gcc -o shortest_path_serial shortest_path.c
```
Run the Program: Execute the compiled program:
bash
Copy code
```
./shortest_path_serial
```
The program will display the execution time and the shortest paths from the source vertex to various destination vertices.

Parallel Implementation

Introduction
The parallel implementation (parallel_shortest_path.c) of the Bellman-Ford algorithm utilizes multiple threads to speed up the computation process. It divides the workload among threads, with each thread processing a portion of the graph.

How to Use
To use the parallel implementation:

Compile the Program: Compile the parallel_shortest_path.c file using a C compiler. For example:
Copy code
```
gcc -o parallel_shortest_path parallel_shortest_path.c -lpthread
```
Run the Program: Execute the compiled program:
bash
Copy code
```
./parallel_shortest_path
```
The program will display the execution time and the shortest paths from the source vertex to various destination vertices, utilizing parallel computation.

Algorithm Explanation

Both implementations of the Bellman-Ford algorithm share the same underlying algorithm explained above. The only difference is the parallel implementation utilizes multiple threads to process the graph concurrently, improving performance.

Performance

The performance of the parallel implementation is expected to be better than the serial implementation, especially for large graphs, as it utilizes multiple threads to process the graph concurrently.

License

This program is provided under the MIT License. You are free to use, modify, and distribute the code for any purpose, with or without attribution.
