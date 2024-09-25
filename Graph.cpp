#include <bits/stdc++.h>
#include <iostream>
#include <string>
#include <typeinfo>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

using namespace std;

#define BUFFER_SIZE 1024

// Declare the variables with extern
extern pthread_cond_t cond;
extern pthread_mutex_t pmutex;
extern bool over_fifty;

class Graph{
    private :
        int size;
        int current_client_fd;
        vector<vector<int>> adjmatrix;

        bool New_graph(int v, int m){   // V - amount of vertecies, M - amount of edges
            size = v;
            adjmatrix.resize(size, vector<int>(size, -1));

            char buffer[BUFFER_SIZE];
            int valread;
            string msg = "Please enter the edges: (for example- 0 1)\n";
            write(current_client_fd,msg.c_str(),msg.size());
            //cout << "Please enter the edges: (for example- 1 2)\n"; // Make it print to client!!
            for (int i = 0; i < m; ++i) {
                if(i != 0){
                    msg = "next edge: (for example- 1 2)\n";
                    write(current_client_fd,msg.c_str(),msg.size());
                }        
                int src, dest, weight;
                valread = read(current_client_fd, buffer, BUFFER_SIZE);
                buffer[valread] = '\0';
                istringstream iss(buffer);
                // Extract the two integers
                iss >> src >> dest;
                if(dest < 0 || dest >= v || src < 0 || src >= v)
                {
                    return false;
                }
                msg = "Please enter the weight you want: ";
                write(current_client_fd,msg.c_str(),msg.size());
                int value;
                valread = read(current_client_fd, buffer, BUFFER_SIZE);
                if (valread > 0) {
                    // Null-terminate the buffer to ensure it's a valid string
                    buffer[valread] = '\0';
                    // Convert the buffer to an integer
                    value = atoi(buffer);
                    printf("The integer value is: %d\n", value);
                }

                adjmatrix[src][dest] = value;
                adjmatrix[dest][src] = value;
            }
            return true;
        }

        bool Removeedge(int v, int m){  // V - first vertex, M - second vertex

            // Remove from the matrics (turn to -1)
            if(adjmatrix[v][m] != -1){
                adjmatrix[v][m] = -1;
                adjmatrix[m][v] = -1;
                return true;
            }
            return false;
        }


    public :
        void New_graph(vector<vector<int>> matrix){
            size = matrix.size();
            adjmatrix = matrix;
        }

        void Command_handle(int client, string command){
            current_client_fd = client;
            int v = 0;
            int m = 0;

            if (command.find("Newgraph") == 0) {
                string numbers_part = command.substr(9);
                istringstream iss(numbers_part);
                char comma;
                // Read the numbers separated by comma
                iss >> v >> comma >> m;
                size = v;
                adjmatrix.clear();
                if(New_graph(v,m)){
                    string success_msg = "New graph created with " + to_string(v) + " vertices and " + to_string(m) + " edges.\n";
                    write(current_client_fd, success_msg.c_str(), success_msg.size());
                    //cout << "New graph created with " << v << " vertices and " << m << " edges.\n"; // Make it print to client!!
                }
                else{
                    string error_msg = "Error: Invalid edge input.\n";
                    write(current_client_fd, error_msg.c_str(), error_msg.size());
                    //cout << "error\n";
                }

            }else if(command.find("Newedge") == 0){
                char buffer[BUFFER_SIZE];
                string numbers_part = command.substr(8);
                istringstream iss(numbers_part);
                char comma;
                // Read the numbers separated by comma
                iss >> v >> comma >> m;
                string success_msg2 = "Please enter the weight you want: ";
                write(current_client_fd, success_msg2.c_str(), success_msg2.size());
                int value;
                int valread = read(current_client_fd, buffer, BUFFER_SIZE);
                if (valread > 0) {
                    // Null-terminate the buffer to ensure it's a valid string
                    buffer[valread] = '\0';
                    // Convert the buffer to an integer
                    value = atoi(buffer);
                    printf("The integer value is: %d\n", value);
                }

                adjmatrix[v][m] = value;
                adjmatrix[m][v] = value;

                string success_msg = "Edge (" + to_string(v) + " -> " + to_string(m) + ") added.\n";
                write(current_client_fd, success_msg.c_str(), success_msg.size());
                //cout << "Edge (" << v << " -> " << m << ") added.\n";   // Make it print to client!!                      
            }else if(command.find("Removeedge") == 0){
                string numbers_part = command.substr(11);
                istringstream iss(numbers_part);
                char comma;
                // Read the numbers separated by comma  
                iss >> v >> comma >> m;
                if(Removeedge(v,m)){
                    string success_msg = "Edge (" + to_string(v) + " -> " + to_string(m) + ") removed.\n";
                    write(current_client_fd, success_msg.c_str(), success_msg.size());
                    //cout << "Edge (" << v << " -> " << m << ") removed.\n"; // Make it print to client!!
                }
                else{
                    string error_msg = "No such edge found.\n";
                    write(current_client_fd, error_msg.c_str(), error_msg.size());
                    //cout << "No such edge";
                }
            }else {
                string error_msg = "Invalid command.\n";
                write(current_client_fd, error_msg.c_str(), error_msg.size());
                //cout << "Invalid command.\n";
            }
        }

        vector<vector<int>> getAdjacencyMatrix() const{
            return adjmatrix;
        }

        int getEdgeWeight(int u, int v) const{
            return adjmatrix[u][v];
        }

        // Print the adjacency matrix
        void printGraph() const {
            for (int i = 0; i < size; ++i) {
                for (int j = 0; j < size; ++j) {
                    std::cout << adjmatrix[i][j] << " ";
                }
                std::cout << std::endl;
            }
        }

};