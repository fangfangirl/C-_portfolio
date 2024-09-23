#include <iostream>
#include <cstdint>
#include <vector>
#include <thread>
#include <cmath>

using namespace std;

int n, m;
vector<uint64_t> subsets;
uint64_t one = static_cast<uint64_t>(1), global_count = 0;
vector<int> local_counts;
int num_threads = 1;
int log_num_thread = 0;

void solve(int id, int index, uint64_t current)
{
    if (index == m)
    {
        if (current == (one << n) - 1)
            local_counts[id]++;
    }
    else
    {
        solve(id, index + 1, current);
        solve(id, index + 1, current | subsets[index]);
    }
}

int main(int argc, char *argv[])
{
    for (int i = 1; i < argc; i++)
    {
        if (string(argv[i]) == "-t" && i + 1 < argc)
        {
            num_threads = stoi(argv[i + 1]);
            if ((num_threads & (num_threads - 1)) != 0)
            {
                num_threads = 1;
            }
            log_num_thread = log2(num_threads);
        }
    }

    local_counts.resize(num_threads, 0);

    cin >> n >> m;

    subsets.resize(m);
    for (int i = 0; i < m; i++)
    {
        int p, temp;
        cin >> p;
        for (int j = 0; j < p; j++)
        {
            cin >> temp;
            subsets[i] |= (one << temp);
        }
    }

    vector<uint64_t> initial_values;

    // 動態生成 initial_values
    for (int i = 0; i < num_threads; ++i)
    {
        uint64_t value = 0;
        for (int j = 0; j < num_threads; ++j)
        {
            if ((i >> j) & 1)
            {
                value |= subsets[j];
            }
        }
        initial_values.push_back(value);
    }

    vector<thread> threads;

    for (int i = 0; i < num_threads; ++i)
    {
        threads.emplace_back(solve, i, log_num_thread, initial_values[i]);
    }

    for (auto &t : threads)
    {
        t.join();
    }

    for (int i = 0; i < num_threads; ++i)
    {
        global_count += local_counts[i];
    }

    cout << global_count << endl;
    return 0;
}
