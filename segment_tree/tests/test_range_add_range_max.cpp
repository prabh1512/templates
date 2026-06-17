// Stress test for range_add_range_max.hpp against brute force (includes negative values).
// Build & run:  g++ -std=c++17 -O2 -o test test_range_add_range_max.cpp && ./test
#include "../range_add_range_max.hpp"

signed main(){
    mt19937_64 rng(42);
    int trials = 3000;
    for (int t = 0; t < trials; t++) {
        int n = (rng() % 30) + 1;
        vector<int> a(n);
        for (int i = 0; i < n; i++) a[i] = (int)(rng() % 401) - 200;
        vector<int> brute = a;
        LazySegTree st(n, a);
        int ops = (rng() % 100) + 1;
        for (int o = 0; o < ops; o++) {
            int kind = rng() % 2;
            int l = rng() % n, r = rng() % n;
            if (l > r) swap(l, r);
            if (kind == 0) {
                int vv = (int)(rng() % 401) - 200;
                for (int i = l; i <= r; i++) brute[i] += vv;
                st.upd(l, r, vv);
            } else {
                int expect = LLONG_MIN;
                for (int i = l; i <= r; i++) expect = max(expect, brute[i]);
                int got = st.query(l, r);
                if (expect != got) {
                    cout << "FAIL: trial " << t << " query [" << l << "," << r
                         << "] expected=" << expect << " got=" << got << "\n";
                    return 1;
                }
            }
        }
    }
    cout << "PASS: " << trials << " trials, range-add + range-max (negatives included)\n";

    int big = 300;
    for (int t = 0; t < big; t++) {
        int n = (rng() % 200) + 2;
        vector<int> a(n);
        for (int i = 0; i < n; i++) a[i] = (int)(rng() % 2001) - 1000;
        vector<int> brute = a;
        LazySegTree st(n, a);
        for (int o = 0; o < 1000; o++) {
            int kind = rng() % 2;
            int l = rng() % n, r = rng() % n;
            if (l > r) swap(l, r);
            if (kind == 0) {
                int vv = (int)(rng() % 2001) - 1000;
                for (int i = l; i <= r; i++) brute[i] += vv;
                st.upd(l, r, vv);
            } else {
                int expect = LLONG_MIN;
                for (int i = l; i <= r; i++) expect = max(expect, brute[i]);
                int got = st.query(l, r);
                if (expect != got) { cout << "FAIL (big)\n"; return 1; }
            }
        }
    }
    cout << "PASS: " << big << " big trials, n in [2,200], 1000 ops each\n";
    return 0;
}