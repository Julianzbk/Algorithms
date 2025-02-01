#include <iostream>
using std::cout, std::endl;

#include <format>
#include <algorithm>
#include <numeric>

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
#include <list>
template <typename T>
std::ostream& operator<< (std::ostream& out, std::list<T> L)
{
    out << '[';
    auto it = L.begin();
    for (; it != std::prev(L.end()); ++it)
    {
        out << *it << ", ";
    }
    out << *it;
    out << ']';
    return out;
}
#include <map>
std::ostream& operator<< (std::ostream& out, std::map<unsigned int, unsigned int> M)
{
    out << '{';
    for (auto [key, value]: M)
    {
        out << std::format("P{}: {}, ", key, value);
    }
    out << "\b\b}";
    return out;
}
#include <tuple>

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
    :public std::list<Process>
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
    using std::list<Process>::size;

    ProcessTable(std::initializer_list<_Process_noid> table)
        :std::list<Process>()
    {
        unsigned int i = 0;
        for (_Process_noid const& proc: table)
        {
            if (proc.has_negative_fields())
                throw std::out_of_range("Negative Process fields");
            this->push_back(Process(i + 1,
                                    proc.arrival_time,
                                    proc.burst_time,
                                    proc.priority));
            ++i;
        }
    }

    ProcessTable(std::vector<Process> table) // cant believe this worked LMAO
        :std::list<Process>()
    {
        for (Process proc: table)
        {
            this->push_back(proc);
        }
    }

    friend std::ostream& operator << (std::ostream& out, ProcessTable const& L)
    {
        out << "Table[\n";
        auto it = L.begin();
        for (; it != std::prev(L.end()); ++it)
        {
            out << *it << ",\n";
        }
        out << *it;
        out << "\n]";
        return out;
    }
};


class Scheduler
{
public:
    std::string name;
protected:
    ProcessTable table;
    unsigned int total_burst_time;
    std::map<unsigned int, unsigned int> waiting_times; // <PID, time>
    std::map<unsigned int, unsigned int> turnaround_times; // <PID, time>
    
    void check_for_arrival(unsigned int t, unsigned int last_process_id = -1)
    {
        for (Process const& proc: table)
        {
            if (t > proc.arrival_time)
            {
                cout << std::format("t = {}: Process {} arrived", proc.arrival_time, proc.id);
                if (last_process_id != std::numeric_limits<unsigned int>::max())
                    cout << std::format(" during execution of Process {} \n", last_process_id);
                else
                    cout << endl;
            }
        }
    }

#pragma region utilities
    auto select_first_process(unsigned int t) const
    {/*
        Selects the first Process in (sorted according to scheduling policy) this->table
        that has arrived before current time t.
        returns the frontmost Process in table if none has arrived.
        (return type is const iterator to Process)
     */
        for (auto proc = table.cbegin(); proc != table.cend(); ++proc)
        {
            if (t >= proc->arrival_time)
            {
                return proc;
            }
        }
        return table.cbegin();
    }
    
    static double average(std::map<unsigned int, unsigned int> const& M)
    {
        double acc = 0.0;
        for (auto [PID, t]: M)
        {
            acc += t;
        }
        return acc / M.size();
    }
    
    static inline std::string format_time(unsigned int t)
    {
        return std::format("t = {}: ", t);
    }

#pragma endregion utilities
public:
    Scheduler(ProcessTable const& table, std::string name = "InvalidBaseScheduler")
        :name(name), table(table), total_burst_time(0)
    {
        for (Process const& proc: table)
        {
            waiting_times[proc.id] = 0;
            turnaround_times[proc.id] = 0;
            total_burst_time += proc.burst_time;
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
        table.sort(cmp_arrival); // stable sort too!
        for (unsigned int t = 0; t < total_burst_time; )
        {
            auto _proc_it = select_first_process(t);
            Process const& proc = *_proc_it;
            if (t < proc.arrival_time) // then Process has not arrived;
                t = proc.arrival_time; // wait till Process arrives;
            
            cout << TIME << "Process " << proc.id << " began\n";
            waiting_times[proc.id] = t - proc.arrival_time;

            t += proc.burst_time;
            check_for_arrival(t, proc.id);
            cout << TIME << std::format("Process {} finished ({}{})\n", proc.id, proc.burst_time, UNIT);
            turnaround_times[proc.id] = t - proc.arrival_time;

            table.erase(_proc_it); // Finished processes are removed.
        }
        cout << "All processes finished in " << total_burst_time << UNIT << endl;
    }

    virtual double avg_waiting_time() override
    {
        cout << waiting_times << endl;
        return average(waiting_times);
    }

    virtual double avg_turnaround_time() override
    {
        cout << turnaround_times << endl;
        return average(turnaround_times);
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
        table.sort(cmp_burst);
        for (unsigned int t = 0; t < total_burst_time; )
        {
            auto _proc_it = select_first_process(t);
            Process const& proc = *_proc_it;

            if (t < proc.arrival_time) // then Process has not arrived;
                t = proc.arrival_time; // wait till Process arrives;
            cout << TIME << "Process " << proc.id << " began\n";
            waiting_times[proc.id] = t - proc.arrival_time;

            t += proc.burst_time;
            check_for_arrival(t, proc.id);
            cout << TIME << std::format("Process {} finished ({}{})\n", proc.id, proc.burst_time, UNIT);
            turnaround_times[proc.id] = t - proc.arrival_time;

            table.erase(_proc_it); // Finished processes are removed.
        }
        cout << "All processes finished in " << total_burst_time << UNIT << endl;
    }

    virtual double avg_waiting_time() override
    {
        cout << waiting_times << endl;
        return average(waiting_times);
    }

    virtual double avg_turnaround_time() override
    {
        cout << turnaround_times << endl;
        return average(turnaround_times);
    }
};

/*
class SRTF_Scheduler // preemptive
    :public Scheduler
{
public:    
    SRTF_Scheduler(ProcessTable const& table)
        :Scheduler(table, "Shortest-Remaining-Time-First (Preemptive SJF)")
    {
    }

    virtual void schedule() override
    {
        #define TIME format_time(t)

        auto cmp_burst = [](Process const& lhs, Process const& rhs)
                         {return lhs.burst_time < rhs.burst_time;};
        table.sort(cmp_burst);
        for (unsigned int t = 0; t < total_burst_time; )
        {
            auto _proc_it = select_first_process(t);
            Process const& proc = *_proc_it;

            if (t < proc.arrival_time) // then Process has not arrived;
                t = proc.arrival_time; // wait till Process arrives;
            cout << TIME << "Process " << proc.id << " began\n";
            waiting_times[proc.id] = t - proc.arrival_time;

            t += proc.burst_time;
            check_for_arrival(t, proc.id);
            cout << TIME << std::format("Process {} finished ({}{})\n", proc.id, proc.burst_time, UNIT);
            turnaround_times[proc.id] = t - proc.arrival_time;

            table.erase(_proc_it); // Finished processes are removed.
        }
        cout << "All processes finished in " << total_burst_time << UNIT << endl;
    }

    virtual double avg_waiting_time() override
    {
        cout << waiting_times << endl;
        return average(waiting_times);
    }

    virtual double avg_turnaround_time() override
    {
        cout << turnaround_times << endl;
        return average(turnaround_times);
    }
};
*/


class RR_Scheduler // non-preemptive
    :public Scheduler
{
    std::vector<unsigned int> arrived_procs;
    std::vector<unsigned int> began_procs;
    inline bool has_not_arrived(unsigned int PID)
    {
        return std::find(arrived_procs.begin(), arrived_procs.end(), PID) == arrived_procs.end();
    }

    inline bool has_not_begun(unsigned int PID)
    {
        return std::find(began_procs.begin(), began_procs.end(), PID) == began_procs.end();
    }


    void check_for_arrival(unsigned int t, unsigned int last_process_id = -1)
    {
        for (Process const& proc: table)
        {
            if (t > proc.arrival_time && has_not_arrived(proc.id))
            {
                cout << std::format("t = {}: Process {} arrived", proc.arrival_time, proc.id);
                arrived_procs.push_back(proc.id);
                if (last_process_id != std::numeric_limits<unsigned int>::max())
                    cout << std::format(" during execution of Process {} \n", last_process_id);
                else
                    cout << endl;
            }
        }
    }
public:
    unsigned int quantum;

    RR_Scheduler(unsigned int quantum, ProcessTable const& table)
        :Scheduler(table, std::format("Round Robin (quantum = {})", quantum)),
         quantum(quantum)
    {
        arrived_procs.reserve(table.size());
        began_procs.reserve(table.size());
    }

    virtual void schedule() override
    {
        #define TIME format_time(t)

        for (unsigned int t = 0; t < total_burst_time; )
        {
            for (auto _proc_it = table.begin(); _proc_it != table.end(); ++_proc_it)
            {
                Process& proc = *_proc_it;
                if (t < proc.arrival_time) // then Process has not arrived;
                    continue;
                if (has_not_begun(proc.id))
                {
                    cout << TIME << "Process " << proc.id << " began\n";
                    waiting_times[proc.id] = t - proc.arrival_time;
                    began_procs.push_back(proc.id);
                }
                else
                {
                    cout << TIME << "Process " << proc.id << " continuing\n";
                }
                // Process "execution" happens here
                if (proc.burst_time <= quantum) // if remaining time is less than quantum,
                {                              // Process finishes and is removed.
                    t += proc.burst_time;
                    check_for_arrival(t, proc.id);
                    cout << TIME << std::format("Process {} finished ({}{})\n", proc.id, proc.burst_time, UNIT);
                    turnaround_times[proc.id] = t - proc.arrival_time;                        
                    table.erase(_proc_it);
                    continue;
                }
                proc.burst_time -= quantum;
                t += quantum;
                check_for_arrival(t, proc.id);
                cout << TIME << std::format("Process {} finished it's turn ({}{} remaining)\n", proc.id, proc.burst_time, UNIT);
            }
        }
        cout << "All processes finished in " << total_burst_time << UNIT << endl;
    }

    virtual double avg_waiting_time() override
    {
        cout << waiting_times << endl;
        return average(waiting_times);
    }

    virtual double avg_turnaround_time() override
    {
        cout << turnaround_times << endl;
        return average(turnaround_times);
    }
};


void eval_all(ProcessTable const& table)
{
    Scheduler* schedulers[] = {new RR_Scheduler(4, table)};
    for (Scheduler* sched: schedulers)
    {
        cout << sched->name << ":\n";
        sched->schedule();
        cout << "Average Waiting time: " << sched->avg_waiting_time() << "\n";
        cout << "Average Turnaround time: " << sched->avg_turnaround_time() << "\n";
        delete sched;
    }    
}

#pragma region tests
#include <cassert>
#include <fstream>

void test()
{
    bool PRINT = true;
    #pragma region redirect_cout;
    std::ofstream nullstream; // sorta UB
    std::streambuf* backup_cout;
    if (PRINT)
    {
        backup_cout = std::cout.rdbuf();
        std::cout.rdbuf(nullstream.rdbuf());
    }
    #pragma endregion redirect_cout;

    ProcessTable table1({{0, 60, 3}, {3, 30, 2}, {4, 40, 1}, {9, 10, 4}});
    {
    FCFS_Scheduler scheduler(table1);
    scheduler.schedule();
    assert(scheduler.avg_waiting_time() == 66.0);
    assert(scheduler.avg_turnaround_time() == 101.0);
    }
    {
    FCFS_Scheduler scheduler({{0, 24}, {0, 3}, {0, 3}});
    scheduler.schedule();
    assert(scheduler.avg_waiting_time() == 17.0);
    assert(scheduler.avg_turnaround_time() == 27.0);
    }
    {
    SJF_Scheduler scheduler(table1);
    scheduler.schedule();
    assert(scheduler.avg_waiting_time() == 53.5);
    assert(scheduler.avg_turnaround_time() == 88.5);
    }
    {
    SJF_Scheduler scheduler(ProcessTable{{0, 6}, {2, 8}, {5, 7}, {0, 3}});
    scheduler.schedule();
    assert(scheduler.avg_waiting_time() == 5.25);
    assert(scheduler.avg_turnaround_time() == 11.25);
    }
    
    if (PRINT)
    {
        nullstream.close();
        std::cout.rdbuf(backup_cout);
    }
    cout << "All tests passed!\n* * * * * * * * * *" << endl;
}
#pragma endregion tests


int main()
{
    // test();
    ProcessTable table1({{0, 60, 3}, {3, 30, 2}, {4, 40, 1}, {9, 10, 4}});
    ProcessTable table2({{0, 53}, {0, 17}, {0, 68}, {0, 24}});
    ProcessTable table3({{0, 24}, {0, 3}, {0, 3}});
    eval_all(table3);
}