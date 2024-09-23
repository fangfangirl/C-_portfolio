#include <iostream>
#include <queue>
#include <limits.h>
#include <vector>
#include <algorithm>
using namespace std;

struct Mode
{
    int mode;    // 排程方式
    int quantum; // 限制時間
};

struct Process
{
    int arr;   // 到達時間
    int burst; // 執行時間
    int wait;  // 等待時間
    int turnaround;
    int remaining; // 剩餘執行時間
};

void multiple_queue(int n_queue, vector<Process> &processVector, vector<Mode> &modeVector)
{
    int currentTime = 0;                           // 現在執行到哪一個時間
    int remainingProcesses = processVector.size(); // 剩下的程式

    vector<queue<int>> processQueues(n_queue); // 存放索引

    int kk = 0;              // 已抵達幾個
    int lastQueueIndex = -1; // 上一個queue是哪一個
    int nowQueueIndex = -1;  // 知道現在執行的queue是哪一個
    int lastProcessIndex = -1;
    int currentProcess = -1; // 讀取現在是哪一個queue的第一個
    int havequantum = 0;     // 紀錄RR的已經過的時間
    int high_QueueIndex;     // 我現在上面的優先queue

    while (remainingProcesses > 0)
    {

        for (int i = kk; i < processVector.size(); i++)
        {
            if (processVector[i].arr <= currentTime && processVector[i].remaining > 0)
            {
                processQueues[0].push(i);
                kk += 1;
            }
        }

        //====================================================

        lastQueueIndex = nowQueueIndex;

        if (lastProcessIndex == -1)
        {
            nowQueueIndex = -1;
            high_QueueIndex = n_queue;
        }
        else
        {
            if (havequantum != -1)
            {
                high_QueueIndex = nowQueueIndex;
            }
            else
            {
                high_QueueIndex = n_queue;
            }
        }

        for (int i = 0; i < high_QueueIndex; i++)
        {
            if (!processQueues[i].empty())
            {
                nowQueueIndex = i;
                break;
            }
        }

        if (nowQueueIndex == -1) // 現在所有都為空，繼續增加時間
        {
            currentTime++;
        }
        else
        {

            if (modeVector[nowQueueIndex].mode == 0) // FCFS
            {
                havequantum = 0;

                if (lastQueueIndex == nowQueueIndex) // 決定現在的process
                {

                    if (lastProcessIndex >= 0)
                    {
                        currentProcess = lastProcessIndex;
                    }
                    else
                    {
                        currentProcess = processQueues[nowQueueIndex].front();
                        processQueues[nowQueueIndex].pop();
                    }
                }
                else
                {
                    if (lastProcessIndex >= 0) // 上一個進程要塞回去下一個如果是最後一個那就塞回原來
                    {
                        if (lastQueueIndex + 1 < n_queue)
                        {
                            processQueues[lastQueueIndex + 1].push(lastProcessIndex);
                        }
                        else
                        {
                            processQueues[lastQueueIndex].push(lastProcessIndex);
                        }
                    }
                    currentProcess = processQueues[nowQueueIndex].front();
                    processQueues[nowQueueIndex].pop();
                }

                Process &process = processVector[currentProcess];
                process.remaining--;
                currentTime++;

                if (process.remaining == 0)
                {
                    remainingProcesses--;
                    process.turnaround = currentTime - process.arr;
                    process.wait = currentTime - process.arr - process.burst;
                    lastProcessIndex = -1;
                }
                else
                {
                    lastProcessIndex = currentProcess;
                }
            }
            else if (modeVector[nowQueueIndex].mode == 1) // SRTF
            {
                havequantum = 0;

                // 如果和上一次的queue不同，将上一次的进程放回队列
                if (lastQueueIndex != nowQueueIndex)
                {
                    if (lastProcessIndex >= 0)
                    {
                        if (lastQueueIndex + 1 < n_queue)
                        {
                            processQueues[lastQueueIndex + 1].push(lastProcessIndex);
                        }
                        else
                        {
                            processQueues[lastQueueIndex].push(lastProcessIndex);
                        }
                    }

                    int shortestRemainingTime = INT_MAX;
                    int shortestProcessIndex = -1;

                    queue<int> tempQueue = processQueues[nowQueueIndex];

                    while (!tempQueue.empty())
                    {
                        int temp_processIndex = tempQueue.front();
                        tempQueue.pop();
                        if (processVector[temp_processIndex].remaining <= shortestRemainingTime)
                        {

                            if ((processVector[temp_processIndex].remaining < shortestRemainingTime) || (processVector[temp_processIndex].remaining == shortestRemainingTime && processVector[temp_processIndex].arr < processVector[shortestProcessIndex].arr))
                            {
                                shortestRemainingTime = processVector[temp_processIndex].remaining;
                                shortestProcessIndex = temp_processIndex;
                            }
                        }
                    }

                    int temp_processIndex;
                    do
                    {
                        temp_processIndex = processQueues[nowQueueIndex].front();
                        processQueues[nowQueueIndex].pop();

                        if (temp_processIndex != shortestProcessIndex)
                        {
                            processQueues[nowQueueIndex].push(temp_processIndex);
                        }
                    } while (temp_processIndex != shortestProcessIndex);

                    currentProcess = shortestProcessIndex;
                }
                else if (lastQueueIndex == nowQueueIndex)
                {
                    if (lastProcessIndex >= 0)
                    {
                        processQueues[lastQueueIndex].push(lastProcessIndex);
                    }

                    int shortestRemainingTime = INT_MAX;
                    int shortestProcessIndex = -1;

                    queue<int> tempQueue = processQueues[nowQueueIndex];

                    while (!tempQueue.empty())
                    {
                        int temp_processIndex = tempQueue.front();
                        tempQueue.pop();
                        if (processVector[temp_processIndex].remaining <= shortestRemainingTime)

                        {
                            if ((processVector[temp_processIndex].remaining < shortestRemainingTime) || (processVector[temp_processIndex].remaining == shortestRemainingTime && processVector[temp_processIndex].arr < processVector[shortestProcessIndex].arr))
                            {
                                shortestRemainingTime = processVector[temp_processIndex].remaining;
                                shortestProcessIndex = temp_processIndex;
                            }
                        }
                    }

                    int temp_processIndex;
                    do
                    {
                        temp_processIndex = processQueues[nowQueueIndex].front();
                        processQueues[nowQueueIndex].pop();

                        if (temp_processIndex != shortestProcessIndex)
                        {
                            processQueues[nowQueueIndex].push(temp_processIndex);
                        }
                    } while (temp_processIndex != shortestProcessIndex);

                    //=========================================
                    if (lastProcessIndex >= 0)
                    {
                        if (lastProcessIndex != shortestProcessIndex)
                        {
                            int temp_processIndex;
                            int k = 0;
                            do
                            {
                                k += 1;
                                temp_processIndex = processQueues[nowQueueIndex].front();
                                processQueues[nowQueueIndex].pop();

                                if (temp_processIndex != lastProcessIndex)
                                {
                                    processQueues[nowQueueIndex].push(temp_processIndex);
                                }
                            } while ((temp_processIndex != lastProcessIndex) && (k <= processQueues[nowQueueIndex].size()));

                            if (lastQueueIndex + 1 < n_queue)
                            {
                                processQueues[lastQueueIndex + 1].push(lastProcessIndex);
                            }
                            else
                            {
                                processQueues[lastQueueIndex].push(lastProcessIndex);
                            }
                        }
                    }
                    // ============================================================

                    // ===========================================================================

                    currentProcess = shortestProcessIndex;
                }

                // 當前程式
                Process &process = processVector[currentProcess];
                process.remaining--;
                currentTime++;

                if (process.remaining == 0)
                {
                    remainingProcesses--;
                    process.turnaround = currentTime - process.arr;
                    process.wait = currentTime - process.arr - process.burst;
                    lastProcessIndex = -1;
                }
                else
                {
                    lastProcessIndex = currentProcess;
                }
            }
            else if (modeVector[nowQueueIndex].mode == 2) // RR
            {

                // 如果和上一次的queue不同，将上一次的进程放回队列
                if (lastQueueIndex != nowQueueIndex)
                {
                    havequantum = 0;           // 確保新的來自於其他,RR回到初始值
                    if (lastProcessIndex >= 0) // 上一個進程要塞回去下一個如果是最後一個那就塞回原來
                    {
                        if (lastQueueIndex + 1 < n_queue)
                        {
                            processQueues[lastQueueIndex + 1].push(lastProcessIndex);
                        }
                        else
                        {
                            processQueues[lastQueueIndex].push(lastProcessIndex);
                        }
                    }
                    currentProcess = processQueues[nowQueueIndex].front();
                    processQueues[nowQueueIndex].pop();
                }
                else if (lastQueueIndex == nowQueueIndex)
                {
                    if (lastProcessIndex >= 0)
                    {
                        if ((havequantum < modeVector[nowQueueIndex].quantum) && (havequantum >= 0))
                        {
                            currentProcess = lastProcessIndex;
                        }
                        else
                        {
                            if (lastQueueIndex + 1 < n_queue)
                            {
                                processQueues[lastQueueIndex + 1].push(lastProcessIndex);
                            }
                            else
                            {
                                processQueues[lastQueueIndex].push(lastProcessIndex);
                            }
                            havequantum = 0;
                            // processQueues[nowQueueIndex].push(lastProcessIndex);
                            currentProcess = processQueues[nowQueueIndex].front();
                            processQueues[nowQueueIndex].pop();
                        }
                    }
                    else
                    {
                        currentProcess = processQueues[nowQueueIndex].front();
                        processQueues[nowQueueIndex].pop();
                    }
                }

                Process &process = processVector[currentProcess];
                process.remaining--;
                currentTime++;
                havequantum++;
                if (process.remaining == 0)
                {
                    remainingProcesses--;
                    process.turnaround = currentTime - process.arr;
                    process.wait = currentTime - process.arr - process.burst;
                    lastProcessIndex = -1;
                    havequantum = 0;
                }
                else
                {
                    lastProcessIndex = currentProcess;
                }

                if (havequantum == modeVector[nowQueueIndex].quantum)
                {
                    havequantum = -1;
                }
            }
        }
    }
}

int main()
{
    int M, N; // N 為幾個Queue , M 為幾個process
    cin >> N >> M;

    if (N > 5 || N < 1)
    {
        cout << "N is out of range." << endl;
        return 1;
    }
    else if (M > 100 || M < 1)
    {
        cout << "M is out of range." << endl;
        return 1;
    }

    vector<Mode> modeVector_for_all;
    vector<Process> processVector_for_all;

    for (int n = 0; n < N; n++)
    {
        int mod, q;
        cin >> mod >> q;
        if (((mod == 0) || (mod == 1)) && (q == -1))
        {
            Mode mode_all;
            mode_all.mode = mod;
            mode_all.quantum = q;
            modeVector_for_all.push_back(mode_all);
        }
        else if ((mod == 2) && (1 <= q <= 100))
        {
            Mode mode_all;
            mode_all.mode = mod;
            mode_all.quantum = q;
            modeVector_for_all.push_back(mode_all);
        }
        else
        {
            cout << "The input format is incorrect." << endl;
            return 1;
        }
    }

    for (int m = 0; m < M; m++)
    {
        int a, b;
        cin >> a >> b;

        if ((0 <= a <= 10000) && (1 <= b <= 1000))
        {
            Process p;
            p.arr = a;
            p.burst = b;
            p.wait = 0;
            p.turnaround = 0;
            p.remaining = b;
            processVector_for_all.push_back(p);
        }
        else
        {
            cout << "The input format is incorrect." << endl;
            return 1;
        }
    }

    if (N == 1)
    {
        multiple_queue(N, processVector_for_all, modeVector_for_all);
    }
    else
    {
        multiple_queue(N, processVector_for_all, modeVector_for_all);
    }

    int all_waiting = 0;
    int all_turnaround = 0;
    for (int i = 0; i < processVector_for_all.size(); i++)
    {
        all_waiting += processVector_for_all[i].wait;
        all_turnaround += processVector_for_all[i].turnaround;
        cout << processVector_for_all[i].wait << " " << processVector_for_all[i].turnaround << endl;
    }

    cout << all_waiting << endl;
    cout << all_turnaround << endl;

    return 0;
}