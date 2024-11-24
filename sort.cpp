#include "sort.h"
using namespace utilities;

bool PRINT = true; // Change to false to turn off debug print()'s.

// Call print(Vector) anywhere!
template <typename T>
inline void print(Vector& A)
{
    if (PRINT)
        cout << A << endl;
}

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
        print(A);
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
    if ((int) end - (int) begin <= 0) // terminate at sublists w/ len <= 1 (aka single element)
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
    print(Aux);

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
    if (begin >= end)
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
    Divide Array into pieces of 1, then merge them together in the correct order.
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
    if (begin >= end)
        return;
    
    size_t pivot = partition(A, begin, end);
    qsort_recur(A, begin, pivot-1);
    qsort_recur(A, pivot+1, end);
}

template <typename T>
void quicksort(Vector& A)
{
    qsort_recur(A, 0, A.size() - 1);
}

#define ArrayHeap ArrayHeap<T>

template <typename T>
void sift_up(ArrayHeap& A, size_t i)
{
    T elem = A[i];
    for (size_t parent = A[i - i] / 2; A[parent] < elem; parent = A[i - i] / 2)
    {
        if (i <= 0) // reached the root
            break;
        std::swap(A[i], A[parent]);
        i = parent;
    }
}

template <typename T>
inline size_t larger_child(ArrayHeap& A, size_t i)
{
    size_t left = 2 * i + 1;
    size_t right = left + 1;
    if (right >= A.size())
        return left;
    if (left >= A.size()) // should be never case
        throw std::domain_error("i is a leaf node");
    if (A[left] > A[right])
        return left;
    else
        return right;
}

template <typename T>
void sift_down(ArrayHeap& A, size_t i)
{
    size_t n = A.size();
    T elem = A[i];
    for (size_t child = larger_child(A, i); elem < child; i = child)
    {
        if (child >= n)
            break; // reached the end
        std::swap(A[i], A[child]);
        child = larger_child(A, i);
    }
}

template <typename T>
void heapify(ArrayHeap& A)
{
    for (size_t i = (A.size() - 2) / 2; i >= 0; --i)
        sift_down(A, i);
}

template <typename T>
void heap_sort(Vector& Array)
{
    ArrayHeap& A = static_cast<ArrayHeap&>(Array);
    heapify(A);
    size_t end = A.size();
    while (end > 1)
    {
        --end;
        std::swap(A[end], A[0]);
        sift_down(A, end);
    }
}

template <typename T>
void five_sort(Vector& A)
{
    print(A);
    if (A.size() != 5)
        throw std::domain_error("Invalid length");
    if (A[0] > A[1])
        std::swap(A[0], A[1]);
    if (A[2] > A[3])
        std::swap(A[2], A[3]);
    if (A[1] > A[3])
    {
        std::swap(A[1], A[3]);
        std::swap(A[0], A[2]);
    }
    T E = A[4];
    T C_value = A[2];
    print(A);
    if (E <= A[0])
    {
        for (size_t i = 4; i > 0; --i)
            A[i] = A[i - 1];
        A[0] = E;
    }
    else if (E <= A[1])
    {
        for (size_t i = 4; i > 1; --i)
            A[i] = A[i - 1];
        A[1] = E;
    }
    else // TODO
    {
        A[4] = A[3];
        A[3] = E;
    }
    print(A);
    size_t i = 0;
    for (; i < A.size(); ++i)
    {
        if (A[i] == C_value)
            break;
    }
    T& C = A[i];
    cout << C << endl;
    if (C <= A[1]) 
    {
        std::swap(A[1], C);
    }
    else if (C <= A[2])
    {
        std::swap(A[2], C);
    }
    else
    {
        std::swap(A[4], C);
    }


}

void bucket_sort(std::vector<int>& A, size_t num_buckets, size_t min, size_t max)
{
    auto buckets = new std::vector<int>[num_buckets];
    for (int elem: A)
    {
        size_t key = (elem * num_buckets) / (max + 1);
        buckets[key].push_back(elem);
    }
    size_t i = 0;
    size_t bucket_size = (max - min + 1) / (float)num_buckets;
    bool is_uneven = (max - min + 1) % num_buckets == 1; // TODO has a lot more cases
    for (size_t j = 0; j < num_buckets; ++j)
    {
        std::vector<int>& bucket = buckets[j];
        if (!is_uneven)
            cout << "[" << min + bucket_size * j << ", " << min + bucket_size * (j+1) << "): "; 
        else
            cout << "[" << min + bucket_size * j << ", " << min + bucket_size * (j+1) << "]: "; 
        std::sort(bucket.begin(), bucket.end()); // efficient sort for small A
        if (bucket.size() != 0)
            cout << bucket << endl;
        else
            cout << endl;
        for (int elem: bucket)
        {
            A[i] = elem;
            ++i;
        }
    }
    delete[] buckets;
}

template <typename T>
void radix_sort(Vector& A, size_t num_buckets, int min, int max)
{
    for (int i = 0; i < std::log10(max); ++i)
    {
        for (int elem: A)
        {
            int digit = elem - (elem % (unsigned long) pow(10, i));
        }
    }
}

std::vector<int> counting_sort(std::vector<int>& A, int max)
{
    std::vector<int> locator(max, 0);
    for (int i = 0; i < (int) A.size(); ++i)
        ++locator[A[i] - 1];
    cout << "Locator:" << locator << endl;
    for (int i = 1; i < max; ++i)
        locator[i] += locator[i - 1];
    cout << "Locator:" << locator << endl;
    /* Naive implementation of locator initialization
    for (int i = 0; i < locator.size(); ++i)
    {
        int counter = 0;
        for (int elem: A)
        {
            if (elem <= (i + 1))
                ++counter;
        }
        locator[i] = counter;
    }
    */
    std::vector<int> B(A.size() + 1);
    for (int i = A.size() - 1; i >= 0; --i)
    {
        B[locator[A[i] - 1]] = A[i];
        cout << "Copied " << (i + 1)  << " to " << locator[A[i] - 1] << endl;
        cout << "B: " << B << endl;
        --locator[A[i] - 1];
        cout << "Locator:" <<  locator << endl;
    }
    B.erase(B.begin()); // expensive, O(n)
    return B;
}

void example()
{
    auto A = get_random<int>(10);
    cout << "A: " << A << endl;
    PRINT = true;
    selection_sort(A);
    cout << "S: " << A << endl;
    asymp_test(selection_sort<int>);
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
    cout << "S: " << A << endl;
    */
    /*
    std::vector<int> A;
    for (int i = 1; i <= 12; ++i)
    {
        A = get_random<int>(i);
        bucket_sort(A, 3, 0, A.size() - 1);
        cout << "S: " << A << endl;
    }
    */
    std::vector<int> A{1, 3, 5, 7, 5, 7, 3, 8, 7, 4};
    cout << "S: " << counting_sort(A, 8) << endl << endl;
    A = {3, 4, 4, 1, 2, 3, 3, 3, 2};
    cout << "S: " << counting_sort(A, 4) << endl;
    /*
    std::vector<int> A = get_random<int>(5);
    five_sort(A);
    cout << "S: " << A << endl;
    */
}
