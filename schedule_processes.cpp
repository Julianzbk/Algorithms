#include <iostream>
using std::cout, std::endl;

#include <vector>
template <typename T>
std::ostream& operator<< (std::ostream& out, std::vector<T> v)
{
    out << '[';
    int i = 0;
    int size = v.size();
    for (; i < size - 1; ++i)
    {
        out << v[i] << ", ";
    }
    out << v[size - 1];
    out << ']';
    return out;
}
#include <tuple>

#include <format>
#include <algorithm>
#include <numeric>

constexpr std::string UNIT = "ms";

#pragma region uint_utility
unsigned int safe_subtract(unsigned int lhs, unsigned int rhs)
{
    long diff = static_cast<long>(lhs) - static_cast<long>(rhs);
    if (diff < 0)
        throw std::overflow_error("Subtraction results in unsigned int wrap-around");
    return static_cast<unsigned int>(diff);
}

unsigned int safe_decrement(unsigned int i)
{
    if (static_cast<long>(i) - 1 < 0)
        throw std::overflow_error("Decremented unsigned int beyond 0");
    return i - 1;
}

unsigned int safe_uint(const long literal)
{
    if (literal < 0)
        throw std::overflow_error("Assigned negative number literal to unsigned int");
    return static_cast<unsigned int>(literal);
}
#pragma endregion uint_utility


struct Process
{
    unsigned int id;
    unsigned int arrival_time;
    unsigned int burst_time;
    unsigned int priority;

    #pragma region Process_methods
    Process() = default;

    Process(unsigned int id, unsigned int arrival_time, unsigned int burst_time, unsigned int priority = 0)
        :id(id), arrival_time(arrival_time), burst_time(burst_time), priority(priority)
    {
    }

    unsigned int PID() const
    {
        return id;
    }

    unsigned int arrival() const
    {
        return arrival_time;
    }

    unsigned int burst() const
    {
        return burst_time;
    }

    void print(std::ostream& out)
    {
        out << "Process{\n\tPID: " << id << "\n\tArrival = " << arrival_time;
        out << "\n\tBurst = " << burst_time << "\n\tPriority = " << priority << "}";
    }
    #pragma endregion Process_methods

    friend std::ostream& operator << (std::ostream& out, Process const& P)
    {
        out << "Process(PID: " << P.id << "\n\tArrival = " << P.arrival_time;
        out << "\n\tBurst = " << P.burst_time << "\n\tPriority = " << P.priority << ")";
        return out;
    }
};

class ProcessTable
    :public std::vector<Process>
{
    struct _Process_noid
    {
        long arrival_time;
        long burst_time;
        long priority;

        _Process_noid(long arrival_time, long burst_time, long priority = 0)
            :arrival_time(arrival_time), burst_time(burst_time), priority(priority)
        {
        }

        inline bool has_negative_fields() const
        {
            return (arrival_time < 0) || (burst_time < 0) || (priority < 0);
        }
    };
public:
    using std::vector<Process>::size;

    ProcessTable(std::vector<_Process_noid> table)
        :std::vector<Process>(table.size())
    {
        for (unsigned int i = 0; i < table.size(); ++i)
        {
            _Process_noid const& process = table[i];
            if (process.has_negative_fields())
                throw std::domain_error("Negative values");
            this->at(i) = Process(i + 1,
                                  process.arrival_time,
                                  process.burst_time,
                                  process.priority);
        }
    }

    friend std::ostream& operator << (std::ostream& out, ProcessTable const& P)
    {
        out << "Table[\n";
        int i = 0;
        int size = P.size();
        for (; i < size - 1; ++i)
        {
            out << P[i] << ",\n";
        }
        out << P[size - 1];
        out << "\n]";
        return out;
    }
};


class Scheduler
{
public:
    std::string name;
    ProcessTable table;
    unsigned int total_burst_time;
    std::vector<unsigned int> waiting_times;
    std::vector<unsigned int> turnaround_times;
    
    void check_for_arrival(unsigned int t, unsigned int begin_checking_at,
                                         unsigned int last_process_id = -1)
    {
        for (size_t i = begin_checking_at; i < table.size(); ++i)
        {
            Process const& proc = table[i];
            if (t > proc.arrival_time)
            {
                cout << std::format("t = {}: Process {} arrived", proc.arrival_time, proc.id);
                if (last_process_id != std::numeric_limits<unsigned int>::max())
                    cout << std::format(" during execution of Process {} \n", last_process_id);
                else
                    cout << endl;
                ++begin_checking_at;
            }
        }
    }

#pragma region utilities
    Process const& select_first_process(unsigned int t, unsigned int begin_checking_at) const
    {/*
        Selects the first Process in (sorted according to scheduling policy) this->table
        that has arrived before current time t.
        returns the frontmost Process in table if none has arrived.
     */
        for (size_t i = begin_checking_at; i < table.size(); ++i)
        {
            Process const& proc = table[i];
            if (t >= proc.arrival_time)
            {
                ++begin_checking_at;
                return proc;
            }
        }
        return table[0];
    }
    
    static inline unsigned int sum(std::vector<unsigned int> const& V)
    {
        return std::accumulate(V.begin(), V.end(), 0);
    }
    
    static inline std::string format_time(unsigned int t)
    {
        return std::format("t = {}: ", t);
    }

#pragma endregion utilities

    Scheduler(ProcessTable const& table, std::string name = "InvalidBaseScheduler")
        :name(name), table(table), total_burst_time(0),
         waiting_times(table.size()), turnaround_times(table.size())
    {
        for (Process const& p: table)
        {
            total_burst_time += p.burst_time;
        }
    }

    #pragma region virtual_base_methods
    virtual void schedule() = 0;
    virtual double avg_waiting_time() = 0;
    virtual double avg_turnaround_time() = 0;

    virtual ~Scheduler()
    {
    }
#pragma endregion virtual_base_methods
};


class FCFS_Scheduler
    :public Scheduler
{
public:
    FCFS_Scheduler(ProcessTable const& table)
        :Scheduler(table, "First-Come, First-Served Scheduler")
    {
    }

    virtual void schedule() override
    {
        #define TIME format_time(t)

        auto cmp_arrival = [](Process const& lhs, Process const& rhs)
                           {return lhs.arrival_time < rhs.arrival_time;};
        std::stable_sort(table.begin(), table.end(), cmp_arrival);
        size_t proc_i = 0;
        for (unsigned int t = 0; t < total_burst_time; )
        {
            Process const& proc = select_first_process(t, proc_i);
            if (t < proc.arrival_time) // then Process has not arrived;
                t = proc.arrival_time; // wait till Process arrives;
            
            cout << TIME << "Process " << proc.id << " began\n";
            waiting_times[proc_i] = t - proc.arrival_time;

            t += proc.burst_time;
            check_for_arrival(t, proc_i + 1, proc.id);
            cout << TIME << std::format("Process {} finished ({}{})\n", proc.id, proc.burst_time, UNIT);
            turnaround_times[proc_i] = t - proc.arrival_time;

            ++proc_i;
        }
    }

    virtual double avg_waiting_time() override
    {
        cout << waiting_times << endl;
        return sum(waiting_times) / table.size();
    }

    virtual double avg_turnaround_time() override
    {
        cout << turnaround_times << endl;
        return sum(turnaround_times) / table.size();
    }
};


class SJF_Scheduler // non-preemptive
    :public Scheduler
{
public:
    SJF_Scheduler(ProcessTable const& table)
        :Scheduler(table, "Shortest-Job-First")
    {
    }

    virtual void schedule() override
    {
        #define TIME format_time(t)

        auto cmp_burst = [](Process const& lhs, Process const& rhs)
                         {return lhs.burst_time < rhs.burst_time;};
        std::stable_sort(table.begin(), table.end(), cmp_burst);
        cout << table << endl;
        size_t proc_i = 0;
        for (unsigned int t = 0; t < total_burst_time; )
        {
            Process const& proc = select_first_process(t, proc_i);
            if (t < proc.arrival_time) // then Process has not arrived;
                t = proc.arrival_time; // wait till Process arrives;
            cout << TIME << "Process " << proc.id << " began\n";
            waiting_times[proc_i] = t - proc.arrival_time;

            t += proc.burst_time;
            check_for_arrival(t, proc_i + 1, proc.id);
            cout << TIME << std::format("Process {} finished ({}{})\n", proc.id, proc.burst_time, UNIT);
            turnaround_times[proc_i] = t - proc.arrival_time;

            ++proc_i;
        }
    }

    virtual double avg_waiting_time() override
    {
        cout << waiting_times << endl;
        return sum(waiting_times) / table.size();
    }

    virtual double avg_turnaround_time() override
    {
        cout << turnaround_times << endl;
        return sum(turnaround_times) / table.size();
    }
};


void eval_all(ProcessTable const& table)
{
    Scheduler* schedulers[] = {new SJF_Scheduler(table)};
    for (Scheduler* sched: schedulers)
    {
        cout << sched->name << ":\n";
        sched->schedule();
        cout << "Average Waiting time: " << sched->avg_waiting_time() << "\n";
        cout << "Average Turnaround time: " << sched->avg_turnaround_time() << "\n";
        delete sched;
    }    
}

#include <cassert>
void test()
{
    ProcessTable table1({{0, 60, 3}, {3, 30, 2}, {4, 40, 1}, {9, 10, 4}});
    FCFS_Scheduler scheduler(table1);
    scheduler.schedule();
    assert(scheduler.avg_waiting_time() == 66.0);
    assert(scheduler.avg_turnaround_time() == 101.0);
    cout << "All tests passed!\n* * * * * * * * * *" << endl;
}

int main()
{
    //test();
    ProcessTable table1({{0, 60, 3}, {3, 30, 2}, {4, 40, 1}, {9, 10, 4}});
    ProcessTable table2({{0, 24}, {0, 3}, {0, 3}});
    eval_all(table1);
}