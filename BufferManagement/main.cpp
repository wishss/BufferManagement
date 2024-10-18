#include <iostream>
#include <windows.h>
#include <thread>
#include <mutex>
#include <queue>

const int BUFFER_SIZE = 5;
std::queue<int> buffer;
std::mutex mtx;
HANDLE empty, full;

void producer()
{
    for (int i = 0; i < 10; ++i) 
    {
        // �� ������ ���� �� ���
        WaitForSingleObject(empty, INFINITE);

        // ���� ����
        mtx.lock();
        buffer.push(i);
        std::cout << "Produced: " << i << std::endl;
        mtx.unlock();

        ReleaseSemaphore(full, 1, NULL);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void consumer()
{
    for (int i = 0; i < 10; ++i)
    {
        // ���� �� ������ ���� �� ���
        WaitForSingleObject(full, INFINITE);

        // �Һ� ����
        mtx.lock();
        int data = buffer.front();
        buffer.pop();
        std::cout << "Consumed: " << data << std::endl;
        mtx.unlock();

        ReleaseSemaphore(empty, 1, NULL);
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
    }
}

void main()
{
    empty = CreateSemaphore(NULL, BUFFER_SIZE, BUFFER_SIZE, NULL);
    full = CreateSemaphore(NULL, 0, BUFFER_SIZE, NULL);

    std::thread producerThread(producer);
    std::thread consumerThread(consumer);

    producerThread.join();
    consumerThread.join();

    CloseHandle(empty);
    CloseHandle(full);

	return;
}