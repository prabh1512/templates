// Stress test for point_add_range_sum.hpp against brute force.
// Build & run:  g++ -std=c++17 -O2 -o test test_point_add_range_sum.cpp && ./test
#include "../point_add_range_sum.hpp"

signed main(){
    mt19937_64 rng(42);
    int trials = 2000;
    for (int t = 0; t < trials; t++) {
        int n = (rng() % 30) + 1;
        vector<int> a(n);
        for (int i = 0; i < n; i++) a[i] = (int)(rng() % 201) - 100;
        vector<int> brute = a;
        SegTree st(n, a);

        int ops = (rng() % 100) + 1;
        for (int o = 0; o < ops; o++) {
            int kind = rng() % 2;
            if (kind == 0) {
                int i = rng() % n;
                int vv = (int)(rng() % 201) - 100;
                brute[i] += vv;
                st.upd(i, vv);
            } else {
                int l = rng() % n, r = rng() % n;
                if (l > r) swap(l, r);
                int expect = 0;
                for (int i = l; i <= r; i++) expect += brute[i];
                int got = st.query(l, r);
                if (expect != got) {
                    cout << "FAIL: trial " << t << " query [" << l << "," << r
                         << "] expected=" << expect << " got=" << got << "\n";
                    return 1;
                }
            }
        }
    }
    cout << "PASS: " << trials << " trials, mixed point-updates + range-sum queries, n in [1,30]\n";

    int big_trials = 200;
    for (int t = 0; t < big_trials; t++) {
        int n = (rng() % 200) + 1;
        vector<int> a(n);
        for (int i = 0; i < n; i++) a[i] = (int)(rng() % 2001) - 1000;
        vector<int> brute = a;
        SegTree st(n, a);
        int ops = 500;
        for (int o = 0; o < ops; o++) {
            int kind = rng() % 2;
            if (kind == 0) {
                int i = rng() % n;
                int vv = (int)(rng() % 2001) - 1000;
                brute[i] += vv;
                st.upd(i, vv);
            } else {
                int l = rng() % n, r = rng() % n;
                if (l > r) swap(l, r);
                int expect = 0;
                for (int i = l; i <= r; i++) expect += brute[i];
                int got = st.query(l, r);
                if (expect != got) { cout << "FAIL (big)\n"; return 1; }
            }
        }
    }
    cout << "PASS: " << big_trials << " trials, 500 ops each, n in [1,200]\n";
    return 0;
}