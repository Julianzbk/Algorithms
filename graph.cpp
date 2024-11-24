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

struct Edge
{
    Vertex begin;
    Vertex end;
    friend std::ostream& operator << (std::ostream& out, Edge const& E)
    {
        out << "E(" << E.begin << ", " << E.end << ")";
        return out;
    }
};

struct Graph
{
    std::vector<Edge> EdgeList;
    int n;

    Graph(std::vector<Edge> const& EdgeList, int n)
        :EdgeList(EdgeList), n(n)
    {
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

std::vector<Vertex> DFS(std::vector<Edge> const& E, Vertex begin, std::vector<bool>& visited)
{
}

//#include "matrix.hpp"
#include "unordered_list.h"
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


int main()
{
    Graph G = {{Edge{1, 2}, Edge{3, 4}}, 4};
    cout << G << endl;
    Graph GS = {{{1, 2, 3}, {2, 1, 3}, {3, 2, 1, 4}, {4, 3}}};
}