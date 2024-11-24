#pragma once

#include <iostream>
#include <iomanip>
using std::cout, std::endl;
#include <vector>
#include <algorithm>
#include <random>

#include "timer.h"
#include "unordered_list.h"
#include "heap.h"

#define Vector std::vector<T> 

namespace utilities
{
    template <typename T = int>
    std::vector<T> get_random(size_t size)
    {
        std::vector<T> A;
        A.reserve(size);
        for (size_t i = 0; i < size; ++i)
            A.push_back(i);
        std::random_device rd;
        std::mt19937 generator(rd());
        std::shuffle(A.begin(), A.end(), generator);
        return A;
    }

    template <typename T = int>
    bool is_sorted(Vector const& A)
    {
        for (size_t i = 1; i < A.size(); ++i)
        {
            if (A[i] < A[i - 1])
                return false;
        }
        return true;
    }

    template <typename T>
    std::ostream& operator << (std::ostream& out, std::vector<T> v)
    {
        out << '[';
        size_t size = v.size();
        for (size_t i = 0; i < size - 1; ++i)
        {
            out << v[i] << ", ";
        }
        out << v[size - 1];
        out << ']';
        return out;
    }

    template <typename T>
    void print_range(std::vector<T> const& A, size_t begin, size_t end) // end is 1 elem to the right of last elem in range
    {
        if (begin == end)
        {
            cout << "[]";
            return;
        }
        cout << '[';
        for (size_t i = begin; i < end - 1; ++i)
        {
            cout << A[i] << ", ";
        }
        cout << A[end - 1];
        cout << ']';
    }

    void test(std::function<void(std::vector<int>&)> sort_fun,
              size_t num_times, size_t array_size = 10) // must pass <int> instantiation of function to be tested
    {
        std::random_device rd;
        std::mt19937 generator(rd());
        std::vector<int> A = get_random<int>(array_size);
        for (size_t i = 0; i < num_times; ++i)
        {
            sort_fun(A);
            if (!is_sorted(A))
            {
                cout << "Test did not pass!\n";
                cout << "A: " << A << endl;
                return;
            }
            std::shuffle(A.begin(), A.end(), generator);
        }
        cout << "All " << num_times << " tests passed" << endl;
    }

    void timed_test(std::function<void(std::vector<int>&)> sort_fun)
    {// Runs 100 sorts of randomly shuffled arrays of length 100.
        constexpr size_t num_times = 100;
        constexpr size_t array_size = 100;
        Timer T;
        double time_elapsed;
        T.start();
        test(sort_fun, num_times, array_size);
        T.elapsedUserTime(time_elapsed);
        cout << "Time elapsed: " << time_elapsed << "s" << endl;
    }

    std::vector<double> asymp_test(std::function<void(std::vector<int>&)> sort_fun)
    {
        constexpr size_t num_times = 100;
        constexpr size_t array_size = 1000;
        Timer T;
        double time_elapsed;
        double old_time = 0.0;
        std::vector<double> times(10);
        for (int cour = 1; cour <= 10; ++cour)
        {
            T.start();
            test(sort_fun, num_times * cour, array_size);
            T.elapsedUserTime(time_elapsed);
            cout << "\tTime elapsed: " << time_elapsed << "s" << endl;
            cout << "\t\t\t(+" << time_elapsed - old_time << "s)" << endl;
            old_time = time_elapsed;
            times[cour - 1] = time_elapsed;
        }
        cout << times << endl;
        return times;
    }
};

template <typename T>
void pq_sort(Vector& A);

template <typename T>
void insertion_sort(Vector& A);

template <typename T>
void bubble_sort(Vector& A);

template <typename T>
void selection_sort(Vector& A);

template <typename T>
void divide(Vector& A, size_t begin, size_t end, std::string side); // end is index of last elem in range

template <typename T>
void merge_sort(Vector& A);

template <typename T>
void quicksort(Vector& A);

template <typename T>
void heapify(ArrayHeap<T>& A);

template <typename T>
void heap_sort(Vector& A);

template <typename T>
void five_sort(Vector& A);

void bucket_sort(std::vector<int>& A, size_t num_buckets, size_t min, size_t max);

template <typename T>
void radix_sort(Vector& A, size_t num_buckets, size_t min, size_t max);

std::vector<int> counting_sort(std::vector<int>& A, int max);
