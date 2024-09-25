#pragma once
//#include "Graph.cpp"
#include "MSTAlgorithm.hpp"

// Total weight of the MST
void Total_weight(Graph Tree, int client_fd);

// Longest distance between two vertices
void Longest_distance(Graph Tree, int client_fd);

// Average distance between and two edges n the graph.
//  ○ assume distance (x,x)=0 for any X
//  ○ We are interested in avg of all distances Xi,Xj where i=1..n j≥i.
void Average_distance(Graph Tree, int client_fd);

// Shortest distance between two vertices Xi,Xj where i≠j and edge belongs to MST
void Shortest_distance(Graph Tree, int client_fd);