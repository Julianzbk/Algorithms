#include <iostream>
using std::cout, std::endl;

#include <fstream>
#include <map>
#include <queue>
#include <vector>
#include <algorithm>

namespace Huffman
{

struct FreqComparator
{
    bool operator () (std::pair<char, int> const& a, std::pair<char, int> const& b)
    {
        return a.second > b.second;
    }
};


struct TreeNode
{
    char data;
    int freq;
    TreeNode* left;
    TreeNode* right;

    TreeNode(char data, int freq, TreeNode* left, TreeNode* right)
        :data(data), freq(freq), left(left), right(right)
    {
    }

    TreeNode(std::pair<char, int> const& pair, TreeNode* left = nullptr, TreeNode* right=nullptr)
        :data(pair.first), freq(pair.second), left(left), right(right)
    {
    }
};

std::ostream& operator << (std::ostream& out, TreeNode const& N)
{
    out << "(" << N.data << ", " << N.freq << ")";
    return out;
}

struct HuffTree
{
    TreeNode* root;

    HuffTree() = default; 

    HuffTree(std::map<char, int> freq)
        :root(nullptr)
    {
        std::priority_queue<std::pair<char, int>, std::vector<std::pair<char, int>>,
                            FreqComparator> pq;
        for (auto pair: freq)
            pq.push(pair);
        for ( ; !pq.empty(); )
        {
            TreeNode* lnode = new TreeNode(pq.top());
            pq.pop();
            TreeNode* rnode = new TreeNode(pq.top());
            pq.pop();
            TreeNode* parent = new TreeNode('\0', lnode->freq + rnode->freq, lnode, rnode);
            cout << *parent << endl;
            push(parent);
            break;
        }
    }
    
    void push(TreeNode* node)
    {
        if (root == nullptr)
        {
            root = node;
            return;
        }
        TreeNode* p = root;
        for ( ; p->right != nullptr; p = p->right);
        p->right = node;
    }

private:
    static void delete_tree(TreeNode* root)
    {
        if (root == nullptr)
            return;
        delete_tree(root->left);
        delete_tree(root->right);
        delete root;
    }

    static void repr_tree(TreeNode* root, std::ostream& out)
    {// pre-order traversal
        if (root == nullptr)
            return;
        out << *root << " "; 
        repr_tree(root->left, out);
        repr_tree(root->right, out);
    }

public: 
    ~HuffTree()
    {
        delete_tree(root);
    }
    
    friend std::ostream& operator << (std::ostream& out, HuffTree const& T);
};

std::ostream& operator << (std::ostream& out, HuffTree const& T)
{
    HuffTree::repr_tree(T.root, out);
    return out;
}

struct FileEncoder
{
    const char* path;
    std::map<char, int> freq;
    HuffTree tree;

    FileEncoder() = default;

    FileEncoder(const char* path)
        :path(path), freq(get_freq(path)), tree(freq)
    {
        cout << tree << endl;
    }

    static std::map<char, int> get_freq(const char* path)
    {
        std::map<char, int> freq;
        std::ifstream fin(path);
        for (std::string line; getline(fin, line); )
        {
            for (char c: line)
                ++freq[c]; 
        }
        fin.close();
        return freq;
    }
    
    void to_huff(const char* out_path, const char* tree_path)
    {
        std::ifstream fin(out_path);
        std::ofstream fout(out_path);
        
    }

    void from_huff()
    {
    }


    void write(std::string const& d, const char* path)
    {
    }
};

};

int main(int argc, char* argv[])
{
    using namespace Huffman;
    if (argc > 2)
        throw std::domain_error("Invalid command line arguments.");
    FileEncoder encoder;
    if (argc == 2)
        encoder = FileEncoder(argv[1]);

    //FileEncoder encoder("/lorem.txt");
    for (auto [key, value]: encoder.freq)
    {
        cout << key << " " << value << "\n";
    }

    cout << encoder.tree << endl;
}
