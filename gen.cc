#include <iostream>
#include <vector>

using namespace std;

int nombre (int min, int max) {
  int rang = max - min + 1;
  return min + rand()%rang;
}

int main ( ){
  int n;
  cin >> n;

  srand(time(NULL)); // inicialitzo la llavor aleatoria


  vector<int> w(n), v(n);

  for (int i = 0; i < n; ++i) w[i] = nombre(1,100);
  for (int i = 0; i < n; ++i) v[i] = nombre(1,20);

  int total = 0;
  for (int x : w) total += x;

  int cap = total/3;

  cout << n << endl;
  cout << cap << endl;
  for (int i = 0; i < n; ++i) cout << w[i] << " ";
  cout << endl;
  for (int i = 0; i < n; ++i) cout << v[i] << " ";
  cout << endl;
  
}
