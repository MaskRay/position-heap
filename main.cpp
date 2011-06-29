#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <list>
using namespace std;

// #define LINEAR_BUILD
// #define LINEAR_SEARCH

class PositionHeap {
    char *T;
    int n;
    struct Node {
        int childs[256];
        Node() { fill_n(childs, 256, -1); }
    } *nodes;
#ifdef LINEAR_BUILD
    int *parent;
#endif
#ifdef LINEAR_SEARCH
    int *maxReach, *discovery, *finish;
    void dfs(int pos, int &timestamp) {
        if (pos == -1) return;
        discovery[pos] = timestamp++;
        for (int i = 0; i < 256; i++)
            dfs(nodes[pos].childs[i], timestamp);
        finish[pos] = timestamp++;
    }
    bool isDescendant(int v, int u) {
        return discovery[u] <= discovery[v] && finish[v] <= finish[u];
    }
#endif
    void preorderPH(int pos, int depth) {
        if (pos == -1) return;
        printf("%*s%d children:", 2*depth, "", pos);
        for (int i = 0; i < 256; i++)
            if (nodes[pos].childs[i] != -1)
                printf(" (%c,%d)", i, nodes[pos].childs[i]);
        putchar('\n');
        for (int i = 0; i < 256; i++)
            preorderPH(nodes[pos].childs[i], depth+1);
    }
    void appendSubtree(int pos, list<int> &ret) {
        if (pos == -1) return;
        ret.push_back(pos);
        for (int i = 0; i < 256; i++)
            appendSubtree(nodes[pos].childs[i], ret);
    }
public:
#ifdef LINEAR_BUILD
    PositionHeap(const char T[]) {
        n = (int)strlen(T);
        this->T = strdup(T);
        nodes = new Node[n+1];
        parent = new int[n];
        int *depth = new int[n];
        
        for (int i = n; --i >= 0; ) {
            const char *q = &T[i];
            if (nodes[n].childs[(int)*q] == -1) {
                parent[i] = n;
                depth[i] = 1;
                nodes[n].childs[(int)*q] = i;
            } else {
                int u = i+1, v, w;
                do {
                    v = u;
                    u = parent[u];
                    w = nodes[u].childs[(int)*q];
                } while (w == -1);
                parent[i] = w;
                depth[i] = depth[w]+1;
                nodes[v].childs[(int)*q] = i;
            }
        }
        
    #ifdef LINEAR_SEARCH
        maxReach = new int[n];
        for (int u = n, i = n; --i >= 0; ) {
            const char *q = &T[i];
            int w;
            while ((w = nodes[u].childs[(int)*q]) == -1)
                u = parent[u];
            maxReach[i] = w;
            u = w;
        }
    #endif

        for (int i = n; --i >= 0; ) {
            new(&nodes[i]) Node;
            nodes[parent[i]].childs[(int)T[i+depth[i]-1]] = i;
        }
        delete[] parent;
        delete[] depth;

   #ifdef LINEAR_SEARCH
        discovery = new int[n+1];
        finish = new int[n+1];
        int timestamp = 0;
        dfs(n, timestamp);
   #endif
    }
#else
    PositionHeap(const char T[]) {
        n = (int)strlen(T);
        this->T = strdup(T);
        nodes = new Node[n+1];
        for (int i = n; --i >= 0; ) {
            const char *q = &T[i];
            int v = n;
            while (nodes[v].childs[(int)*q] != -1)
                v = nodes[v].childs[(int)*q++];
            nodes[v].childs[(int)*q] = i;
        }
    #ifdef LINEAR_SEARCH
        maxReach = new int[n];
        for (int i = n; --i >= 0; ) {
            const char *q = &T[i];
            int v = n;
            while (nodes[v].childs[(int)*q] != -1)
                v = nodes[v].childs[(int)*q++];
            maxReach[i] = v;
        }
        discovery = new int[n+1];
        finish = new int[n+1];
        int timestamp = 0;
        dfs(n, timestamp);
    #endif
    }
#endif
    void preorder() {
        preorderPH(n, 0);
    }
    ~PositionHeap() {
        free(T);
        delete[] nodes;
#ifdef LINEAR_SEARCH
        delete[] maxReach;
        delete[] discovery;
        delete[] finish;
#endif
    }
#ifdef LINEAR_SEARCH
    list<int> search(const char S[]) {
        list<int> ret;
        const char *p = S;
        int v = n;
        while (*p && nodes[v].childs[(int)*p] != -1)
            v = nodes[v].childs[(int)*p++];
        p = S;
        int u = n;
        while (*p && nodes[u].childs[(int)*p] != -1) {
            u = nodes[u].childs[(int)*p++];
            if (isDescendant(maxReach[u], v))
                ret.push_back(u);
        }
        if (!*p)
            for (int i = 0; i < 256; i++)
                appendSubtree(nodes[v].childs[i], ret);
        while (*p) {
            const char *q = p;
            v = n;
            while (*p && nodes[v].childs[(int)*p] != -1)
                v = nodes[v].childs[(int)*p++];
            if (p == q) {
                ret.clear();
                break;
            }
            for (list<int>::iterator it = ret.begin(); it != ret.end(); )
                if (*it+(q-S) < n && isDescendant(maxReach[*it+(q-S)], v))
                    ++it;
                else
                    it = ret.erase(it);
        }
        return ret;
    }
#else
    list<int> search(const char S[]) {
        list<int> ret;
        int m = (int)strlen(S), depth = 0, v = n;
        while (v != -1 && *S) {
            if (strncmp(T+v+depth, S, m-depth) == 0)
                ret.push_back(v);
            v = nodes[v].childs[(int)*S++];
            depth++;
        }
        appendSubtree(v, ret);
        return ret;
    }
#endif
};

int main()
{
    static char T[3612217], S[3612217];
    
    scanf("%s", T);
    PositionHeap *heap = new PositionHeap(T);
    // heap->preorder();
    
    scanf("%s", S);
    list<int> positions = heap->search(S);
    for (list<int>::const_iterator it = positions.begin(); it != positions.end(); ) {
        printf("%d", *it);
        putchar(++it == positions.end() ? '\n' : ' ');
    }

    delete heap;
}
