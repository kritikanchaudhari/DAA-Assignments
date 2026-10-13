#include <bits/stdc++.h>
using namespace std;

struct Edge {
    int from, to;
    double base_cost, cur_cost;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int S;
    cout << "Enter number of stages: ";
    cin >> S;

    if (S < 2) {
        cout << "Number of stages must be at least 2.\n";
        return 0;
    }

    vector<int> stage_count(S);
    vector<int> stage_start(S);
    vector<int> node_stage;
    int N = 0;

    cout << "Enter number of nodes in each stage: ";
    for (int i = 0; i < S; i++) {
        cin >> stage_count[i];

        if (stage_count[i] <= 0) {
            cout << "Each stage must contain at least one node.\n";
            return 0;
        }

        stage_start[i] = N;
        N += stage_count[i];

        for (int j = 0; j < stage_count[i]; j++) {
            node_stage.push_back(i);
        }
    }

    int M;
    cout << "Enter number of edges: ";
    cin >> M;

    if (M < 0) {
        cout << "Invalid number of edges.\n";
        return 0;
    }

    vector<Edge> edges;
    vector<vector<int>> adj(N);

    cout << "Enter each edge as: u v cost\n";

    for (int i = 0; i < M; i++) {
        int u, v;
        double cost;
        cin >> u >> v >> cost;

        if (u < 0 || u >= N || v < 0 || v >= N ||
            node_stage[v] != node_stage[u] + 1 ||
            !isfinite(cost) || cost < 0) {
            cout << "Invalid edge. Edges must connect consecutive stages with non-negative costs.\n";
            return 0;
        }

        edges.push_back({u, v, cost, cost});
        adj[u].push_back(i);
    }

    const double INF = numeric_limits<double>::infinity();

    vector<double> best_cost(N, INF);
    vector<int> next_edge(N, -1);

    auto calculateDP = [&]() {
        fill(best_cost.begin(), best_cost.end(), INF);
        fill(next_edge.begin(), next_edge.end(), -1);

        int last_stage = S - 1;

        for (int i = 0; i < stage_count[last_stage]; i++) {
            int node = stage_start[last_stage] + i;
            best_cost[node] = 0.0;
        }

        for (int st = S - 2; st >= 0; st--) {
            for (int k = 0; k < stage_count[st]; k++) {
                int u = stage_start[st] + k;

                for (int id : adj[u]) {
                    const Edge &e = edges[id];

                    if (!isfinite(best_cost[e.to])) {
                        continue;
                    }

                    double candidate = e.cur_cost + best_cost[e.to];

                    if (candidate < best_cost[u]) {
                        best_cost[u] = candidate;
                        next_edge[u] = id;
                    }
                }
            }
        }
    };

    auto printRoute = [&](int src) {
        if (src < 0 || src >= N || node_stage[src] != 0) {
            cout << "Invalid source node. Enter a node from Stage 0.\n";
            return;
        }

        if (!isfinite(best_cost[src])) {
            cout << "No route available from node " << src << ".\n";
            return;
        }

        int cur = src;
        double total = 0.0;

        cout << "Route: " << cur;

        while (next_edge[cur] != -1) {
            int id = next_edge[cur];
            const Edge &e = edges[id];

            total += e.cur_cost;
            cur = e.to;

            cout << " -> " << cur;
        }

        cout << "\nMinimum delivery cost: "
             << fixed << setprecision(2) << total << "\n";
    };

    calculateDP();

    int Q;
    cout << "Enter number of delivery requests: ";
    cin >> Q;

    if (Q < 0) {
        cout << "Invalid number of delivery requests.\n";
        return 0;
    }

    vector<int> sources(Q);

    cout << "Enter source node IDs from Stage 0:\n";

    for (int i = 0; i < Q; i++) {
        cin >> sources[i];
    }

    cout << "\nInitial Delivery Routes\n";

    for (int i = 0; i < Q; i++) {
        cout << "\nRequest " << i + 1 << ":\n";
        printRoute(sources[i]);
    }

    int U;
    cout << "\nEnter number of live edge updates: ";
    cin >> U;

    if (U < 0) {
        cout << "Invalid number of updates.\n";
        return 0;
    }

    for (int i = 0; i < U; i++) {
        int id;
        double multiplier;

        cout << "Enter edge ID (0 to " << M - 1
             << ") and cost multiplier: ";
        cin >> id >> multiplier;

        if (id < 0 || id >= M ||
            !isfinite(multiplier) || multiplier < 0) {
            cout << "Invalid update. Skipping.\n";
            continue;
        }

        edges[id].cur_cost = edges[id].base_cost * multiplier;

        calculateDP();

        cout << "Edge " << id << " updated successfully.\n";
    }

    cout << "\nUpdated Delivery Routes\n";

    for (int i = 0; i < Q; i++) {
        cout << "\nRequest " << i + 1 << ":\n";
        printRoute(sources[i]);
    }

    return 0;
}
