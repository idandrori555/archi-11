#include "picker.h"
#include <ctime>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#if CURRENT_TASK == 2
const int NUM_PHILOSOPHERS = 5;
const int MEALS_COUNT = 1000000;

int main(int argc, char *argv[])
{
  if (argc < 2)
    return 1;

  int id = atoi(argv[1]);
  int left_fork = id;
  int right_fork = (id + 1) % NUM_PHILOSOPHERS;

  // הגדרת השמות בהתאם ל-Master
  char left_name[32], right_name[32];
  sprintf(left_name, "Global\\Fork%d", left_fork);
  sprintf(right_name, "Global\\Fork%d", right_fork);

  // פתיחת המוטקסים הקיימים
  HANDLE hLeft = OpenMutexA(MUTEX_ALL_ACCESS, FALSE, left_name);
  HANDLE hRight = OpenMutexA(MUTEX_ALL_ACCESS, FALSE, right_name);

  if (!hLeft || !hRight)
    return 1;

  clock_t start = clock();

  for (int i = 0; i < MEALS_COUNT; ++i)
  {
    // מניעת Deadlock: הפילוסוף האחרון מרים בסדר הפוך
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

    // אכילה...

    ReleaseMutex(hLeft);
    ReleaseMutex(hRight);
  }

  clock_t end = clock();
  double time_taken = (double)(end - start) / CLOCKS_PER_SEC;
  printf("Philosopher %d finished eating %d meals in %f seconds\n", id, MEALS_COUNT, time_taken);

  CloseHandle(hLeft);
  CloseHandle(hRight);

  return 0;
}
#endif
