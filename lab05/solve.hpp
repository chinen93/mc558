#include <climits>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <stack>
#include <unordered_map>
#include <utility>
#include <vector>


using namespace std;

struct atracao{
  int repeticoes;
  vector<int> indexNaoSeq;
};

int solve(int numAtracoes, vector<atracao> atracoes);

