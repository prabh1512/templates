class SegTree2D{
public:
    SegTree2D(int n, int m, vector<vector<int>> &v) : n(n), m(m), v(v){
        tree.resize(4*n, vector<int>(4*m, 0LL));
        // run build function here
        build_x(0, n-1, 0);
    }

    void build_x(int lx, int rx, int idx1){
        if (lx > rx) return;
        if (lx < rx){
            int mid = (lx+rx)/2;
            build_x(lx, mid, 2*idx1+1); build_x(mid+1, rx, 2*idx1+2);
        }
        build_y(lx, rx, 0, m-1, idx1, 0);
    }
    
    void build_y(int lx, int rx, int ly, int ry, int idx1, int idx2){
        if (ly > ry) return;
        if (lx == rx && ly == ry){
            tree[idx1][idx2] = v[lx][ly];
            return;
        }
        if (ly == ry){
            tree[idx1][idx2] = tree[2*idx1+1][idx2] + tree[2*idx1+2][idx2];
            return;
        }
        int mid = (ly+ry)/2;
        build_y(lx, rx, ly, mid, idx1, 2*idx2+1);
        build_y(lx, rx, mid+1, ry, idx1, 2*idx2+2);
        tree[idx1][idx2] = tree[idx1][2*idx2+1] + tree[idx1][2*idx2+2];
    }

    void update_x(int x1, int x2, int ux, int uy, int val, int idx1){
        if (x1 > x2) return;
        if (x1 > ux || x2 < ux) return;
        if (x1 < x2){
            int mid = (x1+x2)/2;
            update_x(x1, mid, ux, uy, val, 2*idx1+1);
            update_x(mid+1, x2, ux, uy, val, 2*idx1 + 2);
        }
        update_y(x1, x2, 0, m-1, ux, uy, val, idx1, 0);
    }

    void update_y(int x1, int x2, int y1, int y2, int ux, int uy, int val, int idx1, int idx2){
        if (x1 > x2) return; 
        if (y1 > y2) return;
        if (ux > x2 || ux < x1) return;
        if (uy > y2 || uy < y1) return;
        tree[idx1][idx2] += val;
        if (y1 == y2) return;
        int mid = (y1+y2)/2;
        update_y(x1, x2, y1, mid, ux, uy, val, idx1, 2*idx2+1);
        update_y(x1, x2, mid+1, y2, ux, uy, val, idx1, 2*idx2+2);
    }

    int query_x(int qx1, int qx2, int qy1, int qy2, int x1, int x2, int idx1){
        if (qx1 > x2 || qx2 < x1) return 0;
        if (x1 > x2) return 0;
        if (qx1 <= x1 && qx2 >= x2){
            return query_y(qx1, qx2, qy1, qy2, x1, x2, 0, m-1, idx1, 0);
        }
        int mid = (x1+x2)/2;
        return query_x(qx1, qx2, qy1, qy2, x1, mid, 2*idx1+1) + query_x(qx1, qx2, qy1, qy2, mid+1, x2, 2*idx1+2);
    }

    int query_y(int qx1, int qx2, int qy1, int qy2, int x1, int x2, int y1, int y2, int idx1, int idx2){
        if (qx1 > x2 || qx2 < x1 || qy1 > y2 || qy2 < y1) return 0;
        if (x1 > x2 || y1 > y2) return 0;
        if (qy1 <= y1 && qy2 >= y2){
            return tree[idx1][idx2];
        }
        int mid = (y1+y2)/2;
        return query_y(qx1, qx2, qy1, qy2, x1, x2, y1, mid, idx1, 2*idx2+1)
                    + query_y(qx1, qx2, qy1, qy2, x1, x2, mid+1, y2, idx1, 2*idx2+2);
    }

    // APIs

    void update(int ux, int uy, int val){
        update_x(0,n-1,ux,uy,val,0);
    }

    int query(int qx1, int qx2, int qy1, int qy2){
        return query_x(qx1, qx2, qy1, qy2, 0, n-1, 0);
    }

private:
    int n, m;
    vector<vector<int>> v;
    vector<vector<int>> tree;
};
