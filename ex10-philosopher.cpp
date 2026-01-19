#include "const.h"
#include <ctime>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#if CURRENT_TASK == 2
int main(int argc, char *argv[])
{
  if (argc < 2)
    return 1;

  int id = atoi(argv[1]);
  int left_fork = id;
  int right_fork = (id + 1) % NUM_PHILOSOPHERS;

  std::string left_name = "Global\\Fork" + std::to_string(left_fork);
  std::string right_name = "Global\\Fork" + std::to_string(right_fork);

  HANDLE hLeft = OpenMutexA(MUTEX_ALL_ACCESS, FALSE, left_name.c_str());
  HANDLE hRight = OpenMutexA(MUTEX_ALL_ACCESS, FALSE, right_name.c_str());

  if (!hLeft || !hRight)
  {
    printf("Failed to open mutexes for philosopher %d\n", id);
    return 1;
  }

  clock_t start = clock();

  for (int i = 0; i < MEALS_COUNT; ++i)
  {
    if (id == NUM_PHILOSOPHERS - 1)
    {
      WaitForSingleObject(hRight, INFINITE);
      WaitForSingleObject(hLeft, INFINITE);
    }
    else
    {
      WaitForSingleObject(hLeft, INFINITE);
      WaitForSingleObject(hRight, INFINITE);
    }

    ReleaseMutex(hLeft);
    ReleaseMutex(hRight);
  }

  clock_t end = clock();
  double time_taken = (double)(end - start) / CLOCKS_PER_SEC;
  printf("Philosopher %d finished eating %d meals in %f seconds\n", id,
         MEALS_COUNT, time_taken);

  CloseHandle(hLeft);
  CloseHandle(hRight);

  return 0;
}
#endif
