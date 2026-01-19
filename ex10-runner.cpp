#include "const.h"
#include <ctime>
#include <iostream>
#include <stdio.h>
#include <windows.h>

constexpr const char *PROCESS_NAME = "philosopher.exe";

#if CURRENT_TASK == 2
int main()
{
  using std::string, std::to_string;

  HANDLE mutexes[NUM_PHILOSOPHERS];
  HANDLE processes[NUM_PHILOSOPHERS];

  for (int i = 0; i < NUM_PHILOSOPHERS; ++i)
  {
    string name = "Global\\Fork" + to_string(i);
    mutexes[i] = CreateMutexA(NULL, FALSE, name.c_str());
    if (mutexes[i] == NULL)
    {
      printf("Failed to create mutex %d\n", i);
      return 1;
    }
  }

  puts("Master: Created 5 Named Mutexes. Starting processes...");
  clock_t start_time = clock();

  for (int i = 0; i < NUM_PHILOSOPHERS; ++i)
  {
    STARTUPINFOA si = {sizeof(si)};
    PROCESS_INFORMATION pi;

    std::string cmd = std::string(PROCESS_NAME) + " " + std::to_string(i);

    if (!CreateProcessA(PROCESS_NAME, const_cast<char *>(cmd.c_str()), NULL, NULL, FALSE, 0, NULL, NULL,
                        &si, &pi))
    {
      printf("Failed to create process %d. Make sure %s exists!\n", i,
             PROCESS_NAME);
    }
    else
    {
      processes[i] = pi.hProcess;
      CloseHandle(pi.hThread);
    }
  }

  WaitForMultipleObjects(NUM_PHILOSOPHERS, processes, TRUE, INFINITE);

  clock_t end_time = clock();

  puts("\n--- Master: All processes finished ---");

  for (int i = 0; i < NUM_PHILOSOPHERS; ++i)
  {
    CloseHandle(processes[i]);
    CloseHandle(mutexes[i]);
  }

  return 0;
}
#endif
