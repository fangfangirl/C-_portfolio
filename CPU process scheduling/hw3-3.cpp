#include <iostream>
#include <thread>
#include <vector>
#include <semaphore.h>

using namespace std;

int n, global_count = 0;
int num_threads = 1;
sem_t semaphore;

bool is_prime(int num)
{
    if (num == 1)
        return false;

    for (int i = 2; i * i <= num; i++)
    {
        if (num % i == 0)
        {
            return false;
        }
    }
    return true;
}

// 每個執行緒計算質數的函式
void count_primes(int start, int end)
{
    for (int i = start; i <= end; i++)
    {
        if (is_prime(i))
        {
            sem_wait(&semaphore);
            global_count++;
            sem_post(&semaphore);
        }
    }
}

int main(int argc, char *argv[])
{
    // 幾個threads
    for (int i = 1; i < argc; i++)
    {
        if (string(argv[i]) == "-t" && i + 1 < argc)
        {
            num_threads = stoi(argv[i + 1]);
        }
    }

    cin >> n;

    // 將數字範圍分配給各執行緒
    int numbers_per_thread = n / num_threads;
    vector<thread> threads;
    sem_init(&semaphore, 0, 1);

    for (int i = 0; i < num_threads; i++)
    {
        int start = i * numbers_per_thread + 1;
        int end = (i == num_threads - 1) ? n : (i + 1) * numbers_per_thread;
        threads.push_back(thread(count_primes, start, end));
    }

    // 等待所有執行緒結束
    for (auto &t : threads)
    {
        t.join();
    }

    cout << global_count << endl;
    return 0;
}
