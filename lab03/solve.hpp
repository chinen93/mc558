#pragma once

#include <iostream>
#include <cstdio>
#include <vector>
#include <map>
#include <unordered_map>
#include <stack>
#include <cmath>
#include <cstdint>
#include <climits>
#include <fstream>

using namespace std;

int cost(vector< vector<int> > &image, pair<int, int> a, pair<int, int> b);
long long int mst_image(vector< vector<int> > &image, vector< pair<int, int> > &points);

