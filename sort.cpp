#include <iostream>
#include <iomanip>
using std::cout, std::endl;

#include <vector>
#include <algorithm>
#include <random>

#include "timer.h"
#include "unordered_list.h"
#include "sort.h"
#include "heap.h"

#define Vector std::vector<T> 

using namespace utilities;

// "A" always refer to the array to be sorted
template <typename T>
void pq_sort(Vector& A)
{
/*
    Get a min PQ to sort it out for you.
    T = O(n^2)
    S = O(n)
*/
    UnorderedList<T> pq; //std::priority_queue<T, std::vector<T>, std::less> pq;
    for (T elem: A)
        pq.insert(elem);
    for (size_t i = 0; i < A.size(); ++i)
    {
        A[i] = pq.pop_min();
    }
}

template <typename T>
void insertion_sort(Vector& A)
{
/*
    Assume elems to the left are sorted and insert curr at the right place.
    T = O(n^2)
    S = in-place
*/
    for (size_t i = 1; i < A.size(); ++i)
    {
        T curr = A[i];
        size_t j_sorted = 0;
        for ( ; curr > A[j_sorted]; ++j_sorted);
        size_t j_unsorted = j_sorted; // = correct spot for curr 
        for (j_unsorted = i ; j_unsorted > j_sorted; --j_unsorted)
        {
            A[j_unsorted] = A[j_unsorted - 1]; // move elems to the right
        }
        A[j_sorted] = curr; // fill in the correct spot
    }
}

template <typename T>
void bubble_sort(Vector& A)
{
/*
    For each elem: go through each elem to the right and swap if out of place.
    T = O(n^2)
    S = in-place
*/
    for (size_t i = 0; i < A.size(); ++i)
    {
        for (size_t j = i; j < A.size(); ++j)
        {
            if (A[j] < A[i])
                std::swap(A[j], A[i]);
        }
    }
}

template <typename T>
void selection_sort(Vector& A)
{
/*
    For each elem: find min of elems to the right and swap them w/ 
    T = O(n^2)
    S = in-place
*/
    for (size_t i = 0; i < A.size() - 1; ++i)
    {
        size_t index_min = i;
        for (size_t j = i + 1; j < A.size(); ++j) 
        {
            if (A[j] < A[index_min])
                index_min = j;
        } // gets the index of min element
        if (i != index_min)
        {
            std::swap(A[i], A[index_min]);
        }
    }
}

template <typename T>
void divide(Vector& A, size_t begin, size_t end, std::string side) // end is index of last elem in range
{
    cout << "side:" << side << " ";
    print_range(A, begin, end + 1);
    cout << endl;
    if (end - begin <= 0) // terminate at sublists w/ len <= 1 (aka single element)
        return; // base case
    size_t mid = (begin + end) / 2;
    divide(A, begin, mid, "left "); // for even number of elems, mid belongs to the left partition 
    divide(A, mid + 1, end, "right");
}

template <typename T>
void merge(Vector& A, size_t begin1, size_t end1, size_t begin2, size_t end2)
{
    Vector Aux(end2 - begin1 + 1); // incurs O(n) additional space
    size_t i_1 = begin1, i_2 = begin2, Aux_i = 0; // 3 index pointers
    //print_range(A, begin1, end1);
    //print_range(A, begin2, end2);
    for ( ; i_1 <= end1 && i_2 <= end2; )
    {// race to the end to see which side has the lesser elem, winner advances a step
        if (A[i_1] <= A[i_2]) // comparison here
        {
            Aux[Aux_i] = A[i_1];
            ++i_1;
        }
        else
        {
            Aux[Aux_i] = A[i_2];
            ++i_2;
        }
        ++Aux_i;
    }
    for ( ; i_1 <= end1; ++i_1) // copy in the remaining portion, which is already sorted
        Aux[Aux_i] = A[i_1];
    for ( ; i_2 <= end2; ++i_2)
        Aux[Aux_i] = A[i_2];
    //cout << Aux << endl;

    Aux_i = 0;
    for (size_t i = begin1; i <= end2; )
    {// copy the contents of Aux from [0, end2 - begin1] to A at [begin1, begin1 + end2].
        A[i] = Aux[Aux_i];
        ++i;
        ++Aux_i;
    }
}

template <typename T>
void merge_recur(Vector& A, size_t begin, size_t end)
{
    if (end - begin <= 0)
    {// I'm using guard clauses at the top for base cases.
        return;
    }
    size_t mid = (begin + end) / 2;
    merge_recur(A, begin, mid);
    merge_recur(A, mid+1, end);
    merge(A, begin, mid, mid+1, end);
    print_range(A, begin, end);
    cout << endl;
}

template <typename T>
void merge_sort(Vector& A)
{
/*
    T = O(nlog(n))
    S = O(n)
*/
    merge_recur(A, 0, A.size());
}

template <typename T>
size_t select_pivot(Vector const& A, size_t begin, size_t end)
{// Naive implementation has pivot as first elem of subarray.
    return begin;
    /*
    std::random_device rd;
    std::uniform_int_distribution<int> dist(begin, end); 
    return dist(rd);
    */
    /*
    return median(begin, (begin + end)/2, end);
    */
}

template <typename T>
size_t partition(Vector& A, size_t begin, size_t end)
{// Naive implementation with pivot at 0
    size_t pivot = select_pivot(A, begin, end);
    T pivot_elem = A[pivot];
    for (size_t i = begin + 1; i <= end; ++i)  
    {// for each (excluding 0) compare, if smaller than pivot_elem swap with the elem after pivot, then advance pivot pointer.
        if (A[i] < pivot_elem) // comparison here
        {
            std::swap(A[pivot + 1], A[i]); // if i = 1 then nothing happens
            ++pivot;
        }
    }
    std::swap(A[begin], A[pivot]);
    return pivot;
}


template <typename T>
void qsort_recur(Vector& A, size_t begin, size_t end)
{
    if (end - begin <= 0)
        return;

    size_t pivot = partition(A, begin, end);
    qsort_recur(A, begin, pivot);
    qsort_recur(A, pivot+1, end);
}

template <typename T>
void quicksort(Vector& A)
{
    qsort_recur(A, 0, A.size());
}

template <typename T>
void heap_sort(Vector& A)
{
}


template <typename T>
void five_sort(Vector& A)
{
    if (A.size() != 5)
        throw std::domain_error("Invalid length");

}

template <typename T>
void bucket_sort(Vector& A, size_t num_buckets)
{
}

template <typename T>
void radix_sort(Vector& A, size_t num_buckets)
{
}

int main(int argc, char* argv[])
{
    //auto A = get_random<int>(10);
    /*
    cout << "A: " << A << endl;
    pq_sort(A);
    cout << "S: " << A << endl;
    A = get_random<int>(10);
    cout << "A: " << A << endl;
    insertion_sort(A);
    cout << "S: " << A << endl; 
    test(bubble_sort<int>, 10);
    test(selection_sort<int>, 10);
    divide(A, 0, 9, "start");
    cout << endl;
    divide(A, 0, 8, "start");
    asymp_test(pq_sort<int>);
    asymp_test(insertion_sort<int>);
    asymp_test(selection_sort<int>);
    asymp_test(bubble_sort<int>);
    */
    ArrayHeap<int> A = {4, 5, 6, 15, 9, 7, 20};
    cout << A << endl;
}
