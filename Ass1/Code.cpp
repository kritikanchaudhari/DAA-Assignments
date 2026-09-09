#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

struct Record
{
    int age;
    float height;
    float weight;
    float bmi;
    string bmiClass;
};

void merge(vector<Record> &arr, int left, int mid, int right)
{
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<Record> L(n1), R(n2);

    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];

    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2)
    {
        if (L[i].bmi <= R[j].bmi)
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(vector<Record> &arr, int left, int right)
{
    if (left < right)
    {
        int mid = left + (right - left) / 2;

        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);

        merge(arr, left, mid, right);
    }
}

int main()
{
    ifstream file("bmi.csv");

    if (!file)
    {
        cout << "Cannot open bmi.csv" << endl;
        return 1;
    }

    vector<Record> records;
    string line;

    // Skip header
    getline(file, line);

    while (getline(file, line))
    {
        if (line.empty())
            continue;

        stringstream ss(line);
        Record r;
        string temp;

        getline(ss, temp, ',');
        r.age = stoi(temp);

        getline(ss, temp, ',');
        r.height = stof(temp);

        getline(ss, temp, ',');
        r.weight = stof(temp);

        getline(ss, temp, ',');
        r.bmi = stof(temp);

        getline(ss, r.bmiClass);

        records.push_back(r);
    }

    file.close();

    if (!records.empty())
    {
        mergeSort(records, 0, records.size() - 1);
    }

    cout << "\nSorted by BMI\n\n";
    cout << "Age\tHeight\tWeight\tBMI\tBMI Class\n";

    for (const auto &r : records)
    {
        cout << r.age << "\t"
             << r.height << "\t"
             << r.weight << "\t"
             << r.bmi << "\t"
             << r.bmiClass << endl;
    }

    ofstream out("sorted_bmi.csv");

    out << "Age,Height,Weight,Bmi,BmiClass\n";

    for (const auto &r : records)
    {
        out << r.age << ","
            << r.height << ","
            << r.weight << ","
            << r.bmi << ","
            << r.bmiClass << "\n";
    }

    out.close();

    cout << "\nSorted data saved to sorted_bmi.csv" << endl;

    return 0;
}
