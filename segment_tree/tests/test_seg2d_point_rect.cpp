// Adversarial stress test for seg2d_point_rect.hpp.
// Build & run:  g++ -std=c++17 -O2 -o test test_seg2d_point_rect.cpp && ./test
#include "../seg2d_point_rect.hpp"

mt19937_64 rng;

bool one_trial(int n, int m, int ops, int val_range, bool exhaustive_queries=false) {
    vector<vector<int>> a(n, vector<int>(m));
    for (int i = 0; i < n; i++) for (int j = 0; j < m; j++)
        a[i][j] = (int)(rng() % (2*val_range+1)) - val_range;
    vector<vector<int>> brute = a;
    SegTree2D st(n, m, a);

    auto check_rect = [&](int x1, int x2, int y1, int y2) -> bool {
        int expect = 0;
        for (int i = x1; i <= x2; i++)
            for (int j = y1; j <= y2; j++) expect += brute[i][j];
        int got = st.query(x1, x2, y1, y2);
        if (expect != got) {
            cout << "FAIL n=" << n << " m=" << m
                 << " rect [" << x1 << "," << x2 << "]x[" << y1 << "," << y2 << "]"
                 << " expect=" << expect << " got=" << got << "\n";
            return false;
        }
        return true;
    };

    if (!check_rect(0, n-1, 0, m-1)) return false;
    if (!check_rect(0, 0, 0, 0)) return false;
    if (n > 1 && !check_rect(n-1, n-1, m-1, m-1)) return false;

    for (int o = 0; o < ops; o++) {
        int kind = rng() % 2;
        if (kind == 0) {
            int x = rng() % n, y = rng() % m;
            int vv = (int)(rng() % (2*val_range+1)) - val_range;
            brute[x][y] += vv;
            st.update(x, y, vv);
        } else {
            int x1 = rng() % n, x2 = rng() % n;
            int y1 = rng() % m, y2 = rng() % m;
            if (x1 > x2) swap(x1, x2);
            if (y1 > y2) swap(y1, y2);
            if (!check_rect(x1, x2, y1, y2)) return false;
        }
    }

    if (exhaustive_queries) {
        for (int x1 = 0; x1 < n; x1++)
            for (int x2 = x1; x2 < n; x2++)
                for (int y1 = 0; y1 < m; y1++)
                    for (int y2 = y1; y2 < m; y2++)
                        if (!check_rect(x1, x2, y1, y2)) return false;
    }
    return true;
}

signed main(){
    rng.seed(42);

    cout << "[1] exhaustive small (n,m in [1..6], exhaustive queries)... " << flush;
    int small_fails = 0, small_trials = 0;
    for (int n = 1; n <= 6; n++) for (int m = 1; m <= 6; m++) for (int t = 0; t < 30; t++) {
        small_trials++;
        if (!one_trial(n, m, 20, 50, true)) { small_fails++; goto small_done; }
    }
    small_done:
    cout << (small_fails ? "FAIL" : "OK") << " (" << small_trials << " trials)\n";
    if (small_fails) return 1;

    cout << "[2] degenerate dimensions (n or m == 1)... " << flush;
    int deg_fails = 0;
    for (int dim = 1; dim <= 50; dim++) {
        if (!one_trial(1, dim, 100, 1000)) { deg_fails++; break; }
        if (!one_trial(dim, 1, 100, 1000)) { deg_fails++; break; }
    }
    cout << (deg_fails ? "FAIL" : "OK") << "\n";
    if (deg_fails) return 1;

    cout << "[3] prime dimensions... " << flush;
    int prime_fails = 0;
    int primes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    for (int p1 : primes) for (int p2 : primes)
        if (!one_trial(p1, p2, 200, 1000)) { prime_fails++; goto prime_done; }
    prime_done:
    cout << (prime_fails ? "FAIL" : "OK") << "\n";
    if (prime_fails) return 1;

    cout << "[4] power-of-2 dimensions... " << flush;
    int p2_fails = 0;
    for (int p : {1, 2, 4, 8, 16, 32, 64}) if (!one_trial(p, p, 500, 1000)) { p2_fails++; break; }
    cout << (p2_fails ? "FAIL" : "OK") << "\n";
    if (p2_fails) return 1;

    cout << "[5] highly asymmetric (100x3, 3x100)... " << flush;
    int asym_fails = 0;
    for (int t = 0; t < 5; t++) {
        if (!one_trial(100, 3, 500, 10000)) { asym_fails++; break; }
        if (!one_trial(3, 100, 500, 10000)) { asym_fails++; break; }
    }
    cout << (asym_fails ? "FAIL" : "OK") << "\n";
    if (asym_fails) return 1;

    cout << "[6] adversarial query patterns (single cells/rows/cols, corners, midpoint strips)... " << flush;
    int adv_fails = 0;
    for (int trial = 0; trial < 20; trial++) {
        int n = (rng() % 20) + 5, m = (rng() % 20) + 5;
        vector<vector<int>> a(n, vector<int>(m));
        for (int i = 0; i < n; i++) for (int j = 0; j < m; j++)
            a[i][j] = (int)(rng() % 2001) - 1000;
        vector<vector<int>> brute = a;
        SegTree2D st(n, m, a);
        for (int o = 0; o < 50; o++) {
            int x = rng() % n, y = rng() % m;
            int vv = (int)(rng() % 2001) - 1000;
            brute[x][y] += vv;
            st.update(x, y, vv);
        }
        auto check = [&](int x1, int x2, int y1, int y2) -> bool {
            int expect = 0;
            for (int i = x1; i <= x2; i++) for (int j = y1; j <= y2; j++) expect += brute[i][j];
            int got = st.query(x1, x2, y1, y2);
            if (expect != got) {
                cout << "FAIL adv rect [" << x1 << "," << x2 << "]x[" << y1 << "," << y2 << "]\n";
                return false;
            }
            return true;
        };
        for (int i = 0; i < n; i++) for (int j = 0; j < m; j++)
            if (!check(i, i, j, j)) { adv_fails++; goto adv_done; }
        for (int i = 0; i < n; i++) if (!check(i, i, 0, m-1)) { adv_fails++; goto adv_done; }
        for (int j = 0; j < m; j++) if (!check(0, n-1, j, j)) { adv_fails++; goto adv_done; }
        if (n >= 2 && m >= 2) {
            if (!check(0, 1, 0, 1)) { adv_fails++; goto adv_done; }
            if (!check(0, 1, m-2, m-1)) { adv_fails++; goto adv_done; }
            if (!check(n-2, n-1, 0, 1)) { adv_fails++; goto adv_done; }
            if (!check(n-2, n-1, m-2, m-1)) { adv_fails++; goto adv_done; }
        }
        if (!check(0, n-1, 0, m-1)) { adv_fails++; goto adv_done; }
        if (n >= 4) {
            int mx = n/2;
            if (!check(mx-1, mx, 0, m-1)) { adv_fails++; goto adv_done; }
            if (!check(mx, mx+1, 0, m-1)) { adv_fails++; goto adv_done; }
        }
        if (m >= 4) {
            int my = m/2;
            if (!check(0, n-1, my-1, my)) { adv_fails++; goto adv_done; }
            if (!check(0, n-1, my, my+1)) { adv_fails++; goto adv_done; }
        }
    }
    adv_done:
    cout << (adv_fails ? "FAIL" : "OK") << "\n";
    if (adv_fails) return 1;

    cout << "[7] same-cell repeated updates (accumulation)... " << flush;
    int rep_fails = 0;
    for (int trial = 0; trial < 50; trial++) {
        int n = (rng() % 15) + 3, m = (rng() % 15) + 3;
        vector<vector<int>> a(n, vector<int>(m, 0));
        for (int i = 0; i < n; i++) for (int j = 0; j < m; j++)
            a[i][j] = (int)(rng() % 21) - 10;
        vector<vector<int>> brute = a;
        SegTree2D st(n, m, a);
        int hx = rng() % n, hy = rng() % m;
        for (int k = 0; k < 100; k++) {
            int vv = (int)(rng() % 21) - 10;
            brute[hx][hy] += vv;
            st.update(hx, hy, vv);
        }
        for (int i = 0; i < n; i++) for (int j = 0; j < m; j++) {
            int e = 0; for (int a1 = 0; a1 <= i; a1++) for (int a2 = 0; a2 <= j; a2++) e += brute[a1][a2];
            int g = st.query(0, i, 0, j);
            if (e != g) { rep_fails++; cout << "FAIL repeated hammer\n"; goto rep_done; }
        }
    }
    rep_done:
    cout << (rep_fails ? "FAIL" : "OK") << "\n";
    if (rep_fails) return 1;

    cout << "[8] heavy stress (n=m=50, 5000 ops, 50 trials)... " << flush;
    int hs_fails = 0;
    for (int t = 0; t < 50; t++) if (!one_trial(50, 50, 5000, 100000)) { hs_fails++; break; }
    cout << (hs_fails ? "FAIL" : "OK") << "\n";
    if (hs_fails) return 1;

    cout << "\nAll adversarial tests PASSED.\n";
    return 0;
}