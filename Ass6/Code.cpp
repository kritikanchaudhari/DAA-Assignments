#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct Item {
    string name;
    int weight;
    int profit;
};

struct State {
    int weight;
    int profit;
    vector<int> selected;
};

vector<State> purge(vector<State> states) {

    sort(states.begin(), states.end(),
         [](const State& a, const State& b) {

             if (a.weight == b.weight)
                 return a.profit > b.profit;

             return a.weight < b.weight;
         });

    vector<State> result;

    int maxProfit = -1;

    for (const State& state : states) {

        if (state.profit > maxProfit) {

            result.push_back(state);
            maxProfit = state.profit;
        }
    }

    return result;
}

int main() {

    int N, capacity;

    cout << "Enter number of items: ";
    cin >> N;

    cout << "Enter capacity: ";
    cin >> capacity;

    vector<Item> items(N);

    cout << "\nEnter item details:\n";

    for (int i = 0; i < N; i++) {

        cout << "\nItem " << i + 1 << endl;

        cout << "Name: ";
        cin >> items[i].name;

        cout << "Weight: ";
        cin >> items[i].weight;

        cout << "Profit: ";
        cin >> items[i].profit;
    }

    vector<State> states;

    State initial;
    initial.weight = 0;
    initial.profit = 0;

    states.push_back(initial);

    for (int i = 0; i < N; i++) {

        vector<State> merged;

        for (const State& state : states) {
            merged.push_back(state);
        }

        for (const State& state : states) {

            int newWeight =
                state.weight + items[i].weight;

            if (newWeight <= capacity) {

                State newState;

                newState.weight = newWeight;

                newState.profit =
                    state.profit + items[i].profit;

                newState.selected =
                    state.selected;

                newState.selected.push_back(i);

                merged.push_back(newState);
            }
        }

        states = purge(merged);

        cout << "\nAfter processing "
             << items[i].name << ":\n";

        for (const State& state : states) {

            cout << "("
                 << state.weight
                 << ", "
                 << state.profit
                 << ") ";
        }

        cout << endl;
    }

    State best = states[0];

    for (const State& state : states) {

        if (state.profit > best.profit) {
            best = state;
        }
    }

    cout << "OPTIMAL SOLUTION\n";
  
    cout << "Maximum Profit = "
         << best.profit << endl;

    cout << "Total Weight   = "
         << best.weight << endl;

    cout << "\nSelected Items:\n";

    for (int index : best.selected) {

        cout << items[index].name
             << " -> Weight = "
             << items[index].weight
             << ", Profit = "
             << items[index].profit
             << endl;
    }

    return 0;
}
