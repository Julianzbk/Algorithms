#include <cmath>
#include <initializer_list>

template <typename T>
struct TreeNode
{
    T data;
    TreeNode* left;
    TreeNode* right;
    TreeNode* parent; // for clarity

    TreeNode(T data, TreeNode* left, TreeNode* right, TreeNode* parent)
        :data(data), left(left), right(right), parent(parent)
    {
    }
};

template <typename T>
class TreeHeap // MinHeap
{
    using Node = TreeNode<T>; 
};

template <typename T>
class ArrayHeap
    :public std::vector<T>
{
public:
    size_t height;
    
    ArrayHeap()
        :std::vector<T>(), height(0)
    {
    }

    ArrayHeap(std::vector<T> const& other)
        :std::vector<T>(other), height(std::log2(other.size()) + 1)
    {
    }

    ArrayHeap(std::initializer_list<T> const& other)
        :std::vector<T>(other), height(std::log2(other.size()) + 1)
    {
    }
    
    void print(std::ostream& out)
    {
        ArrayHeap& A = *this;
        height = std::log2(A.size()) + 1;
        size_t num_leaves = std::pow(2, height - 1);
        out << std::setw(num_leaves);
        for (int i = 1; i < A.size(); ++i)
        {
            size_t lvl = std::log2(i) + 1;
            out << A[lvl] << " ";
            if (std::pow(2, lvl) - 1 == i)
                out << "\n";
        }
    }
};

template <typename T>
std::ostream& operator << (std::ostream& out, TreeNode<T> N)
{
    out << "TreeNode(" << N.data << ", ";
    if (N.left != nullptr)
        out << N.left->data;
    else
        out << "NULL";
    out << ", ";
    if (N.right != nullptr)
        out << N.right->data;
    else
        out << "NULL";
    out << ", ";
    if (N.parent != nullptr)
        out << N.parent->data;
    else
        out << "NODE IS ROOT";
    out << ")";
    return out;
}

template <typename T>
std::ostream& operator << (std::ostream& out, TreeHeap<T> N)
{
    return out;
}

template <typename T>    
std::ostream& operator << (std::ostream& out, ArrayHeap<T>& A)
{
    A.print(out);
    return out;
}


