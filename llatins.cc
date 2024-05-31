#include <iostream>
#include <vector>

using namespace std;

void escriu_quadrats (vector<vector<int>>& q, int i, int j, vector<vector<bool>>& fil,
		      vector<vector<bool>>& col) {

  int n = q.size();

  if (i == n) { // Cas base
    for (int k = 0; k < n; ++k) {
      for (int l = 0; l < n; ++l) cout << q[k][l];
      cout << endl;
    }
    cout << endl;
  }
  else {    
    for (int v = 1; v <= n; ++v) {
      // si no esta utilitzat
      if (not fil[v][i] and not col[v][j]) {
	q[i][j] = v;
	fil[v][i] = true;
	col[v][j] = true;
	int n_i, n_j; // next i, next j
	if (j == n-1) {n_i = i+1; n_j = 0;}
	else { n_i = i; n_j = j + 1;}
	escriu_quadrats(q,n_i,n_j,fil,col);
	fil[v][i] = false;
	col[v][j] = false;
      }
    }
  }
}

void escriu_quadrats (int n) {
  vector<vector<int>> q(n,vector<int>(n));
  int i = 0, j = 0;
  vector<vector<bool>> col(n+1,vector<bool>(n,false)); // col[v][c] cert sii valor v apareix a la columna c
  vector<vector<bool>> fil(n+1,vector<bool>(n,false)); // fil[v][c] cert sii valor v apareix a la fila c
  escriu_quadrats(q,i,j,fil,col);
}

int main ( ){
  int n;
  cin >> n;
  escriu_quadrats(n);
}
