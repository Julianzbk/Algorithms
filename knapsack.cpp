#include <iostream>

#include <vector>
template <typename T>
std::ostream& operator << (std::ostream& out, std::vector<T> const& v)
{
    out << "[";
    for (T elem: v)
    {
        out << elem << ", ";
    }
    out << "\b\b" << "]";
    return out;
}
/*

#include <array>
template <typename T, size_t size> 
std::ostream& operator << (std::ostream& out, std::array<T, size> const& A)
{
    out << "[";
    for (T elem: A)
    {
        out << elem << " ";
    }
    out << '\b' << "]";
    return out;
}
*/

#include "sort.h"

using std::cout, std::endl;

struct Object
{
    std::string name;
    double value;
    double weight;

    Object(std::string name, double value, double weight)
        :name(name), value(value), weight(weight)
    {
    }

    bool operator < (Object const& other)
    {
        return (value / weight) < (other.value / other.weight);
    }

    friend Object operator * (double factor, Object const& O)
    {
        return Object{O.name, O.weight * factor, O.value * factor};
    }

    friend Object operator * (Object const& O, double factor)
    {
        return Object{O.name, O.weight * factor, O.value * factor};
    }

    friend std::ostream& operator << (std::ostream& out, Object const& O)
    {
        out << "Object(" << O.name << ", " << O.weight << ", " << O.value << ")";
        return out;
    }
};

struct Sack
{
    std::vector<Object> contents;
    double value;
    double weight;

    void add(Object O)
    {
        weight += O.weight;
        value += O.value;
        contents.push_back(O);
    }

    friend std::ostream& operator << (std::ostream& out, Sack const& S)
    {
        out << "Sack(" << S.weight << ", " << S.value << "), ";
        out << "contents: " << S.contents;
        return out;
    }
};

std::vector<Sack> distribute_fractional(std::vector<Object> const& objects, size_t num_sacks, size_t capacity)
{
    /*
        Optimal, O(nlog(n)), n=objects.size()
    */
    std::vector<Sack> sacks(num_sacks);
    std::sort(objects.begin(), objects.end(), [](Object A, Object B){return !(A < B);});
    //maxheap(objects);
    cout << "Sorted Objects:" << objects << endl;
    size_t i = 0;
    for (Object O: objects)
    {
        if (i >= num_sacks)
            break;
        if (sacks[i].weight + O.weight > capacity)
        {
            double ratio = (capacity - sacks[i].weight) / O.weight;
            cout << "Added " << ratio << " of " << O << '\n';
            sacks[i].add(ratio * O);
            cout << "Weight / Capacity = " << sacks[i].weight << " / " << capacity << '\n';
            cout << "Value = " << sacks[i].value << '\n';
            cout << "\tSack " << i << " is now full.\n";
            ++i;
        }
        else
        {
            cout << "Added all of " << O << '\n';
            sacks[i].add(O);
            cout << "Weight / Capacity = " << sacks[i].weight << " / " << capacity << '\n';
            cout << "Value = " << sacks[i].value << '\n';
        }
        cout << endl;
    }
    return sacks;
}

#include "matrix.hpp"

struct OpTable
    :public Matrix<int>
{
    using Matrix<int>::num_rows, Matrix<int>::num_columns, Matrix<int>::buf;
    OpTable(size_t length, size_t width)
        :Matrix<int>::Matrix(length, width)
    {
        for (size_t i = 0; i < num_rows; ++i)
            buf[i][0] = 0;
        for (size_t j = 0; j < num_columns; ++j)
            buf[0][j] = 0;
    }

    int max_value() const
    {
        return buf[num_rows-1][num_columns-1];
    }
};

struct KeepTable
    :public Matrix<bool>
{
    using Matrix<int>::num_rows, Matrix<int>::num_columns, Matrix<int>::buf;
    KeepTable(size_t length, size_t width)
        :Matrix<int>::Matrix(length, width)
    {
        for (size_t i = 0; i < num_rows; ++i)
            for (size_t j = 0; j < num_columns; ++j)
                buf[i][j] = false;
    }
};

Sack distribute_whole(std::vector<Object> const& objects, size_t capacity)
{
    OpTable B(objects.size() + 1, capacity + 1);
    KeepTable keep(objects.size(), capacity);
    for (int i = 1; i < B.num_rows; ++i)
    {
        int curr_weight = objects[i].weight;
        for (int j = 1; i < B.num_columns; ++j)
        {
            int new_weight = curr_weight + B[i-1][j - curr_weight];
            if ((curr_weight > j) || new_weight <= B[i-1][j])
            {
                B[i][j] = B[i-1][j];
                keep[i][j] = false;
            }
            else
            {
                B[i][j] = new_weight;
                keep[i][j] = true;
            }   
        }
    }
    cout << "Max Value= " << B.max_value() << endl;
    
    Sack sack;
    for (int i = 0, j = 0; i >= 0; --i)
    {
        if (keep[i][j])
        {
            j -= objects[i].weight;
            sack.contents.push_back(objects[i]);
        }
    }
    return sack;
}

int main()
{
    //std::vector<Object> objects = {Object("1", 10, 10), Object("2", 20, 40), Object("3", 4, 5), Object("4", 12, 6)};
    //cout << distribute_fractional(objects, 1, 40) << endl;
    //cout << distribute_whole({Object("Red", 2, 5), Object("Green", 3, 6)}, 1, 10) << endl;
    //objects = {Object("a", 4, 12), Object("b", 6, 10), Object("c", 5, 8), Object("d", 7, 11), Object("e", 3, 14), Object("f", 1, 7), Object("g", 6, 9)};
    //cout << distribute_fractional(objects, 1, 18) << endl;
    std::vector<Object> objects = {Object("a", 6, 3), Object("b", 5, 2), Object("c", 7, 4), Object("d", 3, 1)};
    //std::vector<Object> objects = {Object("a", 12, 4), Object("b", 10, 6), Object("c", 8, 5), Object("d", 11, 7), Object("e", 14, 3), Object("f", 7, 1), Object("g", 9, 6)};
    cout << distribute_whole(objects, 8) << endl;
}
