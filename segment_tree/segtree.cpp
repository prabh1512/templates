class SegTree{
public:
    // point update, range sum query.
    SegTree(int n, vector<int> &v){
        this->n = n;
        this->v = v; tree.resize(4*n, 0LL);
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
            tree[idx] = tree[2*idx+1] + tree[2*idx+2];
        }
    }

    void update(int l, int r, int idx, int id, int val){
        // v[id] += val;
        if (l > r) return;
        if (id < l || id > r) return;
        tree[idx] += val;
        if (l == r) return;
        int mid = (l+r)/2;
        update(l, mid, 2*idx+1, id, val);
        update(mid+1, r, 2*idx+2, id, val);
    }

    int que(int ql, int qr, int l, int r, int idx){
        if (l > r) return 0;
        if (l == ql && r == qr) return tree[idx];
        if (ql > r || qr < l) return 0;
        int mid = (l+r)/2;
        return que(ql, min(mid,qr), l, mid, 2*idx+1) + que(max(mid+1, ql), qr, mid+1, r, 2*idx+2);
    }

    void upd(int id, int val){
        v[id] += val;
        update(0, n-1, 0, id, val);
    }

    int query(int ql, int qr){
        return que(ql, qr, 0, n-1, 0);
    }

private:
    int n;
    vector<int> v;
    vector<int> tree;
};
