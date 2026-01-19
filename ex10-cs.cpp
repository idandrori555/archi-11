#include "const.h"
#include <ctime>
#include <stdio.h>
#include <windows.h>

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

static CRITICAL_SECTION sticks[NUM_PHILOSOPHERS];

struct PhilosopherData
{
  int id;
};

DWORD WINAPI tryEat(LPVOID lpParameter)
{
  PhilosopherData *pData = (PhilosopherData *)lpParameter;
  int id = pData->id;

  // calc left and right sticks
  int left = id;
  int right = (id + 1) % NUM_PHILOSOPHERS;

  int first = min(left, right);
  int second = max(left, right);

  // start timing
  clock_t start = clock();

  for (int i = 0; i < MEALS_PER_PHILOSOPHER; ++i)
  {
    EnterCriticalSection(&sticks[first]);
    EnterCriticalSection(&sticks[second]);

    if (i == 0)
    {
      // philos is eating
      printf("Philosopher %d is eating\n", id);
    }

    LeaveCriticalSection(&sticks[second]);
    LeaveCriticalSection(&sticks[first]);
  }

  // end timing
  clock_t end = clock();
  double time_taken = (double)(end - start) / CLOCKS_PER_SEC;
  printf("Philosopher %d finished %d meals in %f seconds\n", id, MEALS_COUNT, time_taken);

  return 0;
}

#if CURRENT_TASK == 1
int main(void)
{
  HANDLE threads[NUM_PHILOSOPHERS];
  PhilosopherData pData[NUM_PHILOSOPHERS];

  // init the crit sections
  for (int i = 0; i < NUM_PHILOSOPHERS; ++i)
  {
    InitializeCriticalSection(&sticks[i]);
  }

  puts("Starting to measure time...");
  clock_t start = clock();

  // start creating the threads
  for (int i = 0; i < NUM_PHILOSOPHERS; ++i)
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
  WaitForMultipleObjects(NUM_PHILOSOPHERS, threads, TRUE, INFINITE);

  clock_t end = clock();

  // Cleanup
  for (int i = 0; i < NUM_PHILOSOPHERS; ++i)
  {
    CloseHandle(threads[i]);
    DeleteCriticalSection(&sticks[i]);
  }

  return 0;
}
#endif
