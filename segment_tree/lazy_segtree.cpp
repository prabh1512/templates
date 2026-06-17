class LazySegTree{
public:
    // range sum update, range max query
    LazySegTree(int n, vector<int> &v){
        this->n = n;
        this->v = v; tree.resize(4*n, 0LL);
        lazy.resize(4*n, 0LL);
        build(0, n-1, 0);
    }

    void build(int l, int r, int idx){
        if (l > r) return;
        if (l == r){
            tree[idx] = v[l];
        }
        else{
            int mid = (l+r)/2;
            build(l, mid, 2*idx+1);
            build(mid+1, r, 2*idx+2);
            tree[idx] = max(tree[2*idx+1],tree[2*idx+2]);
        }
    }

    void push(int l, int r, int idx){
        if (l > r) return;
        if (2*idx+1 < 4*n) {lazy[2*idx+1] += lazy[idx]; tree[2*idx+1] += lazy[idx];}
        if (2*idx+2 < 4*n) {lazy[2*idx+2] += lazy[idx]; tree[2*idx+2] += lazy[idx];}
        lazy[idx]=0;
    }

    void update(int l, int r, int idx, int idl, int idr, int val){
        // v[id] += val;
        if (l > r) return;
        if (idr < l || idl > r) return;
        if (idl <= l && r <= idr){
            tree[idx] += val;
            lazy[idx] += val;
            return;
        }
        push(l, r, idx);
        int mid = (l+r)/2;
        update(l, mid, 2*idx+1, idl, idr, val);
        update(mid+1, r, 2*idx+2, idl, idr, val);
        tree[idx] = max(tree[2*idx+1], tree[2*idx+2]);
    }

    int que(int ql, int qr, int l, int r, int idx){
        if (l > r) return LLONG_MIN;
        if (l == ql && r == qr) return tree[idx];
        if (ql > r || qr < l) return LLONG_MIN;
        push(l, r, idx);
        int mid = (l+r)/2;
        return max(que(ql, min(mid,qr), l, mid, 2*idx+1), que(max(mid+1, ql), qr, mid+1, r, 2*idx+2));
    }

    void upd(int idl, int idr, int val){
        update(0, n-1, 0, idl, idr, val);
    }

    int query(int ql, int qr){
        return que(ql, qr, 0, n-1, 0);
    }

private:
    int n;
    vector<int> v;
    vector<int> tree;
    vector<int> lazy;
};
