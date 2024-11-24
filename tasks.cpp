#include <iostream>

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
#include <queue>

#include "sort.h"

using std::cout, std::endl;

struct Task
{
    int start_time;
    int finish_time;

    bool operator < (Task const& other) const
    {
        return finish_time < other.finish_time;
    }
};

std::ostream& operator << (std::ostream& out, Task t)
{
    out << "{" << t.start_time << ", " << t.finish_time << "}";
    return out;
}

bool conflicting(Task const& curr, Task const& other)
{
    return ((curr.finish_time > other.start_time) && (curr.start_time < other.finish_time))
        || ((curr.start_time < other.finish_time) && (curr.start_time > other.finish_time));
}

struct Machine
{
    Task current_task;
    std::vector<Task> history;
};

std::vector<Task> schedule_single(std::vector<Task> T, int end_time)
{
    std::priority_queue<Task> tasks;
    for (Task t: T)
        tasks.push(t);
    Machine machine;
    for (Task task = tasks.top(); !tasks.empty(); tasks.pop())
    {
        if (!conflicting(machine.current_task, task))
        {
            machine.current_task = task;
            machine.history.push_back(task);
        }
    }
    return machine.history;
}

struct VTask
    :public Task
{
    int value;
};

std::ostream& operator << (std::ostream& out, VTask t)
{
    out << "{" << t.start_time << ", " << t.finish_time << ", " << t.value << "}";
    return out;
}

bool conflicting(VTask const& curr, VTask const& other)
{
    return ((curr.finish_time > other.start_time) && (curr.start_time < other.finish_time))
        || ((curr.start_time < other.finish_time) && (curr.start_time > other.finish_time));
}

std::vector<VTask> schedule_telescope(std::vector<VTask> tasks)
{
    std::vector<bool> keep(tasks.size());
    std::sort(tasks.begin(), tasks.end());
    //cout << tasks << endl;
    std::vector<int> pred(tasks.size());
    for (size_t i = 0; i < tasks.size(); ++i)
    {
        for (int j = i; j >= 0; --j)
        {
            if (!conflicting(tasks[i], tasks[j]))
            {
                pred[i] = j + 1;
                break;
            }
            if (j == 0)
                pred[i] = 0;
        }
    }
    //cout << "pred=" << pred << endl;

    std::vector<int> B(tasks.size() + 1);
    B[0] = 0;
    for (size_t i = 1; i < B.size(); ++i)
    {
        int new_benefit = B[pred[i-1]] + tasks[i-1].value;
        //cout << "value[i]=" << tasks[i-1].value << endl;
        if (new_benefit > B[i-1])
        {
            B[i] = new_benefit;
            keep[i-1] = true;
        }
        else
        {
            B[i] = B[i - 1];
            keep[i-1] = false;
        }
        //cout << B << endl;
    }
    cout << "Max Value = " << B.back() << endl;
    
    int j = 0;
    for (int i = keep.size() - 1; i >= 0; --i)
    {
        if (keep[i])
        {
            j = i;
            break;
        }
    }

    std::vector<VTask> schedule;
    for (; j >= 0; )
    {
        if (keep[j])
        {
            schedule.push_back(tasks[j]);
            j = pred[j] - 1;
        }
        else
            --j;
    }
    std::reverse(schedule.begin(), schedule.end());
    return schedule;
}

int main()
{
    //std::vector<Task> tasks = {{12, 14}, {5, 9}, {0, 6}, {6, 10}, {1, 4}, {3, 8}, {8, 12}, {2, 13}};
    //cout << schedule_single(tasks, 14) << endl;
    std::vector<VTask> tasks = {{1, 2, 5}, {1, 3, 4}, {2, 4, 7}, {3, 5, 2}, {1, 6, 3},
                                {4, 7, 5}, {6, 8, 7}, {7, 9, 4}};
    cout << schedule_telescope(tasks) << endl;
    return 0;
}
