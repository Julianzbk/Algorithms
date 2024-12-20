#include <iostream>
using std::cout, std::endl;

#include <vector>
template <typename T>
std::ostream& operator << (std::ostream& out, std::vector<T> const& v)
{
    out << "[";
    for (T elem: v)
    {
        out << elem << " ";
    }
    out << '\b' << "]";
    return out;
}

#include <stack>
#include <queue>

using Vertex = int;

#include "unordered_list.h"
using List = UnorderedList // might wanna change to a sorted array for denser graphs.

struct Edge
{
    Vertex begin;
    Vertex end;
    
    bool operator < (Edge other)
    {
        return begin == other.begin && end < other.end;
    }

    friend std::ostream& operator << (std::ostream& out, Edge const& E)
    {
        out << "E(" << E.begin << ", " << E.end << ")";
        return out;
    }
};

struct Graph
{
    std::vector<Edge> EdgeList;
    std::vector<List<int>> AdjList;
    int n;

    Graph(std::vector<Edge> const& EdgeList, int n)
        :EdgeList(EdgeList), AdjList(std::vector<Vertex>(n)), n(n)
    {
        for (Edge e: EdgeList)
        {
            List<int>& neighbors;
            if (!neighbors.find(e.end))
                neighbors.push(e.end);
        }
    }

    friend std::ostream& operator << (std::ostream& out, Graph const& G)
    {
        out << "G(" << G.EdgeList << ")";
        return out;
    }
};

struct TreeNode
{
    T data;
    std::vector<TreeNode*> children;

    TreeNode(T data, TreeNode* left, std::vector<TreeNode*> const& children)
        :data(data), children(children)
    {
    }
};

struct WeirdTree
{
    TreeNode* root;
    void push(TreeNode node)
    {
        root->children.push_back(node);
    }
};

void DFS_recursive
    (std::vector<List<int>> const& AdjList, Vertex curr, std::vector<bool>& visited)
{
    visited[curr] = true;
    Vertex neighbor = AdjList[curr].find_min(); // adjust neighbor exploring policy here.
    if (!visited[neighbor])
    {
        DFS_recursive(AdjList, neighbor, visited);
    }
}

/*
//#include "matrix.hpp"
struct SparseGraph
{
    //Matrix<int> AdjMatrix;
    std::vector<UnorderedList<int>> AdjList;

    Graph(std::vector<std::vector<int>> init_list)
    {
        AdjList.reserve(init_list.size());
        size_t i = 0;
        for (std::vector<int> L: init_list)
        {
            AdjList.push_back(UnorderedList<int>());
            for (int V: L)
                AdjList[i].insert(L);
            ++i;
        }
    }

    friend std::ostream& operator << (std::ostream& out, SparseGraph const& G)
    {
        out << "G_sparse("
        for (UnorderedList<int> L: AdjList)
            out << L << "\n";
        out << ")";
        return out;
    }
};
*/

int main()
{
    Graph G = {{Edge{1, 2}, Edge{3, 4}}, 4};
    cout << G << endl;
    Graph GS = {{{1, 2, 3}, {2, 1, 3}, {3, 2, 1, 4}, {4, 3}}};
}
