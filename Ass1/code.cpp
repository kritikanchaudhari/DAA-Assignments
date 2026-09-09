#include <iostream> 
#include <fstream> 
#include <sstream> 
#include <vector> 
using namespace std; 
// Structure to store order details 
struct Order 
{ 
 string orderID; 
 string customerID; 
 string purchaseTimestamp; 
}; 
// Merge function 
void merge(vector<Order> &orders, int left, int mid, int right) 
{ 
 int n1 = mid - left + 1; 
 int n2 = right - mid;
 vector<Order> L(n1), R(n2); 
 for (int i = 0; i < n1; i++) 
 L[i] = orders[left + i]; 
 for (int j = 0; j < n2; j++) 
 R[j] = orders[mid + 1 + j]; 
 int i = 0, j = 0, k = left; 
 while (i < n1 && j < n2) 
 { 
 if (L[i].purchaseTimestamp <= R[j].purchaseTimestamp)  orders[k++] = L[i++]; 
 else 
 orders[k++] = R[j++]; 
 } 
 while (i < n1) 
 orders[k++] = L[i++]; 
 while (j < n2) 
 orders[k++] = R[j++]; 
} 
// Merge Sort function 
void mergeSort(vector<Order> &orders, int left, int right) { 
 if (left >= right) 
 return;
 int mid = left + (right - left) / 2; 
 mergeSort(orders, left, mid); 
 mergeSort(orders, mid + 1, right); 
 merge(orders, left, mid, right); 
} 
int main() 
{ 
 vector<Order> orders; 
 ifstream file("olist_orders_dataset.csv"); 
 if (!file.is_open()) 
 { 
 cout << "Error: Could not open dataset file" << endl;  return 1; 
 } 
 string line; 
 getline(file, line); 
 while (getline(file, line)) 
 { 
 stringstream ss(line); 
 Order order; 
 string temp; 
 getline(ss, order.orderID, ','); 
 getline(ss, order.customerID, ',');
 getline(ss, temp, ','); 
 getline(ss, order.purchaseTimestamp, ',');  if (!order.purchaseTimestamp.empty()) 
 orders.push_back(order); 
 } 
 file.close(); 
 cout << "Total Orders Loaded: " << orders.size() << endl;  if (!orders.empty()) 
 mergeSort(orders, 0, orders.size() - 1); 
 cout << "\nFirst 50 Sorted Orders:\n\n"; 
 for (size_t i = 0; i < min((size_t)50, orders.size()); i++)  { 
 cout << orders[i].orderID << " " 
 << orders[i].customerID << " " 
 << orders[i].purchaseTimestamp << endl;  } 
 return 0; 
} 
