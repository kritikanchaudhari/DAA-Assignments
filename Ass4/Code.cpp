#include <bits/stdc++.h>
using namespace std;

// Dijkstra's Algorithm
void dijkstra(
    int source,
    const vector<vector<pair<int, int>>> &graph,
    vector<int> &dist,
    vector<int> &parent
) {
    int V = graph.size();
    dist.assign(V, INT_MAX);
    parent.assign(V, -1);
    priority_queue<
        pair<int, int>,
        vector<pair<int, int>>,
        greater<pair<int, int>>
    > pq;
    dist[source] = 0;
    pq.push({0, source});
    while (!pq.empty()) {
        int d = pq.top().first;
        int u = pq.top().second;
        pq.pop();
        // Ignore outdated entry
        if (d > dist[u])
            continue;
        for (auto edge : graph[u]) {
            int v = edge.first;
            int w = edge.second;
            if (dist[u] != INT_MAX &&
                dist[v] > dist[u] + w) {
                dist[v] = dist[u] + w;
                parent[v] = u;
                pq.push({dist[v], v});
            }
        }
    }
}

// Display shortest path
void printPath(
    int source,
    int destination,
    const vector<int> &parent
) {
    vector<int> path;
    int current = destination;
    while (current != -1) {
        path.push_back(current);
        current = parent[current];
    }
    reverse(path.begin(), path.end());
    if (path[0] != source) {
        cout << "No path exists.\n";
        return;
    }
    cout << "Optimal Route: ";
    for (int i = 0; i < path.size(); i++) {
        cout << path[i];
        if (i != path.size() - 1)
            cout << " -> ";
    }
    cout << endl;
}

// Update traffic weight
void updateTraffic(
    vector<vector<pair<int, int>>> &graph,
    int u,
    int v,
    int newWeight
) {
    bool found = false;
    // Update u -> v
    for (auto &edge : graph[u]) {
        if (edge.first == v) {
            edge.second = newWeight;
            found = true;
        }
    }
    // Update v -> u because road is undirected
    for (auto &edge : graph[v]) {
        if (edge.first == u) {
            edge.second = newWeight;
        }
    }
    if (!found) {
        cout << "Road not found!\n";
    }
    else {
        cout << "Traffic updated successfully.\n";
    }
}

int main() {
    int V, E;
    cout << " SMART TRAFFIC MANAGEMENT SYSTEM\n";
    cout << " Emergency Vehicle Route Optimization\n";
    cout << endl;

    // Number of intersections
    cout << "Enter number of intersections (vertices): ";
    cin >> V;

    // Number of roads
    cout << "Enter number of roads (edges): ";
    cin >> E;

    vector<vector<pair<int, int>>> graph(V);

    cout << "Enter road details:\n";
    cout << "Format: Start_Node End_Node Travel_Time\n";
    cout << endl;

    for (int i = 0; i < E; i++) {
        int u, v, w;
        cout << "Road " << i + 1 << ": ";
        cin >> u >> v >> w;
        graph[u].push_back({v, w});
        graph[v].push_back({u, w});
    }

    // Ambulance source
    int source;
    cout << "Enter ambulance starting location: ";
    cin >> source;

    // Hospitals
    int H;
    cout << "Enter number of hospitals: ";
    cin >> H;

    vector<int> hospitals(H);
    cout << "Enter hospital node numbers:\n";

    for (int i = 0; i < H; i++) {
        cin >> hospitals[i];
    }

    // Run Dijkstra
    vector<int> dist;
    vector<int> parent;
    dijkstra(source, graph, dist, parent);

    // Find nearest hospital
    int nearestHospital = -1;
    int minTime = INT_MAX;

    cout << endl;
    cout << " INITIAL HOSPITAL DISTANCES\n";
    cout << endl;

    for (int h : hospitals) {
        if (dist[h] == INT_MAX) {
            cout << "Hospital " << h
                 << " : Not Reachable\n";
        }
        else {
            cout << "Hospital " << h
                 << " : " << dist[h]
                 << " minutes\n";
            if (dist[h] < minTime) {
                minTime = dist[h];
                nearestHospital = h;
            }
        }
    }

    // Display nearest hospital
    if (nearestHospital == -1) {
        cout << "No hospital is reachable.\n";
        return 0;
    }

    cout << endl;
    cout << " BEST HOSPITAL\n";
    cout << endl;

    cout << "Nearest Hospital : "
         << nearestHospital << endl;

    cout << "Minimum Travel Time : "
         << minTime << " minutes\n";

    printPath(source, nearestHospital, parent);

    // Dynamic traffic update
    char choice;
    cout << "Do you want to update traffic conditions? (y/n): ";
    cin >> choice;

    while (choice == 'y' || choice == 'Y') {
        int u, v, newWeight;

        cout << "Enter road whose traffic has changed:\n";
        cout << "Start node: ";
        cin >> u;

        cout << "End node: ";
        cin >> v;

        cout << "Enter new travel time: ";
        cin >> newWeight;

        updateTraffic(graph, u, v, newWeight);

        // Recalculate shortest paths
        dijkstra(source, graph, dist, parent);

        // Find new nearest hospital
        nearestHospital = -1;
        minTime = INT_MAX;

        cout << endl;
        cout << " UPDATED HOSPITAL DISTANCES\n";
        cout << endl;

        for (int h : hospitals) {
            if (dist[h] == INT_MAX) {
                cout << "Hospital " << h
                     << " : Not Reachable\n";
            }
            else {
                cout << "Hospital " << h
                     << " : "
                     << dist[h]
                     << " minutes\n";

                if (dist[h] < minTime) {
                    minTime = dist[h];
                    nearestHospital = h;
                }
            }
        }

        cout << endl;
        cout << " UPDATED OPTIMAL ROUTE\n";
        cout << endl;

        if (nearestHospital != -1) {
            cout << "Nearest Hospital : "
                 << nearestHospital << endl;

            cout << "Minimum Travel Time : "
                 << minTime
                 << " minutes\n";

            printPath(source, nearestHospital, parent);
        }
        else {
            cout << "No hospital is reachable.\n";
        }

        cout << "Do you want to update traffic again? (y/n): ";
        cin >> choice;
    }

    cout << endl;
    cout << " SYSTEM FINISHED\n";
    cout << endl;

    return 0;
}
