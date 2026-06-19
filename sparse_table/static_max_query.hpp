#pragma once

using namespace std;
#define int long long

class SparseTable{
public:
    SparseTable(int n, vector<int> &v) : n(n), v(v){
        // run build function
        build();
    }

    void build(){
        int num = (n > 0 ? 64 - __builtin_clzll(n) : 0LL);
        table.resize(num);
        table[0] = std::move(v);
        for (int i = 1; i < num; i ++){
            int sz = (1LL << i);
            table[i].resize(n-sz+1);
            for (int j = 0; j <= n-sz; j++){
                table[i][j] = max(table[i-1][j], table[i-1][j+(1LL << (i-1))]);
            }
        }
    }

    int query(int l, int r){
        if (l > r) return LLONG_MIN;
        int sz = (r-l+1);
        int hh = (63 - __builtin_clzll(sz));
        return max(table[hh][l], table[hh][r-(1LL << hh)+1]);
    }


private:
    int n;
    vector<int> v;
    vector<vector<int>> table;
};
