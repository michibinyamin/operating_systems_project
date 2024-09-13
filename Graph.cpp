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
        vector<vector<int>> edges;

        bool condition = false;

        bool New_graph(int v, int m){
            char buffer[BUFFER_SIZE];
            int valread;
            string msg = "Please enter the edges: (for example- 1 2)\n";
            write(current_client_fd,msg.c_str(),msg.size());
            //cout << "Please enter the edges: (for example- 1 2)\n"; // Make it print to client!!
            for (int i = 0; i < m; ++i) {
                int src, dest;
                valread = read(current_client_fd, buffer, BUFFER_SIZE);
                buffer[valread] = '\0';
                istringstream iss(buffer);
                // Extract the two integers
                iss >> src >> dest;
                if(dest < 1 || dest > v || src < 1 || src > v)
                {
                    return false;
                }
                edges.push_back({src, dest});
            }
            return true;
        }

        bool Removeedge(int v, int m){
            for (auto it = edges.begin(); it != edges.end(); ++it) {
                if ((*it)[0] == v && (*it)[1] == m) {
                    edges.erase(it);
                    return true;
                }
            }
            return false;
        }


    public :
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
                edges.clear();
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
                string numbers_part = command.substr(8);
                istringstream iss(numbers_part);
                char comma;
                // Read the numbers separated by comma
                iss >> v >> comma >> m;
                //size++;
                edges.push_back({v, m});
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

        void PrintEdges(){
            cout << "Graph edges are:\n";
            for (size_t i = 0; i < edges.size(); i++)
            {
                cout << edges.at(i)[0];
                cout << ",";
                cout << edges.at(i)[1];
                cout << " ";
            }
            cout << "\n";
        }
};