#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
using namespace std;

// #define LINEAR_BUILD

class PositionHeap {
    char *T;
    int n;
    struct Node {
        int childs[256];
        Node() { fill_n(childs, 256, -1); }
    } *nodes;
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
#ifdef LINEAR_SEARCH
#else
    void appendSubtree(int pos, vector<int> &ret) {
        if (pos == -1) return;
        ret.push_back(pos);
        for (int i = 0; i < 256; i++)
            appendSubtree(nodes[pos].childs[i], ret);
    }
#endif
public:
#ifdef LINEAR_BUILD
    PositionHeap(const char T[]) {
        n = (int)strlen(T);
        this->T = strdup(T);
        nodes = new Node[n];
        int *parent = new int[n], *depth = new int[n];
        
        depth[n-1] = 0;
        for (int i = n-2; i >= 0; i--) {
            const char *q = &T[i];
            if (nodes[n-1].childs[(int)*q] == -1) {
                parent[i] = n-1;
                depth[i] = 1;
                nodes[n-1].childs[(int)*q] = i;
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
        // installMaxReach();

        for (int i = n; --i >= 0; ) {
            new(&nodes[i]) Node;
            if (i < n-1)
                nodes[parent[i]].childs[(int)T[i+depth[i]-1]] = i;
        }
        delete[] parent;
        delete[] depth;
    }
#else
    PositionHeap(const char T[]) {
        n = (int)strlen(T);
        this->T = strdup(T);
        nodes = new Node[n];
        for (int i = n-2; i >= 0; i--) {
            int v = n-1;
            const char *q = &T[i];
            while (nodes[v].childs[(int)*q] != -1)
                v = nodes[v].childs[(int)*q++];
            nodes[v].childs[(int)*q] = i;
        }
    }
#endif
    void preorder() {
        preorderPH(n-1, 0);
    }
    ~PositionHeap() {
        free(T);
        delete[] nodes;
    }
    vector<int> search(const char S[]) {
        vector<int> ret;
        int m = (int)strlen(S), depth = 0, v = n-1;
        while (v != -1 && *S != '\0') {
            if (strncmp(&T[v], S+depth, m-depth) == 0)
                ret.push_back(v);
            v = nodes[v].childs[(int)*S++];
        }
        appendSubtree(v, ret);
        return ret;
    }
};

int main()
{
    char T[3601], S[2217];
    scanf("%s", T);
    PositionHeap *heap = new PositionHeap(T);
    heap->preorder();
    delete heap;
    // scanf("%s", S);
    // vector<int> positions = heap->search(S);
    // for (vector<int>::const_iterator it = positions.begin(); it != positions.end(); it++)
    //     printf("%d ", *it);
    // putchar('\n');
}
