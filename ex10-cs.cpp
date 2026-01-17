#include "picker.h"
#include <ctime>
#include <stdio.h>
#include <windows.h>

constexpr int PHILOSOPHERS_COUNT = 5;
constexpr int MEALS_PER_PHILOSOPHER = 1000000;

template <class T>
constexpr inline T min(T a, T b)
{
  return a < b ? a : b;
}

template <class T>
constexpr inline T max(T a, T b)
{
  return a > b ? a : b;
}

static CRITICAL_SECTION sticks[PHILOSOPHERS_COUNT];

struct PhilosopherData
{
  int id;
};

DWORD WINAPI tryEat(LPVOID lpParameter)
{
  PhilosopherData *pData = (PhilosopherData *)lpParameter;
  int id = pData->id;

  int left = id;
  int right = (id + 1) % PHILOSOPHERS_COUNT;

  int first = min(left, right);
  int second = max(left, right);

  for (int i = 0; i < MEALS_PER_PHILOSOPHER; ++i)
  {
    EnterCriticalSection(&sticks[first]);
    EnterCriticalSection(&sticks[second]);

    printf("Philosopher %d is eating\n", id);

    LeaveCriticalSection(&sticks[second]);
    LeaveCriticalSection(&sticks[first]);
  }

  return 0;
}

#if CURRENT_TASK == 2
int main(void)
{
  HANDLE threads[PHILOSOPHERS_COUNT];
  PhilosopherData pData[PHILOSOPHERS_COUNT];

  // Initiaalize sticks (critical sections)
  for (int i = 0; i < PHILOSOPHERS_COUNT; ++i)
  {
    InitializeCriticalSection(&sticks[i]);
  }

  puts("Starting to measure time...");
  clock_t start = clock();

  // Initialize philosophers (threads)
  for (int i = 0; i < PHILOSOPHERS_COUNT; ++i)
  {
    pData[i].id = i;
    threads[i] = CreateThread(NULL, 0, tryEat, &pData[i], 0, NULL);
    if (!threads[i])
    {
      printf("CreateThread failed\n");
      return 1;
    }
  }

  // Wait for everything to finish
  WaitForMultipleObjects(PHILOSOPHERS_COUNT, threads, TRUE, INFINITE);

  clock_t end = clock();
  double totalTime = (double)(end - start) / CLOCKS_PER_SEC;
  printf("Time taken: %f seconds\n", totalTime);

  // Cleanup
  for (int i = 0; i < PHILOSOPHERS_COUNT; ++i)
  {
    CloseHandle(threads[i]);
    DeleteCriticalSection(&sticks[i]);
  }

  return 0;
}
#endif
