#include <iostream>
#include <omp.h>
#include <vector>
#include <queue>
using namespace std;

class Graph
{
private:
    int vertices;
    vector<vector<int>> adjList;

public:
    Graph(int vertices)
    {
        this->vertices = vertices;
        adjList.resize(vertices);
    }

    void addEdge(int u, int v)
    {
        adjList[u].push_back(v);
        adjList[v].push_back(u); // Undirected graph
    }

    void printGraph()
    {
        cout << "Graph adjacency list:" << endl;
        for (int i = 0; i < vertices; i++)
        {
            cout << i << " -> ";
            for (auto neighbor : adjList[i])
            {
                cout << neighbor << " ";
            }
            cout << endl;
        }
    }

    // Clean BFS
    void BFS(int start)
    {
        vector<bool> visited(vertices, false);
        queue<int> q;

        visited[start] = true;
        q.push(start);

        while (!q.empty())
        {
            int current = q.front();
            q.pop();

            cout << "Visited " << current << endl;

            // Parallelize neighbor checking
#pragma omp parallel for
            for (int i = 0; i < adjList[current].size(); i++)
            {
                int neighbor = adjList[current][i];

#pragma omp critical
                {
                    if (!visited[neighbor])
                    {
                        visited[neighbor] = true;
                        q.push(neighbor);
                    }
                }
            }
        }
    }
    // Clean DFS with OpenMP tasks
    void DFS(int start)
    {
        vector<bool> visited(vertices, false);

#pragma omp parallel
        {
#pragma omp single
            {
                DFSUtill(start, visited);
            }
        }
    }

private:
    void DFSUtil(int v, vector<bool> &visited)
    {
        bool alreadyVisited = false;

#pragma omp critical
        {
            if (visited[v])
                alreadyVisited = true;
            else
            {
                visited[v] = true;
                cout << "Visited " << v << endl;
            }
        }

        if (alreadyVisited)
            return;

#pragma omp parallel for
        for (int i = 0; i < adjList[v].size(); i++)
        {
            int neighbor = adjList[v][i];

#pragma omp task firstprivate(neighbor)
            {
                DFSUtil(neighbor, visited);
            }
        }

    }
     void DFSUtill(int v, vector<bool> &visited)
    {

#pragma omp critical
        {
            visited[v] = true;
            cout << "Thread " << omp_get_thread_num() << " visited " << v << endl;
        }

#pragma omp parallel for
        for (int i = 0; i < adjList[v].size(); i++)
        {
            int u = adjList[v][i];
            if (!visited[u])
            {
                DFSUtil(u, visited);
            }
        }
    }
};

int main()
{
    Graph g(5);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(0, 3);
    g.addEdge(2, 1);
    g.addEdge(2, 4);

    g.printGraph();

    cout << endl;
    cout << "BFS traversal starting from vertex 0: " << endl;
    g.BFS(0);

    cout << endl;
    cout << "DFS traversal starting from vertex 0: " << endl;
    g.DFS(1);

    cout << endl;
    return 0;
}




// Public DFS function
// void DFS(int start)
// {
//     vector<bool> visited(vertices, false);

//     #pragma omp parallel
//     {
//         #pragma omp single
//         {
//             DFSUtil(start, visited);
//         }
//     }
// }

// // Private DFS utility function with OpenMP tasks
// void DFSUtil(int v, vector<bool> &visited)
// {
//     bool alreadyVisited = false;

//     // Ensure only one thread marks and prints this node
//     #pragma omp critical
//     {
//         if (visited[v])
//             alreadyVisited = true;
//         else
//         {
//             visited[v] = true;
//             cout << "Visited " << v << endl;
//         }
//     }

//     if (alreadyVisited)
//         return;

//     // Spawn tasks for each neighbor
//     for (int i = 0; i < adjList[v].size(); i++)
//     {
//         int neighbor = adjList[v][i];

//         #pragma omp task firstprivate(neighbor)
//         {
//             DFSUtil(neighbor, visited);
//         }
//     }

//     // Wait for all tasks spawned by this node to complete
//     #pragma omp taskwait
// }
