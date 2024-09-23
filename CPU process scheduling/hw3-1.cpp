#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;

mutex mtx;
condition_variable cv;
int current_index = 0;

void count(int index)
{
    unique_lock<mutex> lock(mtx);

    // Wait until it's the turn of this thread
    cv.wait(lock, [index]
            { return index == current_index; });

    int num = 1000000;
    while (num--)
    {
    }

    cout << "I'm thread " << index << ", local count: 1000000\n";

    // Move to the next thread
    current_index++;

    // Notify the next thread
    cv.notify_all();
}

int main(void)
{
    thread t[100];

    for (int i = 0; i < 100; i++)
        t[i] = thread(count, i);

    for (int i = 0; i < 100; i++)
        t[i].join();

    return 0;
}
