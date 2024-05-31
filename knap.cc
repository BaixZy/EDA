#include <iostream>
#include <vector>
#include <assert.h>

using namespace std;

int n;
int cap;
vector<int> w, v;

vector<bool> bs; // best solution
int bv; // best value

void knapsack (vector<bool>& sol, int idx, int cw, int cv) {

  if (idx == n) {
    if (cw <= cap and cv > bv) {
      bv = cv;
      bs = sol;
    }    
  }
  else {
    sol[idx] = false;
    knapsack(sol,idx+1,cw,cv);
    
    sol[idx] = true;
    knapsack(sol,idx+1,cw + w[idx], cv + v[idx]);
  } 
}

void knapsack ( ) {
  bv = 0;
  vector<bool> sol(n);
  int idx = 0;
  int cw = 0, cv = 0; // current weight, current value
  knapsack(sol,idx,cw,cv);
}

int main ( ){
  // Llegeixo entrada
  cin >> n >> cap;
  w = vector<int>(n);
  for (int i = 0; i < n; ++i) cin >> w[i];
  v = vector<int>(n);
  for (int i = 0; i < n; ++i) cin >> v[i];

  knapsack();

  int real_weight = 0, real_value = 0;
  cout << "Best solution with value " << bv << ":" << endl;
  for (int i = 0; i < n; ++i) {
    if (bs[i]) {
      cout << "Object " << i << " (weight " << w[i] << ", value " << v[i] << ")" << endl;
      real_weight += w[i];
      real_value += v[i];
    }
  }

  assert(real_weight <= cap);
  assert(real_value == bv);
}
