#include "Functions.hpp"
using namespace std;

void Total_weight(Graph Tree, int client_fd){
    int sum = 0;
    for (size_t i = 0; i < Tree.getAdjacencyMatrix().size(); i++)
    {
        for (size_t j = i; j < Tree.getAdjacencyMatrix().size(); j++)
        {
            if(Tree.getAdjacencyMatrix()[i][j] != -1)
                sum = sum + Tree.getAdjacencyMatrix()[i][j];
        }
    }
    string msg = "The Total weight of the MST is: " + to_string(sum) + "\n";
    write(client_fd, msg.c_str(), msg.size());
}

// Function to perform DFS and find the longest path
void DFS(const vector<vector<int>>& adjMatrix, int node, vector<bool>& visited, int currentDistance, int& maxDistance) {
    visited[node] = true;

    for (int neighbor = 0; neighbor < adjMatrix.size(); neighbor++) {
        if (adjMatrix[node][neighbor] != -1 && !visited[neighbor]) { // Check for an edge and unvisited node
            DFS(adjMatrix, neighbor, visited, currentDistance + adjMatrix[node][neighbor], maxDistance);
        }
    }

    // Update the maximum distance found
    if (currentDistance > maxDistance) {
        maxDistance = currentDistance;
    }

    visited[node] = false; // Backtrack
}

void Longest_distance(Graph Tree, int client_fd) {
    int maxDistance = numeric_limits<int>::min();
    vector<bool> visited(Tree.getAdjacencyMatrix().size(), false);

    for (int i = 0; i < Tree.getAdjacencyMatrix().size(); i++) {
        DFS(Tree.getAdjacencyMatrix(), i, visited, 0, maxDistance);
    }
    string msg = "Longest distance between two vertices is: " + to_string(maxDistance) + "\n";
    write(client_fd, msg.c_str(), msg.size());
}

// Function to perform BFS and find shortest paths from a given start node
void BFS(const vector<vector<int>>& adjMatrix, int start, vector<int>& distances) {
    vector<bool> visited(adjMatrix.size(), false);
    vector<int> queue;
    
    visited[start] = true;
    queue.push_back(start);
    distances[start] = 0;

    while (!queue.empty()) {
        int node = queue.front();
        queue.erase(queue.begin());

        for (int neighbor = 0; neighbor < adjMatrix.size(); neighbor++) {
            if (adjMatrix[node][neighbor] != -1 && !visited[neighbor]) { // Check for an edge
                visited[neighbor] = true;
                distances[neighbor] = distances[node] + adjMatrix[node][neighbor];
                queue.push_back(neighbor);
            }
        }
    }
}

void Average_distance(Graph Tree, int client_fd) {
    double totalDistance = 0;
    int count = 0;
    
    for (int i = 0; i < Tree.getAdjacencyMatrix().size(); i++) {
        vector<int> distances(Tree.getAdjacencyMatrix().size(), numeric_limits<int>::max());
        BFS(Tree.getAdjacencyMatrix(), i, distances);
        
        for (int j = 0; j < distances.size(); j++) {
            if (i != j && distances[j] != numeric_limits<int>::max()) { // Ignore self-distance
                totalDistance += distances[j];
                count++;
            }
        }
    }

    // Calculate the average distance
    double average = count > 0 ? totalDistance / count : 0.0;
    string msg = "The average distance between vertecies in the graph is: " + to_string(average) + "\n";
    write(client_fd, msg.c_str(), msg.size());
}


// Shortest distance between two vertices Xi,Xj where i≠j and edge belongs to MST
void Shortest_distance(Graph Tree, int client_fd){
    int minimul = numeric_limits<int>::max();
    for (size_t i = 0; i < Tree.getAdjacencyMatrix().size(); i++)
    {
        for (size_t j = 0; j < Tree.getAdjacencyMatrix().size(); j++)
        {
            if(Tree.getAdjacencyMatrix()[i][j] != -1){
                if(Tree.getAdjacencyMatrix()[i][j] < minimul)
                    minimul = Tree.getAdjacencyMatrix()[i][j];
            }
        }
    }
    string msg = "The shortest distance between two vertices is: "+ to_string(minimul) + "\n";
    write(client_fd, msg.c_str(), msg.size());
}